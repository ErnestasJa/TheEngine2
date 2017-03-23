#ifndef ENGINE_DEBUG_H
#define ENGINE_DEBUG_H

#if !defined(NDEBUG)
//#define ENGINE_DEBUG
#endif


#ifdef ENGINE_DEBUG
	#define ASSERT(assertion)                                                                          \
		if ((assertion) == false) {                                                                    \
			printf("ASSERT FAILED: %s\n", #assertion);                                                 \
			exit(-1);                                                                                  \
		}
	#define DFUNC __attribute__((noinline))
#else
	#define ASSERT(assertion)
	#define DFUNC
#endif

#endif