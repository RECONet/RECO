#include <grpc++/grpc++.h>
#include "location.grpc.pb.h"
#include <unistd.h>
#include "iot_common_types.hpp"
#include "iot_location_message.hpp"


int location_update_init ();

int generate_iot_update_location (iot_update_location_req_t * ulr_pP);

int send_iot_update_location_req     (const location::Request* request);

int handle_iot_update_location_ans   (const iot_update_location_ans_t * const ula_pP);



