
#include "backend/windows/utils.h"

#include <cstdio>


namespace dpso {
namespace windows {


std::string getErrorMessage(DWORD error)
{
    char* messageBuf = nullptr;
    auto size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        reinterpret_cast<char*>(&messageBuf),
        0, nullptr);

    if (size == 0)
        return "Windows error " + std::to_string(error);

    if (size > 1
            && messageBuf[size - 2] == '\r'
            && messageBuf[size - 1] == '\n')
        size -= 2;

    std::string message{messageBuf, size};

    LocalFree(messageBuf);

    return message;
}


std::string getHresultMessage(HRESULT hresult)
{
    // It seems that FormatMessage() accepts HRESULT, at least
    // _com_error::ErrorMessage() relies on that. Still, this is not
    // documented, so we extract the system error code manually.
    if (HRESULT_FACILITY(hresult) == FACILITY_WIN32)
        return getErrorMessage(HRESULT_CODE(hresult));

    char buf[sizeof(hresult) * 2 + 1];
    if (std::snprintf(buf, sizeof(buf), "%08lX", hresult) < 0)
        buf[0] = 0;

    return std::string{"HRESULT 0x"} + buf;
}


}
}
