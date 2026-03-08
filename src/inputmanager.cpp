//
// Created by styyx on 04/03/2026.
//

#include "inputmanager.h"
#include "Settings.h"

void STNG::InputEventListener::RegisterInput()
{
    if (const auto manager = RE::BSInputDeviceManager::GetSingleton()) {
        manager->AddEventSink(GetSingleton());
        SKSE::log::info("[[REGISTERED]] for {}", typeid(RE::InputEvent).name());
    }
}

void STNG::InputEventListener::SetKeys()
{
    using s = Config::Settings;
    const auto i = GetSingleton();
    if (!i->hotkey_neutral.SetPattern(s::neutral_stance_key.GetValue()))
    {
        logs::error("neutral stance key set failed");
    }
    if (!i->hotkey_bear.SetPattern(s::bear_stance_key.GetValue()))
    {
        logs::error("bear stance key set failed");
    }
    if (!i->hotkey_hawk.SetPattern(s::hawk_stance_key.GetValue()))
    {
        logs::error("hawk stance key set failed");
    }
    if (!i->hotkey_wolf.SetPattern(s::wolf_stance_key.GetValue()))
    {
        logs::error("wolf stance key set failed");
    }
}

void STNG::InputEventListener::ProcessStanceKey(const clib_util::hotkeys::KeyCombination* key)
{
    for(auto& [hotkey, stance] : GetSingleton()->keySpellCombo)
    {
        if (key == &GetSingleton()->hotkey_wolf && key->IsTriggered())
        {
            if (StanceManager::CycleStancesPlayer())
                return;
        }

        if (key == hotkey && key->IsTriggered())
        {
            StanceManager::UpdateStancePlayer(stance);
            break;
        }
    }
}

STNG::EventResult STNG::InputEventListener::ProcessEvent( RE::InputEvent* const* a_event,
                                                          RE::BSTEventSource<RE::InputEvent*>*)
{
    if (!a_event)
        return EventResult::kContinue;

    hotkey_neutral.Process(a_event, true);
    hotkey_wolf.Process(a_event, true);
    hotkey_bear.Process(a_event, true);
    hotkey_hawk.Process(a_event, true);

    return EventResult::kContinue;
}

