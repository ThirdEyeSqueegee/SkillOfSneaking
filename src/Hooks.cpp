#include "Hooks.h"

#include "Settings.h"

constexpr auto sneak_av{ RE::ActorValue::kSneak };

namespace Hooks
{
    void Install()
    {
        if (REL::Module::IsVR())
            PickupObject::idx = 206; // 0xce

        stl::write_vfunc<RE::TESFlora, ActivateFlora>();
        logger::info("Installed TESFlora::Activate hook");

        stl::write_vfunc<RE::PlayerCharacter, AddObjectToContainer>();
        logger::info("Installed PlayerCharacter::AddObjectToContainer hook");

        stl::write_vfunc<RE::PlayerCharacter, PickupObject>();
        logger::info("Installed PlayerCharacter::PickupObject hook");
    }

    bool ActivateFlora::Thunk(RE::TESFlora* a_this, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3,
                              RE::TESBoundObject* a_object, std::int32_t a_targetCount)
    {
        const auto result{ func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount) };

        if (!a_targetRef)
            return result;

        const auto name{ a_targetRef->GetName() };

        if ("Coin Purse"sv.compare(name) || !a_targetRef->IsCrimeToActivate() || !a_activatorRef->IsPlayerRef())
            return result;

        if (const auto player{ RE::PlayerCharacter::GetSingleton() })
        {
            if (!player->IsSneaking())
                return result;

            if (const auto detection_level{ player->RequestDetectionLevel(player) }; detection_level <=> 0 >= 0)
                return result;

            logger::debug("Stealing {} (0x{:x})", name, a_targetRef->GetFormID());

            std::random_device            rd;
            std::mt19937                  rng(rd());
            std::uniform_int_distribution dist(0, 20);

            const auto xp_to_add{ static_cast<float>(dist(rng)) * Settings::sneak_xp_gain_mult };
            player->AddSkillExperience(sneak_av, xp_to_add);
            logger::debug("\tAdded {} Sneak XP", xp_to_add);
        }

        return result;
    }

    void AddObjectToContainer::Thunk(RE::PlayerCharacter* a_this, RE::TESBoundObject* a_object, RE::ExtraDataList* a_extraList, std::int32_t a_count,
                                     RE::TESObjectREFR* a_fromRefr)
    {
        func(a_this, a_object, a_extraList, a_count, a_fromRefr);

        if (!(a_this && a_object && a_extraList && a_fromRefr))
            return;

        if (!a_this->IsSneaking())
            return;

        if (const auto detection_level{ a_this->RequestDetectionLevel(a_this) }; detection_level <=> 0 >= 0)
            return;

        if (const auto origin{ a_fromRefr->GetBaseObject() })
        {
            const auto origin_form_type{ origin->GetFormType() };
            const auto obj_name{ a_object->GetName() };
            const auto obj_form_id{ a_object->GetFormID() };
            if (origin_form_type <=> RE::FormType::Container != 0)
                return;
            logger::debug("Taking object {} (0x{:x}) from {} (0x{:x}) ({})", obj_name, obj_form_id, a_fromRefr->GetName(), a_fromRefr->GetFormID(),
                          origin_form_type);

            if (const auto owner{ a_extraList->GetOwner() })
            {
                const auto owner_name{ owner->GetName() };
                logger::debug("\tOwner: {} (0x{:x})", owner_name, owner->GetFormID());
                if (owner->IsPlayerRef())
                    return;

                if (const auto value{ static_cast<float>(a_object->GetGoldValue()) }; value <=> 0.0f < 0)
                    logger::error("{} (0x{:x}) has negative value", obj_name, obj_form_id);
                else
                {
                    logger::debug("\t{} (0x{:x}) value: {}", obj_name, obj_form_id, value);
                    const auto xp_to_add{ value * Settings::sneak_xp_gain_mult };
                    a_this->AddSkillExperience(sneak_av, xp_to_add);
                    logger::debug("\tAdded {} Sneak XP", xp_to_add);
                }
            }
        }
    }

    void PickupObject::Thunk(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_object, uint32_t a_count, bool a_arg3, bool a_playSound)
    {
        func(a_this, a_object, a_count, a_arg3, a_playSound);

        if (!(a_object && a_this))
            return;

        if (!a_this->IsSneaking() || !a_object->IsCrimeToActivate() || !a_object->GetOwner())
            return;

        if (a_object->GetOwner()->IsPlayerRef())
            return;

        if (const auto detection_level{ a_this->RequestDetectionLevel(a_this) }; detection_level <=> 0 >= 0)
            return;

        const auto obj_name{ a_object->GetName() };
        const auto obj_form_id{ a_object->GetFormID() };
        logger::debug("Stealing {} (0x{:x})", obj_name, obj_form_id);

        if (const auto base_obj{ a_object->GetBaseObject() })
        {
            if (const auto value{ static_cast<float>(base_obj->GetGoldValue()) }; value <=> 0.0f < 0)
                logger::error("{} (0x{:x}) has negative value", obj_name, obj_form_id);
            else
            {
                logger::debug("\t{} (0x{:x}) value: {}", obj_name, obj_form_id, value);
                const auto xp_to_add{ value * Settings::sneak_xp_gain_mult };
                a_this->AddSkillExperience(sneak_av, xp_to_add);
                logger::debug("\tAdded {} Sneak XP", xp_to_add);
            }
        }
    }
} // namespace Hooks
