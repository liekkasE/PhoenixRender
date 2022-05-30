#pragma once

#pragma warning( disable : 6001 6221 )

#include <d3d11.h>

#define  CHK_ERR(hrchk, strOut) \
        case hrchk: \
             return L##strOut;

#define  CHK_ERRA(hrchk) \
        case hrchk: \
             return L## #hrchk;

#define HRESULT_FROM_WIN32b(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))

#define  CHK_ERR_WIN32A(hrchk) \
        case HRESULT_FROM_WIN32b(hrchk): \
        case hrchk: \
             return L## #hrchk;

#define  CHK_ERR_WIN32_ONLY(hrchk, strOut) \
        case HRESULT_FROM_WIN32b(hrchk): \
             return L##strOut;

const WCHAR* WINAPI DXGetErrorStringW(_In_ HRESULT hr);
