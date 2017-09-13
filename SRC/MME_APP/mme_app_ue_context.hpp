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



/*! \file mme_app_ue_context.hpp
 *  \brief MME applicative layer
 *  \author Sebastien ROUX, Lionel Gauthier
 *  \date 2013
 *  \version 1.0
 *  \email: lionel.gauthier@eurecom.fr
 *  @defgroup _mme_app_impl_ MME applicative layer
 *  @ingroup _ref_implementation_
 *  @{
 */

#ifndef FILE_MME_APP_UE_CONTEXT_SEEN
#define FILE_MME_APP_UE_CONTEXT_SEEN
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>   /* For sscanf formats */
#include <time.h>       /* to provide time_t */
#include <pthread.h>

#include "tree.h"
#include "queue.h"
#include "bstrlib.h"
#include "hashtable.h"
#include "obj_hashtable.h"
#include "common_types.h"
#include "s1ap_messages_types.h"
#include "nas_messages_types.h"
#include "s6a_messages_types.h"
#include "security_types.h"
#include "sgw_ie_defs.h"
#include "mme_app_bearer_context.h"
#include "mme_app_pdn_context.h"
#include "emm_data.h"
#include "esm_data.h"

enum EcmState {
  ECM_IDLE = 0,
  ECM_CONNECTED,
};

#define IMSI_DIGITS_MAX 15

class MmeAppImsi {
 public:
  char * GetData() { return _data; }
  uint32_t GetLen() { return _length; }

  bool IsEmpty() { return (GetLen()==0); }
  bool Compare(MmeAppImsi* imsi);

  void Set(MmeAppImsi* imsi_src);
  void Set(const imsi_t *imsi_src);
  void Set(char const * const imsi_str);

  void ToString(char * const imsi_dst);
  uint64_t ToUint64();

  uint32_t _length;
  char _data[IMSI_DIGITS_MAX + 1];
};

/* In mme_app_pdn_context.h but not called by any funtion
 * void mme_app_free_pdn_context (pdn_context_t ** const pdn_context);
 */
struct s11_procedures_s {
    struct mme_app_s11_proc_s *lh_first;
};


/** @struct ue_mm_context_t
 *  @brief Useful parameters to know in MME application layer. They are set
 * according to 3GPP TS.23.401 #5.7.2
 */
class ue_mm_context_t {
 public:
  void FreeContent();
  /** \brief Move content to another context
   * \param dst: The destination context
   **/
  void MoveContentTo(ue_mm_context_t *dst);

  bearer_context_t* CreateAndAddBearerContext(const pdn_cid_t pdn_cid, const ebi_t ebi);
  void AddBearerContext(bearer_context_t  * const bc, const pdn_cid_t pdn_cid);
  bearer_context_t* GetBearerContext(const ebi_t ebi);
  bearer_context_t* GetBearerContextByState(const pdn_cid_t pdn_cx_id, const mme_app_bearer_state_t state);
  ebi_t GetFreeBearerId();

  apn_configuration_t* GetApnConfig(const_bstring const ue_selected_apn);
  apn_configuration_t* GetApnConfig(const context_identifier_t ctx_id);
  pdn_context_t*  CreatePdnContext(const pdn_cid_t pdn_cid, const context_identifier_t context_identifier);

  void S1ReleaseEnbInfo();

 /* Basic identifier for ue. IMSI is encoded on maximum of 15 digits of 4 bits,
  * so usage of an unsigned integer on 64 bits is necessary.
  */
#define IMSI_UNAUTHENTICATED  (false)
#define IMSI_AUTHENTICATED    (true)
  bool                   imsi_auth;           // This is an IMSI indicator to show the IMSI is unauthenticated. set by nas_auth_resp_t

  bstring                msisdn;                    // The basic MSISDN of the UE. The presence is dictated by its storage in the HSS.
                                                    // set by S6A UPDATE LOCATION ANSWER

  EcmState             ecm_state;                // ECM state ECM-IDLE, ECM-CONNECTED.
                                                    // not set/read
  bool                    is_s1_ue_context_release;
  S1ap_Cause_t            s1_ue_context_release_cause;

