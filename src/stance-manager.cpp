//
// Created by styyx on 04/03/2026.
//

#include "stance-manager.h"

#include "Cache.h"
#include "formloader.h"
#include "Settings.h"

namespace STNG
{
    void StanceManager::ApplyStance(available_stances a_stance, RE::Actor* a_actor)
    {
        RemoveAllStances(a_actor);
        const auto spell = GetStanceSpell(a_stance);
        if (!spell)
        {
            logs::debug("no spell");
            return;
        }

        if (!a_actor)
            return;
        logs::debug("selected spell is: {}", spell->GetName());
        MAGIC::ApplySpell(a_actor, a_actor, spell);
    }

    void StanceManager::SetPreviousStance(const available_stances a_stance)
    {
        FormLoader::previous_stance_global->value = static_cast<float>(std::to_underlying(a_stance));
    }

    void StanceManager::SetCurrentStance(const available_stances a_stance)
    {
        FormLoader::current_stance_global->value = static_cast<float>(std::to_underlying(a_stance));
    }

    void StanceManager::RemoveAllStances(RE::Actor* a_actor)
    {
        if (!a_actor)
            return;
        for (const auto& spell : FormLoader::stance_spells)
        {
            if (spell && a_actor->HasSpell(spell))
                a_actor->RemoveSpell(spell);
        }
    }

    void StanceManager::PlayTransitionAnimation(RE::Actor* a_actor)
    {
        if (!a_actor)
            return;
        const auto proc = a_actor->GetActorRuntimeData().currentProcess;
        if (!proc)
            return;
        if (!FormLoader::transition_animation)
            return;
        proc->PlayIdle(a_actor, FormLoader::transition_animation, nullptr);
    }

    RE::SpellItem* StanceManager::GetStanceSpell(const available_stances a_stance)
    {
        switch (a_stance)
        {
        case available_stances::kBearStance:
            return FormLoader::bear_stance_spell;
        case available_stances::kWolfStance:
            return FormLoader::wolf_stance_spell;
        case available_stances::kHawkStance:
            return FormLoader::hawk_stance_spell;
        default:
            return nullptr;
        }
    }

    const char* StanceUtil::GetStanceString(const StanceManager::available_stances a_stance)
    {
        switch (a_stance)
        {
        case StanceManager::available_stances::kBearStance:
            return "kBearStance";
        case StanceManager::available_stances::kWolfStance:
            return "kWolfStance";
        case StanceManager::available_stances::kHawkStance:
            return "kHawkStance";
        case StanceManager::available_stances::kNeutralStance:
            return "kNeutralStance";
        default:
            return "InvalidStance";
        }
    }

    void StanceManager::ApplyDefaultStance()
    {
        if (!Config::Settings::apply_stance_on_start.GetValue())
            return;
        const auto player = Cache::GetPlayerSingleton();
        if (!player)
            return;
        if (HasAnyOfStances(player))
            return;
        ApplyStance(available_stances::kWolfStance, player);
    }

    void StanceManager::UpdateStance(available_stances a_stance, RE::Actor* a_actor)
    {
        const auto stance = static_cast<available_stances>(static_cast<int>(FormLoader::current_stance_global->value));
        logs::debug("last used stance was: {}, switching to: {}", StanceUtil::GetStanceString(stance), StanceUtil::GetStanceString(a_stance));
        SetPreviousStance(stance);
        ApplyStance(a_stance, a_actor);
        SetCurrentStance(a_stance);
        PlayTransitionAnimation(a_actor);
    }

    bool StanceManager::HasAnyOfStances(const RE::Actor* a_actor)
    {
        if (!a_actor)
            return false;

        return std::ranges::any_of(FormLoader::stance_spells, [a_actor](const auto& spell)
        {
            return a_actor->HasSpell(spell);
        });
    }

    void StanceManager::UpdateStancePlayer(available_stances a_stance)
    {
        const auto player = Cache::GetPlayerSingleton();
        if (!player)
            return;
        UpdateStance(a_stance, player);
    }

    bool StanceManager::PlayerHasAnyStance()
    {
        const auto player = Cache::GetPlayerSingleton();
        if (!player)
            return false;
        return HasAnyOfStances(player);
    }

    bool StanceManager::CycleStancesPlayer()
    {
        if (!Config::Settings::use_cycling.GetValue())
            return false;
        auto stance_int = static_cast<int>(FormLoader::current_stance_global->value) + 1;
        if (stance_int > 3)
            stance_int = 1;
        const auto curr_stance = static_cast<available_stances>(stance_int);
        UpdateStancePlayer(curr_stance);
        return true;
    }
}
