#ifndef S6A_LOCATION_MESSAGE_H
#define S6A_LOCATION_MESSAGE_H

#define RETURNok 0
#define RETURNerror -1
#define IMSI_BCD_DIGITS_MAX              15
#define ARD_MAX                             (1U << 6)
#define ACCESS_POINT_NAME_MAX_LENGTH    100
#define PRIORITY_LEVEL_MAX (15)
#define PRIORITY_LEVEL_MIN (1)

typedef struct plmn_s {
  uint8_t mcc_digit2:4;
  uint8_t mcc_digit1:4;
  uint8_t mnc_digit3:4;
  uint8_t mcc_digit3:4;
  uint8_t mnc_digit2:4;
  uint8_t mnc_digit1:4;
} plmn_t;

typedef enum {
  RAT_WLAN           = 0,
  RAT_VIRTUAL        = 1,
  RAT_UTRAN          = 1000,
  RAT_GERAN          = 1001,
  RAT_GAN            = 1002,
  RAT_HSPA_EVOLUTION = 1003,
  RAT_EUTRAN         = 1004,
  RAT_CDMA2000_1X    = 2000,
  RAT_HRPD           = 2001,
  RAT_UMB            = 2002,
  RAT_EHRPD          = 2003,
} rat_type_t;

typedef struct s6a_update_location_req_s {
#define SKIP_SUBSCRIBER_DATA (0x1)
  unsigned skip_subscriber_data:1;
#define INITIAL_ATTACH       (0x1)
  unsigned initial_attach:1;

  char       imsi[IMSI_BCD_DIGITS_MAX + 1]; // username
  uint8_t    imsi_length;               // username

  plmn_t     visited_plmn;              // visited plmn id
  rat_type_t rat_type;                  // rat type

  // missing                           // origin host
  // missing                           // origin realm

  // missing                           // destination host
  // missing                           // destination realm
} s6a_update_location_req_t;

typedef struct s6a_update_location_ans_s {
  s6a_result_t        result;              // Result of the update location request procedure
  subscription_data_t subscription_data;   // subscriber status,
  // Maximum Requested Bandwidth Uplink, downlink
  // access restriction data
  // msisdn
  //apn_config_profile_t  apn_config_profile;// APN configuration profile

  network_access_mode_t access_mode;
  rau_tau_timer_t       rau_tau_timer;
  char    imsi[IMSI_BCD_DIGITS_MAX + 1];
  uint8_t imsi_length;


} s6a_update_location_ans_t;

#endif