  // Globally Unique Temporary Identity can be found in emm_nas_context
  //bool                   is_guti_set;                 // is GUTI has been set
  //guti_t                 guti;                        // Globally Unique Temporary Identity. guti.gummei.plmn set by nas_auth_param_req_t

  // read by S6A UPDATE LOCATION REQUEST
  // was me_identity_t // Mobile Equipment Identity – (e.g. IMEI/IMEISV) Software Version Number not set/read except read by display utility
  //imei_t                   _imei;        /* The IMEI provided by the UE     can be found in emm_nas_context                */
  //imeisv_t                 _imeisv;      /* The IMEISV provided by the UE   can be found in emm_nas_context                */

  tai_list_t             tail_list; // Current Tracking area list

  tai_t                  tai_last_tau ; // TAI of the TA in which the last Tracking Area Update was initiated.

  /* Last known cell identity */
  ecgi_t                  e_utran_cgi;                 // Last known E-UTRAN cell, set by nas_attach_req_t
  // read for S11 CREATE_SESSION_REQUEST
  /* Time when the cell identity was acquired */
  time_t                 cell_age;                    // Time elapsed since the last E-UTRAN Cell Global Identity was acquired. set by nas_auth_param_req_t

  /* TODO: add csg_id */
  /* TODO: add csg_membership */
  /* TODO Access mode: Access mode of last known ECGI when the UE was active */

  // Authentication Vector Temporary authentication and key agreement data that enables an MME to
  // engage in AKA with a particular user. An EPS Authentication Vector consists of four elements:
  // a) network challenge RAND,
  // b) an expected response XRES,
  // c) Key K ASME ,
  // d) a network authentication token AUTN.

  /* Store the radio capabilities as received in S1AP UE capability indication message. */
  // UE Radio Access Capability UE radio access capabilities.
  bstring                 ue_radio_capabilities;       // not set/read


  // UE Network Capability  // UE network capabilities including security algorithms and key derivation functions supported by the UE

  // MS Network Capability  // For a GERAN and/or UTRAN capable UE, this contains information needed by the SGSN.

  /* TODO: add DRX parameter */
  // UE Specific DRX Parameters   // UE specific DRX parameters for A/Gb mode, Iu mode and S1-mode

  // Selected NAS Algorithm       // Selected NAS security algorithm
  // eKSI                         // Key Set Identifier for the main key K ASME . Also indicates whether the UE is using
                                  // security keys derived from UTRAN or E-UTRAN security association.

  // K ASME                       // Main key for E-UTRAN key hierarchy based on CK, IK and Serving network identity

  // NAS Keys and COUNT           // K NASint , K_ NASenc , and NAS COUNT parameter.

  // Selected CN operator id      // Selected core network operator identity (to support network sharing as defined in TS 23.251 [24]).

  // Recovery                     // Indicates if the HSS is performing database recovery.

  ard_t                  access_restriction_data;      // The access restriction subscription information. set by S6A UPDATE LOCATION ANSWER

  // ODB for PS parameters        // Indicates that the status of the operator determined barring for packet oriented services.

  // APN-OI Replacement           // Indicates the domain name to replace the APN-OI when constructing the PDN GW
                                  // FQDN upon which to perform a DNS resolution. This replacement applies for all
                                  // the APNs in the subscriber's profile. See TS 23.003 [9] clause 9.1.2 for more
                                  // information on the format of domain names that are allowed in this field.
  bstring      apn_oi_replacement; // example: "province1.mnc012.mcc345.gprs"

  // MME IP address for S11       // MME IP address for the S11 interface (used by S-GW)
  // LOCATED IN mme_config_t.ipv4.s11

  // MME TEID for S11             // MME Tunnel Endpoint Identifier for S11 interface.
  // LOCATED IN THIS.subscribed_apns[MAX_APN_PER_UE].mme_teid_s11
  teid_t                      mme_teid_s11;                // set by mme_app_send_s11_create_session_req

