#include "table_errors.hpp"

bool is_table_error(int state)
{
    switch (state)
    {
    case ERROR_STATE_INVALID_CHAR:
    case ERROR_STATE_INVALID_TRANSITION:
    case ERROR_STATE_MAX_INT_LEN:
    case ERROR_STATE_MAX_IDENTIFIER_LEN:
        return true;
    default:
        return false;
    }
}