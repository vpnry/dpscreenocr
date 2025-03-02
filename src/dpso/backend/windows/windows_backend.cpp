
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "dpso_utils/windows/error.h"

#include "backend/backend_error.h"
#include "backend/backend.h"
#include "backend/windows/execution_layer/backend_executor.h"
#include "backend/windows/windows_key_manager.h"
#include "backend/windows/windows_screenshot.h"
#include "backend/windows/windows_selection.h"


namespace dpso::backend {
namespace {


class WindowsBackend : public Backend {
public:
    WindowsBackend();

    KeyManager& getKeyManager() override;
    Selection& getSelection() override;
    std::unique_ptr<Screenshot> takeScreenshot(
        const Rect& rect) override;

    void update() override;
private:
    HINSTANCE instance;

    std::unique_ptr<WindowsKeyManager> keyManager;
    std::unique_ptr<WindowsSelection> selection;
};


}


WindowsBackend::WindowsBackend()
    : instance{GetModuleHandleW(nullptr)}
    , keyManager{}
    , selection{}
{
    if (!instance)
        throw BackendError(
            "GetModuleHandle(): "
            + windows::getErrorMessage(GetLastError()));

    try {
        keyManager = std::make_unique<WindowsKeyManager>();
    } catch (BackendError& e) {
        throw BackendError(
            std::string("Can't create key manager: ") + e.what());
    }

    try {
        selection = std::make_unique<WindowsSelection>(instance);
    } catch (BackendError& e) {
        throw BackendError(
            std::string("Can't create selection: ") + e.what());
    }
}


KeyManager& WindowsBackend::getKeyManager()
{
    return *keyManager;
}


Selection& WindowsBackend::getSelection()
{
    return *selection;
}


std::unique_ptr<Screenshot> WindowsBackend::takeScreenshot(
    const Rect& rect)
{
    return takeWindowsScreenshot(rect);
}


void WindowsBackend::update()
{
    keyManager->clearLastHotkeyAction();
    // Update the selection before handling events so that we
    // resize and repaint it on this update() rather than on the next.
    selection->update();

    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        if (msg.message == WM_HOTKEY)
            keyManager->handleWmHotkey(msg);
        else {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
}


std::unique_ptr<Backend> Backend::create()
{
    // We can't do anything in the main thread because its messages
    // may be consumed by a GUI framework. BackendExecutor will do the
    // job of calling the backend in the background thread.
    return createBackendExecutor(
        createBgThreadActionExecutor(),
        *[]() -> std::unique_ptr<Backend>
        {
            return std::make_unique<WindowsBackend>();
        });
}


}
