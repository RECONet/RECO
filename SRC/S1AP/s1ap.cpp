#define ESM_DATA //to resolve the dupicate definition of _esm_data
#define EMM_DATA //to resolve the dupicate definition of _emm_data
#include "module.hpp"
#include "s1ap_mme.h"
#include "s1ap_mme_cxx.h"

class s1ap : public module {
  public:
    //creating s1ap module thread
    virtual int init(mme_config_t * mme_config_p) {
        return s1ap_obj.s1ap_mme_init (mme_config_p);
    }
  private:
    class s1ap_mme_cxx s1ap_obj;
};
/* the class factories */
extern "C" module* create() {
    return new s1ap;
}
extern "C" void destroy(module* p) {
    delete p;
}