  // S-GW IP address for S11/S4   // S-GW IP address for the S11 and S4 interfaces
  // LOCATED IN THIS.subscribed_apns[MAX_APN_PER_UE].s_gw_address_s11_s4

  // S-GW TEID for S11/S4         // S-GW Tunnel Endpoint Identifier for the S11 and S4 interfaces.
  // LOCATED IN THIS.subscribed_apns[MAX_APN_PER_UE].s_gw_teid_s11_s4

  // SGSN IP address for S3       // SGSN IP address for the S3 interface (used if ISR is activated for the GERAN and/or UTRAN capable UE)

  // SGSN TEID for S3             // SGSN Tunnel Endpoint Identifier for S3 interface (used if ISR is activated for the E-UTRAN capable UE)

  // eNodeB Address in Use for S1-MME // The IP address of the eNodeB currently used for S1-MME.
  // implicit with use of SCTP through the use of sctp_assoc_id_key
  sctp_assoc_id_t        sctp_assoc_id_key; // link with eNB id

  // eNB UE S1AP ID,  Unique identity of the UE within eNodeB.
  enb_ue_s1ap_id_t       enb_ue_s1ap_id:24;
  enb_s1ap_id_key_t      enb_s1ap_id_key; // key uniq among all connected eNBs

  // MME UE S1AP ID, Unique identity of the UE within MME.
  mme_ue_s1ap_id_t       mme_ue_s1ap_id;


  // Subscribed UE-AMBR: The Maximum Aggregated uplink and downlink MBR values to be shared across all Non-GBR bearers according to the subscription of the user.
  ambr_t                 suscribed_ue_ambr;
  // UE-AMBR: The currently used Maximum Aggregated uplink and downlink MBR values to be shared across all Non-GBR bearers.
  ambr_t                 used_ue_ambr;
  // EPS Subscribed Charging Characteristics: The charging characteristics for the MS e.g. normal, prepaid, flat rate and/or hot billing.
  // Subscribed RFSP Index: An index to specific RRM configuration in the E-UTRAN that is received from the HSS.
  // RFSP Index in Use: An index to specific RRM configuration in the E-UTRAN that is currently in use.
  // Trace reference: Identifies a record or a collection of records for a particular trace.
  // Trace type: Indicates the type of trace
  // Trigger id: Identifies the entity that initiated the trace
  // OMC identity: Identifies the OMC that shall receive the trace record(s).
  // URRP-MME: URRP-MME indicating that the HSS has requested the MME to notify the HSS regarding UE reachability at the MME.
  // CSG Subscription Data: The CSG Subscription Data is a list of CSG IDs for the visiting PLMN and for each
  //   CSG ID optionally an associated expiration date which indicates the point in time when the subscription to the CSG ID
  //   expires; an absent expiration date indicates unlimited subscription. For a CSG ID that can be used to access specific PDNs via Local IP Access, the
  //   CSG ID entry includes the corresponding APN(s).
  // LIPA Allowed: Specifies whether the UE is allowed to use LIPA in this PLMN.

  // Subscribed Periodic RAU/TAU Timer: Indicates a subscribed Periodic RAU/TAU Timer value.
  rau_tau_timer_t        rau_tau_timer;               // set by S6A UPDATE LOCATION ANSWER

  // MPS CS priority: Indicates that the UE is subscribed to the eMLPP or 1x RTT priority service in the CS domain.

  // MPS EPS priority: Indicates that the UE is subscribed to MPS in the EPS domain.

  // For each active PDN connection:
  pdn_context_t         *pdn_contexts[MAX_APN_PER_UE]; // index is of type pdn_cid_t
  int                    nb_active_pdn_contexts;




  // Not in spec members
  emm_context_t          emm_context;
  bearer_context_t      *bearer_contexts[BEARERS_PER_UE];

