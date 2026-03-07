#pragma once
#include "stance-manager.h"

using Event       = const RE::MenuOpenCloseEvent*;
using EventSource = RE::BSTEventSource<RE::MenuOpenCloseEvent>*;
using EventRes = RE::BSEventNotifyControl;

struct MenuEvent : REX::Singleton<MenuEvent>, RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
    static void Register();
    EventRes ProcessEvent(Event a_event, [[maybe_unused]] EventSource a_src) override;
};

inline EventRes MenuEvent::ProcessEvent(Event a_event, [[maybe_unused]] EventSource a_src)
{
    if (!a_event)
        return EventRes::kContinue;
    if (a_event->menuName != RE::RaceSexMenu::MENU_NAME)
        return EventRes::kContinue;
    if (a_event->opening)
        return EventRes::kContinue;

    STNG::StanceManager::GetSingleton()->ApplyDefaultStance();
    return EventRes::kContinue;
}

inline void MenuEvent::Register()
{
    if (auto* eventSink = GetSingleton()) {
        RE::UI::GetSingleton()->AddEventSink(eventSink);
        logs::info("Registered for Menu Event");
    }
    else {
        SKSE::log::error("Failed to register menu event.");
    }
}
