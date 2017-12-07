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
/*! \file oai_mme.c
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#include <string>
#include <vector>

#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <dlfcn.h>
#if HAVE_CONFIG_H
#  include "config.h"
#endif
#include "bstrlib.h"

#include "log.h"
#include "dynamic_memory_check.h"
#include "assertions.h"
#include "msc.h"
#include "3gpp_23.003.h"
#include "3gpp_24.008.h"
#include "3gpp_33.401.h"
#include "3gpp_24.007.h"
#include "3gpp_36.401.h"
#include "3gpp_36.331.h"
#include "security_types.h"
#include "common_types.h"
#include "common_defs.h"
#include "xml_msg_dump.h"
#include "mme_config.h"
#include "intertask_interface_init.h"
#include "timer.h"
#include "mme_app_extern.h"
/* FreeDiameter headers for support of S6A interface */
#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdcore.h>
#include "pid_file.h"

extern "C"{
  #include "sctp_primitives_server.h"
  #include "udp_primitives_server.h"
  #include "s1ap_mme.h"
  #include "nas_defs.h"
  #include "s11_mme.h"
  #include "mme_scenario_player_task.h"
  #include "s6a_defs.h"
}

#include "module.hpp"
#include "location.hpp"

// The list of modules to load while executing mme 
// same list as the MOD_LIST in mme's cinfiguration file 
std::vector<std::string> mod_list;


int
main (
  int argc,
  char *argv[])
{
  char   *pid_file_name = NULL;
  pid_file_name = get_exe_basename();
#if DAEMONIZE
  pid_t pid, sid; // Our process ID and Session ID
  // Fork off the parent process
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  // If we got a good PID, then we can exit the parent process.
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  // Change the file mode mask
  umask(0);
  // Create a new SID for the child process
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE); // Log the failure
  }
  // Change the current working directory
  if ((chdir("/")) < 0) {
    // Log the failure
    exit(EXIT_FAILURE);
  }
  /* Close out the standard file descriptors */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  if (! is_pid_file_lock_success(pid_file_name)) {
    closelog();
    free_wrapper((void**)&pid_file_name);
    exit (-EDEADLK);
  }
#else
  if (! is_pid_file_lock_success(pid_file_name)) {
    free_wrapper((void**)&pid_file_name);
    exit (-EDEADLK);
  }
#endif
  
  char *error;
  CHECK_INIT_RETURN (shared_log_init (MAX_LOG_PROTOS));
  CHECK_INIT_RETURN (OAILOG_INIT (LOG_SPGW_ENV, OAILOG_LEVEL_DEBUG, MAX_LOG_PROTOS));
  /*
   * Parse the command line for options and set the mme_config accordingly.
   */
  CHECK_INIT_RETURN (mme_config_parse_opt_line (argc, argv, &mme_config));
  /*
   * Calling each layer init function
   */
  CHECK_INIT_RETURN (itti_init (TASK_MAX, THREAD_MAX, MESSAGES_ID_MAX, tasks_info, messages_info,
#if ENABLE_ITTI_ANALYZER
          messages_definition_xml,
#else
          NULL,
#endif
          NULL));
  MSC_INIT (MSC_MME, THREAD_MAX + TASK_MAX);
#if TRACE_XML
  CHECK_INIT_RETURN (mme_scenario_player_init (&mme_config));
#endif

  // Array of pointers pointing to the address of the modules loaded by dlopen()
  void *handle[mod_list.size()];
  // Array of pointers pointing to the address of the created module instance
  module *mod[mod_list.size()];

  /*
   * Load mme modules according to MOD_LIST in mme's cinfiguration file 
   */
  for (int i = 0; i < mod_list.size(); i++) {
    /* open the module's shared library */
    // path for each shared library
    char path[150]= SHARED_LIBRARY_PATH;
    strcat(path, mod_list[i].c_str());
    // open the shared library
    handle[i] = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
    if(!handle[i]) {
      fprintf(stderr, "%s\n", dlerror());
      exit(1);
    }


    /* create module instance */
    //load the "create" class factory symbol
    create_t* create_mod =
                reinterpret_cast<create_t*> (dlsym(handle[i], "create"));
    if((error = dlerror()) != NULL) {
      fprintf(stderr, "%s\n", error);
      exit(1);
    }
    //create an instance of the class
    mod[i] = create_mod();
  }


  /*
   * Initialize and start all modules
   */
  CHECK_INIT_RETURN (sctp_init (&mme_config));
  CHECK_INIT_RETURN (udp_init ());
  for (int i = 0; i < mod_list.size(); i++) { 
    CHECK_INIT_RETURN (mod[i]->init(&mme_config));
  }
  XML_MSG_DUMP_INIT();
  

  /*
   * Handle signals here
   */
  itti_wait_tasks_end ();
  pid_file_unlock();
  free_wrapper((void**)&pid_file_name);
  XML_MSG_DUMP_EXIT();


  /*
   * Destroy(free) each module's instance
   */
  for (int i = 0; i < mod_list.size(); i++) {
    //load the "destroy" class factory symbol
    destroy_t* destroy_mod =
                reinterpret_cast<destroy_t*> (dlsym(handle[i], "destroy"));
    if((error = dlerror()) != NULL) {
      fprintf(stderr, "%s\n", error);
      exit(1);
    }
    destroy_mod(mod[i]);
  }


  /*
   * Close shared libraries
   */
  for (int i = 0; i < mod_list.size(); i++) {
    dlclose(handle[i]);
  }
  
  
  return 0;

}
