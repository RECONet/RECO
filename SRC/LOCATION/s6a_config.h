#ifndef FILE_S6A_CONFIG
#define FILE_S6A_CONFIG

#define S6A_CONFIG_STRING_CONFIG                     "S6A"
#define S6A_CONFIG_STRING_CONF_FILE_PATH             "S6A_CONF"
#define S6A_CONFIG_STRING_HSS_HOSTNAME               "HSS_HOSTNAME"


/*******************************************************************************
 * S6A Constants
 ******************************************************************************/

#define S6A_CONF_FILE "../S6A/freediameter/s6a.conf"


typedef struct s6a_config_s {
  bstring config_file;  // configuration file for S6A in mme.conf

  // further configuration settings for freediameter in mme_fd.conf
  bstring conf_file;
  bstring hss_host_name;
} s6a_config_t;

extern s6a_config_t s6a_config;



int s6a_config_parse(s6a_config_t * s6a_config_p);

#endif /* FILE_S6A_CONFIG */
