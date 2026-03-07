// #pragma once
//
// struct InputEventSink : REX::Singleton<InputEventSink>, RE::BSTEventSink<RE::InputEvent*>
// {
//     using Event       = RE::InputEvent*;
//     using EventSource = RE::BSTEventSource<Event>;
//
//     static void Register();
//     RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;
// };
