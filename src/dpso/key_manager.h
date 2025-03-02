
#pragma once

#include <stdbool.h>

#include "keys.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Get whether hotkeys are enabled.
 *
 * Enabling and disabling hotkeys only affects
 * dpsoGetLastHotkeyAction(), which will always return
 * dpsoNoHotkeyAction if hotkeys are disabled.
 *
 * Hotkeys are disabled by default.
 */
bool dpsoGetHotkeysEnabled(void);


/**
 * Set whether hotkeys are enabled.
 *
 * \sa dpsoGetHotkeysEnabled()
 */
void dpsoSetHotkeysEnabled(bool newHotkeysEnabled);


/**
 * Get action of the pressed hotkey.
 *
 * dpsoGetLastHotkeyAction() returns the action if a bound hotkey
 * that matches the recently pressed key combination. The function
 * returns dpsoNoHotkeyAction if hotkeys are disabled (see
 * dpsoGetHotkeysEnabled()), no keys are pressed, or there is no
 * hotkey for the pressed combination.
 *
 * It's safe to call dpsoGetLastHotkeyAction() many times per update,
 * since the input event handling is done in dpsoUpdate().
 */
DpsoHotkeyAction dpsoGetLastHotkeyAction(void);


/**
 * Bind hotkey to action.
 *
 * If the hotkey is already bound, its action will be updated.
 */
void dpsoBindHotkey(
    const DpsoHotkey* hotkey, DpsoHotkeyAction action);


/**
 * Unbind hotkey.
 */
void dpsoUnbindHotkey(const DpsoHotkey* hotkey);


/**
 * Remove all hotkeys bound to this action.
 */
void dpsoUnbindAction(DpsoHotkeyAction action);


/**
 * Find hotkey bound to action.
 *
 * If no hotkey is bound to the action, the function returns
 * {dpsoNoKey, dpsoNoKeyMods} (dpsoEmptyHotkey).
 *
 * If multiple hotkeys are bound to the same action, which one is
 * returned depends on the implementation.
 */
void dpsoFindActionHotkey(
    DpsoHotkeyAction action, DpsoHotkey* hotkey);


/**
 * Find action of hotkey.
 *
 * Returns dpsoNoHotkeyAction if the hotkey is not bound.
 */
DpsoHotkeyAction dpsoFindHotkeyAction(const DpsoHotkey* hotkey);


#ifdef __cplusplus
}
#endif
