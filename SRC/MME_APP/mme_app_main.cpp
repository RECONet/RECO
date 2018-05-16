/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file mme_app_main.c
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>

#include "log.h"
#include "assertions.h"
#include "dynamic_memory_check.h"

#include "intertask_interface.h"
#include "common_defs.h"

#include "msc.h"

#include "intertask_interface.h"
#include "itti_free_defined_msg.h"
#include "mme_config.h"
#include "timer.h"
#include "mme_app_extern.h"
#include "mme_app_ue_context.hpp"
#include "mme_app_defs.h"
#include "mme_app_statistics.h"
#include "xml_msg_dump.h"
#include "common_defs.h"
#include "mme_app_edns_emulation.h"
#include "mme_app_config.h"

#undef max
#undef min

#include <grpc++/grpc++.h>

#include "location.grpc.pb.h"
#include "conversions.h"
#include <thread>
#include "call_gui.h"
#include <unistd.h>
#include <string>


using grpc::Status;
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using location::LocationUpdate;
using location::Request;
using location::Reply;


extern xmlTextWriterPtr g_xml_text_writer;


void  *mme_app_thread (void *args);

struct AsyncClientCall {
    Reply reply;
    ClientContext context;
    Status status;
    std::unique_ptr<ClientAsyncResponseReader<Reply>> response_reader;
};

struct service_ip_port {
  char* service_ip;
  int service_port;
};



class MME_Client {

public:
	MME_Client(std::shared_ptr<grpc::Channel> channel)
        : stub_(LocationUpdate::NewStub(channel))  { }
	void Send_request(ue_mm_context_t *const ue_mm_context, char imsi[]) {
    		
		Request a;
		a.set_imsi(imsi);
		a.set_mcc_digit1((uint32_t)(ue_mm_context->emm_context.originating_tai.mcc_digit1));
  		a.set_mcc_digit2((uint32_t)(ue_mm_context->emm_context.originating_tai.mcc_digit2));
		a.set_mcc_digit3((uint32_t)(ue_mm_context->emm_context.originating_tai.mcc_digit3));
		a.set_mnc_digit1((uint32_t)(ue_mm_context->emm_context.originating_tai.mnc_digit1));
		a.set_mnc_digit2((uint32_t)(ue_mm_context->emm_context.originating_tai.mnc_digit2));
		a.set_mnc_digit3((uint32_t)(ue_mm_context->emm_context.originating_tai.mnc_digit3));

		AsyncClientCall *call = new AsyncClientCall;
		call->response_reader = stub_->AsyncLocation_update(&call->context, a, &cq_);
		call->response_reader->Finish(&call->reply, &call->status, (void*)call);

    		std::cout << "Call function finished" << std::endl;

        }

