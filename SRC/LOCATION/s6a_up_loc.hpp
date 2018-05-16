
class s6a_up_loc {

public:
	static int s6a_ula_cb(
    		struct msg      **msg_pP,
    		struct avp       *paramavp_pP,
    		struct session   *sess_pP,
    		void             *opaque_pP,
    		enum disp_action *act_pP);
	static int s6a_generate_update_location(s6a_update_location_req_t *ulr_pP);

protected:
	int s6a_parse_subscription_data(struct avp *avp_subscription_data,
                                	subscription_data_t *subscription_data);

	inline int s6a_parse_apn_configuration_profile(struct avp *avp_apn_conf_prof,
                                        apn_config_profile_t *apn_config_profile);

	/*Call by s6a_parse_subscription_data*/
	inline int s6a_parse_subscriber_status(struct avp_hdr *hdr_sub_status,
                                	subscriber_status_t *sub_status);
	inline int s6a_parse_msisdn(struct avp_hdr *hdr_msisdn, char *msisdn, uint8_t *length);
	inline int s6a_parse_network_access_mode(struct avp_hdr *hdr_network_am,
                                  	network_access_mode_t *access_mode);
	inline int s6a_parse_access_restriction_data(struct avp_hdr *hdr_access_restriction,
                                        access_restriction_t *access_restriction);

	/*Call by s6a_parse_apn_configuration & s6a_parse_subscription_data*/
	inline int s6a_parse_bitrate(struct avp_hdr *hdr_bitrate, bitrate_t *bitrate);
	inline int s6a_parse_ambr(struct avp *avp_ambr, ambr_t *ambr);

	/*Call by s6a_parse_apn_configuration_profile*/
	inline int s6a_parse_all_apn_conf_inc_ind(struct avp_hdr *hdr, all_apn_conf_ind_t *ptr);
	inline int s6a_parse_apn_configuration(struct avp *avp_apn_conf_prof, 
					apn_configuration_t *apn_config);

	/*Call by s6a_parse_apn_configuration*/
	inline int s6a_parse_pdn_type(struct avp_hdr *hdr, pdn_type_t *pdn_type);
	inline int s6a_parse_service_selection(struct avp_hdr *hdr_service_selection,
                                	char *service_selection, int *length);	
	inline int s6a_parse_eps_subscribed_qos_profile(struct avp *avp_qos,
                                        eps_subscribed_qos_profile_t *ptr);
	inline int s6a_parse_ip_address(struct avp_hdr *hdr, ip_address_t *ip_address);

	/*Call by s6a_parse_eps_subscribed_qos_profile*/
	inline int s6a_parse_qci(struct avp_hdr *hdr, qci_t *qci);
	inline int s6a_parse_allocation_retention_priority(struct avp *avp_arp,
                                        allocation_retention_priority_t *ptr);

	/*Call by s6a_parse_allocation_retention_priority*/
	inline int s6a_parse_priority_level(struct avp_hdr *hdr, priority_level_t *priority_level);
	inline int s6a_parse_pre_emp_capability(struct avp_hdr *hdr, pre_emption_capability_t * pre_emp_capability);
	inline int s6a_parse_pre_emp_vulnerability(struct avp_hdr *hdr, 
					pre_emption_vulnerability_t * pre_emp_vulnerability);					
};


typedef int (*s6a_ula_cb_cxx)(
    		struct msg      **msg_pP,
    		struct avp       *paramavp_pP,
    		struct session   *sess_pP,
    		void             *opaque_pP,
    		enum disp_action *act_pP);
