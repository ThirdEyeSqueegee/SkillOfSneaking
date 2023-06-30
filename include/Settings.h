#pragma once

class Settings {
protected:
    Settings() = default;
    ~Settings() = default;

public:
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

    static Settings* GetSingleton();
    static void LoadSettings();

    inline static float sneak_xp_gain = 5.0f;
};
