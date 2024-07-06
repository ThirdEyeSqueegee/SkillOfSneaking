#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool debug_logging{};

    inline static float xp_gain_mult{};

    inline static auto av_to_use{ RE::ActorValue::kSneak };
};
