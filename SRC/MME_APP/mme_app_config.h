#ifndef FILE_MME_APP_CONFIG
#define FILE_MME_APP_CONFIG

#include "bstrlib.h"

typedef struct mme_app_config_s {
  bstring config_file;

} mme_app_config_t;

extern mme_app_config_t mme_app_config;

int mme_app_config_parse (mme_app_config_t * mme_app_config_p);

#endif /* FILE_MME_APP_CONFIG */
