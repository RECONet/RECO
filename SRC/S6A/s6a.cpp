#include "module.hpp"
//#include "module_util.h"
#include "s6a_cxx.hpp"

int s6a_init(const struct mme_config_s *mme_config);

class s6a : public module {
 public:
   virtual void bind_api() {
       // do nothing
   }
   // creating s6a module thread
   virtual int init(mme_config_t * mme_config_p) {
       return s6a_obj.s6a_init (mme_config_p);
   	
   }
 private:
   class s6a_cxx s6a_obj;
};
/* the class factories */
extern "C" module* create() {
    return new s6a;
}
extern "C" void destroy(module* p) {
    delete p;
}
