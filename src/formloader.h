//
// Created by styyx on 04/03/2026.
//
#pragma once
#include "mod-data.h"
#include "Settings.h"
#include "utils.h"

namespace STNG
{
    struct FormLoader
    {
        static inline RE::SpellItem* bear_stance_spell;
        static inline RE::SpellItem* wolf_stance_spell;
        static inline RE::SpellItem* hawk_stance_spell;
        static inline RE::TESGlobal* previous_stance_global;
        static inline RE::TESGlobal* current_stance_global;

        static inline RE::TESIdleForm* transition_animation;

        static inline std::vector<RE::SpellItem*> stance_spells;

        static void PopulateStanceSpellVector()
        {
            stance_spells.clear();
            stance_spells.emplace_back(bear_stance_spell);
            stance_spells.emplace_back(wolf_stance_spell);
            stance_spells.emplace_back(hawk_stance_spell);
        };

        static bool AreGameFormsValid()
        {
            return bear_stance_spell && wolf_stance_spell && hawk_stance_spell && previous_stance_global && current_stance_global;
        }

        static bool LoadFormFromConfig(){
            auto form = MISC::GetFormFromString(Config::Settings::transition_animation_form.GetValue());
            if (!form)
                return false;
            transition_animation = form->As<RE::TESIdleForm>();
            return transition_animation != nullptr;
        }

        static void LoadForms()
        {
            if (!MISC::IsModLoaded(PLUGIN_NAME))
            {
                SKSE::stl::report_and_fail(std::format("{} is not loaded, make sure it is enabled!", PLUGIN_NAME));
            }
            const auto dh = RE::TESDataHandler::GetSingleton();
            bear_stance_spell = dh->LookupForm<RE::SpellItem>(BEAR_STANCE_ID, PLUGIN_NAME);
            wolf_stance_spell = dh->LookupForm<RE::SpellItem>(WOLF_STANCE_ID, PLUGIN_NAME);
            hawk_stance_spell = dh->LookupForm<RE::SpellItem>(HAWK_STANCE_ID, PLUGIN_NAME);
            previous_stance_global = dh->LookupForm<RE::TESGlobal>(PREVIOUS_STANCE_GLOBAL_ID, PLUGIN_NAME);
            current_stance_global = dh->LookupForm<RE::TESGlobal>(CURRENT_STANCE_GLOBAL_ID, PLUGIN_NAME);

            if (!AreGameFormsValid())
            {
                SKSE::stl::report_and_fail("Loaded forms are invalid, please don't modify the StancesNG.esp directly");
            }
            PopulateStanceSpellVector();
            if (LoadFormFromConfig())
            {
                logs::info("Loaded form from config");
            }
        };
    };
}