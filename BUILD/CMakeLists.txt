################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under 
# the Apache License, Version 2.0  (the "License"); you may not use this file
# except in compliance with the License.  
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################
# Author: laurent THOMAS, Lionel GAUTHIER
###############################################################################
cmake_minimum_required (VERSION 2.8)
#############################################
# Base directories, compatible with legacy OAI building
################################################
set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/BUILD)
set (OPENAIRCN_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
project (OpenAirInterface)
###########################################
# macros to define options as there is numerous options in oai
################################################
macro(add_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  add_definitions("-D${name}=${value}")
endmacro(add_option)
macro(add_boolean_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  set_property(CACHE ${name} PROPERTY TYPE BOOL)
  if (${value})
    add_definitions("-D${name}=1")
  else (${value})
    add_definitions("-D${name}=0")
  endif (${value})
endmacro(add_boolean_option)
macro(add_integer_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  add_definitions("-D${name}=${value}")
endmacro(add_integer_option)
macro(add_list1_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  set_property(CACHE ${name} PROPERTY STRINGS ${ARGN})
  if(NOT "${value}" STREQUAL "False")
    add_definitions("-D${name}=${value}")
  endif()
endmacro(add_list1_option)
macro(add_list2_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  set_property(CACHE ${name} PROPERTY STRINGS ${ARGN})
  if(NOT "${value}" STREQUAL "False")
    add_definitions("-D${value}=1")
  endif()
endmacro(add_list2_option)
macro(add_list_string_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  set_property(CACHE ${name} PROPERTY STRINGS ${ARGN})
  if(NOT "${value}" STREQUAL "False")
    add_definitions("-D${name}=\"${value}\"")
  endif()
endmacro(add_list_string_option)
################################################################
# Compilation flags
################################################################
# Build type
################################################################
if (CMAKE_BUILD_TYPE STREQUAL "")
   set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
add_list_string_option(CMAKE_BUILD_TYPE "RelWithDebInfo" "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." Debug Release RelWithDebInfo MinSizeRel)
Message("Build type is ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_boolean_option(LOG_OAI True         "Thread safe logging API")
  add_boolean_option(LOG_OAI_MINIMAL True "Thread safe logging API, log only levels above NOTICE")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_boolean_option(LOG_OAI True "Thread safe logging API")
endif()
################################################################
# Processor architecture
################################################################
Message("Architecture is ${CMAKE_SYSTEM_PROCESSOR}")
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l")
  set(C_FLAGS_PROCESSOR "-gdwarf-2 -mfloat-abi=hard -mfpu=neon -lgcc -lrt")
else (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l")
  set(C_FLAGS_PROCESSOR "-msse4.2")
endif()
#
set(CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} ${C_FLAGS_PROCESSOR} -std=gnu99 -Wall -Wstrict-prototypes -fno-strict-aliasing -rdynamic -funroll-loops -Wno-packed-bitfield-compat -fPIC -g")
# add autoTOOLS definitions that were maybe used!
set(CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FCNTL_H=1 -DHAVE_ARPA_INET_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYS_SOCKET_H=1 -DHAVE_STRERROR=1 -DHAVE_SOCKET=1 -DHAVE_MEMSET=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_STDLIB_H=1 -DHAVE_MALLOC=1"
)
# set a flag for changes in the source code
# these changes are related to hardcoded path to include .h files
add_definitions(-DCMAKER)
add_definitions(-DHAVE_LIBSCTP)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fstack-protector-all -g -DMALLOC_CHECK_=3 -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -g -DMALLOC_CHECK_=3  -DDEBUG_IS_ON -O1") 

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g  -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -g -DMALLOC_CHECK_=3  -DDEBUG_IS_ON -O1")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -g  -DDEBUG_IS_ON -O1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++11" )  # disable warning messages for temporary debugging usage
################################################################
# gRPC
################################################################
find_package(Protobuf REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(PROTO_PATH "${CMAKE_SOURCE_DIR}/protos")
set(LOCATION_PROTO "${PROTO_PATH}/location.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})
message("CMAKE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:${PROTO_PATH}")

set(LOCATION_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/location.pb.cc")
set(LOCATION_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/location.pb.h")
set(LOCATION_GRPC_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/location.grpc.pb.cc")
set(LOCATION_GRPC_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/location.grpc.pb.h")
add_custom_command(
    OUTPUT "${LOCATION_PB_H_FILE}"
           "${LOCATION_PB_CPP_FILE}"
           "${LOCATION_GRPC_PB_H_FILE}"
           "${LOCATION_GRPC_PB_CPP_FILE}"
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS "--proto_path=${PROTO_PATH}"
         "--cpp_out=${GENERATED_PROTOBUF_PATH}"
         "${LOCATION_PROTO}"
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS "--proto_path=${PROTO_PATH}"
         "--grpc_out=${GENERATED_PROTOBUF_PATH}"
         "--plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin"
         "${LOCATION_PROTO}"
    )
set(GENERATED_PROTOBUF_FILES ${LOCATION_PB_H_FILE} ${LOCATION_PB_CPP_FILE}
    ${LOCATION_GRPC_PB_H_FILE} ${LOCATION_GRPC_PB_CPP_FILE})

include_directories(${GENERATED_PROTOBUF_PATH})
link_libraries(grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY})


################################################################
# Git Version 
################################################################
set(GIT_BRANCH        "UNKNOWN")
set(GIT_COMMIT_HASH   "UNKNOWN")
set(GIT_COMMIT_DATE   "UNKNOWN")
find_package(Git)
if(GIT_FOUND)
  message("git found: ${GIT_EXECUTABLE}")
  # Get the current working branch
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  # Get the latest abbreviated commit hash of the working branch
  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  
  # Get the latest commit date of the working branch
  execute_process(
    COMMAND git log -1 --format=%cd
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()
add_list_string_option(PACKAGE_NAME "NotDefined" "As per attribute name")
add_definitions("-DPACKAGE_VERSION=\"Branch: ${GIT_BRANCH} Abrev. Hash: ${GIT_COMMIT_HASH} Date: ${GIT_COMMIT_DATE}\"")
add_definitions("-DPACKAGE_BUGREPORT=\"openaircn-user@lists.eurecom.fr\"")
###############################################################################
# Include CMake modules to find other library
###############################################################################
set(CMAKE_MODULE_PATH "${BUILD_TOP_DIR}/MODULES/")
###############################################################################
# Include Check library for unit tests
###############################################################################
include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)
###############################################################################
# Debug and build related options
###############################################################################
add_boolean_option( DAEMONIZE                       False    "Fork executable if true")
add_boolean_option( DISABLE_ITTI_DETECT_SUB_TASK_ID False    " ")
add_boolean_option( DISPLAY_LICENCE_INFO            False    "If a module has a licence banner to show")
add_boolean_option( ENABLE_ITTI                     True     "ITTI is internal messaging, should remain enabled for most targets")
add_boolean_option( ENABLE_ITTI_ANALYZER            False    "ITTI Analyzer is a GUI based on GTK that displays the ITTI messages exchanged between tasks")
add_integer_option( ITTI_TASK_STACK_SIZE            0        "pthread allocated stack size in bytes of an ITTI task, if 0, use default stack size ") 
add_boolean_option( ITTI_LITE                       False    "Do not use ITTI systematically for each message exchanged between layer modules") 
add_boolean_option( MESSAGE_CHART_GENERATOR         False    "For generating sequence diagrams")
# NAS LAYER OPTIONS
##########################
add_boolean_option( EPC_BUILD                       False    "BUILD MME-xGW executable")
add_boolean_option( GTPV1U_LINEAR_TEID_ALLOCATION   False    "Teid allocation id mode versus pseudo random")
# S1AP LAYER OPTIONS
##########################
add_boolean_option(S1AP_DEBUG_LIST                  False    "Traces, option to be removed soon")
# SCTP LAYER OPTIONS
##########################
add_boolean_option(SCTP_DUMP_LIST                   False    "Traces, option to be removed soon")
add_boolean_option( TRACE_HASHTABLE                 False    "Trace hashtables operations ")
add_boolean_option( LOG_OAI                         False    "Thread safe logging utility")
add_boolean_option( LOG_OAI_CLEAN_HARD              False    "Thread safe logging utility option for cleaning inner structs")
add_boolean_option( SECU_DEBUG                      False    "Traces, option to be removed soon")
add_boolean_option( TRACE_3GPP_SPEC                 True     "Log hits of 3GPP specifications requirements")
add_boolean_option( TRACE_XML                       False     "Log some messages in XML (messages necessary for MME scenario player)")
set (ITTI_DIR ${OPENAIRCN_DIR}/SRC/COMMON/ITTI)
if (${ENABLE_ITTI})
  set(ITTI_FILES
    # add .h files if depend on (this one is generated)
    ${ITTI_DIR}/intertask_interface.h
    ${ITTI_DIR}/intertask_interface.c
    ${ITTI_DIR}/backtrace.c
    ${ITTI_DIR}/memory_pools.c
    ${ITTI_DIR}/signals.c
    ${ITTI_DIR}/timer.c
    )
  if (${ENABLE_ITTI_ANALYZER})
    set(ITTI_FILES
    ${ITTI_FILES}
    ${ITTI_DIR}/intertask_interface_dump.c 
    ${OPENAIRCN_BIN_DIR}/messages_xml.h )
    
    include_directories ("${OPENAIRCN_BIN_DIR}")
  endif (${ENABLE_ITTI_ANALYZER})
    
  add_library(ITTI ${ITTI_FILES})
    
  set(ITTI_LIB ITTI)
endif (${ENABLE_ITTI})
add_library(3GPP_TYPES
  ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_cc_ies.c
  ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_common_ies.c
  ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_mm_ies.c
  ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_gmm_ies.c
  ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_gprs_common_ies.c
  ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_sm_ies.c)
set(3GPP_TYPES_LIB 3GPP_TYPES)
###############################################################################
# ASN.1 grammar C code generation & dependancies
###############################################################################
# A difficulty: asn1c generates C code of a un-predictable list of files
# so, generate the c from asn1c once at cmake run time
# So, if someone modify the asn.1 source file in such as way that it will create
# (so creating new asn.1 objects instead of modifying the object attributes)
# New C code source file, cmake must be re-run (instead of re-running make only)
###############################################################################
set(asn1c_call "${BUILD_TOP_DIR}/TOOLS/generate_asn1")
set(asn1_generated_dir ${OPENAIRCN_BIN_DIR})
###############################################################################
# S1AP
# Same limitation as described in RRC: unknown generated file list
# so we generate it at cmake time
###############################################################################
add_list1_option(S1AP_VERSION R10 "S1AP Asn.1 grammar version" R8 R9 R10)
set(S1AP_DIR ${OPENAIRCN_DIR}/SRC/S1AP)
set(ASN1RELDIR R10.5)
set(S1AP_ASN_DIR ${S1AP_DIR}/MESSAGES/ASN1/${ASN1RELDIR})
set(S1AP_ASN_FILES
  ${S1AP_ASN_DIR}/S1AP-CommonDataTypes.asn
  ${S1AP_ASN_DIR}/S1AP-Constants.asn
  ${S1AP_ASN_DIR}/S1AP-IEs.asn
  ${S1AP_ASN_DIR}/S1AP-PDU.asn
  )
set(S1AP_C_DIR ${asn1_generated_dir}/${ASN1RELDIR})
execute_process(COMMAND ${asn1c_call} ${S1AP_C_DIR} ${S1AP_ASN_FILES})
file(GLOB S1AP_source ${S1AP_C_DIR}/*.c)
set(S1AP_OAI_generated
  ${S1AP_C_DIR}/s1ap_decoder.c
  ${S1AP_C_DIR}/s1ap_encoder.c
  ${S1AP_C_DIR}/s1ap_xer_print.c
  ${S1AP_C_DIR}/s1ap_ies_defs.h
  )
file(GLOB s1ap_h ${S1AP_C_DIR}/*.h)
set(s1ap_h ${s1ap_h} )
add_custom_command (
  OUTPUT ${S1AP_OAI_generated}
  COMMAND ${asn1c_call} ${S1AP_C_DIR} ${S1AP_ASN_FILES}
  COMMAND python ${S1AP_DIR}/MESSAGES/ASN1/asn1tostruct.py -f${S1AP_ASN_DIR}/S1AP-PDU-Contents.asn -o${S1AP_C_DIR}
  DEPENDS ${S1AP_ASN_FILES}
  )
add_library(S1AP_LIB
  ${S1AP_OAI_generated}
  ${S1AP_source}
  ${S1AP_DIR}/s1ap_common.c
  )
include_directories ("${S1AP_C_DIR}")
include_directories ("${S1AP_DIR}")
add_library(S1AP MODULE
  ${S1AP_C_DIR}/s1ap_ies_defs.h
  ${S1AP_DIR}/s1ap_mme_cxx.cpp
  ${S1AP_DIR}/s1ap_mme_encoder.cpp
  ${S1AP_DIR}/s1ap_mme_decoder.cpp
  ${S1AP_DIR}/s1ap_mme_handlers.cpp
  ${S1AP_DIR}/s1ap_mme_nas_procedures.cpp
  ${S1AP_DIR}/s1ap_mme.cpp
  ${S1AP_DIR}/s1ap_mme_itti_messaging.cpp
  ${S1AP_DIR}/s1ap_mme_retransmission.cpp
  ${S1AP_DIR}/s1ap_mme_ta.cpp
  ${S1AP_DIR}/s1ap.cpp
  ${S1AP_DIR}/s1ap_config.cpp
  #${S1AP_DIR}/helloworld.cpp
  )

###############################################################################
# add the binary tree to the search path for include files
###############################################################################
# We will find ConfigOAI.h after generation in target directory
###############################################################################
include_directories("${OPENAIRCN_BIN_DIR}")
###############################################################################
# add directories to find all include files
# the internal rule is to use generic names such as defs.h
# but to make it uniq name as adding the relative path in the include directive
# example: #include "RRC/LITE/defs.h"
#find_path (include_dirs_all *.h ${OPENAIRCN_DIR}/SRC)
#find_path (include_dirs_all *.h PATHS /usr/include NO_CMAKE_PATH)
#include_directories("${include_dirs_all}")
###############################################################################
include_directories ("${OPENAIRCN_DIR}/SRC/OAI_MME")
include_directories("${OPENAIRCN_DIR}/SRC/COMMON")
include_directories("${OPENAIRCN_DIR}/SRC/UTILS")
include_directories("${OPENAIRCN_DIR}/SRC/COMMON/ITTI")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/API/MME")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/API/NETWORK")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/EMM/MSG")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/ESM/MSG")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/IES")
include_directories("${OPENAIRCN_DIR}/SRC/NAS/UTIL")
include_directories("${OPENAIRCN_DIR}/SRC/SECU")
include_directories("${OPENAIRCN_DIR}/SRC/SCTP")
include_directories("${OPENAIRCN_DIR}/SRC/S1AP")
include_directories("${OPENAIRCN_DIR}/SRC/UDP")
include_directories("${OPENAIRCN_DIR}/SRC/GTPV1-U")
include_directories("${OPENAIRCN_DIR}/SRC/UTILS/HASHTABLE")
include_directories("${OPENAIRCN_DIR}/SRC/UTILS/MSC")
include_directories("${OPENAIRCN_DIR}/SRC/UTILS/BSTR")
include_directories("${OPENAIRCN_DIR}/SRC/SGW")
include_directories("${OPENAIRCN_DIR}/SRC/MME_APP")
include_directories("${OPENAIRCN_DIR}/SRC/S6A")
include_directories("${OPENAIRCN_DIR}/SRC")
###############################################################################
# Utilities Library
###############################################################################
add_library(BSTR
  ${OPENAIRCN_DIR}/SRC/UTILS/BSTR/bstraux.c
  ${OPENAIRCN_DIR}/SRC/UTILS/BSTR/bstrlib.c
  ${OPENAIRCN_DIR}/SRC/UTILS/BSTR/buniutil.c
  ${OPENAIRCN_DIR}/SRC/UTILS/BSTR/utf8util.c
)
include_directories(${OPENAIRCN_DIR}/SRC/UTILS/BSTR)
add_library(HASHTABLE
  ${OPENAIRCN_DIR}/SRC/UTILS/HASHTABLE/hashtable.c
  ${OPENAIRCN_DIR}/SRC/UTILS/HASHTABLE/obj_hashtable.c
)
include_directories(${OPENAIRCN_DIR}/SRC/UTILS/HASHTABLE)
if (MESSAGE_CHART_GENERATOR)
  add_library(MSC  
    ${OPENAIRCN_DIR}/SRC/UTILS/MSC/msc.c
  )
  set(MSC_LIB MSC)
endif()
include_directories(${OPENAIRCN_DIR}/SRC/UTILS/MSC/msc)
if (TRACE_XML)
  add_library(XML_MSG_DUMP  
    ${OPENAIRCN_DIR}/SRC/UTILS/xml_msg_dump.c
    ${OPENAIRCN_DIR}/SRC/UTILS/xml_msg_dump_itti.c
    ${OPENAIRCN_DIR}/SRC/NAS/API/NETWORK/nas_message_xml.c
  )
  set(XML_MSG_DUMP_LIB XML_MSG_DUMP)
  
 add_library(3GPP_TYPES_XML
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_23.003_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.007_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_cc_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_common_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_mm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_gmm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_gprs_common_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_24.008_sm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_29.274_xml.c
    ${OPENAIRCN_DIR}/SRC/NAS/3gpp_24.301_common_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/NAS/3gpp_24.301_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/NAS/3gpp_24.301_emm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/NAS/3gpp_24.301_esm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/NAS/3gpp_24.301_emm_msg_xml.c
    ${OPENAIRCN_DIR}/SRC/NAS/3gpp_24.301_esm_msg_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_36.331_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_36.401_xml.c
    ${OPENAIRCN_DIR}/SRC/COMMON/3gpp_36.413_xml.c)
  set(3GPP_TYPES_XML_LIB 3GPP_TYPES_XML)  
  add_library(SCENARIO_PLAYER
    ${OPENAIRCN_DIR}/SRC/SECU/etsi_ts_135_206_V10.0.0_annex3.c
    ${OPENAIRCN_DIR}/SRC/SECU/usim_authenticate.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/mme_scenario_player_load.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/mme_scenario_player_play.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/mme_scenario_player_rx_itti.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/mme_scenario_player_task.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_23.003_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.007_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.008_common_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.008_sm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.301_emm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.301_emm_msg_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.301_esm_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.301_esm_msg_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_24.301_ies_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_36.401_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_3gpp_36.413_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_common_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_nas_message_xml.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_xml_compare.c
    ${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/sp_xml_load.c
    ${OPENAIRCN_DIR}/SRC/UTILS/xml_load.c
    ${OPENAIRCN_DIR}/SRC/UTILS/xml_msg_load_itti.c
  )
  set(SCENARIO_PLAYER_LIB SCENARIO_PLAYER)
endif (TRACE_XML)
include_directories(${OPENAIRCN_DIR}/SRC/TEST/SCENARIO_PLAYER/)
###############################################################################
# Security library
###############################################################################
set(SECU_CN_SRC
  ${OPENAIRCN_DIR}/SRC/SECU/kdf.c
  ${OPENAIRCN_DIR}/SRC/SECU/rijndael.c
  ${OPENAIRCN_DIR}/SRC/SECU/snow3g.c
  ${OPENAIRCN_DIR}/SRC/SECU/key_nas_deriver.c
  ${OPENAIRCN_DIR}/SRC/SECU/nas_stream_eea1.c
  ${OPENAIRCN_DIR}/SRC/SECU/nas_stream_eia1.c
  ${OPENAIRCN_DIR}/SRC/SECU/nas_stream_eea2.c
  ${OPENAIRCN_DIR}/SRC/SECU/nas_stream_eia2.c
  )
add_library(SECU_CN ${SECU_CN_SRC})
###############################################################################
# Core Network Utils
###############################################################################
set(CN_UTILS_SRC
  ${OPENAIRCN_DIR}/SRC/UTILS/async_system.c
  ${OPENAIRCN_DIR}/SRC/UTILS/conversions.c
  ${OPENAIRCN_DIR}/SRC/UTILS/dynamic_memory_check.c
  ${OPENAIRCN_DIR}/SRC/UTILS/enum_string.c
  ${OPENAIRCN_DIR}/SRC/UTILS/mcc_mnc_itu.c
  ${OPENAIRCN_DIR}/SRC/UTILS/pid_file.c
  ${OPENAIRCN_DIR}/SRC/UTILS/shared_ts_log.c
  ${OPENAIRCN_DIR}/SRC/UTILS/TLVEncoder.c
  ${OPENAIRCN_DIR}/SRC/UTILS/TLVDecoder.c
  ${OPENAIRCN_DIR}/SRC/UTILS/xml2_wrapper.c
  )
if (LOG_OAI)
  set(CN_UTILS_SRC   ${CN_UTILS_SRC}   ${OPENAIRCN_DIR}/SRC/UTILS/log.c )
endif(LOG_OAI)
add_library(CN_UTILS ${CN_UTILS_SRC})
##########################
# Core network libs
##########################
set(GTPV1U_DIR ${OPENAIRCN_DIR}/SRC/GTPV1-U)
set (GTPV1U_SRC
  ${GTPV1U_DIR}/gtpv1u_task.c
  ${GTPV1U_DIR}/gtpv1u_teid_pool.c
  ${GTPV1U_DIR}/gtp_mod_kernel.c
)
add_library(GTPV1U ${GTPV1U_SRC})
set(GTPV2C_DIR  ${OPENAIRCN_DIR}/SRC/GTPV2-C/nwgtpv2c-0.11/src)
add_library(GTPV2C
  ${GTPV2C_DIR}/NwGtpv2cTrxn.c
  ${GTPV2C_DIR}/NwGtpv2cTunnel.c
  ${GTPV2C_DIR}/NwGtpv2cMsg.c
  ${GTPV2C_DIR}/NwGtpv2cMsgIeParseInfo.c
  ${GTPV2C_DIR}/NwGtpv2cMsgParser.c
  ${GTPV2C_DIR}/NwGtpv2c.c
  )
include_directories(${OPENAIRCN_DIR}/SRC/GTPV2-C/nwgtpv2c-0.11/include/)
include_directories(${OPENAIRCN_DIR}/SRC/GTPV2-C/nwgtpv2c-0.11/shared/)

add_library(SCTP_SERVER
  ${OPENAIRCN_DIR}/SRC/SCTP/sctp_common.c
  ${OPENAIRCN_DIR}/SRC/SCTP/sctp_itti_messaging.c
  ${OPENAIRCN_DIR}/SRC/SCTP/sctp_primitives_server.c
  )

add_library(UDP_SERVER ${OPENAIRCN_DIR}/SRC/UDP/udp_primitives_server.c)
set(S11_DIR ${OPENAIRCN_DIR}/SRC/S11)
add_library(S11_MME MODULE
  ${S11_DIR}/s11_mme.cpp
  ${S11_DIR}/s11_mme_config.cpp
  ${S11_DIR}/s11_common.c
  ${S11_DIR}/s11_ie_formatter.c
  ${S11_DIR}/s11_mme_task.c
  ${S11_DIR}/s11_mme_bearer_manager.c
  ${S11_DIR}/s11_mme_session_manager.c
)
add_library(HMB_S11_MME MODULE
  ${S11_DIR}/s11_mme.cpp
  ${S11_DIR}/s11_mme_config_HMB.cpp
  ${S11_DIR}/s11_common.c
  ${S11_DIR}/s11_ie_formatter.c
  ${S11_DIR}/s11_mme_task_HMB.c
  ${S11_DIR}/s11_mme_bearer_manager.c
  ${S11_DIR}/s11_mme_session_manager.c
)
add_library(S11_SGW
  ${S11_DIR}/s11_common.c
  ${S11_DIR}/s11_ie_formatter.c
  ${S11_DIR}/s11_sgw.c
  ${S11_DIR}/s11_sgw_session_manager.c
  ${S11_DIR}/s11_sgw_bearer_manager.c
)
include_directories(${S11_DIR})
set(S6A_DIR ${OPENAIRCN_DIR}/SRC/S6A)
add_library(S6A MODULE
  ${S6A_DIR}/s6a.cpp
  ${S6A_DIR}/s6a_config.cpp
  ${S6A_DIR}/s6a_auth_info.cpp
  ${S6A_DIR}/s6a_dict.cpp
  ${S6A_DIR}/s6a_error.cpp
  ${S6A_DIR}/s6a_peer.cpp
  ${S6A_DIR}/s6a_subscription_data.cpp
  ${S6A_DIR}/s6a_task.cpp
  ${S6A_DIR}/s6a_up_loc.cpp
  )
add_library(HS_S6A MODULE
  ${S6A_DIR}/s6a.cpp
  ${S6A_DIR}/s6a_config_HS.cpp
  ${S6A_DIR}/s6a_auth_info.cpp
  ${S6A_DIR}/s6a_dict.cpp
  ${S6A_DIR}/s6a_error.cpp
  ${S6A_DIR}/s6a_peer.cpp
  ${S6A_DIR}/s6a_subscription_data.cpp
  ${S6A_DIR}/s6a_task_HS.cpp
  ${S6A_DIR}/s6a_up_loc.cpp
  )
set(SGW_DIR ${OPENAIRCN_DIR}/SRC/SGW)
add_library (SGW
  ${SGW_DIR}/pgw_config.c
  ${SGW_DIR}/pgw_lite_paa.c
  ${SGW_DIR}/pgw_pcef_emulation.c
  ${SGW_DIR}/pgw_pco.c
  ${SGW_DIR}/pgw_procedures.c
  ${SGW_DIR}/s11_causes.c
  ${SGW_DIR}/sgw_config.c
  ${SGW_DIR}/sgw_context_manager.c
  ${SGW_DIR}/sgw_handlers.c
  ${SGW_DIR}/sgw_task.c
  ${SGW_DIR}/spgw_config.c
  )
set(MME_DIR ${OPENAIRCN_DIR}/SRC/MME_APP)
add_library(MME_APP MODULE
  ${MME_DIR}/mme_app.cpp
  ${MME_DIR}/mme_app_config.cpp
  ${MME_DIR}/mme_app_apn_selection.cpp
  ${MME_DIR}/mme_app_authentication.cpp
  ${MME_DIR}/mme_app_bearer.cpp
  ${MME_DIR}/mme_app_bearer_context.cpp
  ${MME_DIR}/mme_app_capabilities.cpp
  ${MME_DIR}/mme_app_context.cpp
  ${MME_DIR}/mme_app_detach.cpp
  ${MME_DIR}/mme_app_edns_emulation.cpp
  ${MME_DIR}/mme_app_itti_messaging.cpp
  ${MME_DIR}/mme_app_location.cpp
  ${MME_DIR}/mme_app_main.cpp
  ${MME_DIR}/mme_app_pdn_context.cpp
  ${MME_DIR}/mme_app_procedures.cpp
  ${MME_DIR}/mme_app_sgw_selection.cpp
  ${MME_DIR}/mme_app_statistics.cpp
  ${MME_DIR}/mme_app_transport.cpp
  ${MME_DIR}/mme_app_ue_context.cpp
  ${MME_DIR}/s6a_2_nas_cause.cpp
  ${MME_DIR}/call_gui.cpp
  )
add_library(HMB_MME_APP MODULE
  ${MME_DIR}/mme_app.cpp
  ${MME_DIR}/mme_app_config_HMB.cpp
  ${MME_DIR}/mme_app_apn_selection.cpp
  ${MME_DIR}/mme_app_authentication.cpp
  ${MME_DIR}/mme_app_bearer.cpp
  ${MME_DIR}/mme_app_bearer_context.cpp
  ${MME_DIR}/mme_app_capabilities.cpp
  ${MME_DIR}/mme_app_context.cpp
  ${MME_DIR}/mme_app_detach.cpp
  ${MME_DIR}/mme_app_edns_emulation.cpp
  ${MME_DIR}/mme_app_itti_messaging.cpp
  ${MME_DIR}/mme_app_location.cpp
  ${MME_DIR}/mme_app_main_HMB.cpp
  ${MME_DIR}/mme_app_pdn_context.cpp
  ${MME_DIR}/mme_app_procedures.cpp
  ${MME_DIR}/mme_app_sgw_selection.cpp
  ${MME_DIR}/mme_app_statistics.cpp
  ${MME_DIR}/mme_app_transport.cpp
  ${MME_DIR}/mme_app_ue_context.cpp
  ${MME_DIR}/s6a_2_nas_cause.cpp
  )

set(NAS_SRC ${OPENAIRCN_DIR}/SRC/NAS/)
set(libnas_api_OBJS
  ${NAS_SRC}API/NETWORK/nas_message.cpp
  )
  
set(libnas_emm_msg_OBJS
  ${NAS_SRC}EMM/MSG/AttachAccept.cpp
  ${NAS_SRC}EMM/MSG/AttachComplete.cpp
  ${NAS_SRC}EMM/MSG/AttachReject.cpp
  ${NAS_SRC}EMM/MSG/AttachRequest.cpp
  ${NAS_SRC}EMM/MSG/AuthenticationFailure.cpp
  ${NAS_SRC}EMM/MSG/AuthenticationReject.cpp
  ${NAS_SRC}EMM/MSG/AuthenticationRequest.cpp
  ${NAS_SRC}EMM/MSG/AuthenticationResponse.cpp
  ${NAS_SRC}EMM/MSG/CsServiceNotification.cpp
  ${NAS_SRC}EMM/MSG/DetachAccept.cpp
  ${NAS_SRC}EMM/MSG/DetachRequest.cpp
  ${NAS_SRC}EMM/MSG/DownlinkNasTransport.cpp
  ${NAS_SRC}EMM/MSG/EmmInformation.cpp
  ${NAS_SRC}EMM/MSG/emm_msg.cpp
  ${NAS_SRC}EMM/MSG/EmmStatus.cpp
  ${NAS_SRC}EMM/MSG/ExtendedServiceRequest.cpp
  ${NAS_SRC}EMM/MSG/GutiReallocationCommand.cpp
  ${NAS_SRC}EMM/MSG/GutiReallocationComplete.cpp
  ${NAS_SRC}EMM/MSG/IdentityRequest.cpp
  ${NAS_SRC}EMM/MSG/IdentityResponse.cpp
  ${NAS_SRC}EMM/MSG/SecurityModeCommand.cpp
  ${NAS_SRC}EMM/MSG/SecurityModeComplete.cpp
  ${NAS_SRC}EMM/MSG/SecurityModeReject.cpp
  ${NAS_SRC}EMM/MSG/ServiceReject.cpp
  ${NAS_SRC}EMM/MSG/ServiceRequest.cpp
  ${NAS_SRC}EMM/MSG/TrackingAreaUpdateAccept.cpp
  ${NAS_SRC}EMM/MSG/TrackingAreaUpdateComplete.cpp
  ${NAS_SRC}EMM/MSG/TrackingAreaUpdateReject.cpp
  ${NAS_SRC}EMM/MSG/TrackingAreaUpdateRequest.cpp
  ${NAS_SRC}EMM/MSG/UplinkNasTransport.cpp
)
  
set(libnas_esm_msg_OBJS
  ${NAS_SRC}ESM/MSG/ActivateDedicatedEpsBearerContextAccept.cpp
  ${NAS_SRC}ESM/MSG/ActivateDedicatedEpsBearerContextReject.cpp
  ${NAS_SRC}ESM/MSG/ActivateDedicatedEpsBearerContextRequest.cpp
  ${NAS_SRC}ESM/MSG/ActivateDefaultEpsBearerContextAccept.cpp
  ${NAS_SRC}ESM/MSG/ActivateDefaultEpsBearerContextReject.cpp
  ${NAS_SRC}ESM/MSG/ActivateDefaultEpsBearerContextRequest.cpp
  ${NAS_SRC}ESM/MSG/BearerResourceAllocationReject.cpp
  ${NAS_SRC}ESM/MSG/BearerResourceAllocationRequest.cpp
  ${NAS_SRC}ESM/MSG/BearerResourceModificationReject.cpp
  ${NAS_SRC}ESM/MSG/BearerResourceModificationRequest.cpp
  ${NAS_SRC}ESM/MSG/DeactivateEpsBearerContextAccept.cpp
  ${NAS_SRC}ESM/MSG/DeactivateEpsBearerContextRequest.cpp
  ${NAS_SRC}ESM/MSG/EsmInformationRequest.cpp
  ${NAS_SRC}ESM/MSG/EsmInformationResponse.cpp
  ${NAS_SRC}ESM/MSG/esm_msg.cpp
  ${NAS_SRC}ESM/MSG/EsmStatus.cpp
  ${NAS_SRC}ESM/MSG/ModifyEpsBearerContextAccept.cpp
  ${NAS_SRC}ESM/MSG/ModifyEpsBearerContextReject.cpp
  ${NAS_SRC}ESM/MSG/ModifyEpsBearerContextRequest.cpp
  ${NAS_SRC}ESM/MSG/PdnConnectivityReject.cpp
  ${NAS_SRC}ESM/MSG/PdnConnectivityRequest.cpp
  ${NAS_SRC}ESM/MSG/PdnDisconnectReject.cpp
  ${NAS_SRC}ESM/MSG/PdnDisconnectRequest.cpp
)
set(libnas_ies_OBJS
  ${NAS_SRC}IES/AdditionalUpdateResult.cpp
  ${NAS_SRC}IES/AdditionalUpdateType.cpp
  ${NAS_SRC}IES/ApnAggregateMaximumBitRate.cpp
  ${NAS_SRC}IES/Cli.cpp
  ${NAS_SRC}IES/CsfbResponse.cpp
  ${NAS_SRC}IES/DetachType.cpp
  ${NAS_SRC}IES/EmmCause.cpp
  ${NAS_SRC}IES/EpsAttachResult.cpp
  ${NAS_SRC}IES/EpsAttachType.cpp
  ${NAS_SRC}IES/EpsBearerContextStatus.cpp
  ${NAS_SRC}IES/EpsMobileIdentity.cpp
  ${NAS_SRC}IES/EpsNetworkFeatureSupport.cpp
  ${NAS_SRC}IES/EpsQualityOfService.cpp
  ${NAS_SRC}IES/EpsUpdateResult.cpp
  ${NAS_SRC}IES/EpsUpdateType.cpp
  ${NAS_SRC}IES/EsmCause.cpp
  ${NAS_SRC}IES/EsmInformationTransferFlag.cpp
  ${NAS_SRC}IES/EsmMessageContainer.cpp
  ${NAS_SRC}IES/GutiType.cpp
  ${NAS_SRC}IES/KsiAndSequenceNumber.cpp
  ${NAS_SRC}IES/LcsClientIdentity.cpp
  ${NAS_SRC}IES/LcsIndicator.cpp
  ${NAS_SRC}IES/LinkedEpsBearerIdentity.cpp
  ${NAS_SRC}IES/MessageType.cpp
  ${NAS_SRC}IES/NasKeySetIdentifier.cpp
  ${NAS_SRC}IES/NasMessageContainer.cpp
  ${NAS_SRC}IES/NasRequestType.cpp
  ${NAS_SRC}IES/NasSecurityAlgorithms.cpp
  ${NAS_SRC}IES/Nonce.cpp
  ${NAS_SRC}IES/PagingIdentity.cpp
  ${NAS_SRC}IES/PdnAddress.cpp
  ${NAS_SRC}IES/PdnType.cpp
  ${NAS_SRC}IES/RadioPriority.cpp
  ${NAS_SRC}IES/SecurityHeaderType.cpp
  ${NAS_SRC}IES/ServiceType.cpp
  ${NAS_SRC}IES/ShortMac.cpp
  ${NAS_SRC}IES/SsCode.cpp
  ${NAS_SRC}IES/TrackingAreaIdentity.cpp
  ${NAS_SRC}IES/TrackingAreaIdentityList.cpp
  ${NAS_SRC}IES/UeNetworkCapability.cpp
  ${NAS_SRC}IES/UeRadioCapabilityInformationUpdateNeeded.cpp
  ${NAS_SRC}IES/UeSecurityCapability.cpp
)
set (libnas_utils_OBJS
  ${NAS_SRC}UTIL/nas_timer.cpp
)
set(libnas_mme_api_OBJS
  ${NAS_SRC}API/MME/mme_api.cpp
)
set(libnas_mme_emm_OBJS
  ${NAS_SRC}EMM/Attach.cpp
  ${NAS_SRC}EMM/Authentication.cpp
  ${NAS_SRC}EMM/Detach.cpp
  ${NAS_SRC}EMM/EmmCommon.cpp
  ${NAS_SRC}EMM/emm_data_ctx.cpp
  ${NAS_SRC}EMM/emm_main.cpp
  ${NAS_SRC}EMM/emm_specific.cpp
  ${NAS_SRC}EMM/EmmStatusHdl.cpp
  ${NAS_SRC}EMM/Identification.cpp
  ${NAS_SRC}EMM/LowerLayer.cpp
  ${NAS_SRC}EMM/SecurityModeControl.cpp
  ${NAS_SRC}EMM/ServiceRequestHdl.cpp
  ${NAS_SRC}EMM/TrackingAreaUpdate.cpp
)
  
set(libnas_mme_emm_sap_OBJS
  ${NAS_SRC}EMM/SAP/emm_as.cpp
  ${NAS_SRC}EMM/SAP/emm_cn.cpp
  ${NAS_SRC}EMM/SAP/EmmCommonProcedureInitiated.cpp
  ${NAS_SRC}EMM/SAP/EmmDeregistered.cpp
  ${NAS_SRC}EMM/SAP/EmmDeregisteredInitiated.cpp
  ${NAS_SRC}EMM/SAP/emm_esm.cpp
  ${NAS_SRC}EMM/SAP/emm_fsm.cpp
  ${NAS_SRC}EMM/SAP/emm_recv.cpp
  ${NAS_SRC}EMM/SAP/emm_reg.cpp
  ${NAS_SRC}EMM/SAP/EmmRegistered.cpp
  ${NAS_SRC}EMM/SAP/emm_sap.cpp
  ${NAS_SRC}EMM/SAP/emm_send.cpp
  )
set (libnas_mme_esm_OBJS
  ${NAS_SRC}ESM/DedicatedEpsBearerContextActivation.cpp
  ${NAS_SRC}ESM/DefaultEpsBearerContextActivation.cpp
  ${NAS_SRC}ESM/EpsBearerContextDeactivation.cpp
  ${NAS_SRC}ESM/esm_data_context.cpp
  ${NAS_SRC}ESM/esm_ebr.cpp
  ${NAS_SRC}ESM/esm_ebr_context.cpp
  ${NAS_SRC}ESM/esm_information.cpp
  ${NAS_SRC}ESM/esm_ip.cpp
  ${NAS_SRC}ESM/esm_main.cpp
  ${NAS_SRC}ESM/esm_pt.cpp
  ${NAS_SRC}ESM/EsmStatusHdl.cpp
  ${NAS_SRC}ESM/PdnConnectivity.cpp
  ${NAS_SRC}ESM/PdnDisconnect.cpp
  )
set(libnas_mme_esm_sap_OBJS
  ${NAS_SRC}ESM/SAP/esm_recv.cpp
  ${NAS_SRC}ESM/SAP/esm_send.cpp
  ${NAS_SRC}ESM/SAP/esm_sap.cpp
  )
  
add_library(NAS MODULE
  ${NAS_SRC}nas_mme_task.cpp
  ${NAS_SRC}nas_itti_messaging.cpp
  ${NAS_SRC}nas_network.cpp
  ${NAS_SRC}nas_proc.cpp
  ${NAS_SRC}nas.cpp
  ${NAS_SRC}nas_config.cpp
  ${libnas_api_OBJS}
  ${libnas_mme_api_OBJS}
  ${libnas_emm_msg_OBJS}
  ${libnas_esm_msg_OBJS}
  ${libnas_ies_OBJS}
  ${libnas_utils_OBJS}
  ${libnas_mme_emm_OBJS}
  ${libnas_mme_emm_sap_OBJS}
  ${libnas_mme_esm_OBJS}
  ${libnas_mme_esm_sap_OBJS} 
  )
add_library(HMB_NAS MODULE
  ${NAS_SRC}nas_mme_task_HMB.cpp
  ${NAS_SRC}nas_itti_messaging.cpp
  ${NAS_SRC}nas_network.cpp
  ${NAS_SRC}nas_proc.cpp
  ${NAS_SRC}nas.cpp
  ${NAS_SRC}nas_config_HMB.cpp
  ${libnas_api_OBJS}
  ${libnas_mme_api_OBJS}
  ${libnas_emm_msg_OBJS}
  ${libnas_esm_msg_OBJS}
  ${libnas_ies_OBJS}
  ${libnas_utils_OBJS}
  ${libnas_mme_emm_OBJS}
  ${libnas_mme_emm_sap_OBJS}
  ${libnas_mme_esm_OBJS}
  ${libnas_mme_esm_sap_OBJS} 
  )
add_library(HS_NAS MODULE
  ${NAS_SRC}nas_mme_task_HS.cpp
  ${NAS_SRC}nas_itti_messaging.cpp
  ${NAS_SRC}nas_network.cpp
  ${NAS_SRC}nas_proc.cpp
  ${NAS_SRC}nas.cpp
  ${NAS_SRC}nas_config_HS.cpp
  ${libnas_api_OBJS}
  ${libnas_mme_api_OBJS}
  ${libnas_emm_msg_OBJS}
  ${libnas_esm_msg_OBJS}
  ${libnas_ies_OBJS}
  ${libnas_utils_OBJS}
  ${libnas_mme_emm_OBJS}
  ${libnas_mme_emm_sap_OBJS}
  ${libnas_mme_esm_OBJS}
  ${libnas_mme_esm_sap_OBJS} 
  )

include_directories(${NAS_SRC})
include_directories(${NAS_SRC}/API/MME)
include_directories(${NAS_SRC}/EMM)
include_directories(${NAS_SRC}/EMM/SAP)
include_directories(${NAS_SRC}/ESM)
include_directories(${NAS_SRC}/ESM/SAP)
# System packages that are required
# We use either the cmake buildin, in ubuntu are in: /usr/share/cmake*/Modules/
# or cmake provide a generic interface to pkg-config that widely used
###################################
set(CMAKE_MODULE_PATH "${OPENAIRCN_DIR}/BUILD/MODULES" "${CMAKE_MODULE_PATH}")
include(FindPkgConfig)
pkg_search_module(LIBXML2 libxml-2.0 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIRS})
pkg_search_module(OPENSSL openssl REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
pkg_search_module(CONFIG libconfig REQUIRED)
include_directories(${CONFIG_INCLUDE_DIRS})
pkg_search_module(CRYPTO libcrypto REQUIRED)
include_directories(${CRYPTO_INCLUDE_DIRS})
pkg_search_module(NETTLE nettle)

#####
pkg_search_module(GTKMM REQUIRED gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})
#####

if(NOT ${NETTLE_FOUND})
  message("PACKAGE nettle not found: some targets will fail")
else()
  include_directories(${NETTLE_INCLUDE_DIRS})
endif()
pkg_search_module(NETTLE nettle)
if(NOT ${NETTLE_FOUND})
  message( FATAL_ERROR "PACKAGE nettle not found: some targets will fail. Run SCRIPTS/build_mme -i or SCRIPTS/build_hss -i  again!")
else()
  include_directories(${NETTLE_INCLUDE_DIRS})
endif()
message ("NETTLE VERSION_INSTALLED  = ${NETTLE_VERSION}")
string(REGEX REPLACE "([0-9]+).*" "\\1" NETTLE_VERSION_MAJOR ${NETTLE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+).*" "\\1" NETTLE_VERSION_MINOR ${NETTLE_VERSION})
message ("NETTLE_VERSION_MAJOR = ${NETTLE_VERSION_MAJOR}")
message ("NETTLE_VERSION_MINOR = ${NETTLE_VERSION_MINOR}")
if ("${NETTLE_VERSION_MAJOR}" STREQUAL "" OR "${NETTLE_VERSION_MINOR}" STREQUAL "")
  message( FATAL_ERROR "The nettle version not detected properly. Try to run SCRIPTS/build_mme -i or SCRIPTS/build_hss -i again" )
endif()
add_definitions("-DNETTLE_VERSION_MAJOR=${NETTLE_VERSION_MAJOR}")
add_definitions("-DNETTLE_VERSION_MINOR=${NETTLE_VERSION_MINOR}")
find_library(LFDS lfds710 PATHS /usr/local/lib /usr/lib )
if ("${LFDS}" STREQUAL "LFDS-NOTFOUND")
  message(FATAL_ERROR "LIB LFDS not found, please install it" )
endif()
pkg_search_module(XPM xpm)
if(NOT ${XPM_FOUND})
  message("PACKAGE xpm not found: some targets will fail")
else()
  include_directories(${XPM_INCLUDE_DIRS})
endif()
# Hack on a test of asn1c version (already dirty)
add_definitions(-DASN1_MINIMUM_VERSION=924)
#################################
# add executables for operation
#################################
  
# MME standalone 
################################
add_definitions("-DSHARED_LIBRARY_PATH=\"${OPENAIRCN_DIR}/BUILD/MME/BUILD/\"")
add_executable(mme
  ${OPENAIRCN_DIR}/SRC/OAI_MME/globals.cpp
  ${OPENAIRCN_DIR}/SRC/OAI_MME/oai_mme.cpp
  ${OPENAIRCN_DIR}/SRC/OAI_MME/mme_config.cpp
  ${OPENAIRCN_DIR}/SRC/COMMON/common_types.c
  ${OPENAIRCN_DIR}/SRC/COMMON/itti_free_defined_msg.c
  ${OPENAIRCN_DIR}/SRC/COMMON/itti_comp.c
  ${GENERATED_PROTOBUF_FILES}
  )
  
if( ITTI_ANALYZER )
  add_executable(mme ${OPENAIRCN_BIN_DIR}/messages_xml.h )
endif( ITTI_ANALYZER )
target_link_libraries(mme ${CMAKE_DL_LIBS})
target_link_libraries (mme
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore ${GTKMM_LIBRARIES}
  )
target_link_libraries (S6A
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (HS_S6A
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (S11_MME
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (HMB_S11_MME
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (NAS
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (HMB_NAS
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (HS_NAS
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (S1AP
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  ${GTKMM_LIBRARIES}
  )
target_link_libraries (MME_APP
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
target_link_libraries (HMB_MME_APP
  -Wl,--start-group
    S1AP_LIB GTPV2C SCTP_SERVER UDP_SERVER SECU_CN 
   ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
   ${3GPP_TYPES_XML_LIB} CN_UTILS ${SCENARIO_PLAYER_LIB} HASHTABLE BSTR 
  -Wl,--end-group
  pthread m sctp  rt crypt ${LFDS} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} ${LIBXML2_LIBRARIES} gnutls fdproto fdcore
  )
# spgw is S-GW +P-GW  nodes all in one implementation
################################
#add_executable(spgw
#  ${OPENAIRCN_DIR}/SRC/OAI_SGW/oai_sgw.c
#  ${OPENAIRCN_DIR}/SRC/COMMON/common_types.c
#  ${OPENAIRCN_DIR}/SRC/COMMON/itti_free_defined_msg.c
#  )
#if( ITTI_ANALYZER )
#  add_executable(spgw ${OPENAIRCN_BIN_DIR}/messages_xml.h )
#endif( ITTI_ANALYZER )
#target_link_libraries (spgw
#  -Wl,--start-group
#  GTPV1U SGW S11_SGW GTPV2C UDP_SERVER ${MSC_LIB} ${ITTI_LIB} ${XML_MSG_DUMP_LIB} ${3GPP_TYPES_LIB} 
#   ${3GPP_TYPES_XML_LIB} CN_UTILS HASHTABLE BSTR
#  -Wl,--end-group
#  pthread m rt gtpnl ${LFDS} ${CONFIG_LIBRARIES}  ${LIBXML2_LIBRARIES}  
#  )
#IF( EPC_BUILD OR MME_BUILD )
#  INCLUDE(FindFreeDiameter)
  # if standalone eNB or UE no need for FreeDiameter
#  IF( FREEDIAMETER_FOUND )
#    IF( NOT FREEDIAMETER_HSS_S6A_ENABLED )
#      MESSAGE( SEND_ERROR "FreeDiameter is not enabled for OPENAIRHSS" )
#    ENDIF( NOT FREEDIAMETER_HSS_S6A_ENABLED )
#  ENDIF( FREEDIAMETER_FOUND )
#ENDIF( EPC_BUILD OR MME_BUILD )
###################################"
# Add executables for tests
####################################
#enable_testing()
#add_subdirectory(${OPENAIRCN_DIR}/SRC/TEST/ ${CMAKE_CURRENT_BINARY_DIR}/TESTS/)
#add_test(NAME test_imsi_convert   COMMAND test_mme_app_ue_context_imsi)
#add_test(NAME Test_aes128_cmac        COMMAND test_aes128_cmac)
#add_test(NAME Test_aes128_ctr_decrypt COMMAND test_aes128_ctr_decrypt)
#add_test(NAME Test_aes128_ctr_encrypt COMMAND test_aes128_ctr_encrypt)
# TODO
#unitary tests for Core NEtwork pieces
#################################
#foreach(myExe s1ap
#    secu_knas_encrypt_eia1
#    secu_kenb
#    aes128_ctr_encrypt
#    aes128_ctr_decrypt
#    secu_knas_encrypt_eea2
#    secu_knas secu_knas_encrypt_eea1
#    kdf
#    aes128_cmac_encrypt
#    secu_knas_encrypt_eia2)
#    
#  add_executable(test_${myExe}
#    ${OPENAIRCN_DIR}/SRC/TEST/test_util.c
#    ${OPENAIRCN_DIR}/SRC/TEST/test_${myExe}.c
#    )
#  target_link_libraries (test_${myExe}
#    -Wl,--start-group SECU_CN UTIL -Wl,--end-group ${LFDS}  m rt crypt ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} ${NETTLE_LIBRARIES} ${CONFIG_LIBRARIES} fdproto fdcore
#    )
#endforeach(myExe)


