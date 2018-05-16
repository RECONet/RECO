#include "module.hpp"
#include "location.hpp"
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>
#include "log.h"
#include "assertions.h"
#include "dynamic_memory_check.h"
#include "intertask_interface.h"
#include "common_defs.h"
#include "msc.h"
#include "itti_free_defined_msg.h"
#include "mme_config.h"
#include "timer.h"
#include "xml_msg_dump.h"

#include "common_types.h"
#include "conversions.h"
#include "gcc_diag.h"
#include "secu_defs.h"
#include "mme_app_defs.h"

#include <string.h>
#include "s6a_up_loc.hpp"

using grpc::Status;
using test::Test;
using test::Request;
using test::Reply;

int finish = 0;
Reply* reply2;

class TestImpl final : public Test::Service {

public:
	Status Location_update(grpc::ServerContext* context, const Request* request,
                  Reply* reply) override {
		
		send_s6a_update_location_req(request);	//location update request	
		while (!finish);
		memcpy( reply, reply2, sizeof(Reply));		

		return Status::OK;
        }

private:


};


class location_update : public module {
  public:
    //creating mme_app module thread
    virtual int init(mme_config_t * mme_config_p) {
        return location_update_init (mme_config_p);
    }
};
/* the class factories */
extern "C" module* create() {
    return new location_update;
}
extern "C" void destroy(module* p) {
    delete p;
}


void *location_update_thread (void *args)
{
  std::string addr = "0.0.0.0:5000";
  TestImpl service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  auto server = builder.BuildAndStart();
  std::cout << "Server listening on " << addr << std::endl;
  server->Wait(); 

}

/*
void *location_update_thread (void *args)
{
  itti_mark_task_ready (TASK_LOCATION_UPDATE);

  while (1) {
    MessageDef                             *received_message_p = NULL;

    /*
     * Trying to fetch a message from the message queue.
     * If the queue is empty, this function will block till a
     * message is sent to the task.
     */
/*
    itti_receive_msg (TASK_LOCATION_UPDATE, &received_message_p);
    DevAssert (received_message_p );

    switch (ITTI_MSG_ID (received_message_p)) {

    case NAS_PDN_CONFIG_REQ: {
        ue_mm_context_t                    *ue_context_p = NULL;
        ue_context_p = mme_ue_context_exists_mme_ue_s1ap_id (&mme_app_desc.mme_ue_contexts, received_message_p->ittiMsg.nas_pdn_config_req.ue_id);
        if (ue_context_p) {
          if (!ue_context_p->is_s1_ue_context_release) {
            send_s6a_update_location_req(ue_context_p);	//location update request
		
          }
        }
      }
      break;

    case S6A_UPDATE_LOCATION_ANS:{ */
        /*
         * We received the update location answer message from HSS -> Handle it
         */
/*
        handle_s6a_update_location_ans (&received_message_p->ittiMsg.s6a_update_location_ans);
      }
      break;

    default:{
      OAILOG_DEBUG (LOG_LOCATION_UPDATE, "Unkwnon message ID %d:%s\n", ITTI_MSG_ID (received_message_p), ITTI_MSG_NAME (received_message_p));
        AssertFatal (0, "Unkwnon message ID %d:%s\n", ITTI_MSG_ID (received_message_p), ITTI_MSG_NAME (received_message_p));
      }
      break;
    }

    itti_free_msg_content(received_message_p);
    itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
    received_message_p = NULL;
  }

  return NULL;
}
*/
//------------------------------------------------------------------------------
int location_update_init (const mme_config_t * mme_config_p)
{
  class s6a_cxx s6a_obj; 

  s6a_obj.s6a_init();
  std::cout << "S6A INIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  /*
   * Create the thread associated with location update
   */
  if (itti_create_task (TASK_LOCATION_UPDATE, &location_update_thread, NULL) < 0) {
    OAILOG_ERROR (LOG_LOCATION_UPDATE, "LOCATION UPDATE create task failed\n");
    OAILOG_FUNC_RETURN (LOG_LOCATION_UPDATE, RETURNerror);
  }
  printf("Initializing LOCATION UPDATE task interface: DONE\n");
  OAILOG_DEBUG (LOG_LOCATION_UPDATE, "Initializing LOCATION UPDATE task interface: DONE\n");
  return 0;
}

//------------------------------------------------------------------------------
void location_update_exit (void)
{
  
}

//------------------------------------------------------------------------------
int send_s6a_update_location_req (
  const Request* ue_mm_context)
{
  OAILOG_FUNC_IN (LOG_LOCATION_UPDATE);
  MessageDef                             *message_p = NULL;
  s6a_update_location_req_t              *s6a_ulr_p = NULL;
  int                                     rc = RETURNok;


  message_p = itti_alloc_new_message (TASK_LOCATION_UPDATE, S6A_UPDATE_LOCATION_REQ);

  if (message_p == NULL) {
    OAILOG_FUNC_RETURN (LOG_LOCATION_UPDATE, RETURNerror);
  }

  s6a_ulr_p = &message_p->ittiMsg.s6a_update_location_req;

  strcpy (s6a_ulr_p->imsi, ue_mm_context->imsi().c_str());
  //IMSI_TO_STRING((ue_mm_context->imsi()),s6a_ulr_p->imsi, IMSI_BCD_DIGITS_MAX+1);
  s6a_ulr_p->imsi_length = strlen (s6a_ulr_p->imsi);
  s6a_ulr_p->initial_attach = INITIAL_ATTACH;

  plmn_t visited_plmn = {0};
  visited_plmn.mcc_digit1 = ue_mm_context->mcc_digit1();
  visited_plmn.mcc_digit2 = ue_mm_context->mcc_digit2();
  visited_plmn.mcc_digit3 = ue_mm_context->mcc_digit3();
  visited_plmn.mnc_digit1 = ue_mm_context->mnc_digit1();
  visited_plmn.mnc_digit2 = ue_mm_context->mnc_digit2();
  visited_plmn.mnc_digit3 = ue_mm_context->mnc_digit3();

  memcpy (&s6a_ulr_p->visited_plmn, &visited_plmn, sizeof (plmn_t));
  s6a_ulr_p->rat_type = RAT_EUTRAN;
  /*
   * Check if we already have UE data
   */
  s6a_ulr_p->skip_subscriber_data = 0;
  MSC_LOG_TX_MESSAGE (MSC_MMEAPP_MME, MSC_S6A_MME, NULL, 0, "0 S6A_UPDATE_LOCATION_REQ imsi %s", s6a_ulr_p->imsi);	//??
  s6a_up_loc::s6a_generate_update_location(s6a_ulr_p);
/*
  rc =  itti_send_msg_to_task (TASK_S6A, INSTANCE_DEFAULT, message_p);
  OAILOG_FUNC_RETURN (LOG_LOCATION_UPDATE, rc);
*/
}
//------------------------------------------------------------------------------

