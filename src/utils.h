//
// Created by styyx on 04/03/2026.
//

#pragma once
#include "mod-data.h"

namespace STNG
{
    namespace MISC
    {
        static bool IsModLoaded(const std::string_view mod_name) {
            const auto data_handler = RE::TESDataHandler::GetSingleton();

            if (const auto main_file = data_handler->LookupModByName(mod_name); !main_file || main_file->compileIndex == 0xFF)
                return false;
            return true;
        }
        static bool IsAnyMenuOpen()
        {
            const auto ui = RE::UI::GetSingleton();
            for (auto& menu : DATA::sl1MenuNames)
            {
                if (ui->IsMenuOpen(menu))
                {
                    return true;
                }
            }
            return false;
        }

        static RE::TESForm* GetFormFromString(const std::string& formIDstring)
        {
            if (formIDstring.empty()) {
                return nullptr;
            }

            if (formIDstring.find('|') != std::string::npos) {
                std::istringstream ss{ formIDstring };
                std::string plugin, id;
                std::getline(ss, plugin, '|');
                std::getline(ss, id);

                if (plugin.empty() || id.empty()) {
                    return nullptr;
                }

                RE::FormID rawFormID{};
                std::istringstream(id) >> std::hex >> rawFormID;

                if (!rawFormID) {
                    return nullptr;
                }

                if (auto* dataHandler = RE::TESDataHandler::GetSingleton()) {
                    auto* form = dataHandler->LookupForm(rawFormID, plugin);
                    return form;
                }
                return nullptr;
            }

            if (auto* form = RE::TESForm::LookupByEditorID(formIDstring)) {
                return form;
            }
            return nullptr;
        }
    };

    namespace MAGIC
    {
        static bool IsPermanent(const RE::MagicItem *item)
        {
            switch (item->GetSpellType())
            {
            case RE::MagicSystem::SpellType::kDisease:
            case RE::MagicSystem::SpellType::kAbility:
            case RE::MagicSystem::SpellType::kAddiction:
                return true;
            default:
                return false;
            }
        }

        static void ApplySpell(RE::Actor *caster, RE::Actor *target, RE::SpellItem *spell)
        {
            if (IsPermanent(spell))
            {
                target->AddSpell(spell);
            }
            else
            {
                caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                    ->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
            }
        }
    }
}
