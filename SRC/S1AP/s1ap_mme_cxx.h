#ifndef MODULES_S1AP_MME_CXX_H_
#define MODULES_S1AP_MME_CXX_H_

#include "common_defs.h"
#include "s1ap_mme.h"
#include "s1ap_mme_retransmission.h"
#include "s1ap_mme_ta.h"
#include "s1ap_mme_decoder.h"
#include "s1ap_mme_handlers.h"

class s1ap_mme_cxx {

  public:

	s1ap_mme_cxx();

	/** \brief S1AP layer top init
	 * @returns -1 in case of failure
	 **/
	int s1ap_mme_init(const struct mme_config_s *mme_config);


	 int s1ap_mme_handle_path_switch_request(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
		                                struct s1ap_message_s *message_p);

	 int s1ap_mme_handle_erab_setup_response (const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream, struct s1ap_message_s *message);
	 int s1ap_mme_handle_erab_setup_failure (const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream, struct s1ap_message_s *message);
	 int s1ap_mme_handle_initial_context_setup_response(const sctp_assoc_id_t assoc_id,const sctp_stream_id_t stream,struct s1ap_message_s *message_p);
	 int s1ap_mme_handle_initial_context_setup_failure(const sctp_assoc_id_t assoc_id,const sctp_stream_id_t stream, struct s1ap_message_s *message_p);
	 int s1ap_mme_handle_initial_ue_message(sctp_assoc_id_t assocId, sctp_stream_id_t stream, struct s1ap_message_s *message);
	 int s1ap_mme_handle_uplink_nas_transport(sctp_assoc_id_t assocId, sctp_stream_id_t stream, struct s1ap_message_s *message);
	 int s1ap_mme_handle_nas_non_delivery(sctp_assoc_id_t assocId, sctp_stream_id_t stream, struct s1ap_message_s *message);
	 int s1ap_mme_handle_s1_setup_request(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,struct s1ap_message_s *message_p);
	 int s1ap_mme_handle_ue_cap_indication(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,struct s1ap_message_s *message);
	 int s1ap_mme_handle_ue_context_release_request(const sctp_assoc_id_t assoc_id,const sctp_stream_id_t stream, struct s1ap_message_s *message_p);
	 int s1ap_mme_handle_ue_context_release_complete(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream, struct s1ap_message_s *message_p);
	 void s1ap_handle_delete_session_rsp (const itti_mme_app_delete_session_rsp_t * const mme_app_delete_session_rsp_p);

	//nas procedure
	void s1ap_handle_conn_est_cnf(const itti_mme_app_connection_establishment_cnf_t * const conn_est_cnf_p);
	int s1ap_generate_downlink_nas_transport (
	  const enb_ue_s1ap_id_t enb_ue_s1ap_id,
	  const mme_ue_s1ap_id_t ue_id,
	  STOLEN_REF bstring *payload);
	int s1ap_generate_s1ap_e_rab_setup_req (itti_s1ap_e_rab_setup_req_t * const e_rab_setup_req);

	typedef int (s1ap_mme_cxx::*s1ap_message_decoded_callback_cxx)(
		    const sctp_assoc_id_t             assoc_id,
		    const sctp_stream_id_t            stream,
		    struct s1ap_message_s *message_p
	);

	//data

	bool hss_associated;

  //protected:

	//s1ap_mme

	int s1ap_send_init_sctp (void);

	/** \brief S1AP layer top exit
	 **/
	void s1ap_mme_exit (void);
	

	//nas_procedure

	/** \brief Handle an Initial UE message.
	 * \param assocId lower layer assoc id (SCTP)
	 * \param stream SCTP stream on which data had been received
	 * \param message The message as decoded by the ASN.1 codec
	 * @returns -1 on failure, 0 otherwise
	 **/

	/** \brief Handle an Uplink NAS transport message.
	 * Process the RRC transparent container and forward it to NAS entity.
	 * \param assocId lower layer assoc id (SCTP)
	 * \param stream SCTP stream on which data had been received
	 * \param message The message as decoded by the ASN.1 codec
	 * @returns -1 on failure, 0 otherwise
	 **/

