#pragma once

#define Chk(hr) \
    do \
    { \
        if (!SUCCEEDED(hr)) \
        { \
            RaiseFailFastException(0, 0, FAIL_FAST_GENERATE_EXCEPTION_ADDRESS); \
        } \
    } \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    while (false) \
    __pragma(warning(pop))
