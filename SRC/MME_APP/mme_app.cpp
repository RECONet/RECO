#include "module.hpp"
#include "mme_app_extern.h"

class mme_app : public module {
  public:
    //creating mme_app module thread
    virtual int init(mme_config_t * mme_config_p) {
        return mme_app_init (mme_config_p);
    }
};
/* the class factories */
extern "C" module* create() {
    return new mme_app;
}
extern "C" void destroy(module* p) {
    delete p;
}
