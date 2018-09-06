#include "garbage.h"
#if GC == GC_TGC
#include "../vendor/tgc/tgc.h"
tgc_t tgc_gc_state;
#endif

