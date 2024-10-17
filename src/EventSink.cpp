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
