#pragma once
#include "EventManager.h"

class MenuEvent final : public Singleton<MenuEvent>, public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
    friend class Singleton<MenuEvent>;
public:
    using Event       = RE::MenuOpenCloseEvent*;
    using EventSource = RE::BSTEventSource<Event>;
    static void Register();
    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
private:
    MenuEvent() = default;
};

RE::BSEventNotifyControl MenuEvent::ProcessEvent(const RE::MenuOpenCloseEvent* event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
{
    auto settings = Settings::GetSingleton();
    if (!event)
        return RE::BSEventNotifyControl::kContinue;
    if (event->menuName != RE::RaceSexMenu::MENU_NAME)
        return RE::BSEventNotifyControl::kContinue;
    if (event->menuName == RE::RaceSexMenu::MENU_NAME) {
        if (event->opening) {
            logger::debug("OPEN MENU {}", event->menuName);
            return RE::BSEventNotifyControl::kContinue;
        }
        else {
            logger::debug("CLOSE MENU {}", event->menuName);
            if (!EventManager::HasAnyStance() && Settings::neutral_stance_key == 0)
                EventManager::ApplyStance(settings->MidStanceSpell);
            logger::debug("applied {} after {} closed", settings->MidStanceSpell->GetName(), event->menuName);
            return RE::BSEventNotifyControl::kContinue;
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}
void MenuEvent::Register()
{
    if (auto* eventSink = MenuEvent::GetSingleton()) {
        RE::UI::GetSingleton()->AddEventSink(eventSink);
        logger::info("Registered for Menu Event");
    }
    else {
        SKSE::log::error("Failed to register menu event.");
    }
}
