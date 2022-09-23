
#include "dirs.h"

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "dpso/error.h"
#include "dpso_utils/windows_utils.h"
#include "dpso/backend/windows/utils/error.h"


// Returns an empty string on error.
static std::wstring getExeDir()
{
    std::wstring result(32, 0);

    while (true) {
        const auto size = GetModuleFileNameW(
            nullptr, &result[0], result.size());

        if (size == 0) {
            dpsoSetError(
                "GetModuleFileNameW() failed: %s",
                dpso::windows::getErrorMessage(
                    GetLastError()).c_str());
            return {};
        }

        if (size < result.size())
            break;

        result.resize(result.size() * 2);
    }

    const auto slashPos = result.rfind(L'\\');
    if (slashPos != result.npos)
        result.resize(slashPos);

    return result;
}


static std::string baseDirPath;


bool uiInitDirs(const char* argv0)
{
    (void)argv0;

    baseDirPath = dpso::windows::utf16ToUtf8(getExeDir().c_str());
    return !baseDirPath.empty();
}


const char* uiGetDir(UiDir dir)
{
    static std::string result;
    result = baseDirPath;

    switch (dir) {
    case UiDirData:
        break;
    case UiDirDoc:
        result += "\\doc";
        break;
    case UiDirLocale:
        result += "\\locale";
        break;
    }

    return result.c_str();
}
