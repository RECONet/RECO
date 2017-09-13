#include "module.hpp"
#include "s11_mme.h"

class s11_mme : public module {
  public:
    //creating s11_mme module thread
    virtual int init(mme_config_t * mme_config_p) {
        return s11_mme_init (mme_config_p);
    }
};
/* the class factories */
extern "C" module* create() {
    return new s11_mme;
}
extern "C" void destroy(module* p) {
    delete p;
}
