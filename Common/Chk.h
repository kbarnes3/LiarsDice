#pragma once

#define FailFast() RaiseFailFastException(0, 0, FAIL_FAST_GENERATE_EXCEPTION_ADDRESS)

#define Chk(hr) \
    do \
    { \
        if (FAILED(hr)) \
        { \
            FailFast(); \
        } \
    } \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    while (false) \
    __pragma(warning(pop))

#define ChkIf(bool) \
    do \
    { \
        if (!(bool)) \
        { \
            FailFast(); \
        } \
    } \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    while (false) \
    __pragma(warning(pop))
