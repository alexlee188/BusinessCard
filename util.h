#ifndef _UTIL_H_
#define _UTIL_H_

#define INTERRUPT_PROTECT(x) {              \
        char saved_ipl;                          \
                                                 \
        SET_AND_SAVE_CPU_IPL(saved_ipl,7);       \
        x;                                       \
        RESTORE_CPU_IPL(saved_ipl); } (void) 0;

#endif
