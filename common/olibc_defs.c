#include "olibc_defs.h" // Search in current directory

char* olibc_retval_get_string (olibc_retval_t retval)
{
    switch (retval) {
        case OLIBC_SUCCESS:
            return "Success";
        case OLIBC_FAILURE:
            return "Failure";
        default:
            return "Invalide retval";
    }
}
