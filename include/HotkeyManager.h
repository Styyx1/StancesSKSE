#pragma once
#include "Settings.h"
#include <PCH.h>
#include <Xinput.h>

class HotkeyManager
{
public:
    static void        Process(const RE::InputEvent* const* a_event);
    inline static bool once;
};
