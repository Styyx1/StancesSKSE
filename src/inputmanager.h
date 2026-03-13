//
// Created by styyx on 04/03/2026.
//
#pragma once

#include "stance-manager.h"

namespace STNG
{
    using st = StanceManager::available_stances;
    using namespace clib_util::hotkeys;
    using EventResult = RE::BSEventNotifyControl;
    struct InputEventListener : REX::Singleton<InputEventListener>, RE::BSTEventSink<RE::InputEvent*>
    {
        static void RegisterInput();
        static void SetKeys();
        static void ProcessStanceKey(const KeyCombination* key);
        KeyCombination hotkey_bear{ProcessStanceKey};
        KeyCombination hotkey_wolf{ProcessStanceKey};
        KeyCombination hotkey_hawk{ProcessStanceKey};
        KeyCombination hotkey_neutral{ProcessStanceKey};

        std::array<std::pair<KeyCombination*, st>, 4> keySpellCombo = {
            {
                { &hotkey_bear,  st::kBearStance},
                { &hotkey_wolf,  st::kWolfStance},
                {&hotkey_hawk, st::kHawkStance},
                {&hotkey_neutral, st::kNeutralStance}
            }
        };
    protected:
        EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;
    };
}
