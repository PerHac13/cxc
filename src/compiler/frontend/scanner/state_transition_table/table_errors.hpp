#pragma once

typedef enum
{
    // Error states

    ERROR_STATE_INVALID_CHAR = -10,
    ERROR_STATE_INVALID_TRANSITION = -20,
    ERROR_STATE_MAX_INT_LEN = -30,
    ERROR_STATE_MAX_IDENTIFIER_LEN = -40,
} StateTransitionTableErrors;

bool is_table_error(int state);