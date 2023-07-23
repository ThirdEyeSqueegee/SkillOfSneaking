#pragma once

namespace Events {
    class OnActivateEventHandler : public RE::BSTEventSink<RE::TESActivateEvent> {
    protected:
        OnActivateEventHandler() = default;

    public:
        OnActivateEventHandler(const OnActivateEventHandler&) = delete;
        OnActivateEventHandler(OnActivateEventHandler&&) = delete;
        OnActivateEventHandler& operator=(const OnActivateEventHandler&) = delete;
        OnActivateEventHandler& operator=(OnActivateEventHandler&&) = delete;

        static OnActivateEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) override;

        static void Register();
    };

    class OnContainerChangedEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
    protected:
        OnContainerChangedEventHandler() = default;

    public:
        OnContainerChangedEventHandler(const OnContainerChangedEventHandler&) = delete;
        OnContainerChangedEventHandler(OnContainerChangedEventHandler&&) = delete;
        OnContainerChangedEventHandler& operator=(const OnContainerChangedEventHandler&) = delete;
        OnContainerChangedEventHandler& operator=(OnContainerChangedEventHandler&&) = delete;

        static OnContainerChangedEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* a_event,
                                              RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

        static void Register();
    };
}
