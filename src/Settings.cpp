#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\SkillOfSneaking.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    xp_gain_mult = static_cast<float>(ini.GetDoubleValue("General", "fXPGainMult"));

    if (RE::TESDataHandler::GetSingleton()->LookupLoadedModByName("HandtoHand.esp"sv)) {
        logger::info("Found HandtoHand.esp, using Security skill");
        av_to_use = RE::ActorValue::kPickpocket;
    }

    logger::info("Loaded settings");
    logger::info("\tfXPGainMult = {:.2f}", xp_gain_mult);
    logger::info("");
}
