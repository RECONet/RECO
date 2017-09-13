#include <libconfig.h>
#include <cstdlib>
#include "bstrlib.h"
#include "assertions.h"
#include "log.h"
#include "s1ap_config.h"

s1ap_config_t                      s1ap_config;

//------------------------------------------------------------------------------
static void s1ap_config_init (s1ap_config_t * s1ap_config_p)
{
  memset(s1ap_config_p, 0, sizeof(s1ap_config_t));
  s1ap_config_p->config_file = NULL;
  s1ap_config_p->outcome_drop_timer_sec = S1AP_OUTCOME_TIMER_DEFAULT;
  s1ap_config_p->port_number = S1AP_PORT_NUMBER;
}


//------------------------------------------------------------------------------
static int s1ap_config_parse_file (s1ap_config_t * s1ap_config_p)
{
  config_t                                cfg = {0};
  config_setting_t                       *setting_s1ap = NULL;
  int                                     aint = 0;


  config_init (&cfg);

  if (s1ap_config_p->config_file != NULL) {
    /*
     * Read the file. If there is an error, report it and exit.
     */
    if (!config_read_file (&cfg, bdata(s1ap_config_p->config_file))) {
      OAILOG_ERROR (LOG_CONFIG, ": %s:%d - %s\n", bdata(s1ap_config_p->config_file), config_error_line (&cfg), config_error_text (&cfg));
      config_destroy (&cfg);
      AssertFatal (1 == 0, "Failed to parse S1AP configuration file %s!\n", bdata(s1ap_config_p->config_file));
    }
  } else {
    OAILOG_ERROR (LOG_CONFIG, " No S1AP configuration file provided!\n");
    config_destroy (&cfg);
    AssertFatal (0, "No S1AP configuration file provided!\n");
  }

  setting_s1ap = config_lookup (&cfg, S1AP_CONFIG_STRING_CONFIG);

  if (setting_s1ap != NULL) { 

    if ((config_setting_lookup_int (setting_s1ap, S1AP_CONFIG_STRING_OUTCOME_TIMER, &aint))) {
      s1ap_config_p->outcome_drop_timer_sec = (uint8_t) aint;
    }

    if ((config_setting_lookup_int (setting_s1ap, S1AP_CONFIG_STRING_PORT, &aint))) {
      s1ap_config_p->port_number = (uint16_t) aint;
    }
    
  }
  config_destroy (&cfg);
  return 0;
}


//------------------------------------------------------------------------------
static void s1ap_config_display (s1ap_config_t * s1ap_config_p)
{
  OAILOG_INFO (LOG_CONFIG, "S1AP Configuration:\n");
  OAILOG_INFO (LOG_CONFIG, "- File .................................: %s\n", bdata(s1ap_config_p->config_file));
  OAILOG_INFO (LOG_CONFIG, "    port number ......: %d\n", s1ap_config_p->port_number);
  OAILOG_INFO (LOG_CONFIG, "    outcome timer ......: %d\n", s1ap_config_p->outcome_drop_timer_sec);
}


int s1ap_config_parse (s1ap_config_t * s1ap_config_p)
{

  s1ap_config_init (s1ap_config_p);

  /*
   * Parse the configuration file using libconfig
   */
  if (!s1ap_config_p->config_file) {
    s1ap_config_p->config_file = bfromcstr("/usr/local/etc/oai/mme.conf");
  }
  if (s1ap_config_parse_file (s1ap_config_p) != 0) {
    return -1;
  }

  /*
   * Display the configuration
   */
  s1ap_config_display (s1ap_config_p);
  return 0;
}
