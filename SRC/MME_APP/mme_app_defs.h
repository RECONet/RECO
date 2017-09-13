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

/*! \file mme_app_defs.h
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

/* This file contains definitions related to mme applicative layer and should
 * not be included within other layers.
 * Use mme_app_extern.h to expose mme applicative layer procedures/data.
 */


#ifndef FILE_MME_APP_DEFS_SEEN
#define FILE_MME_APP_DEFS_SEEN
#include "intertask_interface.h"
#include "mme_app_ue_context.hpp"

class mme_app_desc_t {
 public:
  ue_mm_context_t* GetUeContextByImsi(const imsi64_t imsi) {
    return mme_ue_contexts.GetUeContextByImsi(imsi);
  }
  ue_mm_context_t* GetUeContextByS11Teid(const s11_teid_t s11_teid) {
    return mme_ue_contexts.GetUeContextByS11Teid(s11_teid);
  }
  ue_mm_context_t* GetUeContextByMmeUeS1apId(const mme_ue_s1ap_id_t mme_ue_s1ap_id) {
    return mme_ue_contexts.GetUeContextByMmeUeS1apId(mme_ue_s1ap_id);
  }
  ue_mm_context_t* GetUeContextByEnbUeS1apId(const enb_s1ap_id_key_t enb_key){
    return mme_ue_contexts.GetUeContextByEnbUeS1apId(enb_key);
  }
  ue_mm_context_t* GetUeContextByGuti(const guti_t * const guti) {
    return mme_ue_contexts.GetUeContextByGuti(guti);
  }
  void UpdateCollKeys(ue_mm_context_t     * const ue_context_p,
                    const enb_s1ap_id_key_t  enb_s1ap_id_key,
                    const mme_ue_s1ap_id_t   mme_ue_s1ap_id,
                    const imsi64_t     imsi,
                    const s11_teid_t         mme_s11_teid,
                    const guti_t     * const guti_p) {
    mme_ue_contexts.UpdateCollKeys(
                ue_context_p, enb_s1ap_id_key,
                mme_ue_s1ap_id, imsi,
                mme_s11_teid, guti_p
                );
  }
  void DumpCollKeys() {
    mme_ue_contexts.DumpCollKeys();
  }
  int InsertUeContext(const ue_mm_context_t * const ue_context_p) {
    mme_ue_contexts.InsertUeContext(ue_context_p);
  }
  void RemoveUeContext(ue_mm_context_t * const ue_context_p) {
    mme_ue_contexts.RemoveUeContext(ue_context_p);
  }
  void NotifyUeContextReleased(ue_mm_context_t *ue_context_p) {
    mme_ue_contexts.NotifyUeContextReleased(ue_context_p);
  }
  void DumpUeContexts() {mme_ue_contexts.DumpUeContexts();}

  /* UE contexts + some statistics variables */
  MmeUeContextManager mme_ue_contexts;

  long statistic_timer_id;
  uint32_t statistic_timer_period;
};

extern mme_app_desc_t mme_app_desc;

void mme_app_handle_s1ap_enb_deregistered_ind (const itti_s1ap_eNB_deregistered_ind_t * const enb_dereg_ind);

int mme_app_handle_s1ap_ue_capabilities_ind  (const itti_s1ap_ue_cap_ind_t * const s1ap_ue_cap_ind_pP);

void mme_app_handle_s1ap_ue_context_release_complete (const itti_s1ap_ue_context_release_complete_t * const s1ap_ue_context_release_complete);

void mme_app_send_s1ap_ue_context_release_command (ue_mm_context_t *ue_context, S1ap_Cause_t cause);

int mme_app_send_s11_create_session_req (ue_mm_context_t *const ue_mm_context, const pdn_cid_t pdn_cid);

int mme_app_send_s6a_update_location_req     (ue_mm_context_t * const ue_context_pP);

int mme_app_handle_s6a_update_location_ans   (const s6a_update_location_ans_t * const ula_pP);

int mme_app_handle_nas_pdn_connectivity_req  ( itti_nas_pdn_connectivity_req_t * const nas_pdn_connectivity_req_p);

void mme_app_handle_detach_req (const itti_nas_detach_req_t * const detach_req_p);

void mme_app_handle_conn_est_cnf             (itti_nas_conn_est_cnf_t * const nas_conn_est_cnf_pP);

void mme_app_handle_initial_ue_message       (itti_s1ap_initial_ue_message_t * const conn_est_ind_pP);

int mme_app_handle_create_sess_resp          (itti_s11_create_session_response_t * const create_sess_resp_pP); //not const because we need to free internal stucts

void mme_app_handle_erab_setup_req (itti_erab_setup_req_t * const itti_erab_setup_req);

void mme_app_handle_delete_session_rsp	     (const itti_s11_delete_session_response_t * const delete_sess_respP);

int mme_app_handle_establish_ind             (const nas_establish_ind_t * const nas_establish_ind_pP);

int mme_app_handle_authentication_info_answer(const s6a_auth_info_ans_t * const s6a_auth_info_ans_pP);

void  mme_app_handle_release_access_bearers_resp (const itti_s11_release_access_bearers_response_t * const rel_access_bearers_rsp_pP);

void mme_app_handle_s11_create_bearer_req        (const itti_s11_create_bearer_request_t * const create_bearer_request_pP);

void mme_app_bearer_create_workflow (ue_mm_context_t * const ue_context, const pdn_cid_t cid);

//nas_cause_t s6a_error_2_nas_cause            (const uint32_t s6a_errorP, const int experimentalP);

void mme_app_handle_nas_auth_param_req       (const itti_nas_auth_param_req_t * const nas_auth_param_req_pP);

void mme_app_handle_initial_context_setup_rsp(itti_mme_app_initial_context_setup_rsp_t * const initial_ctxt_setup_rsp_pP);

bool mme_app_dump_ue_context (const hash_key_t keyP, void *const ue_context_pP, void *unused_param_pP, void **unused_result_pP);

int mme_app_handle_nas_dl_req ( itti_nas_dl_data_req_t *const nas_dl_req_pP);

void mme_app_handle_e_rab_setup_rsp (itti_s1ap_e_rab_setup_rsp_t  * const e_rab_setup_rsp);

void mme_app_handle_create_dedicated_bearer_rsp (itti_mme_app_create_dedicated_bearer_rsp_t   * const create_dedicated_bearer_rsp);

void mme_app_handle_create_dedicated_bearer_rej (itti_mme_app_create_dedicated_bearer_rej_t   * const create_dedicated_bearer_rej);

void mme_app_trigger_mme_initiated_dedicated_bearer_deactivation_procedure (ue_mm_context_t * const ue_context, const pdn_cid_t cid);


#endif /* MME_APP_DEFS_H_ */
