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

/*! \file mme_app_ue_context.c
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "mme_app_ue_context.hpp"
#include "log.h"
#include "assertions.h"
#include "dynamic_memory_check.h"
#include "conversions.h"
#include "common_defs.h"
#include "mme_app_procedures.h"
//------------------------------------------------------------------------------
ue_mm_context_t *mme_create_new_ue_context (void)
{
  ue_mm_context_t* new_p = static_cast<ue_mm_context_t*>(calloc (1, sizeof (ue_mm_context_t)));
  emm_init_context(&new_p->emm_context, true);
  return new_p;
}

//------------------------------------------------------------------------------
void ue_mm_context_t::MoveContentTo(ue_mm_context_t *dst) {
  OAILOG_FUNC_IN (LOG_MME_APP);
  if (dst) {
    OAILOG_DEBUG (LOG_MME_APP,
           "mme_app_move_context("ENB_UE_S1AP_ID_FMT " <- " ENB_UE_S1AP_ID_FMT ")\n",
           dst->enb_ue_s1ap_id, enb_ue_s1ap_id);
    dst->FreeContent();
    memcpy(dst, this, sizeof(ue_mm_context_t)); // dangerous (memory leaks), TODO check
    memset(this, 0, sizeof(ue_mm_context_t));
  }
  OAILOG_FUNC_OUT (LOG_MME_APP);
}
//------------------------------------------------------------------------------
void ue_mm_context_t::FreeContent() {
  bdestroy_wrapper (&msisdn);
  bdestroy_wrapper (&ue_radio_capabilities);
  bdestroy_wrapper (&apn_oi_replacement);

  for (int i = 0; i < MAX_APN_PER_UE; i++) {
    if (pdn_contexts[i]) {
      mme_app_free_pdn_connection(&pdn_contexts[i]);
    }
  }

  for (int i = 0; i < BEARERS_PER_UE; i++) {
    if (bearer_contexts[i]) {
      mme_app_free_bearer_context(&bearer_contexts[i]);
    }
  }
  if (s11_procedures) {
    mme_app_delete_s11_procedures(this);
  }
  memset(this, 0, sizeof(ue_mm_context_t));
}
//------------------------------------------------------------------------------
void ue_mm_context_t::AddBearerContext(bearer_context_t  * const bc, const pdn_cid_t pdn_cid) {
  AssertFatal((EPS_BEARER_IDENTITY_LAST >= bc->ebi) && (EPS_BEARER_IDENTITY_FIRST <= bc->ebi), "Bad ebi %u", bc->ebi);
  int index = EBI_TO_INDEX(bc->ebi);
  if (!bearer_contexts[index]) {
    if (pdn_contexts[pdn_cid]) {
      bc->pdn_cx_id       = pdn_cid;
      pdn_contexts[pdn_cid]->bearer_contexts[index] = index;
      bearer_contexts[index] = bc;

      bc->preemption_capability    = pdn_contexts[pdn_cid]->default_bearer_eps_subscribed_qos_profile.allocation_retention_priority.pre_emp_capability;
      bc->preemption_vulnerability = pdn_contexts[pdn_cid]->default_bearer_eps_subscribed_qos_profile.allocation_retention_priority.pre_emp_vulnerability;
      bc->priority_level           = pdn_contexts[pdn_cid]->default_bearer_eps_subscribed_qos_profile.allocation_retention_priority.priority_level;
      return;
    }
    OAILOG_WARNING (LOG_MME_APP, "No PDN id %u exist for ue id " MME_UE_S1AP_ID_FMT "\n", pdn_cid, mme_ue_s1ap_id);
    return;
  }
  OAILOG_WARNING (LOG_MME_APP, "Bearer ebi %u PDN id %u already exist for ue id " MME_UE_S1AP_ID_FMT "\n", bc->ebi, pdn_cid, mme_ue_s1ap_id);
}

//------------------------------------------------------------------------------
bearer_context_t *  mme_app_create_bearer_context(ue_mm_context_t * const ue_mm_context, const pdn_cid_t pdn_cid, const ebi_t ebi, const bool is_default) {
  return ue_mm_context->CreateAndAddBearerContext(pdn_cid, ebi);
}

bearer_context_t*  ue_mm_context_t::CreateAndAddBearerContext(const pdn_cid_t pdn_cid, const ebi_t ebi) {
  ebi_t lebi = ebi;
  if ((EPS_BEARER_IDENTITY_FIRST > lebi) || (EPS_BEARER_IDENTITY_LAST < lebi)) {
    lebi = GetFreeBearerId();
  }

  if (EPS_BEARER_IDENTITY_UNASSIGNED == lebi) {
    return NULL;
  }

  bearer_context_t * bearer_context = static_cast<bearer_context_t *>(malloc(sizeof(*bearer_context)));

  if (bearer_context) {
    bearer_context->Init();
    bearer_context->ebi = lebi;
    AddBearerContext(bearer_context, pdn_cid);
  }
}
//------------------------------------------------------------------------------
ebi_t ue_mm_context_t::GetFreeBearerId() {
  for (int i = 0; i < BEARERS_PER_UE; i++) {
    if (!bearer_contexts[i]) {
      return INDEX_TO_EBI(i);
    }
  }
  return EPS_BEARER_IDENTITY_UNASSIGNED;
}
//------------------------------------------------------------------------------
bearer_context_t* ue_mm_context_t::GetBearerContext(const ebi_t ebi) {
  if ((EPS_BEARER_IDENTITY_LAST >= ebi) && (EPS_BEARER_IDENTITY_FIRST <= ebi)) {
    return bearer_contexts[EBI_TO_INDEX(ebi)];
  }
  return NULL;
}

bearer_context_t* mme_app_get_bearer_context(ue_mm_context_t * const ue_context, const ebi_t ebi)
{
  if(ue_context)
    return ue_context->GetBearerContext(ebi);
  return NULL;
}

bearer_context_t* ue_mm_context_t::GetBearerContextByState(const pdn_cid_t pdn_cx_id, const mme_app_bearer_state_t state) {
  for (int i = 0; i < BEARERS_PER_UE; i++) {
    bearer_context_t *bc = bearer_contexts[i];
    if ((bc) && (state == bc->bearer_state)) {
      if (pdn_cx_id == bc->pdn_cx_id) {
        return bc;
      }
      // if no specific PDN id selected
      if (MAX_APN_PER_UE == pdn_cx_id) {
        return bc;
      }
    }
  }
  return NULL;
}
//------------------------------------------------------------------------------
void ue_mm_context_t::S1ReleaseEnbInfo() {
  for (int i = 0; i < BEARERS_PER_UE; i++) {
    bearer_context_t *bc = bearer_contexts[i];
    if (bc)
      bc->S1ReleaseEnbInfo();
  }
}

/**
 * @brief compares to imsis returns
 * @param imsi
 * @return true if the same else false
 */
