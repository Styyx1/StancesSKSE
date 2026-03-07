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
        void UpdateStance(available_stances a_stance, RE::Actor* a_actor);
        static void UpdateStancePlayer(available_stances a_stance);
        [[nodiscard]] bool PlayerHasAnyStance() const;
        static bool CycleStancesPlayer();

    private:
        bool HasAnyOfStances(const RE::Actor* a_actor) const;
        [[nodiscard]] RE::SpellItem* GetStanceSpell(available_stances a_stance) const;
        void ApplyStance(available_stances a_stance, RE::Actor* a_actor) const;
        void SetPreviousStance(available_stances a_stance) const;
        void SetCurrentStance(available_stances a_stance) const;
        void RemoveAllStances(RE::Actor* a_actor) const;
        void PlayTransitionAnimation(RE::Actor* a_actor);
    };

    struct StanceUtil
    {
        static const char* GetStanceString(StanceManager::available_stances a_stance);
    };

}
