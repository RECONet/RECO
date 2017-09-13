#include <libconfig.h>
#include <cstdlib>
#include "bstrlib.h"
#include "assertions.h"
#include "log.h"
#include "s11_mme_config_HMB.h"

s11_mme_config_t                      s11_mme_config;

//------------------------------------------------------------------------------
static void s11_mme_config_init (s11_mme_config_t * s11_mme_config_p)
{
  memset(s11_mme_config_p, 0, sizeof(s11_mme_config_t));
  s11_mme_config_p->config_file = NULL;
}


//------------------------------------------------------------------------------
static int s11_mme_config_parse_file (s11_mme_config_t * s11_mme_config_p)
{
  config_t                                cfg = {0};

  config_init (&cfg);

  if (s11_mme_config_p->config_file != NULL) {
    /*
     * Read the file. If there is an error, report it and exit.
     */
    if (!config_read_file (&cfg, bdata(s11_mme_config_p->config_file))) {
      OAILOG_ERROR (LOG_CONFIG, ": %s:%d - %s\n", bdata(s11_mme_config_p->config_file), config_error_line (&cfg), config_error_text (&cfg));
      config_destroy (&cfg);
      AssertFatal (1 == 0, "Failed to parse S11_MME configuration file %s!\n", bdata(s11_mme_config_p->config_file));
    }
  } else {
    OAILOG_ERROR (LOG_CONFIG, " No S11_MME configuration file provided!\n");
    config_destroy (&cfg);
    AssertFatal (0, "No S11_MME configuration file provided!\n");
  }

  config_destroy (&cfg);
  return 0;
}


//------------------------------------------------------------------------------
static void s11_mme_config_display (s11_mme_config_t * s11_mme_config_p)
{
  OAILOG_INFO (LOG_CONFIG, "HMB_S11_MME Configuration:\n");
  OAILOG_INFO (LOG_CONFIG, "- File .................................: %s\n", bdata(s11_mme_config_p->config_file));
}


int s11_mme_config_parse (s11_mme_config_t * s11_mme_config_p)
{

  s11_mme_config_init (s11_mme_config_p);

  /*
   * Parse the configuration file using libconfig
   */
  if (!s11_mme_config_p->config_file) {
    s11_mme_config_p->config_file = bfromcstr("/usr/local/etc/oai/mme.conf");
  }
  if (s11_mme_config_parse_file (s11_mme_config_p) != 0) {
    return -1;
  }

  /*
   * Display the configuration
   */
  s11_mme_config_display (s11_mme_config_p);
  return 0;
}
