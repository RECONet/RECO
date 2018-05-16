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

/*****************************************************************************

  Source      emm_fsm.c

  Version     0.1

  Date        2012/10/03

  Product     NAS stack

  Subsystem   EPS Mobility Management

  Author      Frederic Maurel

  Description Defines the EPS Mobility Management procedures executed at
        the EMMREG Service Access Point.

*****************************************************************************/
#include <pthread.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "emm_fsm.h"
#include "commonDef.h"
#include "log.h"
#include "common_defs.h"
#include "3gpp_24.007.h"
#include "3gpp_24.008.h"
#include "3gpp_29.274.h"
#include "mme_app_ue_context.hpp"
#include "mme_api.h"
#include "emm_data.h"
#include "assertions.h"
#include "msc.h"
#include "mme_app_defs.h"
#include "mme_time.h"

/****************************************************************************/
/****************  E X T E R N A L    D E F I N I T I O N S  ****************/
/****************************************************************************/


#define EMM_FSM_NB_UE_MAX   (MME_API_NB_UE_MAX + 1)

/****************************************************************************/
/*******************  L O C A L    D E F I N I T I O N S  *******************/
/****************************************************************************/

/*
   -----------------------------------------------------------------------------
            Data used for trace logging
   -----------------------------------------------------------------------------
*/

/* String representation of EMM events */
static const char                      *_emm_fsm_event_str[] = {
  "COMMON_PROC_REQ",
  "COMMON_PROC_CNF",
  "COMMON_PROC_REJ",
  "PROC_ABORT",
  "ATTACH_CNF",
  "ATTACH_REJ",
  "DETACH_INIT",
  "DETACH_REQ",
  "DETACH_FAILED",
  "DETACH_CNF",
  "TAU_REQ",
  "TAU_CNF",
  "TAU_REJ",
  "SERVICE_REQ",
  "SERVICE_CNF",
  "SERVICE_REJ",
  "LOWERLAYER_SUCCESS",
  "LOWERLAYER_FAILURE",
  "LOWERLAYER_RELEASE",
  "LOWERLAYER_NON_DELIVERY",
};

/* String representation of EMM state */
static const char                      * const _emm_fsm_status_str[EMM_STATE_MAX] = {
  "INVALID",
  "EMM-DEREGISTERED",
  "EMM-REGISTERED",
  "EMM-DEREGISTERED-INITIATED",
  "EMM-COMMON-PROCEDURE-INITIATED",
};

/*
   -----------------------------------------------------------------------------
        EPS Mobility Management state machine handlers
   -----------------------------------------------------------------------------
*/

#ifdef __cplusplus
extern "C" {
#endif
/* Type of the EPS Mobility Management state machine handler */
typedef int                             (
  *emm_fsm_handler_t)                     (
  const emm_reg_t *);

int                                     EmmDeregistered (
  const emm_reg_t *);
int                                     EmmRegistered (
  const emm_reg_t *);
int                                     EmmDeregisteredInitiated (
  const emm_reg_t *);
int                                     EmmCommonProcedureInitiated (
  const emm_reg_t *);

/* EMM state machine handlers */
static const emm_fsm_handler_t          _emm_fsm_handlers[EMM_STATE_MAX] = {
  NULL,
  EmmDeregistered,
  EmmRegistered,
  EmmDeregisteredInitiated,
  EmmCommonProcedureInitiated,
};

#ifdef __cplusplus
}
#endif
/*
   -----------------------------------------------------------------------------
            Current EPS Mobility Management state
   -----------------------------------------------------------------------------
*/

/****************************************************************************/
/******************  E X P O R T E D    F U N C T I O N S  ******************/
/****************************************************************************/

/****************************************************************************
 **                                                                        **
 ** Name:    emm_fsm_initialize()                                      **
 **                                                                        **
 ** Description: Initializes the EMM state machine                         **
 **                                                                        **
 ** Inputs:  None                                                      **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    None                                       **
 **      Others:    _emm_fsm_status                            **
 **                                                                        **
 ***************************************************************************/
