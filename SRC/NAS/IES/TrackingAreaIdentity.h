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

#ifndef TRACKING_AREA_IDENTITY_SEEN
#define TRACKING_AREA_IDENTITY_SEEN

#define TRACKING_AREA_IDENTITY_MINIMUM_LENGTH 6
#define TRACKING_AREA_IDENTITY_MAXIMUM_LENGTH 6

#define INVALID_TAC_0000                  (uint16_t)0x0000 /*!< \brief  The following are reserved hexadecimal values of the TAC: 0000, and FFFE.   */
#define INVALID_TAC_FFFE                  (uint16_t)0xFFFE /*!< \brief  The following are reserved hexadecimal values of the TAC: 0000, and FFFE.   */

typedef uint16_t    tac_t;                                 /*!< \brief  Tracking Area Code (TAC) is a fixed length code (of 2 octets) identifying
                                                                        a Tracking Area within a PLMN. This part of the tracking area identification
                                                                        shall be coded using a full hexadecimal representation. The following are
                                                                        reserved hexadecimal values of the TAC: 0000, and FFFE.   */
typedef struct tai_s {
  uint8_t  mcc_digit2:4;
  uint8_t  mcc_digit1:4;
  uint8_t  mnc_digit3:4;
  uint8_t  mcc_digit3:4;
  uint8_t  mnc_digit2:4;
  uint8_t  mnc_digit1:4;
  tac_t    tac;
} tai_t;


/* Checks Mobile Country Code equality */
#define MCCS_ARE_EQUAL(n1, n2)  (((n1).mcc_digit1 == (n2).mcc_digit1) && \
                                 ((n1).mcc_digit2 == (n2).mcc_digit2) && \
                                 ((n1).mcc_digit3 == (n2).mcc_digit3))

/* Checks Mobile Network Code equality */
#define MNCS_ARE_EQUAL(n1, n2)  (((n1).mnc_digit1 == (n2).mnc_digit1) &&  \
                                 ((n1).mnc_digit2 == (n2).mnc_digit2) &&  \
                                 ((n1).mnc_digit3 == (n2).mnc_digit3))

/* Checks PLMNs equality */
#define PLMNS_ARE_EQUAL(p1, p2) ((MCCS_ARE_EQUAL((p1),(p2))) && \
                                 (MNCS_ARE_EQUAL((p1),(p2))))

// MCC digit 2 MCC digit 1 octet 1
// MNC digit 3 MCC digit 3 octet 2
// MNC digit 2 MNC digit 1 octet 3
// The coding of this field is the responsibility of each administration but BCD coding
// shall be used. The MNC shall consist of 2 or 3 digits. If a network operator decides
// to use only two digits in the MNC, bits 5 to 8 of octet 2 shall be coded as "1111".
#define PLMN_FMT "%c%c%c.%c%c%c"
#define PLMN_ARG(PlMn_PtR) \
  (char)((PlMn_PtR)->mcc_digit1+0x30), (char)((PlMn_PtR)->mcc_digit2+0x30), (char)((PlMn_PtR)->mcc_digit3+0x30), \
  (char)((PlMn_PtR)->mnc_digit1+0x30), (char)((PlMn_PtR)->mnc_digit2+0x30), \
  (((PlMn_PtR)->mnc_digit3) == 0x0f) ? ' ':(char)((PlMn_PtR)->mnc_digit3+0x30)

/* Checks PLMN validity !?! */
#define PLMN_IS_VALID(plmn) (((plmn).mcc_digit1 &    \
                              (plmn).mcc_digit2 &    \
                              (plmn).mcc_digit3) != 0x0F)


/* Checks TAIs equality */
#define TAIS_ARE_EQUAL(t1, t2)  ((PLMNS_ARE_EQUAL((t1),(t2))) && \
                                 ((t1).tac == (t2).tac))
#define TAC_FMT "0x%"PRIx16
#define TAI_FMT PLMN_FMT"-"TAC_FMT
#define TAI_ARG(tAi_PtR) \
  PLMN_ARG((tAi_PtR)),\
  (tAi_PtR)->tac

/* Checks TAC validity */
#define TAC_IS_VALID(tac)   (((tac) != INVALID_TAC_0000) && ((tac) != INVALID_TAC_FFFE))

/* Checks TAI validity */
#define TAI_IS_VALID(tai)   (PLMN_IS_VALID((tai).plmn) &&   \
                             TAC_IS_VALID((tai).tac))



// defined in 24.008_xml.h
#if !defined(MOBILE_COUNTRY_CODE_ATTR_XML_STR)
#  define MOBILE_COUNTRY_CODE_ATTR_XML_STR              "mcc"
#endif

// defined in 24.008_xml.h
#if !defined(MOBILE_NETWORK_CODE_ATTR_XML_STR)
#  define MOBILE_NETWORK_CODE_ATTR_XML_STR              "mnc"
#endif


#ifdef __cplusplus
extern "C" {
#endif
int encode_tracking_area_identity(tai_t *tai, uint8_t iei, uint8_t *buffer, uint32_t len);
int decode_tracking_area_identity(tai_t *tai, uint8_t iei, uint8_t *buffer, uint32_t len);
void clear_tai(tai_t * const tai);

#ifdef __cplusplus
}
#endif

#endif /* TRACKING AREA IDENTITY_SEEN */

