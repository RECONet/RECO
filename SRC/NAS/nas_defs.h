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

#include "nas_network.h"
#include "nas_proc.h"

#ifndef FILE_NAS_DEFS_SEEN
#define FILE_NAS_DEFS_SEEN

class nas_cxx {
  public:
    int nas_init(mme_config_t *mme_config_p);
    void nas_exit(void);

    /* nas_network */
    void nas_network_initialize(mme_config_t *mme_config_p, void *args_p);
    void nas_network_cleanup(void *args_p);

    /* nas_procedure */
    void nas_proc_initialize(mme_config_t *mme_config_p);
    void nas_proc_cleanup(void);
    int nas_proc_establish_ind( const enb_s1ap_id_key_t enb_ue_s1ap_id_key,
                            const mme_ue_s1ap_id_t ue_id,
                            const tai_t originating_tai,
                            const ecgi_t ecgi,
                            const as_cause_t as_cause,
                            const s_tmsi_t s_tmsi,
                            STOLEN_REF bstring *msg);

    int nas_proc_dl_transfer_cnf(const mme_ue_s1ap_id_t ueid, const nas_error_code_t status);
    int nas_proc_dl_transfer_rej(const mme_ue_s1ap_id_t ueid, const nas_error_code_t status);
    int nas_proc_ul_transfer_ind(const mme_ue_s1ap_id_t ueid,
                             const tai_t originating_tai,
                             const ecgi_t cgi,
                             STOLEN_REF bstring *msg);
    int nas_proc_authentication_info_answer (s6a_auth_info_ans_t * ans);
    int nas_proc_auth_param_res (mme_ue_s1ap_id_t ue_id, uint8_t nb_vectors, eutran_vector_t *vectors);
    int nas_proc_auth_param_fail(mme_ue_s1ap_id_t ue_id, nas_cause_t cause);
    int nas_proc_deregister_ue(uint32_t ue_id);
    int nas_proc_pdn_config_res (emm_cn_pdn_config_res_t * emm_cn_pdn_config_res);
    int nas_proc_pdn_connectivity_res(emm_cn_pdn_res_t *nas_pdn_connectivity_rsp);
    int nas_proc_pdn_connectivity_fail(emm_cn_pdn_fail_t *nas_pdn_connectivity_fail);
    int nas_proc_create_dedicated_bearer(emm_cn_activate_dedicated_bearer_req_t * emm_cn_activate);
    int nas_proc_signalling_connection_rel_ind (mme_ue_s1ap_id_t ue_id);
    nas_cause_t s6a_error_2_nas_cause (uint32_t s6a_error,int experimental);

  private:
    nas_network nas_network_obj;
    nas_procedure nas_procedure_obj;

};

#endif /* FILE_NAS_DEFS_SEEN */
