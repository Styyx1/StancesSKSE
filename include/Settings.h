#pragma once

class Settings : public Singleton<Settings>
{
public:
    void LoadSettings() noexcept;

    void LoadForms() noexcept;

    std::string            FileName;
    inline static uint32_t high_key;
    inline static uint32_t mid_key;
    inline static uint32_t low_key;
    inline static uint32_t mod_key_high;
    inline static uint32_t mod_key_mid;
    inline static uint32_t mod_key_low;

    RE::FormID HighStanceSpellFormID;
    RE::FormID MidStanceSpellFormID;
    RE::FormID LowStanceSpellFormID;

    RE::SpellItem* HighStanceSpell;
    RE::SpellItem* MidStanceSpell;
    RE::SpellItem* LowStanceSpell;

    static RE::FormID ParseFormID(const std::string& str);

    inline static bool debug_logging{};
};
