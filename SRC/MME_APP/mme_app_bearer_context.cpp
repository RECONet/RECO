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


/*! \file mme_app_bearer_context.c
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
#include "mme_config.h"
#include "mme_app_ue_context.hpp"
#include "mme_app_defs.h"

//------------------------------------------------------------------------------
bstring bearer_state2string(const mme_app_bearer_state_t bearer_state) {
  bstring bsstr = NULL;
  if  (BEARER_STATE_NULL == bearer_state) {
    bsstr = bfromcstr("BEARER_STATE_NULL");
    return bsstr;
  }
  bsstr = bfromcstr(" ");
  if  (BEARER_STATE_SGW_CREATED & bearer_state) bcatcstr(bsstr, "SGW_CREATED ");
  if  (BEARER_STATE_MME_CREATED & bearer_state) bcatcstr(bsstr, "MME_CREATED ");
  if  (BEARER_STATE_ENB_CREATED & bearer_state) bcatcstr(bsstr, "ENB_CREATED ");
  if  (BEARER_STATE_ACTIVE & bearer_state) bcatcstr(bsstr, "ACTIVE");
  return bsstr;
}

//------------------------------------------------------------------------------
void bearer_context_t::Init() {
  memset(this, 0, sizeof(bearer_context_t));
  bearer_state = BEARER_STATE_NULL;

  esm_bearer_context_init(&esm_ebr_context);
}
//------------------------------------------------------------------------------
void bearer_context_t::S1ReleaseEnbInfo() {
  bearer_state = BEARER_STATE_S1_RELEASED;
  memset(&enb_fteid_s1u, 0, sizeof(enb_fteid_s1u));
  enb_fteid_s1u.teid = INVALID_TEID;
}

//------------------------------------------------------------------------------
void mme_app_free_bearer_context (bearer_context_t ** const bearer_context)
{
  free_esm_bearer_context(&(*bearer_context)->esm_ebr_context);
  free_wrapper((void**)bearer_context);
}
