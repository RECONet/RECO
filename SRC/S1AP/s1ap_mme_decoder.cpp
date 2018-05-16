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


/*! \file s1ap_mme_decoder.c
   \brief s1ap decode procedures for MME
   \author Sebastien ROUX <sebastien.roux@eurecom.fr>
   \date 2012
   \version 0.1
*/
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

#include "bstrlib.h"

#include "log.h"
#include "assertions.h"
#include "common_defs.h"
#include "intertask_interface.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "s1ap_common.h"
#include "s1ap_ies_defs.h"
#include "s1ap_mme_decoder.h"
#include "s1ap_mme_handlers.h"
#include "dynamic_memory_check.h"
#ifdef __cplusplus
}
#endif
#include "s1ap_mme_cxx.h"

int s1ap_mme_cxx::s1ap_mme_decode_initiating (
  s1ap_message *message,
  S1ap_InitiatingMessage_t *initiating_p) {
  int                                     ret = RETURNerror;
  //MessageDef                             *message_p = NULL;
  //char                                   *message_string = NULL;
  //size_t                                  message_string_size;
  //MessagesIds                             message_id = MESSAGES_ID_MAX;
  DevAssert (initiating_p != NULL);
  //message_string = calloc (10000, sizeof (char));
  //s1ap_string_total_size = 0;
  message->procedureCode = initiating_p->procedureCode;
  message->criticality = initiating_p->criticality;

  switch (initiating_p->procedureCode) {
    case S1ap_ProcedureCode_id_uplinkNASTransport: {
        ret = s1ap_decode_s1ap_uplinknastransporties (&message->msg.s1ap_UplinkNASTransportIEs, &initiating_p->value);
        //s1ap_xer_print_s1ap_uplinknastransport (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_UPLINK_NAS_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_S1Setup: {
        ret = s1ap_decode_s1ap_s1setuprequesties (&message->msg.s1ap_S1SetupRequestIEs, &initiating_p->value);
        //s1ap_xer_print_s1ap_s1setuprequest (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_S1_SETUP_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_initialUEMessage: {
        ret = s1ap_decode_s1ap_initialuemessageies (&message->msg.s1ap_InitialUEMessageIEs, &initiating_p->value);
        //s1ap_xer_print_s1ap_initialuemessage (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_INITIAL_UE_MESSAGE_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_UEContextReleaseRequest: {
        ret = s1ap_decode_s1ap_uecontextreleaserequesties (&message->msg.s1ap_UEContextReleaseRequestIEs, &initiating_p->value);
        //s1ap_xer_print_s1ap_uecontextreleaserequest (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_UE_CONTEXT_RELEASE_REQ_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_UECapabilityInfoIndication: {
        ret = s1ap_decode_s1ap_uecapabilityinfoindicationies (&message->msg.s1ap_UECapabilityInfoIndicationIEs, &initiating_p->value);
        //s1ap_xer_print_s1ap_uecapabilityinfoindication (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_UE_CAPABILITY_IND_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_NASNonDeliveryIndication: {
        ret = s1ap_decode_s1ap_nasnondeliveryindication_ies (&message->msg.s1ap_NASNonDeliveryIndication_IEs, &initiating_p->value);
        //s1ap_xer_print_s1ap_nasnondeliveryindication_ (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_NAS_NON_DELIVERY_IND_LOG;
      }
      break;

    default: {
        //OAILOG_ERROR (LOG_S1AP, "Unknown procedure ID (%d) for initiating message\n", (int)initiating_p->procedureCode);
        //AssertFatal (0, "Unknown procedure ID (%d) for initiating message\n", (int)initiating_p->procedureCode);
      }
      break;
  }

//  message_string_size = strlen (message_string);
//  message_p = itti_alloc_new_message_sized (TASK_S1AP, message_id, message_string_size + sizeof (IttiMsgText));
//  message_p->ittiMsg.s1ap_uplink_nas_log.size = message_string_size;
//  memcpy (&message_p->ittiMsg.s1ap_uplink_nas_log.text, message_string, message_string_size);
//  itti_send_msg_to_task (TASK_UNKNOWN, INSTANCE_DEFAULT, message_p);
//  free_wrapper ((void**)&message_string);
  return ret;
}

