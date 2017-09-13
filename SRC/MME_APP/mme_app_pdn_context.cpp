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


/*! \file mme_app_pdn_context.c
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "dynamic_memory_check.h"
#include "log.h"
#include "msc.h"
#include "assertions.h"
#include "conversions.h"
#include "common_types.h"
//#include "intertask_interface.h"
#include "mme_config.h"
#include "mme_app_extern.h"
#include "mme_app_ue_context.hpp"
#include "mme_app_defs.h"
#include "sgw_ie_defs.h"
#include "common_defs.h"
#include "mme_app_pdn_context.h"
#include "mme_app_apn_selection.h"

void mme_app_free_pdn_connection (pdn_context_t ** const pdn_connection)
{
  bdestroy_wrapper(&(*pdn_connection)->apn_in_use);
  bdestroy_wrapper(&(*pdn_connection)->apn_oi_replacement);
  /* ToDo: Check whether the following codes are needed
  if ((*pdn_context)->apn_oi_replacement) {
    bdestroy_wrapper(&(*pdn_context)->apn_oi_replacement);
  }
  if ((*pdn_context)->pco) {
    free_protocol_configuration_options(&(*pdn_context)->pco);
  }
  */
  free_wrapper((void**)pdn_connection);
}

//------------------------------------------------------------------------------
void pdn_context_t::Init() {
  memset(this, 0, sizeof(pdn_context_t));
  is_active = false;
  for (int i=0; i < BEARERS_PER_UE; i++) {
    bearer_contexts[i] = -1; // contains bearer indexes in ue_mm_context_t.bearer_contexts[]
  }
}
//------------------------------------------------------------------------------
pdn_context_t* mme_app_create_pdn_context(ue_mm_context_t * const ue_mm_context, const pdn_cid_t pdn_cid, const context_identifier_t context_identifier) {
  return ue_mm_context->CreatePdnContext(pdn_cid, context_identifier);
}

pdn_context_t* ue_mm_context_t::CreatePdnContext(const pdn_cid_t pdn_cid, const context_identifier_t ctx_id) {

  OAILOG_FUNC_IN (LOG_MME_APP);
  if (!pdn_contexts[pdn_cid]) {
    apn_configuration_t *apn_configuration = GetApnConfig(ctx_id);

    if (apn_configuration) {
      pdn_context_t* pdn_context = static_cast<pdn_context_t *>(calloc(1, sizeof(pdn_context_t)));

      if (pdn_context) {

        pdn_context->Init();
        pdn_context->context_identifier     = apn_configuration->context_identifier;
        pdn_context->pdn_type               = apn_configuration->pdn_type;
        if (apn_configuration->nb_ip_address) {
          pdn_context->paa.pdn_type           = apn_configuration->ip_address[0].pdn_type;// TODO check this later...
          pdn_context->paa.ipv4_address       = apn_configuration->ip_address[0].address.ipv4_address;
          if (2 == apn_configuration->nb_ip_address) {
            pdn_context->paa.ipv6_address       = apn_configuration->ip_address[1].address.ipv6_address;
            pdn_context->paa.ipv6_prefix_length = 64;
          }
        }
        //pdn_context->apn_oi_replacement
        memcpy (&pdn_context->default_bearer_eps_subscribed_qos_profile, &apn_configuration->subscribed_qos, sizeof(eps_subscribed_qos_profile_t));
        //pdn_context->subscribed_apn_ambr = ;
        //pdn_context->pgw_apn_ambr = ;
        pdn_context->is_active = true;
        pdn_context->apn_subscribed = blk2bstr(apn_configuration->service_selection, apn_configuration->service_selection_length);
        pdn_context->default_ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
        // TODO pdn_context->apn_in_use     =

        pdn_contexts[pdn_cid] = pdn_context;
        MSC_LOG_EVENT (MSC_NAS_ESM_MME, "0 Create PDN cid %u APN %s", pdn_cid, apn_configuration->service_selection);

        OAILOG_FUNC_RETURN (LOG_MME_APP, pdn_context);
      }
    }
  }
  OAILOG_FUNC_RETURN (LOG_MME_APP, NULL);
}
