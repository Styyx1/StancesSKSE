#include "EventManager.h"
#include "PCH.h"
#include "settings.h"

bool EventManager::IsPermanent(RE::MagicItem* item)
{
    switch (item->GetSpellType()) {
    case RE::MagicSystem::SpellType::kDisease:
    case RE::MagicSystem::SpellType::kAbility:
    case RE::MagicSystem::SpellType::kAddiction: {
        return true;
    }
    default: {
        return false;
    }
    }
};

void EventManager::ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
{
    if (EventManager::IsPermanent(spell)) {
        target->AddSpell(spell);
    }
    else {
        caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
    }
};

void EventManager::ApplyStance(RE::SpellItem* a_spell)
{
    auto player = RE::PlayerCharacter::GetSingleton();
    EventManager::ApplySpell(player, player, a_spell);
}
