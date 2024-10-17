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
    inline static uint32_t cycleKey;

    RE::FormID HighStanceSpellFormID;
    RE::FormID MidStanceSpellFormID;
    RE::FormID LowStanceSpellFormID;

    RE::SpellItem* HighStanceSpell;
    RE::SpellItem* MidStanceSpell;
    RE::SpellItem* LowStanceSpell;

    static RE::FormID  ParseFormID(const std::string& str);
    inline static bool useCycle{};

    inline static bool debug_logging{};

    static constexpr std::array slMenuNames{
        RE::BarterMenu::MENU_NAME,    RE::BookMenu::MENU_NAME,        RE::Console::MENU_NAME,      RE::ContainerMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME,
        RE::DialogueMenu::MENU_NAME,  RE::FavoritesMenu::MENU_NAME,   RE::GiftMenu::MENU_NAME,     RE::InventoryMenu::MENU_NAME, RE::JournalMenu::MENU_NAME,
        RE::LevelUpMenu::MENU_NAME,   RE::LockpickingMenu::MENU_NAME, RE::MagicMenu::MENU_NAME,    RE::MapMenu::MENU_NAME,       RE::RaceSexMenu::MENU_NAME,
        RE::SleepWaitMenu::MENU_NAME, RE::StatsMenu::MENU_NAME,       RE::TrainingMenu::MENU_NAME, RE::TutorialMenu::MENU_NAME,  RE::TweenMenu::MENU_NAME,
    };
    std::vector<std::string> sl1MenuNames{ slMenuNames.begin(), slMenuNames.end() };
};