  apn_config_profile_t   apn_config_profile;                  // set by S6A UPDATE LOCATION ANSWER


#define SUBSCRIPTION_UNKNOWN    false
#define SUBSCRIPTION_KNOWN      true
  bool                   subscription_known;        // set by S6A UPDATE LOCATION ANSWER
  ambr_t                 used_ambr;
  subscriber_status_t    subscriber_status;        // set by S6A UPDATE LOCATION ANSWER
  network_access_mode_t  network_access_mode;       // set by S6A UPDATE LOCATION ANSWER
  struct s11_procedures_s *s11_procedures;        // LIST_HEAD(s11_procedures_s, mme_app_s11_proc_s) *s11_procedures;
};

/** \brief Allocate memory for a new UE context
 * @returns Pointer to the new structure, NULL if allocation failed
 **/
ue_mm_context_t *mme_create_new_ue_context(void);

#ifdef __cplusplus
extern "C" {
#endif
bearer_context_t *  mme_app_create_bearer_context(ue_mm_context_t * const ue_mm_context, const pdn_cid_t pdn_cid, const ebi_t ebi, const bool is_default);
bearer_context_t* mme_app_get_bearer_context(ue_mm_context_t * const ue_context, const ebi_t ebi);
#ifdef __cplusplus
}
#endif

class MmeUeContextManager {
 public:
  ue_mm_context_t* GetUeContextByImsi(const imsi64_t imsi);
  ue_mm_context_t* GetUeContextByS11Teid(const s11_teid_t s11_teid);
  ue_mm_context_t* GetUeContextByMmeUeS1apId(const mme_ue_s1ap_id_t mme_ue_s1ap_id);
  ue_mm_context_t* GetUeContextByEnbUeS1apId(const enb_s1ap_id_key_t enb_key);
  ue_mm_context_t* GetUeContextByGuti(const guti_t * const guti_p);

  void UpdateCollKeys(ue_mm_context_t     * const ue_context_p,
                      const enb_s1ap_id_key_t  enb_s1ap_id_key,
                      const mme_ue_s1ap_id_t   mme_ue_s1ap_id,
                      const imsi64_t     imsi,
                      const s11_teid_t         mme_teid_s11,
                      const guti_t     * const guti_p);
  /** \brief dump MME associative collections **/
  void DumpCollKeys();
  /** \brief Insert a new UE context in the tree of known UEs.
   * At least the IMSI should be known to insert the context in the tree.
   * \param ue_context_p The UE context to insert
   * @returns 0 in case of success, -1 otherwise
   **/
  int InsertUeContext(const ue_mm_context_t * const ue_context_p);
  /** \brief Remove a UE context of the tree of known UEs.
   * \param ue_context_p The UE context to remove
   **/
  void RemoveUeContext(ue_mm_context_t * const ue_context_p);
  /** \brief TODO WORK HERE Remove UE context unnecessary information.
   *  mark it as released. It is necessary to keep track of the association (s_tmsi (guti), mme_ue_s1ap_id)
   * \param ue_context_p The UE context to remove
   **/
  void NotifyUeContextReleased(ue_mm_context_t *ue_context_p);
  /** \brief Dump the UE contexts present in the tree **/
  void DumpUeContexts();

  uint32_t               nb_ue_managed;
  uint32_t               nb_ue_idle;

  uint32_t               nb_bearers_managed;

  uint32_t               nb_ue_since_last_stat;
  uint32_t               nb_bearers_since_last_stat;

  hash_table_ts_t       *imsi_ue_context_htbl;
  hash_table_ts_t       *tun11_ue_context_htbl;
  hash_table_ts_t       *mme_ue_s1ap_id_ue_context_htbl;
  hash_table_ts_t       *enb_ue_s1ap_id_ue_context_htbl;
  obj_hash_table_t      *guti_ue_context_htbl;
};

