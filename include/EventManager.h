#pragma once

#include "Cache.h"
#include "PCH.h"

class EventManager final : public Singleton<EventManager>
{
    friend class Singleton<EventManager>;

public:
    static bool IsPermanent(RE::MagicItem* item);
    static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell);
    static void ApplyStance(RE::SpellItem* a_spell);
    static bool HasAnyStance();
    static void RemoveSpells(RE::SpellItem* a_newStance);

private:
    EventManager() = default;
};
