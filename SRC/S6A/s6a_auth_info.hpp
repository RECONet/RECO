#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "bstrlib.h"

#include "dynamic_memory_check.h"
#include "log.h"
#include "msc.h"
#include "mme_config.h"
#include "assertions.h"
#include "conversions.h"
#include "common_types.h"
#include "common_defs.h"
#include "intertask_interface.h"
#include "s6a_defs.h"
#include "s6a_messages.h"

class s6a_auth_info_base {

public:
	static int s6a_aia_cb(struct msg **msg, struct avp *paramavp,
               struct session *sess, void *opaque,
               enum disp_action *act);
	static int s6a_generate_authentication_info_req(s6a_auth_info_req_t *air_p);


private:
	int s6a_parse_rand(struct avp_hdr *hdr, uint8_t *rand_p);
	int s6a_parse_xres(struct avp_hdr *hdr, res_t *xres);
	int s6a_parse_autn(struct avp_hdr *hdr, uint8_t *autn);
	int s6a_parse_kasme(struct avp_hdr *hdr, uint8_t *kasme);
	inline int s6a_parse_e_utran_vector(struct avp *avp_vector, eutran_vector_t *vector);
	inline int s6a_parse_authentication_info_avp(struct avp *avp_auth_info,
                   authentication_info_t *authentication_info);
};

class s6a_auth_info_walker:public s6a_auth_info_base {
};

class s6a_auth_info_vehicle:public s6a_auth_info_base {
};

class s6a_auth_info_hsr:public s6a_auth_info_base {
};

class s6a_auth_info:public s6a_auth_info_walker,public s6a_auth_info_vehicle,public s6a_auth_info_hsr {
};

typedef int (*s6a_aia_cb_cxx)(struct msg **msg, struct avp *paramavp,
               struct session *sess, void *opaque,
               enum disp_action *act);

//int (s6a_auth_info::*s6a_aia_cb_cxx)(struct msg **, struct avp *,
 //              struct session *, void *,
  //             enum disp_action *) = &s6a_auth_info::s6a_aia_cb;
