#ifndef __OLIBC_DEFS_H__
#define __OLIBC_DEFS_H__
#include <stdio.h>

typedef enum olibc_retval_t_ {
    OLIBC_RETVAL_SUCCESS,
    OLIBC_RETVAL_FAILURE,
}olibc_retval_t;

extern char* olibc_retval_get_string (olibc_retval_t retval);
#endif //__OLIBC_DEFS_H__
