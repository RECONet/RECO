#include <libconfig.h>
#include <cstdlib>
#include "bstrlib.h"
#include "assertions.h"
#include "log.h"
#include "nas_config.h"
#include "3gpp_24.301.h"
#include "3gpp_33.401.h"


nas_config_t                      nas_config;

//------------------------------------------------------------------------------
static void nas_config_init (nas_config_t * nas_config_p)
{
  memset(nas_config_p, 0, sizeof(nas_config_t));
  nas_config_p->config_file = NULL;

  nas_config_p->t3402_min = T3402_DEFAULT_VALUE;
  nas_config_p->t3412_min = T3412_DEFAULT_VALUE;
  nas_config_p->t3422_sec = T3422_DEFAULT_VALUE;
  nas_config_p->t3450_sec = T3450_DEFAULT_VALUE;
  nas_config_p->t3460_sec = T3460_DEFAULT_VALUE;
  nas_config_p->t3470_sec = T3470_DEFAULT_VALUE;
  nas_config_p->t3485_sec = T3485_DEFAULT_VALUE;
  nas_config_p->t3486_sec = T3486_DEFAULT_VALUE;
  nas_config_p->t3489_sec = T3489_DEFAULT_VALUE;
  nas_config_p->t3495_sec = T3495_DEFAULT_VALUE;
  nas_config_p->force_reject_tau = true;
  nas_config_p->force_reject_sr  = true;
  nas_config_p->disable_esm_information = false;
}


//------------------------------------------------------------------------------
static int nas_config_parse_file (nas_config_t * nas_config_p)
{
  config_t                                cfg = {0};
  config_setting_t                       *setting_nas = NULL;
  config_setting_t                       *setting = NULL;
  int                                     aint = 0;
  int                                     i = 0,
                                          num = 0;
  const char                             *astring = NULL;

  config_init (&cfg);

  if (nas_config_p->config_file != NULL) {
    /*
     * Read the file. If there is an error, report it and exit.
     */
    if (!config_read_file (&cfg, bdata(nas_config_p->config_file))) {
      OAILOG_ERROR (LOG_CONFIG, ": %s:%d - %s\n", bdata(nas_config_p->config_file), config_error_line (&cfg), config_error_text (&cfg));
      config_destroy (&cfg);
      AssertFatal (1 == 0, "Failed to parse NAS configuration file %s!\n", bdata(nas_config_p->config_file));
    }
  } else {
    OAILOG_ERROR (LOG_CONFIG, " No NAS configuration file provided!\n");
    config_destroy (&cfg);
    AssertFatal (0, "No NAS configuration file provided!\n");
  }

  setting_nas = config_lookup (&cfg, NAS_CONFIG_STRING_CONFIG);

  if (setting_nas != NULL) {

    setting = config_setting_get_member (setting_nas, NAS_CONFIG_STRING_SUPPORTED_INTEGRITY_ALGORITHM_LIST);

    if (setting != NULL) {
      num = config_setting_length (setting);

      if (num <= 8) {
        for (i = 0; i < num; i++) {
          astring = config_setting_get_string_elem (setting, i);

          if (strcmp ("EIA0", astring) == 0)
            nas_config_p->prefered_integrity_algorithm[i] = EIA0_ALG_ID;
          else if (strcmp ("EIA1", astring) == 0)
            nas_config_p->prefered_integrity_algorithm[i] = EIA1_128_ALG_ID;
          else if (strcmp ("EIA2", astring) == 0)
            nas_config_p->prefered_integrity_algorithm[i] = EIA2_128_ALG_ID;
          else
            nas_config_p->prefered_integrity_algorithm[i] = EIA0_ALG_ID;
        }

        for (i = num; i < 8; i++) {
          nas_config_p->prefered_integrity_algorithm[i] = EIA0_ALG_ID;
        }
      }
    }

    setting = config_setting_get_member (setting_nas, NAS_CONFIG_STRING_SUPPORTED_CIPHERING_ALGORITHM_LIST);

    if (setting != NULL) {
      num = config_setting_length (setting);

      if (num <= 8) {
        for (i = 0; i < num; i++) {
          astring = config_setting_get_string_elem (setting, i);

          if (strcmp ("EEA0", astring) == 0)
            nas_config_p->prefered_ciphering_algorithm[i] = EEA0_ALG_ID;
          else if (strcmp ("EEA1", astring) == 0)
            nas_config_p->prefered_ciphering_algorithm[i] = EEA1_128_ALG_ID;
          else if (strcmp ("EEA2", astring) == 0)
            nas_config_p->prefered_ciphering_algorithm[i] = EEA2_128_ALG_ID;
          else
            nas_config_p->prefered_ciphering_algorithm[i] = EEA0_ALG_ID;
        }

        for (i = num; i < 8; i++) {
          nas_config_p->prefered_ciphering_algorithm[i] = EEA0_ALG_ID;
        }
      }
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3402_TIMER, &aint))) {
      nas_config_p->t3402_min = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3412_TIMER, &aint))) {
      nas_config_p->t3412_min = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3422_TIMER, &aint))) {
      nas_config_p->t3422_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3450_TIMER, &aint))) {
      nas_config_p->t3450_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3460_TIMER, &aint))) {
      nas_config_p->t3460_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3470_TIMER, &aint))) {
      nas_config_p->t3470_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3485_TIMER, &aint))) {
      nas_config_p->t3485_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3486_TIMER, &aint))) {
      nas_config_p->t3486_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3489_TIMER, &aint))) {
      nas_config_p->t3489_sec = (uint32_t) aint;
    }
    if ((config_setting_lookup_int (setting_nas, NAS_CONFIG_STRING_T3495_TIMER, &aint))) {
      nas_config_p->t3495_sec = (uint32_t) aint;
    }

    if ((config_setting_lookup_string (setting_nas, NAS_CONFIG_STRING_FORCE_REJECT_TAU, (const char **)&astring))) {
      if (strcasecmp (astring, "yes") == 0)
        nas_config_p->force_reject_tau = true;
      else
        nas_config_p->force_reject_tau = false;
    }
    if ((config_setting_lookup_string (setting, NAS_CONFIG_STRING_FORCE_REJECT_SR, (const char **)&astring))) {
      if (strcasecmp (astring, "yes") == 0)
        nas_config_p->force_reject_sr = true;
      else
        nas_config_p->force_reject_sr = false;
    }
    if ((config_setting_lookup_string (setting_nas, NAS_CONFIG_STRING_DISABLE_ESM_INFORMATION_PROCEDURE, (const char **)&astring))) {
      if (strcasecmp (astring, "yes") == 0)
        nas_config_p->disable_esm_information = true;
      else
        nas_config_p->disable_esm_information = false;
    }
    
  }

  config_destroy (&cfg);
  return 0;
}


