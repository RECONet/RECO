#include "intertask_interface.h"
#include "mme_app_ue_context.hpp"

#undef max
#undef min

#include <grpc++/grpc++.h>

#include "test.grpc.pb.h"
#include <unistd.h>
#include "s6a_cxx.hpp"

#ifdef __cplusplus
extern "C" {
#endif
int location_update_init(const mme_config_t *mme_config);
#ifdef __cplusplus
}
#endif
void location_update_exit (void);

int send_s6a_update_location_req     (const test::Request* request);

int handle_s6a_update_location_ans   (const s6a_update_location_ans_t * const ula_pP);