int s1ap_mme_cxx::s1ap_mme_decode_successfull_outcome (
  s1ap_message *message,
  S1ap_SuccessfulOutcome_t *successfullOutcome_p) {
  int                                     ret = RETURNerror;
  //MessageDef                             *message_p = NULL;
  //char                                   *message_string = NULL;
  //size_t                                  message_string_size = 0;
  //MessagesIds                             message_id = MESSAGES_ID_MAX;
  DevAssert (successfullOutcome_p != NULL);
  //message_string = calloc (10000, sizeof (char));
  //s1ap_string_total_size = 0;
  message->procedureCode = successfullOutcome_p->procedureCode;
  message->criticality = successfullOutcome_p->criticality;

  switch (successfullOutcome_p->procedureCode) {
    case S1ap_ProcedureCode_id_InitialContextSetup: {
        ret = s1ap_decode_s1ap_initialcontextsetupresponseies (&message->msg.s1ap_InitialContextSetupResponseIEs, &successfullOutcome_p->value);
        //s1ap_xer_print_s1ap_initialcontextsetupresponse (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_INITIAL_CONTEXT_SETUP_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_UEContextRelease: {
        ret = s1ap_decode_s1ap_uecontextreleasecompleteies (&message->msg.s1ap_UEContextReleaseCompleteIEs, &successfullOutcome_p->value);
        //s1ap_xer_print_s1ap_uecontextreleasecomplete (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_UE_CONTEXT_RELEASE_LOG;
      }
      break;

    case S1ap_ProcedureCode_id_E_RABSetup: {
        ret = s1ap_decode_s1ap_e_rabsetupresponseies (&message->msg.s1ap_E_RABSetupResponseIEs, &successfullOutcome_p->value);
        //s1ap_xer_print_s1ap_e_rabsetupresponse (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_E_RABSETUP_RESPONSE_LOG;
      }
      break;

    default: {
        //OAILOG_ERROR (LOG_S1AP, "Unknown procedure ID (%ld) for successfull outcome message\n", successfullOutcome_p->procedureCode);
      }
      break;
  }

  return ret;
}

int s1ap_mme_cxx::s1ap_mme_decode_unsuccessfull_outcome (
  s1ap_message *message,
  S1ap_UnsuccessfulOutcome_t *unSuccessfulOutcome_p) {
  int                                     ret = RETURNerror;
  //MessageDef                             *message_p = NULL;
  //char                                   *message_string = NULL;
  //size_t                                  message_string_size = 0;
  //MessagesIds                             message_id = MESSAGES_ID_MAX;
  DevAssert (unSuccessfulOutcome_p != NULL);
  //message_string = calloc (10000, sizeof (char));
  //s1ap_string_total_size = 0;
  message->procedureCode = unSuccessfulOutcome_p->procedureCode;
  message->criticality = unSuccessfulOutcome_p->criticality;

  switch (unSuccessfulOutcome_p->procedureCode) {
    case S1ap_ProcedureCode_id_InitialContextSetup: {
        ret = s1ap_decode_s1ap_initialcontextsetupfailureies (&message->msg.s1ap_InitialContextSetupFailureIEs, &unSuccessfulOutcome_p->value);
        //s1ap_xer_print_s1ap_initialcontextsetupfailure (s1ap_xer__print2sp, message_string, message);
        //message_id = S1AP_INITIAL_CONTEXT_SETUP_LOG;
      }
      break;

    default: {
        //OAILOG_ERROR (LOG_S1AP, "Unknown procedure ID (%d) for unsuccessfull outcome message\n", (int)unSuccessfulOutcome_p->procedureCode);
      }
      break;
  }

//  message_string_size = strlen (message_string);
//  message_p = itti_alloc_new_message_sized (TASK_S1AP, message_id, message_string_size + sizeof (IttiMsgText));
//  message_p->ittiMsg.s1ap_initial_context_setup_log.size = message_string_size;
//  memcpy (&message_p->ittiMsg.s1ap_initial_context_setup_log.text, message_string, message_string_size);
//  itti_send_msg_to_task (TASK_UNKNOWN, INSTANCE_DEFAULT, message_p);
//  free_wrapper ((void**)&message_string);
  return ret;
}

int s1ap_mme_cxx::s1ap_mme_decode_pdu (
  s1ap_message *message,
  const_bstring const raw) {
  S1AP_PDU_t                              pdu = {static_cast<S1AP_PDU_PR>(0)};
  S1AP_PDU_t                             *pdu_p = &pdu;
  asn_dec_rval_t                          dec_ret = {static_cast<asn_dec_rval_code_e>(0)};
  DevAssert (raw != NULL);
  memset ((void *)pdu_p, 0, sizeof (S1AP_PDU_t));
  dec_ret = aper_decode (NULL, &asn_DEF_S1AP_PDU, (void **)&pdu_p, bdata(raw), blength(raw), 0, 0);

  if (dec_ret.code != RC_OK) {
    OAILOG_ERROR (LOG_S1AP, "Failed to decode PDU\n");
    return -1;
  }

  message->direction = pdu_p->present;

  switch (pdu_p->present) {
    case S1AP_PDU_PR_initiatingMessage:
      return s1ap_mme_decode_initiating (message, &pdu_p->choice.initiatingMessage);

    case S1AP_PDU_PR_successfulOutcome:
      return s1ap_mme_decode_successfull_outcome (message, &pdu_p->choice.successfulOutcome);

    case S1AP_PDU_PR_unsuccessfulOutcome:
      return s1ap_mme_decode_unsuccessfull_outcome (message, &pdu_p->choice.unsuccessfulOutcome);

    default:
      OAILOG_ERROR (LOG_S1AP, "Unknown message outcome (%d) or not implemented", (int)pdu_p->present);
      break;
  }

  return RETURNerror;
}
