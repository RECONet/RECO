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

#include "test.grpc.pb.h"
#include "conversions.h"


#include <mysql/mysql.h>
#include <map>


#include "test.grpc.pb.h"
#include "conversions.h"
#include <thread>
#include "call_gui.h"
#include <unistd.h>
#include <pthread.h>

using grpc::Status;
using test::Test;
using test::Request;
using test::Reply;

// mme_app_desc_t                          mme_app_desc;

extern xmlTextWriterPtr g_xml_text_writer;


void     *mme_app_thread (void *args);


class TestClient {
public:
        TestClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(Test::NewStub(channel))  { }
        Status Send_request(ue_mm_context_t *const ue_mm_context) {

                char imsi[16];
                Request a;
                IMSI_TO_STRING((&ue_mm_context->emm_context._imsi), imsi, 16);
                a.set_imsi(imsi);
                a.set_mcc_digit1((uint32_t)(ue_mm_context->emm_context.originating_tai.mcc_digit1));
                a.set_mcc_digit2((uint32_t)(ue_mm_context->emm_context.originating_tai.mcc_digit2));
                a.set_mcc_digit3((uint32_t)(ue_mm_context->emm_context.originating_tai.mcc_digit3));
                a.set_mnc_digit1((uint32_t)(ue_mm_context->emm_context.originating_tai.mnc_digit1));
                a.set_mnc_digit2((uint32_t)(ue_mm_context->emm_context.originating_tai.mnc_digit2));
                a.set_mnc_digit3((uint32_t)(ue_mm_context->emm_context.originating_tai.mnc_digit3));

                grpc::ClientContext context;
                Reply b;
                stub_->Location_update(&context, a, &b);
                std::cout << "Call function finished." << std::endl;

                s6a_update_location_ans_t * ula_pP = new s6a_update_location_ans_t;
                ula_pP->result.present = b.present();
                if (b.base() == test::Reply_Base_DIAMETER_SUCCESS) ula_pP->result.choice.base = DIAMETER_SUCCESS;
                strcpy(ula_pP->imsi, b.imsi().c_str());

                if (b.subscriber_status() == test::Reply_subscriber_status_t_SS_SERVICE_GRANTED) ula_pP->subscription_data.subscriber_status = SS_SERVICE_GRANTED;
                else if (b.subscriber_status() == test::Reply_subscriber_status_t_SS_OPERATOR_DETERMINED_BARRING) ula_pP->subscription_data.subscriber_status = SS_OPERATOR_DETERMINED_BARRING;
                else if (b.subscriber_status() == test::Reply_subscriber_status_t_SS_MAX) ula_pP->subscription_data.subscriber_status = SS_MAX;

                ula_pP->subscription_data.access_restriction = b.access_restriction();
                ula_pP->subscription_data.subscribed_ambr.br_ul = b.subscribed_ambr().br_ul();
                ula_pP->subscription_data.subscribed_ambr.br_dl = b.subscribed_ambr().br_dl();
                strcpy(ula_pP->subscription_data.msisdn, b.msisdn().c_str());
                ula_pP->subscription_data.msisdn_length = b.msisdn_length();
                ula_pP->subscription_data.rau_tau_timer = b.rau_tau_timer();

                if (b.access_mode() == test::Reply_access_mode_t_NAM_PACKET_AND_CIRCUIT) ula_pP->subscription_data.access_mode = NAM_PACKET_AND_CIRCUIT;
                else if (b.access_mode() == test::Reply_access_mode_t_NAM_RESERVED) ula_pP->subscription_data.access_mode = NAM_RESERVED;
                else if (b.access_mode() == test::Reply_access_mode_t_NAM_ONLY_PACKET) ula_pP->subscription_data.access_mode = NAM_ONLY_PACKET;
                else if (b.access_mode() == test::Reply_access_mode_t_NAM_MAX) ula_pP->subscription_data.access_mode = NAM_MAX;

                ula_pP->subscription_data.apn_config_profile.context_identifier = b.context_identifier();

                if (b.all_apn_conf_ind() == test::Reply_all_apn_conf_ind_t_ALL_APN_CONFIGURATIONS_INCLUDED) ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind = ALL_APN_CONFIGURATIONS_INCLUDED;
                else if (b.all_apn_conf_ind() == test::Reply_all_apn_conf_ind_t_MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED) ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind = MODIFIED_ADDED_APN_CONFIGURATIONS_INCLUDED;
                else if (b.all_apn_conf_ind() == test::Reply_all_apn_conf_ind_t_ALL_APN_MAX) ula_pP->subscription_data.apn_config_profile.all_apn_conf_ind = ALL_APN_MAX;

                ula_pP->subscription_data.apn_config_profile.nb_apns = b.nb_apns();
                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].context_identifier = b.apn_context_identifier();
                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].nb_ip_address = b.nb_ip_address();
                memset(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ip_address, 0, sizeof(ip_address_t));

                if (b.pdn_type() == test::Reply_pdn_type_t_IPv4) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv4;
              else if (b.pdn_type() == test::Reply_pdn_type_t_IPv6) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv6;
                else if (b.pdn_type() == test::Reply_pdn_type_t_IPv4_AND_v6) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv4_AND_v6;
                else if (b.pdn_type() == test::Reply_pdn_type_t_IPv4_OR_v6) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IPv4_OR_v6;
                else if (b.pdn_type() == test::Reply_pdn_type_t_IP_MAX) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].pdn_type = IP_MAX;

                strcpy(ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection, b.service_selection().c_str());
                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].service_selection_length = b.service_selection_length();
                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.qci = b.qci();
                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.priority_level = b.priority_level();

                if (b.pre_emp_vulnerability() == test::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_ENABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability = (pre_emption_vulnerability_t)PRE_EMPTION_VULNERABILITY_ENABLED;
                else if (b.pre_emp_vulnerability() == test::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_DISABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability = (pre_emption_vulnerability_t)PRE_EMPTION_VULNERABILITY_DISABLED;
                else if (b.pre_emp_vulnerability() == test::Reply_pre_emption_vulnerability_t__PRE_EMPTION_VULNERABILITY_MAX) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_vulnerability = (pre_emption_vulnerability_t)PRE_EMPTION_VULNERABILITY_MAX;


                if (b.pre_emp_capability() == test::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_ENABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability = (pre_emption_capability_t)PRE_EMPTION_CAPABILITY_ENABLED;
                else if (b.pre_emp_capability() == test::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_DISABLED) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability = (pre_emption_capability_t)PRE_EMPTION_CAPABILITY_DISABLED;
                else if (b.pre_emp_capability() == test::Reply_pre_emption_capability_t__PRE_EMPTION_CAPABILITY_MAX) ula_pP->subscription_data.apn_config_profile.apn_configuration[0].subscribed_qos.allocation_retention_priority.pre_emp_capability = (pre_emption_capability_t)PRE_EMPTION_CAPABILITY_MAX;

                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_ul = b.ambr().br_ul();
                ula_pP->subscription_data.apn_config_profile.apn_configuration[0].ambr.br_dl = b.ambr().br_dl();

                mme_app_handle_s6a_update_location_ans (ula_pP);

                return Status::OK;
        }
private:
        std::unique_ptr<Test::Stub> stub_;

};

struct service_ip_port {
  char *service_ip;
  int service_port;
};

void service_table_nonsql(int pseudo_user_type, ue_mm_context_t *ue_context_p){
  std::map<char *, int> user_table;
  std::map<int,service_ip_port> service_table;

  char user_imsi[16]="12345678901234";
  char user_imsi2[16]="123";
  int user_type;
  IMSI_TO_STRING((&ue_context_p->emm_context._imsi), user_imsi2, 16);

  user_table[user_imsi]=1;
  service_table[1].service_ip="10.0.0.4";
  service_table[1].service_port=5000;
  service_table[2].service_ip="10.0.0.4";
  service_table[2].service_port=5001;


  if(user_table.empty())
    std::cout << "empty\n";


  std::map<char*, int>::iterator it;
  it = user_table.find(user_imsi2);
  if( it == user_table.end() ){
    std::cout << "New user\n";
    user_table[user_imsi2]=pseudo_user_type;
    user_type = pseudo_user_type;
  }else{
    std::cout << "user: " << it->first << ", user_type: " << it->second << std::endl;
    user_type = it->second;
  }
  //print user_table
  std::cout << "=========user_table==========\n";
  for(it = user_table.begin(); it != user_table.end(); it++){
    std::cout << it->first << " => " << it->second << '\n';
 }
  std::cout << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n";
  std::map<int, service_ip_port>::iterator it2;
  it2 = service_table.find(user_type);
  std::cout << it2->first << " => " << it2->second.service_ip <<  " => " << it2->second.service_port <<  '\n';
/*
  //print service_table
  cout << "=========service_table==========\n";
  for(it2 = service_table.begin(); it2 != service_table.end(); it2++){
    cout << it2->first << " => " << it2->second.service_ip <<  " => " << it2->second.service_port <<  '\n';
  }
*/
  char service_ipadd_port[30];
  sprintf(service_ipadd_port, "%s:%d", it2->second.service_ip, it2->second.service_port);
  std::cout << "Target service location\n";
  std::cout << service_ipadd_port << std::endl;

  std::cout << "start create channel\n";
  auto channel = grpc::CreateChannel(service_ipadd_port, grpc::InsecureChannelCredentials());
  TestClient client(channel);

  client.Send_request(ue_context_p);



}



void service_table(ue_mm_context_t *ue_context_p){
  MYSQL mysql;
  MYSQL_RES *res;
  MYSQL_ROW row;
  char query[300];

  char user_imsi[16];
  IMSI_TO_STRING((&ue_context_p->emm_context._imsi), user_imsi, 16);

  //connecting
  mysql_init(&mysql);
  if (!mysql_real_connect(&mysql,"192.188.2.11", "root", "123", NULL, 3306, NULL,0)){
    printf( "Error connecting to database%s\n",mysql_error(&mysql));
  }else printf("Connected to mysql\n");

  //use database
  sprintf(query,"USE reco");
  if(mysql_real_query(&mysql,query,(unsigned int)strlen(query))) {
    printf("Use database failed\n");
  }else printf("Database changed\n");

  //inquery data
  sprintf(query,"SELECT imsi FROM test ORDER BY aaa");
  if( mysql_real_query(&mysql,query,(unsigned int)strlen(query)) ){
    printf("Inquery failed\n");
  }else printf("Inquery successfully\n");

  if( !( res=mysql_store_result(&mysql) ) ){
    printf("%s", mysql_error(&mysql));
  }
  while( (row=mysql_fetch_row(res)) ){
    for(int tt=1;tt<=mysql_num_fields(res);tt++){
      std:: cout << "| " << row[tt-1] << " |";
      //fprintf(fp,"| %s |",row[tt-1]);
    }
    std::cout << std::endl;
  }
  mysql_free_result(res);

  //find existed imsi

  //check existed imsi

  //char user_imsi[16];
  //cin >> user_imsi;
  sprintf(query,"SELECT * FROM user WHERE imsi = \"%s\"", user_imsi);
  std::cout << query << std::endl;
  if( mysql_real_query(&mysql,query,(unsigned int)strlen(query)) ){
    printf("Inquery failed\n");
  }else printf("Inquery successfully\n");

  if( !( res=mysql_store_result(&mysql) ) ){
    printf("%s", mysql_error(&mysql));
  }
  bool imsi_existed = true;
  if (mysql_fetch_row(res) == 0){
    imsi_existed = false;
  }
  mysql_free_result(res);

  if( imsi_existed == true ){
    std::cout << "IMSI existed\nIMSI existed\nIMSI existed\nIMSI existed\n";
  }else{
   //insert imsi into user table
    //TODO: input user_type
    //user_type: 0=human, 1=iot
    int user_type = 0;
    sprintf(query,"INSERT INTO user VALUES('%s', %d)", user_imsi, user_type);
    if( mysql_real_query(&mysql,query,(unsigned int)strlen(query)) ){
      printf("Insert imsi failed\n");
    }else printf("Insert imsi successfully\n");

    //create channel
    sprintf(query,"SELECT * FROM service where user_type = \"%d\"", user_type);
    if( mysql_real_query(&mysql,query,(unsigned int)strlen(query)) ){
      printf("Inquery failed\n");
    }else printf("Inquery successfully\n");

    if( !( res=mysql_store_result(&mysql) ) ){
      printf("%s", mysql_error(&mysql));
    }

    char service_ip_port[30];
    row = mysql_fetch_row(res);
    sprintf(service_ip_port, "%s:%s", row[1], row[2]);
    std::cout << service_ip_port << std::endl;
    mysql_free_result(res);

    std::cout << "FIRST TIME\n";
    auto channel = grpc::CreateChannel(service_ip_port, grpc::InsecureChannelCredentials());
    TestClient client(channel);

    std::cout << "SECOND TIME\n";
    auto channel2 = grpc::CreateChannel(service_ip_port, grpc::InsecureChannelCredentials());
    TestClient client2(channel2);

    client2.Send_request(ue_context_p);

  }

  mysql_close(&mysql);

}
//------------------------------------------------------------------------------
void *mme_app_thread (void *args)
{
  itti_mark_task_ready (TASK_MME_APP);

  //auto channel = grpc::CreateChannel("192.188.2.6:5000", grpc::InsecureChannelCredentials());
  //TestClient client(channel);

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
         	//gPRC
		std::cout << "Send request!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!." << std::endl;
		std::string a = exec("/home/wirelab/hello");
		if (a == "1") {
			std::cout << "Human" << std::endl;
			//client.Send_request(1,ue_context_p);
			service_table_nonsql(1, ue_context_p);

		}
		else if (a == "2") {
			std::cout << "IoT" << std::endl;
			service_table_nonsql(2, ue_context_p);
			//client2.Send_request(ue_context_p);		
		}
		std::cout << "client call FINISH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!." << std::endl;
		//mme_app_send_s6a_update_location_req(ue_context_p);
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
         // TO DO
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
