#include <libconfig.h>
#include "bstrlib.h"
#include "assertions.h"
#include "log.h"
#include "s6a_config_HS.h"

s6a_config_t                      s6a_config;

//------------------------------------------------------------------------------
static void s6a_config_init(s6a_config_t * s6a_config_p) {
  memset(s6a_config_p, 0, sizeof(s6a_config_t));
  s6a_config_p->config_file = NULL;
  s6a_config_p->conf_file = bfromcstr(S6A_CONF_FILE);
}


//------------------------------------------------------------------------------
static int s6a_config_parse_file(s6a_config_t * s6a_config_p) {
  config_t                                cfg = {0};
  config_setting_t                       *setting_s6a = NULL;
  const char                             *astring = NULL;

  config_init(&cfg);

  if (s6a_config_p->config_file != NULL) {
    /*
     * Read the file. If there is an error, report it and exit.
     */
    if (!config_read_file(&cfg, bdata(s6a_config_p->config_file))) {
      OAILOG_ERROR(LOG_CONFIG, ": %s:%d - %s\n", \
                   bdata(s6a_config_p->config_file), \
                   config_error_line(&cfg), config_error_text(&cfg));
      config_destroy(&cfg);
      AssertFatal(1 == 0, "Failed to parse S6A configuration file %s!\n", \
                  bdata(s6a_config_p->config_file));
    }
  } else {
    OAILOG_ERROR(LOG_CONFIG, " No S6A configuration file provided!\n");
    config_destroy(&cfg);
    AssertFatal(0, "No S6A configuration file provided!\n");
  }

  setting_s6a = config_lookup(&cfg, S6A_CONFIG_STRING_CONFIG);

  if (setting_s6a != NULL) {
    if ((config_setting_lookup_string(setting_s6a, \
                                      S6A_CONFIG_STRING_CONF_FILE_PATH, \
                                      (const char **)&astring))) {
      if (astring != NULL) {
        if (s6a_config_p->conf_file) {
          bassigncstr(s6a_config_p->conf_file , astring);
        } else {
          s6a_config_p->conf_file = bfromcstr(astring);
        }
      }
    }

    if ((config_setting_lookup_string(setting_s6a, \
         S6A_CONFIG_STRING_HSS_HOSTNAME, (const char **)&astring))) {
      if (astring != NULL) {
        if (s6a_config_p->hss_host_name) {
          bassigncstr(s6a_config_p->hss_host_name , astring);
        } else {
          s6a_config_p->hss_host_name = bfromcstr(astring);
        }
      } else {
        AssertFatal(1 == 0, \
                    "You have to provide a valid HSS hostname %s=...\n", \
                    S6A_CONFIG_STRING_HSS_HOSTNAME);
      }
    }
  }



  config_destroy(&cfg);
  return 0;
}


//------------------------------------------------------------------------------
static void s6a_config_display(s6a_config_t * s6a_config_p) {
  OAILOG_INFO(LOG_CONFIG, "HS_S6A Configuration:\n");
  OAILOG_INFO(LOG_CONFIG, "- File .................................: %s\n", \
              bdata(s6a_config_p->config_file));
  OAILOG_INFO(LOG_CONFIG, "    freediameter config file ........: %s\n", \
              bdata(s6a_config_p->conf_file));
  OAILOG_INFO(LOG_CONFIG, "    hss host name ........: %s\n", \
              bdata(s6a_config_p->hss_host_name));
}


int s6a_config_parse(s6a_config_t * s6a_config_p) {
  s6a_config_init(s6a_config_p);

  /*
   * Parse the configuration file using libconfig
   */
  if (!s6a_config_p->config_file) {
    s6a_config_p->config_file = bfromcstr("/usr/local/etc/oai/mme.conf");
  }
  if (s6a_config_parse_file(s6a_config_p) != 0) {
    return -1;
  }

  /*
   * Display the configuration
   */
  s6a_config_display(s6a_config_p);
  return 0;
}
