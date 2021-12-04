
#include "os.h"

#include <cerrno>

#include "windows_utils.h"


const char* const dpsoDirSeparators = "\\/";


FILE* dpsoFopenUtf8(const char* fileName, const char* mode)
{
    std::wstring fileNameUtf16;
    std::wstring modeUtf16;

    try {
        fileNameUtf16 = dpso::windows::utf8ToUtf16(fileName);
        modeUtf16 = dpso::windows::utf8ToUtf16(mode);
    } catch (std::runtime_error&) {
        errno = EINVAL;
        return nullptr;
    }

    return _wfopen(fileNameUtf16.c_str(), modeUtf16.c_str());
}
