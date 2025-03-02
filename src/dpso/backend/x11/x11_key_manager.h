
#pragma once

#include <vector>

#include <X11/Xlib.h>

#include "backend/key_manager.h"
#include "backend/x11/x11_backend_component.h"


namespace dpso::backend {


struct X11HotkeyBinding {
    HotkeyBinding binding;
    KeyCode keyCode;
};


class X11KeyManager : public KeyManager, public X11BackendComponent {
public:
    explicit X11KeyManager(Display* display);
    ~X11KeyManager();

    bool getHotkeysEnabled() const override;
    void setHotkeysEnabled(bool newHotkeysEnabled) override;
    DpsoHotkeyAction getLastHotkeyAction() const override;

    void bindHotkey(
        const DpsoHotkey& hotkey, DpsoHotkeyAction action) override;

    int getNumBindings() const override;
    HotkeyBinding getBinding(int idx) const override;
    void removeBinding(int idx) override;

    void updateStart() override;
    void handleEvent(const XEvent& event) override;
private:
    Display* display;
    std::vector<X11HotkeyBinding> x11bindings;
    bool hotkeysEnabled;
    DpsoHotkeyAction hotkeyAction;
};


}
