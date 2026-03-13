//
// Created by styyx on 04/03/2026.
//

#pragma once

namespace STNG
{
    struct StanceManager : REX::Singleton<StanceManager>
    {
        enum class available_stances : uint32_t
        {
            kNeutralStance = 0,
            kBearStance = 1,
            kWolfStance = 2,
            kHawkStance = 3,
        };

        static void ApplyDefaultStance();
        static void UpdateStance(available_stances a_stance, RE::Actor* a_actor);
        static void UpdateStancePlayer(available_stances a_stance);
        [[nodiscard]] static bool PlayerHasAnyStance() ;
        static bool CycleStancesPlayer();

    private:
        static bool HasAnyOfStances(const RE::Actor* a_actor);
        [[nodiscard]] static RE::SpellItem* GetStanceSpell(available_stances a_stance) ;
        static void ApplyStance(available_stances a_stance, RE::Actor* a_actor);
        static void SetPreviousStance(available_stances a_stance) ;
        static void SetCurrentStance(available_stances a_stance) ;
        static void RemoveAllStances(RE::Actor* a_actor) ;
        static void PlayTransitionAnimation(RE::Actor* a_actor);
    };

    struct StanceUtil
    {
        static const char* GetStanceString(StanceManager::available_stances a_stance);
    };

}
