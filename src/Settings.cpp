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

    sneak_xp_gain_mult = static_cast<float>(ini.GetDoubleValue("General", "fSneakXPGainMult"));

    logger::info("Loaded settings");
    logger::info("\tfSneakXPGainMult = {:.2f}", sneak_xp_gain_mult);
    logger::info("");
}
