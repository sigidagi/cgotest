#pragma once

#include "better/enum.h"

namespace chip {

BETTER_ENUM(ChipError, int, 
    NO_ERROR = 0, 
    KEY_NOT_FOUND,
    INVALID_ARGUMENT,
    BUFFER_TOO_SMALL,
    NO_MEMORY,
    UNEXPECTED_TLV_ELEMENT,
    INVALID_TLV_TAG,
    END_OF_TLV,
    INVALID_TLV_ELEMENT,
    TLV_UNDERRUN,
    UNKNOWN_IMPLICIT_TLV_TAG,
    INCORRECT_STATE,
    INTERNAL,
    WRONG_TLV_TYPE,
    NOT_IMPLEMENTED,
    INVALID_INTEGER_VALUE,
    INVALID_STRING_LENGTH)

enum CHIP_ERROR {
    CHIP_NO_ERROR = 0,
    CHIP_ERROR_KEY_NOT_FOUND = 2,
    CHIP_ERROR_INVALID_ARGUMENT = 3,
    CHIP_ERROR_BUFFER_TOO_SMALL = 4,
    CHIP_ERROR_NO_MEMORY = 5, 
    CHIP_ERROR_UNEXPECTED_TLV_ELEMENT = 6,
    CHIP_ERROR_INVALID_TLV_TAG = 7,
    CHIP_END_OF_TLV = 8,
    CHIP_ERROR_INVALID_TLV_ELEMENT = 9,
    CHIP_ERROR_TLV_UNDERRUN = 10,
    CHIP_ERROR_UNKNOWN_IMPLICIT_TLV_TAG = 11,
    CHIP_ERROR_INCORRECT_STATE = 12,
    CHIP_ERROR_INTERNAL = 13,
    CHIP_ERROR_WRONG_TLV_TYPE = 14,
    CHIP_ERROR_NOT_IMPLEMENTED = 15,
    CHIP_ERROR_INVALID_INTEGER_VALUE = 16,
    CHIP_ERROR_INVALID_STRING_LENGTH = 17,
};

} // namespace 

