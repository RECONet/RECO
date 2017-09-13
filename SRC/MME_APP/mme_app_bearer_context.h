/*
 * Copyright (c) 2015, EURECOM (www.eurecom.fr)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

/*! \file mme_app_bearer_context.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_MME_APP_BEARER_CONTEXT_SEEN
#define FILE_MME_APP_BEARER_CONTEXT_SEEN


#define BEARER_STATE_NULL        0
#define BEARER_STATE_SGW_CREATED (1 << 0)
#define BEARER_STATE_MME_CREATED (1 << 1)
#define BEARER_STATE_ENB_CREATED (1 << 2)
#define BEARER_STATE_ACTIVE      (1 << 3)
#define BEARER_STATE_S1_RELEASED (1 << 4)

typedef uint8_t mme_app_bearer_state_t;


/** @struct bearer_context_t
 *  @brief Parameters that should be kept for an eps bearer.
 */
class bearer_context_t {
 public:
  void Init();
  void S1ReleaseEnbInfo();
  // EPS Bearer ID: An EPS bearer identity uniquely identifies an EPS bearer for one UE accessing via E-UTRAN
  ebi_t                       ebi;

  // TI Transaction Identifier
  proc_tid_t                  transaction_identifier;

  // S-GW IP address for S1-u: IP address of the S-GW for the S1-u interfaces.
  // S-GW TEID for S1u: Tunnel Endpoint Identifier of the S-GW for the S1-u interface.
  fteid_t                      s_gw_fteid_s1u;            // set by S11 CREATE_SESSION_RESPONSE

  // PDN GW TEID for S5/S8 (user plane): P-GW Tunnel Endpoint Identifier for the S5/S8 interface for the user plane. (Used for S-GW change only).
  // NOTE:
  // The PDN GW TEID is needed in MME context as S-GW relocation is triggered without interaction with the source S-GW, e.g. when a TAU
  // occurs. The Target S-GW requires this Information Element, so it must be stored by the MME.
  // PDN GW IP address for S5/S8 (user plane): P GW IP address for user plane for the S5/S8 interface for the user plane. (Used for S-GW change only).
  // NOTE:
  // The PDN GW IP address for user plane is needed in MME context as S-GW relocation is triggered without interaction with the source S-GW,
  // e.g. when a TAU occurs. The Target S GW requires this Information Element, so it must be stored by the MME.
  fteid_t                      p_gw_fteid_s5_s8_up;

  // EPS bearer QoS: QCI and ARP, optionally: GBR and MBR for GBR bearer
  qci_t                       qci;

  // TFT: Traffic Flow Template. (For PMIP-based S5/S8 only)
  //traffic_flow_template_t          *tft_pmip;

  // extra 23.401 spec members
  pdn_cid_t                         pdn_cx_id;
  mme_app_bearer_state_t            bearer_state;
  esm_ebr_context_t                 esm_ebr_context;
  fteid_t                           enb_fteid_s1u;

  /* QoS for this bearer */
  priority_level_t            priority_level;
  pre_emption_vulnerability_t preemption_vulnerability;
  pre_emption_capability_t    preemption_capability;

};

void mme_app_free_bearer_context(bearer_context_t ** const bearer_context);
bstring bearer_state2string(const mme_app_bearer_state_t bearer_state);

#endif
