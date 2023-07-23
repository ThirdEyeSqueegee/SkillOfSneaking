#include "Events.h"

#include "Settings.h"

namespace Events {
    OnActivateEventHandler* OnActivateEventHandler::GetSingleton() {
        static OnActivateEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event,
                                                                  RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->actionRef->IsPlayerRef()) {
            if (a_event->objectActivated->IsCrimeToActivate()
                && a_event->objectActivated->GetFormType() != RE::FormType::Container) {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->IsSneaking()) {
                    if (player->RequestDetectionLevel(player->As<RE::Actor>()) < 0) {
                        if (const auto obj_value = static_cast<float>(a_event->objectActivated->GetBaseObject()->GetGoldValue());
                            obj_value > 0) {
                            const auto xp_gain = obj_value * Settings::sneak_xp_gain_mult;
                            player->AddSkillExperience(RE::ActorValue::kSneak, xp_gain);
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnActivateEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered activate event handler");
    }

    OnContainerChangedEventHandler* OnContainerChangedEventHandler::GetSingleton() {
        static OnContainerChangedEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnContainerChangedEventHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event,
                                                                          RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (const auto player = RE::PlayerCharacter::GetSingleton();
            player->GetParentCell() && player->Is3DLoaded() && player->IsSneaking()) {
            if (player->RequestDetectionLevel(player->As<RE::Actor>()) < 0) {
                if (player->GetFormID() == a_event->newContainer) {
                    if (a_event->oldContainer) {
                        const auto changes = player->GetInventoryChanges(true);
                        const auto obj = RE::TESForm::LookupByID<RE::TESBoundObject>(a_event->baseObj);
                        for (const auto item : *changes->entryList) {
                            if (item->object == obj) {
                                if (!item->GetOwner()->As<RE::PlayerCharacter>()) {
                                    if (const auto obj_value = static_cast<float>(obj->GetGoldValue()); obj_value > 0) {
                                        const auto xp_gain = obj_value * Settings::sneak_xp_gain_mult;
                                        player->AddSkillExperience(RE::ActorValue::kSneak, xp_gain);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnContainerChangedEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered container changed event handler");
    }
}
