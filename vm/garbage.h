#ifndef GARBAGE_H
#define GARBAGE_H

#define GC_NONE 0   /* use system malloc without GC */
#define GC_BOEHM 1  /* use system-installed boehm libgc */
#define GC_TGC 2    /* use built-in tgc */

#ifndef GC
#define GC GC_TGC
#endif

#if GC == GC_BOEHM
# include <gc.h>
# define TR_MALLOC            GC_malloc
# define TR_CALLOC(m,n)       GC_MALLOC((m)*(n))
# define TR_REALLOC           GC_realloc
# define TR_FREE(S)           UNUSED(S)
# define TR_START(X)          do { GC_INIT(); (void) (X); } while(0)
# define TR_STOP()            GC_gcollect()

#elif GC == GC_NONE
# define TR_MALLOC(n)         malloc(n)
# define TR_CALLOC(m,n)       calloc(m, n)
# define TR_REALLOC(p, n)     realloc(p, n)
# define TR_FREE(S)           do { } while(0)
# define TR_START(X)          do { } while(0)
# define TR_STOP()            do { } while(0)

#elif GC == GC_TGC
# include "../vendor/tgc/tgc.h"
# define TR_MALLOC(n)         tgc_alloc(&tgc_gc_state, n)
# define TR_CALLOC(m,n)       tgc_calloc(&tgc_gc_state, m, n)
# define TR_REALLOC(p, n)     tgc_realloc(&tgc_gc_state, p, n)
# define TR_FREE(S)           tgc_free(&tgc_gc_state, S)
# define TR_START(X)          tgc_start(&tgc_gc_state, X)
# define TR_STOP()            tgc_stop(&tgc_gc_state)
extern tgc_t tgc_gc_state;
#endif


#endif

