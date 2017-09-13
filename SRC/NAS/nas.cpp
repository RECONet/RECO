#include "module.hpp"
#include "nas_defs.h"

class nas : public module {
  public:
    //creating nas module thread
    virtual int init(mme_config_t * mme_config_p) {
        return nas_obj.nas_init (mme_config_p);
    }
  private:
    class nas_cxx nas_obj;
};
/* the class factories */
extern "C" module* create() {
    return new nas;
}
extern "C" void destroy(module* p) {
    delete p;
}

/* nas_network */
void nas_cxx::nas_network_initialize(mme_config_t *mme_config_p, void *args_p) {
  return nas_network_obj.nas_network_initialize(mme_config_p, args_p);
}

void nas_cxx::nas_network_cleanup(void *args_p) {
  return nas_network_obj.nas_network_cleanup(args_p);
}

/* nas_procedure */
void nas_cxx::nas_proc_initialize(mme_config_t *mme_config_p) {
  return nas_procedure_obj.nas_proc_initialize(mme_config_p);
}

void nas_cxx::nas_proc_cleanup(void) {
  return nas_procedure_obj.nas_proc_cleanup();
}

int nas_cxx::nas_proc_establish_ind( const enb_s1ap_id_key_t enb_ue_s1ap_id_key,
                    const mme_ue_s1ap_id_t ue_id,
                    const tai_t originating_tai,
                    const ecgi_t ecgi,
                    const as_cause_t as_cause,
                    const s_tmsi_t s_tmsi,
                    STOLEN_REF bstring *msg) {
  return nas_procedure_obj.nas_proc_establish_ind(enb_ue_s1ap_id_key, ue_id, originating_tai, ecgi, as_cause, s_tmsi, msg);
}

int nas_cxx::nas_proc_dl_transfer_cnf(const mme_ue_s1ap_id_t ueid, const nas_error_code_t status) {
  return nas_procedure_obj.nas_proc_dl_transfer_cnf(ueid, status);
}

int nas_cxx::nas_proc_dl_transfer_rej(const mme_ue_s1ap_id_t ueid, const nas_error_code_t status) {
  return nas_procedure_obj.nas_proc_dl_transfer_rej(ueid, status);
}

int nas_cxx::nas_proc_ul_transfer_ind(const mme_ue_s1ap_id_t ueid,
                     const tai_t originating_tai,
                     const ecgi_t cgi,
                     STOLEN_REF bstring *msg) {
  return nas_procedure_obj.nas_proc_ul_transfer_ind(ueid, originating_tai, cgi, msg);
}

int nas_cxx::nas_proc_authentication_info_answer (s6a_auth_info_ans_t * ans) {
  return nas_procedure_obj.nas_proc_authentication_info_answer (ans);
}

int nas_cxx::nas_proc_auth_param_res (mme_ue_s1ap_id_t ue_id, uint8_t nb_vectors, eutran_vector_t *vectors) {
  return nas_procedure_obj.nas_proc_auth_param_res (ue_id, nb_vectors, vectors);
}

int nas_cxx::nas_proc_auth_param_fail(mme_ue_s1ap_id_t ue_id, nas_cause_t cause) {
  return nas_procedure_obj.nas_proc_auth_param_fail(ue_id, cause);
}

int nas_cxx::nas_proc_deregister_ue(uint32_t ue_id) {
  return nas_procedure_obj.nas_proc_deregister_ue(ue_id);
}

int nas_cxx::nas_proc_pdn_config_res (emm_cn_pdn_config_res_t * emm_cn_pdn_config_res) {
  return nas_procedure_obj.nas_proc_pdn_config_res (emm_cn_pdn_config_res);
}

int nas_cxx::nas_proc_pdn_connectivity_res(emm_cn_pdn_res_t *nas_pdn_connectivity_rsp) {
  return nas_procedure_obj.nas_proc_pdn_connectivity_res(nas_pdn_connectivity_rsp);
}

int nas_cxx::nas_proc_pdn_connectivity_fail(emm_cn_pdn_fail_t *nas_pdn_connectivity_fail) {
  return nas_procedure_obj.nas_proc_pdn_connectivity_fail(nas_pdn_connectivity_fail);
}

int nas_cxx::nas_proc_create_dedicated_bearer(emm_cn_activate_dedicated_bearer_req_t * emm_cn_activate) {
  return nas_procedure_obj.nas_proc_create_dedicated_bearer(emm_cn_activate);
}

int nas_cxx::nas_proc_signalling_connection_rel_ind (mme_ue_s1ap_id_t ue_id) {
  return nas_procedure_obj.nas_proc_signalling_connection_rel_ind (ue_id);
}

nas_cause_t nas_cxx::s6a_error_2_nas_cause (uint32_t s6a_error,int experimental) {
  return nas_procedure_obj.s6a_error_2_nas_cause (s6a_error, experimental);
}


