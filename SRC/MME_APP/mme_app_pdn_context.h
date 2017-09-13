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
/*! \file mme_app_pdn_context.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_MME_APP_PDN_CONTEXT_SEEN
#define FILE_MME_APP_PDN_CONTEXT_SEEN

/** @struct subscribed_apn_t
 *  @brief Parameters that should be kept for a subscribed apn by the UE.
 */
// For each active PDN connection:
struct pdn_context_t {
  void Init();
  context_identifier_t context_identifier; // key

  //APN in Use: The APN currently used. This APN shall be composed of the APN Network
  //            Identifier and the default APN Operator Identifier, as specified in TS 23.003 [9],
  //            clause 9.1.2 (EURECOM: "mnc<MNC>.mcc<MCC>.gprs"). Any received value in the APN OI Replacement field is not applied
  //            here.
  bstring                     apn_in_use;        // an ID for P-GW through which a user can access the Subscribed APN

  // APN Restriction: Denotes the restriction on the combination of types of APN for the APN associated
  //                  with this EPS bearer Context.

  // APN Subscribed: The subscribed APN received from the HSS.
  bstring                     apn_subscribed;

  // PDN Type: IPv4, IPv6 or IPv4v6
  pdn_type_t                  pdn_type;

  // IP Address(es): IPv4 address and/or IPv6 prefix
  //                 NOTE:
  //                 The MME might not have information on the allocated IPv4 address.
  //                 Alternatively, following mobility involving a pre-release 8 SGSN, this
  //                 IPv4 address might not be the one allocated to the UE.
  paa_t              paa;                         // set by S11 CREATE_SESSION_RESPONSE



  // EPS PDN Charging Characteristics: The charging characteristics of this PDN connection, e.g. normal, prepaid, flat-rate
  // and/or hot billing.

  // APN-OI Replacement: APN level APN-OI Replacement which has same role as UE level APN-OI
  // Replacement but with higher priority than UE level APN-OI Replacement. This is
  // an optional parameter. When available, it shall be used to construct the PDN GW
  // FQDN instead of UE level APN-OI Replacement.
  bstring         apn_oi_replacement;

  // SIPTO permissions: Indicates whether the traffic associated with this APN is allowed or prohibited for SIPTO
  // LIPA permissions: Indicates whether the PDN can be accessed via Local IP Access. Possible values
  //                   are: LIPA-prohibited, LIPA-only and LIPA-conditional.

  // VPLMN Address Allowed: Specifies whether the UE is allowed to use the APN in the domain of the HPLMN
  //                        only, or additionally the APN in the domain of the VPLMN.

  // PDN GW Address in Use(control plane): The IP address of the PDN GW currently used for sending control plane signalling.
  ip_address_t                p_gw_address_s5_s8_cp;

  // PDN GW TEID for S5/S8 (control plane): PDN GW Tunnel Endpoint Identifier for the S5/S8 interface for the control plane.
  //                                        (For GTP-based S5/S8 only).
  teid_t                      p_gw_teid_s5_s8_cp;

  // MS Info Change Reporting Action: Need to communicate change in User Location Information to the PDN GW with this EPS bearer Context.

  // CSG Information Reporting Action: Need to communicate change in User CSG Information to the PDN GW with this
  //                                   EPS bearer Context.
  //                                   This field denotes separately whether the MME/SGSN are requested to send
  //                                   changes in User CSG Information for (a) CSG cells, (b) hybrid cells in which the
  //                                   subscriber is a CSG member and (c) hybrid cells in which the subscriber is not a
  //                                   CSG member.

  // EPS subscribed QoS profile: The bearer level QoS parameter values for that APN's default bearer (QCI and
  // ARP) (see clause 4.7.3).
  eps_subscribed_qos_profile_t  default_bearer_eps_subscribed_qos_profile;

  // Subscribed APN-AMBR: The Maximum Aggregated uplink and downlink MBR values to be shared across
  //                      all Non-GBR bearers, which are established for this APN, according to the
  //                      subscription of the user.
  ambr_t                       subscribed_apn_ambr;

  // APN-AMBR: The Maximum Aggregated uplink and downlink MBR values to be shared across
  // all Non-GBR bearers, which are established for this APN, as decided by the PDN GW.
  ambr_t                       p_gw_apn_ambr;

  // PDN GW GRE Key for uplink traffic (user plane): PDN GW assigned GRE Key for the S5/S8 interface for the user plane for uplink traffic. (For PMIP-based S5/S8 only)

  // Default bearer: Identifies the EPS Bearer Id of the default bearer within the given PDN connection.
  ebi_t                       default_ebi;

  int                         bearer_contexts[BEARERS_PER_UE]; // contains bearer indexes in ue_mm_context_t.bearer_contexts[], or -1

  //apn_configuration_t         apn_configuration; // set by S6A UPDATE LOCATION ANSWER
  //bstring                     pgw_id;            // an ID for P-GW through which a user can access the Subscribed APN

  /* S-GW IP address for User-Plane */
  ip_address_t                s_gw_address_s11_s4;
  teid_t                      s_gw_teid_s11_s4;            // set by S11 CREATE_SESSION_RESPONSE

  esm_pdn_t                   esm_data;
  bool                        is_active;
  protocol_configuration_options_t *pco; // temp storage of information waiting for activation of required procedure

};

void mme_app_free_pdn_connection (pdn_context_t ** const pdn_connection);

#ifdef __cplusplus
extern "C" {
#endif
pdn_context_t *  mme_app_create_pdn_context(ue_mm_context_t * const ue_mm_context, const pdn_cid_t pdn_cid, const context_identifier_t context_identifier);
#ifdef __cplusplus
}
#endif

#endif