	void AsyncCompleteRpc() 
    	{
        	void* got_tag;
        	bool ok = false;
        	
        	while (cq_.Next(&got_tag, &ok)) 
        	{
            		AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
            		if (call->status.ok()) {
            	    		std::cout << "Location update reply received" << std::endl;
				
				s6a_update_location_ans_t * ula_pP = new s6a_update_location_ans_t;
				ula_pP->result.present = call->reply.present();
				
				if (call->reply.base() == location::Reply_Base_DIAMETER_SUCCESS) ula_pP->result.choice.base = DIAMETER_SUCCESS;
				strcpy(ula_pP->imsi, call->reply.imsi().c_str());

				if (call->reply.subscriber_status() == location::Reply_subscriber_status_t_SS_SERVICE_GRANTED) ula_pP->subscription_data.subscriber_status = SS_SERVICE_GRANTED;
				else if (call->reply.subscriber_status() == location::Reply_subscriber_status_t_SS_OPERATOR_DETERMINED_BARRING) ula_pP->subscription_data.subscriber_status = SS_OPERATOR_DETERMINED_BARRING;
				else if (call->reply.subscriber_status() == location::Reply_subscriber_status_t_SS_MAX) ula_pP->subscription_data.subscriber_status = SS_MAX;

				ula_pP->subscription_data.access_restriction = call->reply.access_restriction();
				ula_pP->subscription_data.subscribed_ambr.br_ul = call->reply.subscribed_ambr().br_ul();
				ula_pP->subscription_data.subscribed_ambr.br_dl = call->reply.subscribed_ambr().br_dl();
				strcpy(ula_pP->subscription_data.msisdn, call->reply.msisdn().c_str());
				ula_pP->subscription_data.msisdn_length = call->reply.msisdn_length();
				ula_pP->subscription_data.rau_tau_timer = call->reply.rau_tau_timer();
		
				if (call->reply.access_mode() == location::Reply_access_mode_t_NAM_PACKET_AND_CIRCUIT) ula_pP->subscription_data.access_mode = NAM_PACKET_AND_CIRCUIT;
				else if (call->reply.access_mode() == location::Reply_access_mode_t_NAM_RESERVED) ula_pP->subscription_data.access_mode = NAM_RESERVED;
				else if (call->reply.access_mode() == location::Reply_access_mode_t_NAM_ONLY_PACKET) ula_pP->subscription_data.access_mode = NAM_ONLY_PACKET;
				else if (call->reply.access_mode() == location::Reply_access_mode_t_NAM_MAX) ula_pP->subscription_data.access_mode = NAM_MAX;		

				ula_pP->subscription_data.apn_config_profile.context_identifier = call->reply.context_identifier();
		
				if (call->reply.all_apn_conf_ind() == location::Reply_all_apn_conf_ind_t_ALL_APN_CONFIGURATIONS_INCLUDED) ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind = ALL_APN_CONFIGURATIONS_INCLUDED;
		  		else if (call->reply.all_apn_conf_ind() == location::Reply_all_apn_conf_ind_t_MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED) ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind = MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED;
		  		else if (call->reply.all_apn_conf_ind() == location::Reply_all_apn_conf_ind_t_ALL_APN_MAX) ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind = ALL_APN_MAX;
		
				ula_pP->subscription_data.apn_config_profile.nb_apns = call->reply.nb_apns();
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].context_identifier = call->reply.apn_context_identifier();
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].nb_ip_address = call->reply.nb_ip_address();
		 		memset(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ip_address, 0, sizeof(ip_address_t));

				if (call->reply.pdn_type() == location::Reply_pdn_type_t_IPv4) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv4;
		  		else if (call->reply.pdn_type() == location::Reply_pdn_type_t_IPv6) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv6;
		  		else if (call->reply.pdn_type() == location::Reply_pdn_type_t_IPv4_AND_v6) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv4_AND_v6;
		  		else if (call->reply.pdn_type() == location::Reply_pdn_type_t_IPv4_OR_v6) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv4_OR_v6;
		  		else if (call->reply.pdn_type() == location::Reply_pdn_type_t_IP_MAX) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IP_MAX;

				strcpy(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection, call->reply.service_selection().c_str());
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection_length = call->reply.service_selection_length();
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.qci = call->reply.qci();
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.priority_level = call->reply.priority_level();

				if (call->reply.pre_emp_vulnerability() == location::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_ENABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability = (pre_emption_vulnerability_t)PRE_EMPTION_VULNERABILITY_ENABLED;
				else if (call->reply.pre_emp_vulnerability() == location::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_DISABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability = (pre_emption_vulnerability_t)PRE_EMPTION_VULNERABILITY_DISABLED;
				else if (call->reply.pre_emp_vulnerability() == location::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_MAX) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability = (pre_emption_vulnerability_t)PRE_EMPTION_VULNERABILITY_MAX;


				if (call->reply.pre_emp_capability() == location::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_ENABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability = (pre_emption_capability_t)PRE_EMPTION_CAPABILITY_ENABLED;
				else if (call->reply.pre_emp_capability() == location::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_DISABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability = (pre_emption_capability_t)PRE_EMPTION_CAPABILITY_DISABLED;
				else if (call->reply.pre_emp_capability() == location::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_MAX) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability = (pre_emption_capability_t)PRE_EMPTION_CAPABILITY_MAX;	
	
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_ul = call->reply.ambr().br_ul();
				ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_dl = call->reply.ambr().br_dl();

				mme_app_handle_s6a_update_location_ans (ula_pP); 			
			}           		
 			else
                 		std::cout << "RPC failed" << std::endl;
			
            		delete call;   
			
			break;
		}
        }

    
private:
	std::unique_ptr<LocationUpdate::Stub> stub_;
	CompletionQueue cq_;


};

std::string service_table_nonsql(int pseudo_user_type, char user_imsi2[]){
  std::map<char *, int> user_table;
  std::map<int,service_ip_port> service_table;

  int user_type;

  service_table[1].service_ip="10.0.0.4";
  service_table[1].service_port=5000;
  service_table[2].service_ip="10.0.0.4";
  service_table[2].service_port=5001;


  std::map<char*, int>::iterator it;
  it = user_table.find(user_imsi2);
  
  if( it == user_table.end() ){
    user_table[user_imsi2] = pseudo_user_type;
    user_type = pseudo_user_type;
  }
  else	user_type = it->second;
  
  std::map<int, service_ip_port>::iterator it2;
  it2 = service_table.find(user_type);

  char service_ipadd_port[30];
  sprintf(service_ipadd_port, "%s:%d", it2->second.service_ip, it2->second.service_port);

  return std::string(service_ipadd_port);

}


//------------------------------------------------------------------------------
void *mme_app_thread (void *args)
{
  itti_mark_task_ready (TASK_MME_APP);

  while (1) {
    MessageDef                             *received_message_p = NULL;

    /*
     * Trying to fetch a message from the message queue.
     * If the queue is empty, this function will block till a
     * message is sent to the task.
     */
    itti_receive_msg (TASK_MME_APP, &received_message_p);
    DevAssert (received_message_p );

    switch (ITTI_MSG_ID (received_message_p)) {

    case MESSAGE_TEST:{
        OAI_FPRINTF_INFO("TASK_MME_APP received MESSAGE_TEST\n");
      }
      break;

    case MME_APP_INITIAL_CONTEXT_SETUP_RSP:{
        mme_app_handle_initial_context_setup_rsp (&MME_APP_INITIAL_CONTEXT_SETUP_RSP (received_message_p));
      }
      break;

    case MME_APP_CREATE_DEDICATED_BEARER_RSP:{
      mme_app_handle_create_dedicated_bearer_rsp (&MME_APP_CREATE_DEDICATED_BEARER_RSP (received_message_p));
    }
    break;

    case MME_APP_CREATE_DEDICATED_BEARER_REJ:{
      mme_app_handle_create_dedicated_bearer_rej (&MME_APP_CREATE_DEDICATED_BEARER_REJ (received_message_p));
    }
    break;

    case NAS_CONNECTION_ESTABLISHMENT_CNF:{
        mme_app_handle_conn_est_cnf (&NAS_CONNECTION_ESTABLISHMENT_CNF (received_message_p));
      }
      break;

    case NAS_DETACH_REQ: {
        mme_app_handle_detach_req(&received_message_p->ittiMsg.nas_detach_req);
      }
      break;

    case NAS_DOWNLINK_DATA_REQ: {
        mme_app_handle_nas_dl_req (&received_message_p->ittiMsg.nas_dl_data_req);
      }
      break;

    case NAS_ERAB_SETUP_REQ:{
      mme_app_handle_erab_setup_req (&NAS_ERAB_SETUP_REQ (received_message_p));
    }
    break;

    case NAS_PDN_CONFIG_REQ: {
        ue_mm_context_t                    *ue_context_p = NULL;
        ue_context_p = mme_ue_context_exists_mme_ue_s1ap_id (&mme_app_desc.mme_ue_contexts, received_message_p->ittiMsg.nas_pdn_config_req.ue_id);
        if (ue_context_p) {
          if (!ue_context_p->is_s1_ue_context_release) {
		//GUI
		std::string a = exec("../identifier");

		//GRPC
		char user_imsi2[16];
		IMSI_TO_STRING((&ue_context_p->emm_context._imsi), user_imsi2, 16);
		std::string service_ip_port;

		service_ip_port = service_table_nonsql(std::stoi(a), user_imsi2);
		auto channel = CreateChannel(service_ip_port.c_str(), grpc::InsecureChannelCredentials());
  		MME_Client client(channel);
  		typedef void *(*THREADFUNCPTR)(void *);
		pthread_t t;
		pthread_create(&t, NULL, (THREADFUNCPTR)&MME_Client::AsyncCompleteRpc, &client);
	
		client.Send_request(ue_context_p, user_imsi2);
		
		std::cout << "client call FINISH" << std::endl;
          }
        }
      }
      break;

    case NAS_PDN_CONNECTIVITY_REQ:{
        mme_app_handle_nas_pdn_connectivity_req (&received_message_p->ittiMsg.nas_pdn_connectivity_req);
      }
      break;

    case S11_CREATE_BEARER_REQUEST:
      mme_app_handle_s11_create_bearer_req (&received_message_p->ittiMsg.s11_create_bearer_request);
      break;

    case S11_CREATE_SESSION_RESPONSE:{
        mme_app_handle_create_sess_resp (&received_message_p->ittiMsg.s11_create_session_response);
      }
      break;

    case S11_DELETE_SESSION_RESPONSE: {
      mme_app_handle_delete_session_rsp (&received_message_p->ittiMsg.s11_delete_session_response);
      }
      break;

    case S11_MODIFY_BEARER_RESPONSE:{
        ue_mm_context_t                    *ue_context_p = NULL;
        ue_context_p = mme_ue_context_exists_s11_teid (&mme_app_desc.mme_ue_contexts, received_message_p->ittiMsg.s11_modify_bearer_response.teid);

        if (ue_context_p == NULL) {
          MSC_LOG_RX_DISCARDED_MESSAGE (MSC_MMEAPP_MME, MSC_S11_MME, NULL, 0, "0 MODIFY_BEARER_RESPONSE local S11 teid " TEID_FMT " ",
            received_message_p->ittiMsg.s11_modify_bearer_response.teid);
          OAILOG_WARNING (LOG_MME_APP, "We didn't find this teid in list of UE: %08x\n", received_message_p->ittiMsg.s11_modify_bearer_response.teid);
        } else {
          MSC_LOG_RX_MESSAGE (MSC_MMEAPP_MME, MSC_S11_MME, NULL, 0, "0 MODIFY_BEARER_RESPONSE local S11 teid " TEID_FMT " IMSI " IMSI_64_FMT " ",
            received_message_p->ittiMsg.s11_modify_bearer_response.teid, ue_context_p->emm_context._imsi64);
        }

      }
      break;

    case S11_RELEASE_ACCESS_BEARERS_RESPONSE:{
        mme_app_handle_release_access_bearers_resp (&received_message_p->ittiMsg.s11_release_access_bearers_response);
      }
      break;


    case S1AP_E_RAB_SETUP_RSP:{
        mme_app_handle_e_rab_setup_rsp (&S1AP_E_RAB_SETUP_RSP (received_message_p));
      }
      break;

    case S1AP_ENB_DEREGISTERED_IND: {
        mme_app_handle_s1ap_enb_deregistered_ind (&received_message_p->ittiMsg.s1ap_eNB_deregistered_ind);
      }
      break;

    case S1AP_INITIAL_UE_MESSAGE:{
        mme_app_handle_initial_ue_message (&S1AP_INITIAL_UE_MESSAGE (received_message_p));
      }
      break;

    case S1AP_UE_CAPABILITIES_IND:{
        mme_app_handle_s1ap_ue_capabilities_ind (&received_message_p->ittiMsg.s1ap_ue_cap_ind);
      }
      break;

    case S1AP_UE_CONTEXT_RELEASE_COMPLETE:{
        mme_app_handle_s1ap_ue_context_release_complete (&received_message_p->ittiMsg.s1ap_ue_context_release_complete);
      }
      break;

    case S1AP_UE_CONTEXT_RELEASE_REQ:{
        mme_app_handle_s1ap_ue_context_release_req (&received_message_p->ittiMsg.s1ap_ue_context_release_req);
      }
      break;

    case S6A_UPDATE_LOCATION_ANS:{
        /*
         * We received the update location answer message from HSS -> Handle it
         */
        mme_app_handle_s6a_update_location_ans (&received_message_p->ittiMsg.s6a_update_location_ans);
      }
      break;

    case TERMINATE_MESSAGE:{
        /*
         * Termination message received TODO -> release any data allocated
         */
        mme_app_exit();
        itti_free_msg_content(received_message_p);
        itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
        OAI_FPRINTF_INFO("TASK_MME_APP terminated\n");
        itti_exit_task ();
      }
      break;

    case TIMER_HAS_EXPIRED:{
        /*
         * Check if it is the statistic timer
         */
        if (received_message_p->ittiMsg.timer_has_expired.timer_id == mme_app_desc.statistic_timer_id) {
          mme_app_statistics_display ();
        }
      }
      break;

    default:{
      OAILOG_DEBUG (LOG_MME_APP, "Unkwnon message ID %d:%s\n", ITTI_MSG_ID (received_message_p), ITTI_MSG_NAME (received_message_p));
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

//------------------------------------------------------------------------------
int mme_app_init (const mme_config_t * mme_config_p)
{
  // parse mme_app configuration file
  mme_app_config_parse(&mme_app_config);


  OAILOG_FUNC_IN (LOG_MME_APP);
  memset (&mme_app_desc, 0, sizeof (mme_app_desc));
  bstring b = bfromcstr("mme_app_imsi_ue_context_htbl");
  mme_app_desc.mme_ue_contexts.imsi_ue_context_htbl = hashtable_ts_create (mme_config.max_ues, NULL, hash_free_int_func, b);
  btrunc(b, 0);
  bassigncstr(b, "mme_app_tun11_ue_context_htbl");
  mme_app_desc.mme_ue_contexts.tun11_ue_context_htbl = hashtable_ts_create (mme_config.max_ues, NULL, hash_free_int_func, b);
  AssertFatal(sizeof(uintptr_t) >= sizeof(uint64_t), "Problem with mme_ue_s1ap_id_ue_context_htbl in MME_APP");
  btrunc(b, 0);
  bassigncstr(b, "mme_app_mme_ue_s1ap_id_ue_context_htbl");
  mme_app_desc.mme_ue_contexts.mme_ue_s1ap_id_ue_context_htbl = hashtable_ts_create (mme_config.max_ues, NULL, hash_free_int_func, b);
  btrunc(b, 0);
  bassigncstr(b, "mme_app_enb_ue_s1ap_id_ue_context_htbl");
  mme_app_desc.mme_ue_contexts.enb_ue_s1ap_id_ue_context_htbl = hashtable_ts_create (mme_config.max_ues, NULL, NULL, b);
  btrunc(b, 0);
  bassigncstr(b, "mme_app_guti_ue_context_htbl");
  mme_app_desc.mme_ue_contexts.guti_ue_context_htbl = obj_hashtable_ts_create (mme_config.max_ues, NULL, hash_free_int_func, hash_free_int_func, b);
  bdestroy_wrapper (&b);

  if (mme_app_edns_init(mme_config_p)) {
    OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
  }
  /*
   * Create the thread associated with MME applicative layer
   */
  if (itti_create_task (TASK_MME_APP, &mme_app_thread, NULL) < 0) {
    OAILOG_ERROR (LOG_MME_APP, "MME APP create task failed\n");
    OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
  }

  mme_app_desc.statistic_timer_period = mme_config_p->mme_statistic_timer;

  /*
   * Request for periodic timer
   */
  if (timer_setup (mme_config_p->mme_statistic_timer, 0, TASK_MME_APP, INSTANCE_DEFAULT, TIMER_PERIODIC, NULL, &mme_app_desc.statistic_timer_id) < 0) {
    OAILOG_ERROR (LOG_MME_APP, "Failed to request new timer for statistics with %ds " "of periocidity\n", mme_config_p->mme_statistic_timer);
    mme_app_desc.statistic_timer_id = 0;
  }

  OAILOG_DEBUG (LOG_MME_APP, "**************************************\n");
  OAILOG_DEBUG (LOG_MME_APP, "******Initializing MME_APP layer******\n");
  OAILOG_DEBUG (LOG_MME_APP, "**************************************\n");
  OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNok);
}

//------------------------------------------------------------------------------
void mme_app_exit (void)
{
  mme_app_edns_exit();
  XML_MSG_DUMP_EXIT();
  hashtable_ts_destroy (mme_app_desc.mme_ue_contexts.imsi_ue_context_htbl);
  hashtable_ts_destroy (mme_app_desc.mme_ue_contexts.tun11_ue_context_htbl);
  hashtable_ts_destroy (mme_app_desc.mme_ue_contexts.mme_ue_s1ap_id_ue_context_htbl);
  hashtable_ts_destroy (mme_app_desc.mme_ue_contexts.enb_ue_s1ap_id_ue_context_htbl);
  obj_hashtable_ts_destroy (mme_app_desc.mme_ue_contexts.guti_ue_context_htbl);
  mme_config_exit();
}
