
#include "os.h"

#include <cerrno>
#include <cstring>
#include <io.h>

#include "error.h"
#include "windows/error.h"
#include "windows/utf.h"


const char* const dpsoDirSeparators = "\\/";


int64_t dpsoGetFileSize(const char* filePath)
{
    std::wstring filePathUtf16;
    try {
        filePathUtf16 = dpso::windows::utf8ToUtf16(filePath);
    } catch (std::runtime_error& e) {
        dpsoSetError(
            "Can't convert filePath to UTF-16: %s", e.what());
        return -1;
    }

    WIN32_FILE_ATTRIBUTE_DATA attrs;
    if (!GetFileAttributesExW(
            filePathUtf16.c_str(), GetFileExInfoStandard, &attrs)) {
        dpsoSetError(
            "GetFileAttributesExW(): %s",
            dpso::windows::getErrorMessage(GetLastError()).c_str());
        return -1;
    }

    return
        (static_cast<int64_t>(attrs.nFileSizeHigh) << 32)
        | attrs.nFileSizeLow;
}


FILE* dpsoFopen(const char* filePath, const char* mode)
{
    std::wstring filePathUtf16;
    std::wstring modeUtf16;

    try {
        filePathUtf16 = dpso::windows::utf8ToUtf16(filePath);
        modeUtf16 = dpso::windows::utf8ToUtf16(mode);
    } catch (std::runtime_error&) {
        errno = EINVAL;
        return nullptr;
    }

    return _wfopen(filePathUtf16.c_str(), modeUtf16.c_str());
}


int dpsoRemove(const char* filePath)
{
    std::wstring filePathUtf16;

    try {
        filePathUtf16 = dpso::windows::utf8ToUtf16(filePath);
    } catch (std::runtime_error&) {
        errno = EINVAL;
        return -1;
    }

    return _wremove(filePathUtf16.c_str());
}


bool dpsoReplace(const char* src, const char* dst)
{
    std::wstring srcUtf16;
    try {
        srcUtf16 = dpso::windows::utf8ToUtf16(src);
    } catch (std::runtime_error& e) {
        dpsoSetError("Can't convert src to UTF-16: %s", e.what());
        return false;
    }

    std::wstring dstUtf16;
    try {
        dstUtf16 = dpso::windows::utf8ToUtf16(dst);
    } catch (std::runtime_error& e) {
        dpsoSetError("Can't convert dst to UTF-16: %s", e.what());
        return false;
    }

    if (MoveFileExW(
            srcUtf16.c_str(),
            dstUtf16.c_str(),
            MOVEFILE_REPLACE_EXISTING
                | MOVEFILE_WRITE_THROUGH) == 0) {
        dpsoSetError(
            "MoveFileExW(): %s",
            dpso::windows::getErrorMessage(GetLastError()).c_str());
        return false;
    }

    return true;
}


bool dpsoSyncFile(FILE* fp)
{
    const auto fd = _fileno(fp);
    if (fd == -1) {
        dpsoSetError("_fileno(): %s", std::strerror(errno));
        return false;
    }

    if (_commit(fd) == -1) {
        dpsoSetError("_commit(): %s", std::strerror(errno));
        return false;
    }

    return true;
}


bool dpsoSyncFileDir(const char* filePath)
{
    (void)filePath;
    // Windows doesn't support directory synchronization.
    return true;
}
