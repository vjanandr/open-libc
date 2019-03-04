#ifndef __OLIBC_DEFS_H__
#define __OLIBC_DEFS_H__
#include <stdio.h>
#include <stdint.h>

typedef enum olibc_retval_t_ {
    OLIBC_RETVAL_SUCCESS,
    OLIBC_RETVAL_FAILURE,
    OLIBC_RETVAL_DUPLICATE_DATA,
    OLIBC_RETVAL_INVALID_SWITCH,
    OLIBC_RETVAL_DATA_NOT_FOUND,
} olibc_retval_t;

typedef enum olibc_callbk_ret_type_t_ {
    OLIBC_CBK_RET_EQUAL,
    OLIBC_CBK_RET_GRTR,
    OLIBC_CBK_RET_LSR
}olibc_cbk_ret_type_t;

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

extern char* olibc_retval_get_string(olibc_retval_t retval);
#endif //__OLIBC_DEFS_H__