#ifdef __cplusplus
extern "C" {
#endif
/** \brief Retrieve an UE context by selecting the provided IMSI
 * \param imsi Imsi to find in UE map
 * @returns an UE context matching the IMSI or NULL if the context doesn't exists
 **/
ue_mm_context_t *mme_ue_context_exists_imsi(MmeUeContextManager * const mme_ue_context,
    const imsi64_t imsi);

/** \brief Retrieve an UE context by selecting the provided S11 teid
 * \param teid The tunnel endpoint identifier used between MME and S-GW
 * @returns an UE context matching the teid or NULL if the context doesn't exists
 **/
ue_mm_context_t *mme_ue_context_exists_s11_teid(MmeUeContextManager * const mme_ue_context,
    const s11_teid_t teid);

/** \brief Retrieve an UE context by selecting the provided mme_ue_s1ap_id
 * \param mme_ue_s1ap_id The UE id identifier used in S1AP MME (and NAS)
 * @returns an UE context matching the mme_ue_s1ap_id or NULL if the context doesn't exists
 **/
ue_mm_context_t *mme_ue_context_exists_mme_ue_s1ap_id(MmeUeContextManager * const mme_ue_context,
    const mme_ue_s1ap_id_t mme_ue_s1ap_id);

/** \brief Retrieve an UE context by selecting the provided enb_ue_s1ap_id
 * \param enb_ue_s1ap_id The UE id identifier used in S1AP MME
 * @returns an UE context matching the enb_ue_s1ap_id or NULL if the context doesn't exists
 **/
ue_mm_context_t *mme_ue_context_exists_enb_ue_s1ap_id (
  MmeUeContextManager * const mme_ue_context_p,
  const enb_s1ap_id_key_t enb_key);

/** \brief Retrieve an UE context by selecting the provided guti
 * \param guti The GUTI used by the UE
 * @returns an UE context matching the guti or NULL if the context doesn't exists
 **/

ue_mm_context_t *mme_ue_context_exists_guti(MmeUeContextManager * const mme_ue_context,
    const guti_t * const guti);

/** \brief Update an UE context by selecting the provided guti
 * \param mme_ue_context_p The MME context
 * \param ue_context_p The UE context
 * \param enb_s1ap_id_key The eNB UE id identifier
 * \param mme_ue_s1ap_id The UE id identifier used in S1AP MME (and NAS)
 * \param imsi
 * \param mme_s11_teid The tunnel endpoint identifier used between MME and S-GW
 * \param nas_ue_id The UE id identifier used in S1AP MME and NAS
 * \param guti_p The GUTI used by the UE
 **/
void mme_ue_context_update_coll_keys(
    MmeUeContextManager * const mme_ue_context_p,
    ue_mm_context_t     * const ue_context_p,
    const enb_s1ap_id_key_t  enb_s1ap_id_key,
    const mme_ue_s1ap_id_t   mme_ue_s1ap_id,
    const imsi64_t     imsi,
    const s11_teid_t         mme_s11_teid,
    const guti_t     * const guti_p);


/* ToDo: Wrap the following methods into class */
/** \brief Notify the MME_APP that a duplicated ue_context_t exist (both share the same mme_ue_s1ap_id)
 * \param enb_key        The UE id identifier used in S1AP and MME_APP (agregated with a enb_id)
 * \param mme_ue_s1ap_id The UE id identifier used in MME_APP and NAS
 * \param is_remove_old  Remove old UE context or new UE context ?
 **/
ue_mm_context_t *
mme_ue_context_duplicate_enb_ue_s1ap_id_detected (
  const enb_s1ap_id_key_t enb_key,
  const mme_ue_s1ap_id_t  mme_ue_s1ap_id,
  const bool              is_remove_old);

/** \brief Create the association between mme_ue_s1ap_id and an UE context (enb_ue_s1ap_id key)
 * \param enb_key        The UE id identifier used in S1AP and MME_APP (agregated with a enb_id)
 * \param mme_ue_s1ap_id The UE id identifier used in MME_APP and NAS
 * @returns RETURNerror or RETURNok
 **/
int
mme_ue_context_notified_new_ue_s1ap_id_association (
    const enb_s1ap_id_key_t  enb_key,
    const mme_ue_s1ap_id_t   mme_ue_s1ap_id);

#ifdef __cplusplus
}
#endif

void mme_app_handle_s1ap_ue_context_release_req(const itti_s1ap_ue_context_release_req_t * const s1ap_ue_context_release_req);

#endif /* FILE_MME_APP_UE_CONTEXT_SEEN */

/* @} */
