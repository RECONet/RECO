/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */
/*! \file mme_app_apn_selection.c
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "log.h"
#include "mme_app_ue_context.hpp"
#include "mme_app_apn_selection.h"

//------------------------------------------------------------------------------
apn_configuration_t* mme_app_select_apn(ue_mm_context_t * const ue_context, const_bstring const ue_selected_apn) {
  return ue_context->GetApnConfig(ue_selected_apn);
}

apn_configuration_t* ue_mm_context_t::GetApnConfig(const const_bstring ue_selected_apn) {

  context_identifier_t default_context_identifier = apn_config_profile.context_identifier;
  int                  index;

  for (index = 0; index < apn_config_profile.nb_apns; index++) {
    if (!ue_selected_apn) {
      /*
       * OK we got our default APN
       */
      if (apn_config_profile.apn_configuration[index].context_identifier == default_context_identifier) {
        OAILOG_DEBUG (LOG_MME_APP, "Selected APN %s for UE " IMSI_64_FMT "\n",
            apn_config_profile.apn_configuration[index].service_selection,
            emm_context._imsi64);
        return &apn_config_profile.apn_configuration[index];
      }
    } else {
      /*
       * OK we got the UE selected APN
       */
      if (biseqcaselessblk (ue_selected_apn,
          apn_config_profile.apn_configuration[index].service_selection,
          strlen(apn_config_profile.apn_configuration[index].service_selection)) == 1) {
          OAILOG_DEBUG (LOG_MME_APP, "Selected APN %s for UE " IMSI_64_FMT "\n",
              apn_config_profile.apn_configuration[index].service_selection,
              emm_context._imsi64);
        return &apn_config_profile.apn_configuration[index];
      }
    }
  }
  return NULL;
}


//------------------------------------------------------------------------------
apn_configuration_t* ue_mm_context_t::GetApnConfig(const context_identifier_t ctx_id) {

  for (int index = 0; index < apn_config_profile.nb_apns; index++) {
    if (apn_config_profile.apn_configuration[index].context_identifier == ctx_id) {
      return &apn_config_profile.apn_configuration[index];
    }
  }
  return NULL;
}