int handle_s6a_update_location_ans (
  const s6a_update_location_ans_t * const ula_pP)
{
  reply2 = new Reply;

  reply2->set_present(ula_pP->result.present);
  if (ula_pP->result.choice.base == DIAMETER_SUCCESS) reply2->set_base(test::Reply_Base_DIAMETER_SUCCESS);

  reply2->set_imsi(ula_pP->imsi);
  
  if (ula_pP->subscription_data.subscriber_status == 0) reply2->set_subscriber_status(test::Reply_subscriber_status_t_SS_SERVICE_GRANTED);
  else if (ula_pP->subscription_data.subscriber_status == 1) reply2->set_subscriber_status(test::Reply_subscriber_status_t_SS_OPERATOR_DETERMINED_BARRING);
  else if (ula_pP->subscription_data.subscriber_status == 2) reply2->set_subscriber_status(test::Reply_subscriber_status_t_SS_MAX);

  reply2->set_access_restriction(ula_pP->subscription_data.access_restriction);
  reply2->mutable_subscribed_ambr()->set_br_ul(ula_pP->subscription_data.subscribed_ambr.br_ul);
  reply2->mutable_subscribed_ambr()->set_br_dl(ula_pP->subscription_data.subscribed_ambr.br_dl);
  reply2->set_msisdn(ula_pP->subscription_data.msisdn);
  reply2->set_msisdn_length((uint32_t)ula_pP->subscription_data.msisdn_length);
  reply2->set_rau_tau_timer(ula_pP->subscription_data.rau_tau_timer);
  
  if (ula_pP->subscription_data.access_mode == 0)  reply2->set_access_mode(test::Reply_access_mode_t_NAM_PACKET_AND_CIRCUIT);
  else if (ula_pP->subscription_data.access_mode == 1) reply2->set_access_mode(test::Reply_access_mode_t_NAM_RESERVED);
  else if (ula_pP->subscription_data.access_mode == 2) reply2->set_access_mode(test::Reply_access_mode_t_NAM_ONLY_PACKET);
  else if (ula_pP->subscription_data.access_mode == 3) reply2->set_access_mode(test::Reply_access_mode_t_NAM_MAX);
  
  reply2->set_context_identifier(ula_pP->subscription_data.apn_config_profile.context_identifier);

  if (ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind == 0) reply2->set_all_apn_conf_ind(test::Reply_all_apn_conf_ind_t_ALL_APN_CONFIGURATIONS_INCLUDED);
  else if (ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind == 1) reply2->set_all_apn_conf_ind(test::Reply_all_apn_conf_ind_t_MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED);
  else if (ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind == 2) reply2->set_all_apn_conf_ind(test::Reply_all_apn_conf_ind_t_ALL_APN_MAX);
  
  reply2->set_nb_apns((uint32_t)ula_pP->subscription_data.apn_config_profile.nb_apns);
  reply2->set_apn_context_identifier(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].context_identifier);
  reply2->set_nb_ip_address((uint32_t)ula_pP->subscription_data.apn_config_profile.apn_configuration[0].nb_ip_address);

  if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 0) reply2->set_pdn_type(test::Reply_pdn_type_t_IPv4);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 1) reply2->set_pdn_type(test::Reply_pdn_type_t_IPv6);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 2) reply2->set_pdn_type(test::Reply_pdn_type_t_IPv4_AND_v6);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 3) reply2->set_pdn_type(test::Reply_pdn_type_t_IPv4_OR_v6);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 4) reply2->set_pdn_type(test::Reply_pdn_type_t_IP_MAX);

  reply2->set_service_selection(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection);
  reply2->set_service_selection_length(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection_length);
  reply2->set_qci(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.qci);
  reply2->set_priority_level(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.priority_level); 
  
  if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == 0) reply2->set_pre_emp_vulnerability(test::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_ENABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == 1) reply2->set_pre_emp_vulnerability(test::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_DISABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == 2) reply2->set_pre_emp_vulnerability(test::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_MAX);

  if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability == 0) reply2->set_pre_emp_capability(test::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_ENABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability == 1) reply2->set_pre_emp_capability(test::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_DISABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability == 2) reply2->set_pre_emp_capability(test::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_MAX);
  
  reply2->mutable_ambr()->set_br_ul(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_ul);
  reply2->mutable_ambr()->set_br_dl(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_dl);

  finish = 1;

}

