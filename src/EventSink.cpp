#include "EventSink.h"
#include "EventManager.h"
#include "HotkeyManager.h"

void InputEventSink::Register()
{
    if (auto manager = RE::BSInputDeviceManager::GetSingleton()) {
        manager->AddEventSink(GetSingleton());
        SKSE::log::info("Successfully registered input event.");
    }
    else {
        SKSE::log::error("Failed to register input event.");
    }
}

RE::BSEventNotifyControl InputEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event || !RE::Main::GetSingleton()->gameActive) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (RE::PlayerCharacter* player = Cache::GetPlayerSingleton(); !player || !player->Is3DLoaded()) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (RE::UI* ui = RE::UI::GetSingleton(); !ui || ui->IsMenuOpen(RE::MainMenu::MENU_NAME)) {
        return RE::BSEventNotifyControl::kContinue;
    }

    HotkeyManager::Process(a_event);

    return RE::BSEventNotifyControl::kContinue;
}

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
        }
        else {
            logger::debug("CLOSE MENU {}", event->menuName);
            if (!EventManager::HasAnyStance())
                EventManager::ApplyStance(settings->MidStanceSpell);
            logger::debug("applied {} after {} closed", settings->MidStanceSpell->GetName(), event->menuName);
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
