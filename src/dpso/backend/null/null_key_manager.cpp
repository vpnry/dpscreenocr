
#include "backend/null/null_key_manager.h"

#include <cstdio>

#include "hotkeys.h"


namespace dpso::backend {


#define MSG(...) std::printf("NullKeyManager: " __VA_ARGS__)


NullKeyManager::NullKeyManager()
    : hotkeysEnabled{}
    , bindings{}
{
}


bool NullKeyManager::getHotkeysEnabled() const
{
    return hotkeysEnabled;
}


void NullKeyManager::setHotkeysEnabled(bool newHotkeysEnabled)
{
    hotkeysEnabled = newHotkeysEnabled;
    MSG("Hotkeys %s\n", hotkeysEnabled ? "enabled" : "disabled");
}


DpsoHotkeyAction NullKeyManager::getLastHotkeyAction() const
{
    return dpsoNoHotkeyAction;
}


void NullKeyManager::bindHotkey(
    const DpsoHotkey& hotkey, DpsoHotkeyAction action)
{
    MSG("Bind hotkey %s; action %i\n",
        dpsoHotkeyToString(&hotkey), action);

    for (auto& binding : bindings)
        if (binding.hotkey == hotkey) {
            binding.action = action;
            return;
        }

    bindings.push_back({hotkey, action});
}


int NullKeyManager::getNumBindings() const
{
    return bindings.size();
}


HotkeyBinding NullKeyManager::getBinding(int idx) const
{
    MSG("Get binding %i (%s; action %i)\n",
        idx,
        dpsoHotkeyToString(&bindings[idx].hotkey),
        bindings[idx].action);

    return bindings[idx];
}


void NullKeyManager::removeBinding(int idx)
{
    MSG("Remove binding %i (%s; action %i)\n",
        idx,
        dpsoHotkeyToString(&bindings[idx].hotkey),
        bindings[idx].action);

    if (idx + 1 < static_cast<int>(bindings.size()))
        bindings[idx] = bindings.back();

    bindings.pop_back();
}


}
