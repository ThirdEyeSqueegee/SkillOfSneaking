#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings();

    inline static bool debug_logging{};

    inline static float sneak_xp_gain_mult{};
};
