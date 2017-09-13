#ifndef FILE_S11_MME_CONFIG_HMB
#define FILE_S11_MEE_CONFIG_HMB

#include "bstrlib.h"

typedef struct s11_mme_config_s {
  bstring config_file;

} s11_mme_config_t;

extern s11_mme_config_t s11_mme_config;


#ifdef __cplusplus
extern "C" {
#endif

int s11_mme_config_parse (s11_mme_config_t * s11_mme_config_p);

#ifdef __cplusplus
}
#endif


#endif /* FILE_S11_MME_CONFIG_HMB */
