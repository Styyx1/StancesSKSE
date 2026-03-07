#pragma once
#include "mod-data.h"

namespace Config
{
    struct Settings
    {
        static inline REX::TOML::Str bear_stance_key{STNG::KEYBINDS, "sBearStanceKey", "shift+x"};
        static inline REX::TOML::Str wolf_stance_key{STNG::KEYBINDS, "sWolfStanceKey", "x"};
        static inline REX::TOML::Str hawk_stance_key{STNG::KEYBINDS, "sHawkStanceKey", "control+x"};
        static inline REX::TOML::Str neutral_stance_key{STNG::KEYBINDS, "sNeutralStanceKey", "alt+v"};

        static inline REX::TOML::Bool use_cycling {STNG::GENERAL, "bUseCycling", false};
        static inline REX::TOML::Bool apply_stance_on_start{STNG::GENERAL, "bApplyStanceOnStart", false};
        static inline REX::TOML::Bool play_transition_animation{STNG::GENERAL, "bPlayTransitionAnimation", false};
        static inline REX::TOML::Str transition_animation_form{STNG::FORMS, "sTransitionAnimationForm", ""};

        static void UpdateSettings(const bool a_save)
        {
            const auto toml = REX::TOML::SettingStore::GetSingleton();
            toml->Init(STNG::TOML_PATH_MAIN, STNG::TOML_PATH_CUSTOM);
            if (!a_save)
                toml->Load();
            else
                toml->Save();
        }
    };
}

