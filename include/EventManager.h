#pragma once

#include "Cache.h"
#include "PCH.h"
#include "Settings.h"

class EventManager final : public Singleton<EventManager>
{
    friend class Singleton<EventManager>;

public:
    static bool IsPermanent(RE::MagicItem* item);
    static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell);
    static void ApplyStance(RE::SpellItem* a_spell);
    static bool HasAnyStance();
    static void RemoveSpells(RE::SpellItem* a_newStance);
    inline bool IsAnyOfMenuOpen(RE::UI* a_ui, const std::vector<std::string>& a_menuNames);
    bool        IsInMenu(const Settings* a_config, RE::UI* a_ui);

private:
    EventManager() = default;
};