bool MmeAppImsi::Compare (MmeAppImsi* imsi) {
  if((strncmp(GetData(), imsi->GetData(), IMSI_BCD_DIGITS_MAX) == 0)
          && GetLen() == imsi->GetLen()) {
    return true;
  } else {
    return false;
  }
}
/**
 * @brief copies an mme_app_imsi to this mme_app_imsi
 * @param imsi_src
 */
void MmeAppImsi::Set(MmeAppImsi* imsi_src) {
  strncpy(GetData(), imsi_src->GetData(), IMSI_BCD_DIGITS_MAX + 1);
  _length = imsi_src->GetLen();
}
/**
 * @brief converts the imsi_t struct to the imsi mme struct
 * @param imsi_src
 */
void MmeAppImsi::Set(const imsi_t *imsi_src) {
  memset(GetData(),  (uint8_t) '\0', sizeof(_data));
  IMSI_TO_STRING(imsi_src, GetData(), IMSI_BCD_DIGITS_MAX + 1);
  _length = strlen(GetData());
}
/**
 * @brief converst the a string to the imsi mme structure
 * @param imsi_string
 */
void MmeAppImsi::Set(char const * const imsi_str) {
  strncpy(GetData(), imsi_str, IMSI_BCD_DIGITS_MAX + 1);
  _length = strlen(GetData());
}
/**
 * @brief converts mme_app imsi structure to a string
 * @param imsi_dst
 */
void MmeAppImsi::ToString(char * const imsi_dst) {
  strncpy(imsi_dst, GetData(), IMSI_BCD_DIGITS_MAX + 1);
}
/**
 * @brief mme_app_imsi_to_u64: converts imsi to uint64 (be careful leading 00 will be cut off)
 * @param imsi_src
 * @return uint64 transformed from imsi
 */
uint64_t MmeAppImsi::ToUint64() {
  uint64_t imsi_ret;
  sscanf(GetData(), "%" SCNu64, &imsi_ret);
  return imsi_ret;
}
