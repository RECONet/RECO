#ifndef MODULE_HPP_
#define MODULE_HPP_

#include "mme_config.h"

class module {
public:
  virtual int init(mme_config_t *) = 0;
};
// the types of the class factories
typedef module* create_t();
typedef void destroy_t(module*);
#endif