//------------------------------------------------------------------------------
static void nas_config_display (nas_config_t * nas_config_p)
{
  OAILOG_INFO (LOG_CONFIG, "NAS Configuration:\n");
  OAILOG_INFO (LOG_CONFIG, "- File .................................: %s\n", bdata(nas_config_p->config_file));
  OAILOG_INFO (LOG_CONFIG, "    Prefered Integrity Algorithms .: EIA%d EIA%d EIA%d EIA%d (decreasing priority)\n",
      nas_config_p->prefered_integrity_algorithm[0],
      nas_config_p->prefered_integrity_algorithm[1],
      nas_config_p->prefered_integrity_algorithm[2],
      nas_config_p->prefered_integrity_algorithm[3]);
  OAILOG_INFO (LOG_CONFIG, "    Prefered Integrity Algorithms .: EEA%d EEA%d EEA%d EEA%d (decreasing priority)\n",
      nas_config_p->prefered_ciphering_algorithm[0],
      nas_config_p->prefered_ciphering_algorithm[1],
      nas_config_p->prefered_ciphering_algorithm[2],
      nas_config_p->prefered_ciphering_algorithm[3]);
  OAILOG_INFO (LOG_CONFIG, "    T3402 ....: %d min\n", nas_config_p->t3402_min);
  OAILOG_INFO (LOG_CONFIG, "    T3412 ....: %d min\n", nas_config_p->t3412_min);
  OAILOG_INFO (LOG_CONFIG, "    T3422 ....: %d sec\n", nas_config_p->t3422_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3450 ....: %d sec\n", nas_config_p->t3450_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3460 ....: %d sec\n", nas_config_p->t3460_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3470 ....: %d sec\n", nas_config_p->t3470_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3485 ....: %d sec\n", nas_config_p->t3485_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3486 ....: %d sec\n", nas_config_p->t3486_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3489 ....: %d sec\n", nas_config_p->t3489_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3470 ....: %d sec\n", nas_config_p->t3470_sec);
  OAILOG_INFO (LOG_CONFIG, "    T3495 ....: %d sec\n", nas_config_p->t3495_sec);
  OAILOG_INFO (LOG_CONFIG, "    NAS non standart features .:\n");
  OAILOG_INFO (LOG_CONFIG, "      Force reject TAU ............: %s\n", (nas_config_p->force_reject_tau) ? "true":"false");
  OAILOG_INFO (LOG_CONFIG, "      Force reject SR .............: %s\n", (nas_config_p->force_reject_sr) ? "true":"false");
  OAILOG_INFO (LOG_CONFIG, "      Disable Esm information .....: %s\n", (nas_config_p->disable_esm_information) ? "true":"false");

}


int nas_config_parse (nas_config_t * nas_config_p)
{

  nas_config_init (nas_config_p);

  /*
   * Parse the configuration file using libconfig
   */
  if (!nas_config_p->config_file) {
    nas_config_p->config_file = bfromcstr("/usr/local/etc/oai/mme.conf");
  }
  if (nas_config_parse_file (nas_config_p) != 0) {
    return -1;
  }

  /*
   * Display the configuration
   */
  nas_config_display (nas_config_p);
  return 0;
}
