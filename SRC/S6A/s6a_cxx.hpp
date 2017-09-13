#ifndef MODULES_S6A_CXX_H_
#define MODULES_S6A_CXX_H_
#include "mme_config.h"
#include "s6a_defs.h"

class s6a_cxx {

  public:
	s6a_cxx();
	int s6a_init (const mme_config_t * mme_config_p);
	static void s6a_exit(void);
	static void *s6a_thread (void *args);
	
	/*functions for debugging*/
	static void fd_gnutls_debug (
  		int loglevel,
  		const char *str);
	static void oai_fd_logger(int loglevel, const char * format, va_list args);
  protected:
	//struct session_handler                 *ts_sess_hdl;
	int s6a_fd_init_dict_objs (void);
	//s6a_fd_cnf_t                            s6a_fd_cnf;
	
	/*s6a peer related*/
	int s6a_fd_new_peer ( void);
	void s6a_peer_connected_cb (struct peer_info *info, void *arg);
};

#endif S6A_CXX_H_
