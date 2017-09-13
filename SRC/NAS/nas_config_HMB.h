#ifndef FILE_NAS_CONFIG_HMB
#define FILE_NAS_CONFIG_HMB

#include "bstrlib.h"


#define NAS_CONFIG_STRING_CONFIG                     "HMB_NAS"
#define NAS_CONFIG_STRING_SUPPORTED_INTEGRITY_ALGORITHM_LIST  "ORDERED_SUPPORTED_INTEGRITY_ALGORITHM_LIST"
#define NAS_CONFIG_STRING_SUPPORTED_CIPHERING_ALGORITHM_LIST  "ORDERED_SUPPORTED_CIPHERING_ALGORITHM_LIST"

#define NAS_CONFIG_STRING_T3402_TIMER                "T3402"
#define NAS_CONFIG_STRING_T3412_TIMER                "T3412"
#define NAS_CONFIG_STRING_T3422_TIMER                "T3422"
#define NAS_CONFIG_STRING_T3450_TIMER                "T3450"
#define NAS_CONFIG_STRING_T3460_TIMER                "T3460"
#define NAS_CONFIG_STRING_T3470_TIMER                "T3470"
#define NAS_CONFIG_STRING_T3485_TIMER                "T3485"
#define NAS_CONFIG_STRING_T3486_TIMER                "T3486"
#define NAS_CONFIG_STRING_T3489_TIMER                "T3489"
#define NAS_CONFIG_STRING_T3495_TIMER                "T3495"
#define NAS_CONFIG_STRING_FORCE_REJECT_TAU           "FORCE_REJECT_TAU"
#define NAS_CONFIG_STRING_FORCE_REJECT_SR            "FORCE_REJECT_SR"
#define NAS_CONFIG_STRING_DISABLE_ESM_INFORMATION_PROCEDURE    "DISABLE_ESM_INFORMATION_PROCEDURE"
#define NAS_CONFIG_STRING_FORCE_PUSH_DEDICATED_BEARER "FORCE_PUSH_DEDICATED_BEARER"


typedef struct nas_config_s {
  bstring config_file;

  uint8_t  prefered_integrity_algorithm[8];
  uint8_t  prefered_ciphering_algorithm[8];
  uint32_t t3402_min;
  uint32_t t3412_min;
  uint32_t t3422_sec;
  uint32_t t3450_sec;
  uint32_t t3460_sec;
  uint32_t t3470_sec;
  uint32_t t3485_sec;
  uint32_t t3486_sec;
  uint32_t t3489_sec;
  uint32_t t3495_sec;

  // non standart features
  bool     force_reject_tau;
  bool     force_reject_sr;
  bool     disable_esm_information;

} nas_config_t;

extern nas_config_t nas_config;



int nas_config_parse (nas_config_t * nas_config_p);

#endif /* FILE_NAS_CONFIG_HMB */
