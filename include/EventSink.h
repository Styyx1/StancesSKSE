#pragma once

class InputEventSink final : public Singleton<InputEventSink>, public RE::BSTEventSink<RE::InputEvent*>
{
    friend class Singleton<InputEventSink>;

public:
    using Event       = RE::InputEvent*;
    using EventSource = RE::BSTEventSource<Event>;

    static void Register();

    RE::BSEventNotifyControl ProcessEvent(const Event* a_event, EventSource* a_eventSource) override;

private:
    InputEventSink() = default;
};

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
