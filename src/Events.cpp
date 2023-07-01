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
                    const auto detection_level = player->RequestDetectionLevel(player->As<RE::Actor>());
                    if (detection_level == -1000) {
                        const auto obj_value = static_cast<float>(a_event->objectActivated->GetBaseObject()->
                                                                           GetGoldValue());
                        if (obj_value > 0) {
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
}
