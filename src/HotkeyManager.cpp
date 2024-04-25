#include "HotkeyManager.h"

#include "EventManager.h"
#include "PCH.h"


namespace
{
    inline std::uint32_t RemapKey(std::uint32_t a_key, RE::INPUT_DEVICE a_device)
    {
        switch (a_device) {
        case RE::INPUT_DEVICE::kKeyboard:
            break;
        case RE::INPUT_DEVICE::kMouse:
            a_key += SKSE::InputMap::kMacro_MouseButtonOffset;
            break;
        case RE::INPUT_DEVICE::kGamepad:
            a_key = SKSE::InputMap::GamepadMaskToKeycode(a_key);
            break;
        default:
            break;
        }
        return a_key;
    }

    class KeyCombo
    {
    public:
        constexpr KeyCombo(std::uint32_t a_targetHotkey, std::uint32_t a_targetModifier) noexcept : targetHotkey(a_targetHotkey), targetModifier(a_targetModifier) {}

        bool IsActive() const noexcept //
        {
            return (hasHotkey && hasModifier) || (hasHotkey && targetModifier == 0);
        }

        void UpdateDown(std::uint32_t a_key) noexcept
        {
            if (targetHotkey != 0 && a_key == targetHotkey) {
                hasHotkey = true;
            }
        }

        void UpdatePressed(std::uint32_t a_key) noexcept
        {
            if (targetModifier != 0 && a_key == targetModifier) {
                hasModifier = true;
            }
        }

    private:
        const std::uint32_t targetHotkey;
        const std::uint32_t targetModifier;

        bool hasHotkey{ false };
        bool hasModifier{ false };
    };

    class HotkeyContext
    {
    public:
        HotkeyContext(const Settings* config)
            : hotkey_high(config->high_key, config->mod_key_high), hotkey_mid(config->mid_key, config->mod_key_mid), hotkey_low(config->low_key, config->mod_key_low)
        {
        }

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed()) {
                auto key = RemapKey(a_button->GetIDCode(), a_button->GetDevice());

                hotkey_high.UpdatePressed(key);
                hotkey_mid.UpdatePressed(key);
                hotkey_low.UpdatePressed(key);

                if (a_button->IsDown()) {
                    hotkey_high.UpdateDown(key);
                    hotkey_mid.UpdateDown(key);
                    hotkey_low.UpdateDown(key);
                }
            }
        }

        void Finalize(EventManager* input)
        {
            auto settings = Settings::GetSingleton();

            if (hotkey_mid.IsActive()) {
                input->ApplyStance(settings->MidStanceSpell);
            }

            if (hotkey_low.IsActive()) {
                input->ApplyStance(settings->LowStanceSpell);
            }

            if (hotkey_high.IsActive()) {
                input->ApplyStance(settings->HighStanceSpell);
            }
        }

    private:
        KeyCombo hotkey_high;
        KeyCombo hotkey_mid;
        KeyCombo hotkey_low;
    };
}

    void HotkeyManager::Process(const RE::InputEvent* const* a_event)

    {
        const auto settings = Settings::GetSingleton();

        HotkeyContext ctx{ settings };

        for (auto event = *a_event; event; event = event->next) {
            if (auto button = event->AsButtonEvent()) {
                ctx.Update(button);
            }
        }

        auto input = EventManager::GetSingleton();

        ctx.Finalize(input);
    }




