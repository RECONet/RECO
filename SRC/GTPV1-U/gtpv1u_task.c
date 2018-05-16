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
/*! \file gtpv1u_task.c
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>
#include "bstrlib.h"
#include "queue.h"

#include "dynamic_memory_check.h"
#include "log.h"
#include "msc.h"
#include "assertions.h"
#include "conversions.h"
#include "mme_scenario_player.h"
#include "3gpp_23.003.h"
#include "3gpp_24.008.h"
#include "3gpp_33.401.h"
#include "3gpp_24.007.h"
#include "3gpp_36.401.h"
#include "3gpp_36.331.h"
#include "3gpp_24.301.h"
#include "security_types.h"
#include "common_types.h"
#include "common_defs.h"
#include "intertask_interface.h"
#include "gtpv1u.h"
#include "sgw_config.h"
#include "pgw_config.h"
#include "spgw_config.h"
#include "gtpv1u_sgw_defs.h"
#include "gtp_mod_kernel.h"
#include "sgw.h"

extern sgw_app_t                               sgw_app;

struct gtp1_header_short {	/*    Descriptions from 3GPP 29060 */
	uint8_t flags;		/* 01 bitfield, with typical values */
	/*    001..... Version: 1 */
	/*    ...1.... Protocol Type: GTP=1, GTP'=0 */
	/*    ....0... Spare = 0 */
	/*    .....0.. Extension header flag: 0 */
	/*    ......0. Sequence number flag: 0 */
	/*    .......0 PN: N-PDU Number flag */
	uint8_t type;		/* 02 Message type. T-PDU = 0xff */
	uint16_t length;	/* 03 Length (of IP packet or signalling) */
	uint32_t tei;		/* 05 - 08 Tunnel Endpoint ID */
};

struct gtp1_header_long {	/*    Descriptions from 3GPP 29060 */
	uint8_t flags;		/* 01 bitfield, with typical values */
	/*    001..... Version: 1 */
	/*    ...1.... Protocol Type: GTP=1, GTP'=0 */
	/*    ....0... Spare = 0 */
	/*    .....0.. Extension header flag: 0 */
	/*    ......1. Sequence number flag: 1 */
	/*    .......0 PN: N-PDU Number flag */
	uint8_t type;		/* 02 Message type. T-PDU = 0xff */
	uint16_t length;	/* 03 Length (of IP packet or signalling) */
	uint32_t tei;		/* 05 Tunnel Endpoint ID */
	uint16_t seq;		/* 10 Sequence Number */
	uint8_t npdu;		/* 11 N-PDU Number */
	uint8_t next;		/* 12 Next extension header type. Empty = 0 */
};

struct gtp1_packet_short {
	struct gtp1_header_short h;
	uint8_t p[65536];
} __attribute__ ((packed));

struct gtp1_packet_long {
	struct gtp1_header_long h;
	uint8_t p[65536];
} __attribute__ ((packed));


static void *gtpv1u_thread (void *args);

//------------------------------------------------------------------------------
static void  *gtpv1u_thread (void *args)
{
  itti_mark_task_ready (TASK_GTPV1_U);

  gtpv1u_data_t * gtpv1u_data = (gtpv1u_data_t*)args;

  while (1) {
    /*
     * Trying to fetch a message from the message queue.
     * * * * If the queue is empty, this function will block till a
     * * * * message is sent to the task.
     */
    unsigned char buffer[8196];
    struct sockaddr_in peer;
    socklen_t peerlen;
    int status;
    peerlen = sizeof(peer);
    status = recvfrom(gtpv1u_data->fd1u, buffer, sizeof(buffer), 0, (struct sockaddr*)&peer, &peerlen);
    struct gtp1_header_short * pheader = (struct gtp1_header_short *)(buffer);  
    struct gtp1_header_long * pheader_l = (struct gtp1_header_long *)(buffer);

    /* Send Echo Response */
    struct gtp1_packet_long packet;
    unsigned int length = 12;
    struct gtp1_header_long *gtp1_default = (struct gtp1_header_long *)&packet;

    memset(gtp1_default, 0, sizeof(struct gtp1_header_long));
    gtp1_default->flags = 0x32;	
    gtp1_default->type = 2;
    /* gtpie_tv1 function is not implemented */
    
    packet.h.length = htons(length - 8);
    packet.h.seq = (pheader_l->seq);
    /* packet.h.tei = htonl(pdp->teid_gn); TEI? */

    /* fill Recovery IE */
    length += 2;
    packet.h.length = htons(length - 8);
    packet.p[0]=(uint8_t) 14;
    packet.p[1]=0;

    sendto(gtpv1u_data->fd1u, &packet, length, 0, (struct sockaddr *)&peer, sizeof(struct sockaddr_in)) ;

  }

  return NULL;
}

#include <arpa/inet.h>

//------------------------------------------------------------------------------
int gtpv1u_init (spgw_config_t *spgw_config)
{
  OAILOG_DEBUG (LOG_GTPV1U , "Initializing GTPV1U interface\n");
  memset (&sgw_app.gtpv1u_data, 0, sizeof (sgw_app.gtpv1u_data));
  sgw_app.gtpv1u_data.sgw_ip_address_for_S1u_S12_S4_up = sgw_app.sgw_ip_address_S1u_S12_S4_up;

  // START-GTP quick integration only for evaluation purpose
  // Clean hard previous mappings.
  if (spgw_config->pgw_config.use_gtp_kernel_module) {
    if (spgw_config->pgw_config.enable_loading_gtp_kernel_module) {
      int rv = system ("rmmod gtp");
      rv = system ("modprobe gtp");
      if (rv != 0) {
        //OAILOG_CRITICAL (TASK_GTPV1_U, "ERROR in loading gtp kernel module (check if built in kernel)\n");
        return -1;
      }
    }

    AssertFatal(spgw_config->pgw_config.num_ue_pool == 1, "No more than 1 UE pool allowed actually");
    for (int i = 0; i < spgw_config->pgw_config.num_ue_pool; i++) {
      // GTP device same MTU as SGi.
      gtp_mod_kernel_init(&sgw_app.gtpv1u_data.fd0, &sgw_app.gtpv1u_data.fd1u,
          &spgw_config->pgw_config.ue_pool_addr[i],
          spgw_config->pgw_config.ue_pool_mask[i],
          spgw_config->pgw_config.ipv4.mtu_SGI);
    }
    // END-GTP quick integration only for evaluation purpose

    if (itti_create_task (TASK_GTPV1_U, &gtpv1u_thread, &sgw_app.gtpv1u_data) < 0) {
      OAILOG_ERROR (LOG_GTPV1U , "gtpv1u phtread_create: %s", strerror (errno));
      gtp_mod_kernel_stop();
      return -1;
    }
  }
  OAILOG_DEBUG (LOG_GTPV1U , "Initializing GTPV1U interface: DONE\n");
  return 0;
}

//------------------------------------------------------------------------------
void gtpv1u_exit (gtpv1u_data_t * const gtpv1u_data)
{

  if (spgw_config.pgw_config.use_gtp_kernel_module) {
    gtp_mod_kernel_stop();
  }
  // END-GTP quick integration only for evaluation purpose
  itti_exit_task ();
}
