#include "Events.h"

#include "Settings.h"

namespace Events {
    OnActivateEventHandler* OnActivateEventHandler::GetSingleton() {
        static OnActivateEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event,
                                                                  RE::BSTEventSource<RE::TESActivateEvent>*
                                                                  a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (a_event->actionRef->IsPlayerRef()) {
            if (a_event->objectActivated->IsCrimeToActivate()) {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->IsSneaking()) {
                    if (const auto detection_level = player->RequestDetectionLevel(player->As<RE::Actor>());
                        detection_level == -1000) {
                        player->AddSkillExperience(RE::ActorValue::kSneak, Settings::sneak_xp_gain);
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
}
