#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\StancesNG.ini)");

    std::string fileName(ini.GetValue("General", "sModFileName", ""));
    std::string high_stance_spell_ID(ini.GetValue("FormID", "HighStanceSpellFormID", ""));
    std::string mid_stance_spell_ID(ini.GetValue("FormID", "MidStanceSpellFormID", ""));
    std::string low_stance_spell_ID(ini.GetValue("FormID", "LowStanceSpellFormID", ""));

    high_key                = std::stoi(ini.GetValue("Keys", "iHighStanceKey", "257"));
    mid_key                 = std::stoi(ini.GetValue("Keys", "iMidStanceKey", "258"));
    low_key                 = std::stoi(ini.GetValue("Keys", "iLowStanceKey", "259"));
    mod_key_high            = std::stoi(ini.GetValue("Keys", "iModifierKeyHighStance", "260"));
    mod_key_mid             = std::stoi(ini.GetValue("Keys", "iModifierKeyMidStance", "260"));
    mod_key_low             = std::stoi(ini.GetValue("Keys", "iModifierKeyLowStance", "260"));
    neutral_stance_key      = std::stoi(ini.GetValue("Keys", "iNeutralStanceKey", "256"));
    neutral_stance_modifier = std::stoi(ini.GetValue("Keys", "iModifierNeutralStance", "260"));

    useCycle      = ini.GetBoolValue("Keys", "bUseCycling");
    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (!high_stance_spell_ID.empty()) {
        HighStanceSpellFormID = ParseFormID(high_stance_spell_ID);
    }

    if (!mid_stance_spell_ID.empty()) {
        MidStanceSpellFormID = ParseFormID(mid_stance_spell_ID);
    }
    if (!low_stance_spell_ID.empty()) {
        LowStanceSpellFormID = ParseFormID(low_stance_spell_ID);
    }

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    };

    // Load settings
    FileName = fileName;
    logger::info("Loaded settings");
};

RE::FormID Settings::ParseFormID(const std::string& str)
{
    RE::FormID         result;
    std::istringstream ss{ str };
    ss >> std::hex >> result;
    return result;
}

void Settings::LoadForms() noexcept
{
    auto dataHandler = RE::TESDataHandler::GetSingleton();

    logger::info("Loading forms");
    if (HighStanceSpellFormID) {
        HighStanceSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(HighStanceSpellFormID, FileName));
        logger::debug("High stance spell {} loaded", HighStanceSpell->GetName());
        logger::debug("ID for it is {}", HighStanceSpell->GetFormID());
    }

    if (MidStanceSpellFormID) {
        MidStanceSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MidStanceSpellFormID, FileName));
        logger::debug("High stance spell {} loaded", MidStanceSpell->GetName());
        logger::debug("ID for it is {}", MidStanceSpell->GetFormID());
    }

    if (LowStanceSpellFormID) {
        LowStanceSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(LowStanceSpellFormID, FileName));
        logger::debug("High stance spell {} loaded", LowStanceSpell->GetName());
        logger::debug("ID for it is {}", LowStanceSpell->GetFormID());
    }

    logger::info("All Forms loaded");

} // Credits: Colinswrath https://github.com/colinswrath/BladeAndBlunt
