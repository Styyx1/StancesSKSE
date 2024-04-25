#pragma once
#include "Settings.h"
#include <Xinput.h>
#include <PCH.h>

class HotkeyManager
{
public:
    static void Process(const RE::InputEvent* const* a_event);
};

