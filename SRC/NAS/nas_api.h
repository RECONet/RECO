#ifndef NAS_API_H
#define NAS_API_H

#include <stdbool.h>
 
#include "bstrlib.h"
#include "common_types.h"
#include "common_defs.h"
#include "emm_data.h"
#include "esm_ebr.h"

void *get_api_pointer();

typedef void (*esm_bearer_context_init_t) (esm_ebr_context_t * esm_ebr_context);
typedef const char * (*esm_ebr_state2string_t) (esm_ebr_state esm_ebr_state);
typedef void (*free_esm_bearer_context_t) (esm_ebr_context_t * esm_ebr_context);
typedef void (*emm_context_dump_t) (const struct emm_context_s * const elm_pP, 
              const uint8_t indent_spaces, bstring bstr_dump) __attribute__ ((nonnull)) ;
typedef void (*emm_init_context_t) (struct emm_context_s * const emm_ctx, const bool init_esm_ctxt);


typedef struct {
//  esm_bearer_context_init_t esm_bearer_context_init_f;
//  esm_ebr_state2string_t    esm_ebr_state2string_f;
//  free_esm_bearer_context_t free_esm_bearer_context_f;
//  emm_context_dump_t        emm_context_dump_f;
//  emm_init_context_t        emm_init_context_f;
} nas_api_t;

#endif  // NAS_API_H
