#ifndef FILE_S1AP_CONFIG
#define FILE_S1AP_CONFIG

#include "bstrlib.h"

#define S1AP_CONFIG_STRING_CONFIG                    "S1AP"
#define S1AP_CONFIG_STRING_OUTCOME_TIMER             "S1AP_OUTCOME_TIMER"
#define S1AP_CONFIG_STRING_PORT                      "S1AP_PORT"


/*******************************************************************************
 * S1AP Constants
 ******************************************************************************/

#define S1AP_PORT_NUMBER (36412) ///< S1AP SCTP IANA ASSIGNED Port Number
#define S1AP_SCTP_PPID   (18)    ///< S1AP SCTP Payload Protocol Identifier (PPID)

#define S1AP_OUTCOME_TIMER_DEFAULT (5)     ///< S1AP Outcome drop timer (s)


typedef struct s1ap_config_s {
  bstring config_file;

  uint16_t port_number;
  uint8_t  outcome_drop_timer_sec;



} s1ap_config_t;

extern s1ap_config_t s1ap_config;

#ifdef __cplusplus
extern "C" {
#endif

int s1ap_config_parse (s1ap_config_t * s1ap_config_p);

#ifdef __cplusplus
}
#endif


#endif /* FILE_S1AP_CONFIG */
