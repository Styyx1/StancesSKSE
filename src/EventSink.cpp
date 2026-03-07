// #include "EventSink.h"
//
// #include "Cache.h"
// #include "HotkeyManager.h"
// #include "utils.h"
//
// void InputEventSink::Register()
// {
//     if (const auto manager = RE::BSInputDeviceManager::GetSingleton()) {
//         manager->AddEventSink(GetSingleton());
//         SKSE::log::info("[[REGISTERED]] for {}", typeid(RE::InputEvent).name());
//     }
// }
//
// RE::BSEventNotifyControl InputEventSink::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*)
// {
//     if (!a_event || !RE::Main::GetSingleton()->gameActive) {
//         return RE::BSEventNotifyControl::kContinue;
//     }
//     if (const RE::PlayerCharacter* player = Cache::GetPlayerSingleton(); !player || !player->Is3DLoaded()) {
//         return RE::BSEventNotifyControl::kContinue;
//     }
//     if (STNG::MISC::IsAnyMenuOpen()) {
//         return RE::BSEventNotifyControl::kContinue;
//     }
//     HotkeyManager::Process(a_event);
//     return RE::BSEventNotifyControl::kContinue;
// }
