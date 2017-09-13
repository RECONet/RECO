#include <libconfig.h>
#include <cstdlib>
#include "bstrlib.h"
#include "assertions.h"
#include "log.h"
#include "mme_app_config_HMB.h"

mme_app_config_t                      mme_app_config;

//------------------------------------------------------------------------------
static void mme_app_config_init (mme_app_config_t * mme_app_config_p)
{
  memset(mme_app_config_p, 0, sizeof(mme_app_config_t));
  mme_app_config_p->config_file = NULL;
}


//------------------------------------------------------------------------------
static int mme_app_config_parse_file (mme_app_config_t * mme_app_config_p)
{
  config_t                                cfg = {0};

  config_init (&cfg);

  if (mme_app_config_p->config_file != NULL) {
    /*
     * Read the file. If there is an error, report it and exit.
     */
    if (!config_read_file (&cfg, bdata(mme_app_config_p->config_file))) {
      OAILOG_ERROR (LOG_CONFIG, ": %s:%d - %s\n", bdata(mme_app_config_p->config_file), config_error_line (&cfg), config_error_text (&cfg));
      config_destroy (&cfg);
      AssertFatal (1 == 0, "Failed to parse MME_APP configuration file %s!\n", bdata(mme_app_config_p->config_file));
    }
  } else {
    OAILOG_ERROR (LOG_CONFIG, " No MME_APP configuration file provided!\n");
    config_destroy (&cfg);
    AssertFatal (0, "No MME_APP configuration file provided!\n");
  }

  config_destroy (&cfg);
  return 0;
}


//------------------------------------------------------------------------------
static void mme_app_config_display (mme_app_config_t * mme_app_config_p)
{
  OAILOG_INFO (LOG_CONFIG, "HMB_MME_APP Configuration:\n");
  OAILOG_INFO (LOG_CONFIG, "- File .................................: %s\n", bdata(mme_app_config_p->config_file));
}


int mme_app_config_parse (mme_app_config_t * mme_app_config_p)
{

  mme_app_config_init (mme_app_config_p);

  /*
   * Parse the configuration file using libconfig
   */
  if (!mme_app_config_p->config_file) {
    mme_app_config_p->config_file = bfromcstr("/usr/local/etc/oai/mme.conf");
  }
  if (mme_app_config_parse_file (mme_app_config_p) != 0) {
    return -1;
  }

  /*
   * Display the configuration
   */
  mme_app_config_display (mme_app_config_p);
  return 0;
}