void
emm_fsm_initialize (
  void)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  OAILOG_FUNC_OUT (LOG_NAS_EMM);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_fsm_set_status()                                      **
 **                                                                        **
 ** Description: Set the EPS Mobility Management state to the given state **
 **                                                                        **
 ** Inputs:  ue_id:      Lower layers UE identifier                 **
 **      state:    The new EMM state                         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    RETURNok, RETURNerror                      **
 **      Others:    _emm_fsm_status                            **
 **                                                                        **
 ***************************************************************************/
int
emm_fsm_set_state (
  const mme_ue_s1ap_id_t ue_id,
  struct emm_context_s * const emm_context,
  const emm_fsm_state_t state)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  //emm_context_t                     *emm_context = (emm_context_t *) ctx;

  DevAssert (emm_context);
  if (state < EMM_STATE_MAX) {
    if (state != emm_context->_emm_fsm_state) {
      OAILOG_INFO (LOG_NAS_EMM, "UE " MME_UE_S1AP_ID_FMT" EMM-FSM   - Status changed: %s ===> %s\n", ue_id, _emm_fsm_status_str[emm_context->_emm_fsm_state], _emm_fsm_status_str[state]);
      MSC_LOG_EVENT (MSC_NAS_EMM_MME, "EMM state %s UE " MME_UE_S1AP_ID_FMT" ", _emm_fsm_status_str[state], ue_id);
      emm_context->_emm_fsm_state = state;
	
      //if (state == EMM_REGISTERED) PRINT_TIME
    }

    OAILOG_FUNC_RETURN (LOG_NAS_EMM, RETURNok);
  }
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, RETURNerror);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_fsm_get_state()                                      **
 **                                                                        **
 ** Description: Get the current value of the EPS Mobility Management      **
 **      state                                                    **
 **                                                                        **
 ** Inputs:  ue_id:      Lower layers UE identifier                 **
 **      Others:    _emm_fsm_status                            **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    The current value of the EMM state        **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
emm_fsm_state_t
emm_fsm_get_state (const struct emm_context_s * const emm_context)
{
  if (emm_context ) {
    AssertFatal((emm_context->_emm_fsm_state < EMM_STATE_MAX) && (emm_context->_emm_fsm_state > EMM_STATE_MIN),
        "ue_id " MME_UE_S1AP_ID_FMT " BAD EMM state %d",
        PARENT_STRUCT(emm_context, ue_mm_context_t, emm_context)->mme_ue_s1ap_id,
        emm_context->_emm_fsm_state);
    return emm_context->_emm_fsm_state;
  }
  return EMM_INVALID;
}

//------------------------------------------------------------------------------
const char *
emm_fsm_get_state_str (const struct emm_context_s * const emm_context)
{
  if (emm_context ) {
    emm_fsm_state_t state  = emm_fsm_get_state (emm_context);
    return _emm_fsm_status_str[state];
  }
  return  _emm_fsm_status_str[EMM_INVALID];
}


/****************************************************************************
 **                                                                        **
 ** Name:    emm_fsm_process()                                         **
 **                                                                        **
 ** Description: Executes the EMM state machine                            **
 **                                                                        **
 ** Inputs:  evt:       The EMMREG-SAP event to process            **
 **      Others:    _emm_fsm_status                            **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    RETURNok, RETURNerror                      **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_fsm_process (
  const emm_reg_t * evt)
{
  int                                     rc = RETURNerror;
  emm_fsm_state_t                         state;
  emm_reg_primitive_t                     primitive;

  OAILOG_FUNC_IN (LOG_NAS_EMM);
  primitive = evt->primitive;
  emm_context_t                     *emm_ctx = (emm_context_t *) evt->ctx;

  if (emm_ctx) {
    state = emm_fsm_get_state (emm_ctx);
    OAILOG_INFO (LOG_NAS_EMM, "EMM-FSM   - Received event %s (%d) in state %s\n", _emm_fsm_event_str[primitive - _EMMREG_START - 1], primitive, _emm_fsm_status_str[state]);
    /*
     * Execute the EMM state machine
     */
    rc = (_emm_fsm_handlers[state]) (evt);
  } else {
    OAILOG_WARNING (LOG_NAS_EMM, "EMM-FSM   - Received event %s (%d) but no EMM data context provided\n", _emm_fsm_event_str[primitive - _EMMREG_START - 1], primitive);
  }
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

/****************************************************************************/
/*********************  L O C A L    F U N C T I O N S  *********************/
/****************************************************************************/
