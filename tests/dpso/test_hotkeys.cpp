
#include <string>

#include "dpso/dpso.h"

#include "flow.h"


const auto allMods =
    dpsoKeyModCtrl
    | dpsoKeyModAlt
    | dpsoKeyModShift
    | dpsoKeyModWin;


static void testHotkeyToString()
{
    const DpsoHotkey hotkeys[] = {
        {dpsoNoKey, dpsoKeyModCtrl},
        {dpsoKeyY, dpsoNoKeyMods},
        {dpsoKeyY, allMods},
    };

    for (const auto& hotkey : hotkeys) {
        const char* str = dpsoHotkeyToString(&hotkey);

        DpsoHotkey outHotkey;
        dpsoHotkeyFromString(str, &outHotkey);

        if (hotkey == outHotkey)
            continue;

        test::failure(
            "testHotkeyToString: \"%s\" wasn't successfully parsed "
            "by dpsoHotkeyFromString())\n",
            str);
    }
}


static std::string hotkeyToStr(const DpsoHotkey& hotkey)
{
    return dpsoHotkeyToString(&hotkey);
}


static void testHotkeyFromString()
{
    struct Test {
        const char* str;
        DpsoHotkey expectedHotkey;
    };

    const Test tests[] = {
        {"", dpsoEmptyHotkey},
        {"???", dpsoEmptyHotkey},
        {"Ctrl + ???", dpsoEmptyHotkey},
        {"Ctrl +", dpsoEmptyHotkey},
        {"Ctrl + ", dpsoEmptyHotkey},
        {"Ctrl + + ", dpsoEmptyHotkey},
        {"+ Ctrl", dpsoEmptyHotkey},
        {"A", {dpsoKeyA, dpsoNoKeyMods}},
        {"Ctrl", {dpsoNoKey, dpsoKeyModCtrl}},
        {"Ctrl + A", {dpsoKeyA, dpsoKeyModCtrl}},
        {"Ctrl A", dpsoEmptyHotkey},
        {"A + Ctrl", dpsoEmptyHotkey},
        {"Ctrl + Ctrl + A", dpsoEmptyHotkey},
        {"Keypad +", {dpsoKeyKpPlus, dpsoNoKeyMods}},
        {"Keypad  +", dpsoEmptyHotkey},

        {"Ctrl + Alt + Shift + Windows + A", {dpsoKeyA, allMods}},
        {"Alt + Shift + Ctrl + Windows + A", {dpsoKeyA, allMods}},
        {"Alt+Shift+Ctrl+Windows+A", {dpsoKeyA, allMods}},
        {
            "  CTRL  +  alt  +  shifT  +  WiNdows  +  a  ",
            {dpsoKeyA, allMods}
        },
        {"Ctrl+Alt+Shift+Windows+Keypad +", {dpsoKeyKpPlus, allMods}},

        {"Windows", {dpsoNoKey, dpsoKeyModWin}},
        {"Command", {dpsoNoKey, dpsoKeyModWin}},
        {"Super", {dpsoNoKey, dpsoKeyModWin}},

        {"Alt", {dpsoNoKey, dpsoKeyModAlt}},
        {"Option", {dpsoNoKey, dpsoKeyModAlt}},
    };

    for (const auto& test : tests) {
        DpsoHotkey got;
        dpsoHotkeyFromString(test.str, &got);

        if (got == test.expectedHotkey)
            continue;

        test::failure(
            "testHotkeyFromString: dpsoHotkeyFromString(\"%s\"): "
            "expected \"%s\", got \"%s\"\n",
            test.str,
            hotkeyToStr(test.expectedHotkey).c_str(),
            hotkeyToStr(got).c_str());
    }
}


static void testHotkeys()
{
    testHotkeyToString();
    testHotkeyFromString();
}


REGISTER_TEST(testHotkeys);
