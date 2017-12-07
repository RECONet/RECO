#include "module.hpp"
#include "location.hpp"
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>
#include "log.h"
#include "assertions.h"
#include "dynamic_memory_check.h"
#include "intertask_interface.h"
#include "common_defs.h"
#include "msc.h"
#include "itti_free_defined_msg.h"
#include "mme_config.h"
#include "timer.h"
#include "xml_msg_dump.h"

class location_update : public module {
  public:
    //creating mme_app module thread
    virtual int init(mme_config_t * mme_config_p) {
        return location_update_init (mme_config_p);
    }
};
/* the class factories */
extern "C" module* create() {
    return new location_update;
}
extern "C" void destroy(module* p) {
    delete p;
}

void *location_update_thread (void *args)
{
  itti_mark_task_ready (TASK_LOCATION_UPDATE);

  while (1) {
    MessageDef                             *received_message_p = NULL;

    /*
     * Trying to fetch a message from the message queue.
     * If the queue is empty, this function will block till a
     * message is sent to the task.
     */
    itti_receive_msg (TASK_LOCATION_UPDATE, &received_message_p);
    DevAssert (received_message_p );

    switch (ITTI_MSG_ID (received_message_p)) {

    case S6A_UPDATE_LOCATION_ANS:{
        /*
         * We received the update location answer message from HSS -> Handle it
         */
        //mme_app_handle_s6a_update_location_ans (&received_message_p->ittiMsg.s6a_update_location_ans);
      }
      break;

    default:{
      OAILOG_DEBUG (LOG_LOCATION_UPDATE, "Unkwnon message ID %d:%s\n", ITTI_MSG_ID (received_message_p), ITTI_MSG_NAME (received_message_p));
        AssertFatal (0, "Unkwnon message ID %d:%s\n", ITTI_MSG_ID (received_message_p), ITTI_MSG_NAME (received_message_p));
      }
      break;
    }

    itti_free_msg_content(received_message_p);
    itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
    received_message_p = NULL;
  }

  return NULL;
}

int location_update_init (const mme_config_t * mme_config_p)
{
  /*
   * Create the thread associated with location update
   */
  if (itti_create_task (TASK_LOCATION_UPDATE, &location_update_thread, NULL) < 0) {
    OAILOG_ERROR (LOG_LOCATION_UPDATE, "LOCATION UPDATE create task failed\n");
    OAILOG_FUNC_RETURN (LOG_LOCATION_UPDATE, RETURNerror);
  }
  printf("Initializing LOCATION UPDATE task interface: DONE\n");
  OAILOG_DEBUG (LOG_LOCATION_UPDATE, "Initializing LOCATION UPDATE task interface: DONE\n");
  return 0;
}

//------------------------------------------------------------------------------
void location_update_exit (void)
{
  
}
