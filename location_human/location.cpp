#include "module.hpp"
#include "location.hpp"

#include <string.h>
#include "s6a_up_loc.hpp"

using grpc::Status;
using location::LocationUpdate;
using location::Request;
using location::Reply;

int finish = 0;
Reply* reply_;

class Human_service final : public LocationUpdate::Service {

public:
	Status Location_update(grpc::ServerContext* context, const Request* request,
                  Reply* reply) override {

		std::cout << "***********************************************************************************" << std::endl;
		std::cout << "***********************Receive Human Location Update Request***********************" << std::endl;
		std::cout << "***********************************************************************************" << std::endl;
		send_s6a_update_location_req(request);	//location update request	
		
		while (!finish);
		finish = 0;
		memcpy( reply, reply_, sizeof(Reply));		
		
  		std::cout << "Send Human Location Update Reply"  << std::endl;
		return Status::OK;
        }

private:


};


class location_update : public module {
  public:

    virtual int init() {
        return location_update_init ();
    }

};
/* the class factories */
extern "C" module* create() {
    return new location_update;
}
extern "C" void destroy(module* p) {
    delete p;
}

int main(){
	class location_update a;
	a.init();

}

void location_update_server ()
{
  std::string addr = "0.0.0.0:5000";
  Human_service service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  auto server = builder.BuildAndStart();
  std::cout << "Server listening on " << addr << std::endl;
  server->Wait(); 

}

//------------------------------------------------------------------------------
int location_update_init ()
{

  class s6a_cxx s6a_obj; 

  s6a_obj.s6a_init();
  std::cout << "************************S6A INIT FINISH************************" << std::endl;
  location_update_server();

  printf("Initializing LOCATION UPDATE task interface: DONE!!\n");
  return 0;

}

//------------------------------------------------------------------------------
int send_s6a_update_location_req (
  const Request* ue_mm_context)
{
 
  s6a_update_location_req_t              *s6a_ulr_p = new s6a_update_location_req_t;
  int                                     rc = RETURNok;

  strcpy (s6a_ulr_p->imsi, ue_mm_context->imsi().c_str());
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
  
  s6a_up_loc::s6a_generate_update_location(s6a_ulr_p);

}
//------------------------------------------------------------------------------

int handle_s6a_update_location_ans (
  const s6a_update_location_ans_t * const ula_pP)
{
  std::cout << "Handle Human Location Update Request" << std::endl;
  reply_ = new Reply;

  reply_->set_present(ula_pP->result.present);
  if (ula_pP->result.choice.base == DIAMETER_SUCCESS) reply_->set_base(location::Reply_Base_DIAMETER_SUCCESS);

  reply_->set_imsi(ula_pP->imsi);
  
  if (ula_pP->subscription_data.subscriber_status == 0) reply_->set_subscriber_status(location::Reply_subscriber_status_t_SS_SERVICE_GRANTED);
  else if (ula_pP->subscription_data.subscriber_status == 1) reply_->set_subscriber_status(location::Reply_subscriber_status_t_SS_OPERATOR_DETERMINED_BARRING);
  else if (ula_pP->subscription_data.subscriber_status == 2) reply_->set_subscriber_status(location::Reply_subscriber_status_t_SS_MAX);

  reply_->set_access_restriction(ula_pP->subscription_data.access_restriction);
  reply_->mutable_subscribed_ambr()->set_br_ul(ula_pP->subscription_data.subscribed_ambr.br_ul);
  reply_->mutable_subscribed_ambr()->set_br_dl(ula_pP->subscription_data.subscribed_ambr.br_dl);
  reply_->set_msisdn(ula_pP->subscription_data.msisdn);
  reply_->set_msisdn_length((uint32_t)ula_pP->subscription_data.msisdn_length);
  reply_->set_rau_tau_timer(ula_pP->subscription_data.rau_tau_timer);
  
  if (ula_pP->subscription_data.access_mode == 0)  reply_->set_access_mode(location::Reply_access_mode_t_NAM_PACKET_AND_CIRCUIT);
  else if (ula_pP->subscription_data.access_mode == 1) reply_->set_access_mode(location::Reply_access_mode_t_NAM_RESERVED);
  else if (ula_pP->subscription_data.access_mode == 2) reply_->set_access_mode(location::Reply_access_mode_t_NAM_ONLY_PACKET);
  else if (ula_pP->subscription_data.access_mode == 3) reply_->set_access_mode(location::Reply_access_mode_t_NAM_MAX);
  
  reply_->set_context_identifier(ula_pP->subscription_data.apn_config_profile.context_identifier);

  if (ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind == 0) reply_->set_all_apn_conf_ind(location::Reply_all_apn_conf_ind_t_ALL_APN_CONFIGURATIONS_INCLUDED);
  else if (ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind == 1) reply_->set_all_apn_conf_ind(location::Reply_all_apn_conf_ind_t_MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED);
  else if (ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind == 2) reply_->set_all_apn_conf_ind(location::Reply_all_apn_conf_ind_t_ALL_APN_MAX);
  
  reply_->set_nb_apns((uint32_t)ula_pP->subscription_data.apn_config_profile.nb_apns);
  reply_->set_apn_context_identifier(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].context_identifier);
  reply_->set_nb_ip_address((uint32_t)ula_pP->subscription_data.apn_config_profile.apn_configuration[0].nb_ip_address);

  if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 0) reply_->set_pdn_type(location::Reply_pdn_type_t_IPv4);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 1) reply_->set_pdn_type(location::Reply_pdn_type_t_IPv6);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 2) reply_->set_pdn_type(location::Reply_pdn_type_t_IPv4_AND_v6);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 3) reply_->set_pdn_type(location::Reply_pdn_type_t_IPv4_OR_v6);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type == 4) reply_->set_pdn_type(location::Reply_pdn_type_t_IP_MAX);

  reply_->set_service_selection(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection);
  reply_->set_service_selection_length(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection_length);
  reply_->set_qci(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.qci);
  reply_->set_priority_level(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.priority_level); 
  
  if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == 0) reply_->set_pre_emp_vulnerability(location::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_ENABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == 1) reply_->set_pre_emp_vulnerability(location::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_DISABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == 2) reply_->set_pre_emp_vulnerability(location::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_MAX);

  if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability == 0) reply_->set_pre_emp_capability(location::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_ENABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability == 1) reply_->set_pre_emp_capability(location::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_DISABLED);
  else if (ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability == 2) reply_->set_pre_emp_capability(location::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_MAX);
  
  reply_->mutable_ambr()->set_br_ul(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_ul);
  reply_->mutable_ambr()->set_br_dl(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_dl);

  finish = 1;

}