	/** \brief Handle a NAS non delivery indication message from eNB
	 * \param assocId lower layer assoc id (SCTP)
	 * \param stream SCTP stream on which data had been received
	 * \param message The message as decoded by the ASN.1 codec
	 * @returns -1 on failure, 0 otherwise
	 **/

	
	//retransmission
	

	int s1ap_handle_timer_expiry(timer_has_expired_t *timer_has_expired);

	int s1ap_timer_insert(const mme_ue_s1ap_id_t mme_ue_s1ap_id, const long timer_id);

	int s1ap_timer_remove_ue(const mme_ue_s1ap_id_t mme_ue_s1ap_id);
		

	//ta

	int s1ap_mme_compare_ta_lists(S1ap_SupportedTAs_t *ta_list);
	
	int s1ap_mme_compare_plmn(const S1ap_PLMNidentity_t * const plmn);
	
	int s1ap_mme_compare_plmns(S1ap_BPLMNs_t * b_plmns);

	int s1ap_mme_compare_tac(const S1ap_TAC_t * const tac);

	//decoder

	//int s1ap_mme_decode_pdu(s1ap_message *message, const_bstring const raw) __attribute__ ((warn_unused_result));
	int s1ap_mme_decode_pdu(s1ap_message *message, const_bstring const raw) __attribute__ ((warn_unused_result));

	int s1ap_mme_decode_initiating (s1ap_message *message, S1ap_InitiatingMessage_t *initiating_p);

	int s1ap_mme_decode_successfull_outcome (s1ap_message *message, S1ap_SuccessfulOutcome_t *successfullOutcome_p);
	
	int s1ap_mme_decode_unsuccessfull_outcome (s1ap_message *message, S1ap_UnsuccessfulOutcome_t *unSuccessfulOutcome_p);
	
	//handlers
	
	/** \brief Handle decoded incoming messages from SCTP
	 * \param assoc_id SCTP association ID
	 * \param stream Stream number
	 * \param message_p The message decoded by the ASN1C decoder
	 * @returns int
	 **/
	int s1ap_mme_handle_message(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
		                    struct s1ap_message_s *message_p);


	/** \brief Handle an S1 Setup request message.
	 * Typically add the eNB in the list of served eNB if not present, simply reset
	 * UEs association otherwise. S1SetupResponse message is sent in case of success or
	 * S1SetupFailure if the MME cannot accept the configuration received.
	 * \param assoc_id SCTP association ID
	 * \param stream Stream number
	 * \param message_p The message decoded by the ASN1C decoder
	 * @returns int
	 **/


	

	int s1ap_handle_ue_context_release_command(
			const itti_s1ap_ue_context_release_command_t * const ue_context_release_command_pP);




	int s1ap_handle_sctp_deconnection(const sctp_assoc_id_t assoc_id);

	int s1ap_handle_new_association(sctp_new_peer_t *sctp_new_peer_p);

	int s1ap_mme_set_cause(S1ap_Cause_t *cause_p, const S1ap_Cause_PR cause_type, const long cause_value);

	int s1ap_mme_generate_s1_setup_failure(const sctp_assoc_id_t assoc_id, const S1ap_Cause_PR cause_type, const long cause_value, const long time_to_wait);


	int s1ap_generate_s1_setup_response (enb_description_t * enb_association);

	int s1ap_mme_generate_ue_context_release_command (ue_description_t * ue_ref_p, S1ap_Cause_t cause);

	static bool s1ap_send_enb_deregistered_ind (const hash_key_t keyP, void * const dataP, void *argP, void ** resultP);





	//////////////////////////////////////////////////////
	//data

	const char *s1ap_direction2String[];
	typedef struct arg_s1ap_send_enb_dereg_ind_s {
	  uint         current_ue_index;
	  uint         handled_ues;
	  MessageDef  *message_p;
	}arg_s1ap_send_enb_dereg_ind_t;

};

#endif S1AP_MME_CXX_H_
