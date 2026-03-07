//
// Created by styyx on 04/03/2026.
//

#pragma once

namespace STNG
{
    inline constexpr auto TOML_PATH_MAIN = "Data/SKSE/Plugins/StancesNG.toml";
    inline constexpr auto TOML_PATH_CUSTOM = "Data/SKSE/Plugins/StancesNG_custom.toml";
    inline constexpr auto KEYBINDS = "Keybinds";
    inline constexpr auto GENERAL = "General";
    inline constexpr auto FORMS = "Forms";

    inline constexpr RE::FormID BEAR_STANCE_ID = 0x800;
    inline constexpr RE::FormID WOLF_STANCE_ID = 0x801;
    inline constexpr RE::FormID HAWK_STANCE_ID = 0x802;
    inline constexpr RE::FormID PREVIOUS_STANCE_GLOBAL_ID = 0x916;
    inline constexpr RE::FormID CURRENT_STANCE_GLOBAL_ID = 0x917;

    inline constexpr auto PLUGIN_NAME = "StancesNG.esp";

    namespace DATA
    {
        static constexpr std::array slMenuNames{
            RE::BarterMenu::MENU_NAME,    RE::BookMenu::MENU_NAME,        RE::Console::MENU_NAME,      RE::ContainerMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME,
            RE::DialogueMenu::MENU_NAME,  RE::FavoritesMenu::MENU_NAME,   RE::GiftMenu::MENU_NAME,     RE::InventoryMenu::MENU_NAME, RE::JournalMenu::MENU_NAME,
            RE::LevelUpMenu::MENU_NAME,   RE::LockpickingMenu::MENU_NAME, RE::MagicMenu::MENU_NAME,    RE::MapMenu::MENU_NAME,       RE::RaceSexMenu::MENU_NAME,
            RE::SleepWaitMenu::MENU_NAME, RE::StatsMenu::MENU_NAME,       RE::TrainingMenu::MENU_NAME, RE::TutorialMenu::MENU_NAME,  RE::TweenMenu::MENU_NAME,
        };
        static inline std::vector<std::string> sl1MenuNames{ slMenuNames.begin(), slMenuNames.end() };
    }


}