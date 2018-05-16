#include <grpc++/grpc++.h>
#include "location.grpc.pb.h"
#include <unistd.h>
#include "s6a_cxx.hpp"
#include "s6a_location_message.hpp"

void location_update_server ();

int location_update_init ();

int send_s6a_update_location_req     (const location::Request* request);

int handle_s6a_update_location_ans   (const s6a_update_location_ans_t * const ula_pP);



