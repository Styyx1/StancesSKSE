#include "HotkeyManager.h"
#include "EventManager.h"
#include "PCH.h"

namespace
{
    class HotkeyContext
    {
    public:
        inline static bool isDisabled{ false };

        explicit HotkeyContext(const Settings* settings)
            : hotkey_high(settings->high_key, settings->mod_key_high), hotkey_mid(settings->mid_key, settings->mod_key_mid), hotkey_low(settings->low_key, settings->mod_key_low)
        {
        }

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed()) {
                auto key = CLib::ParseKey(a_button->GetIDCode(), a_button->GetDevice());
                hotkey_low.UpdatePressed(key);
                hotkey_mid.UpdatePressed(key);
                hotkey_high.UpdatePressed(key);

                if (a_button->IsDown()) {
                    hotkey_low.UpdateDown(key);
                    hotkey_mid.UpdateDown(key);
                    hotkey_high.UpdateDown(key);
                }
            }
        }

        

        void Finalize(EventManager* input)
        {
            auto settings = Settings::GetSingleton();
            RE::PlayerCharacter* const player   = Cache::GetPlayerSingleton();
            RE::UI* ui = RE::UI::GetSingleton();

            // vector with key-stance pairs for easy access in the cycle function and in the regular function
            std::vector<std::pair<CLib::KeyCombo, RE::SpellItem*>> keySpellCombo = {
                { hotkey_low,  settings->LowStanceSpell},
                { hotkey_mid,  settings->MidStanceSpell},
                {hotkey_high, settings->HighStanceSpell}
            };

            for (std::uint32_t count = 2; count > 0; --count) {
                bool done = false;
                if (settings->useCycle) {
                    if (player && player->Is3DLoaded() && !EventManager::HasAnyStance()) {
                        input->ApplyStance(settings->MidStanceSpell);
                        logger::debug("applied default stance on key lookup");                                              
                    }
                    // Handle cycle mode
                    if (hotkey_mid.IsActive() && !input->IsInMenu(settings, ui)) {
                        for (std::size_t i = 0; i < keySpellCombo.size(); ++i) {
                            if (player->HasSpell(keySpellCombo[i].second)) {
                                player->RemoveSpell(
                                    keySpellCombo[i].second); // needed because the spell is the condition for applying the other spell. does not work without the spell condition
                                input->ApplyStance(keySpellCombo[(i + 1) % keySpellCombo.size()].second); // Activate the next stance in cycle
                                logger::debug("Exiting loop after stance application (cycle mode)");
                                break;
                            }
                        }
                    }
                }
                else {
                    if (player && player->Is3DLoaded() && EventManager::HasAnyStance() == false) {
                        HotkeyManager::once = true;
                        input->ApplyStance(settings->MidStanceSpell);
                        if (HotkeyManager::once) {
                            HotkeyManager::once = false;
                            logger::debug("applied default stance on key lookup regular mode");
                        }                        
                    }
                    // Vector
                    for (auto& i : keySpellCombo) {
                        if (i.first.Count() == count && i.first.IsActive() && !input->IsInMenu(settings, ui)) {
                            input->ApplyStance(i.second);
                            logger::debug("Exiting loop after stance application (regular mode)");
                            done = true;
                            break;
                        }
                    }
                }
                // needed to break out of the for loop and wait for the next key (i think). not sure but needed anyway.
                if (done)
                    break;
            }
        }
    private:
        CLib::KeyCombo hotkey_high;
        CLib::KeyCombo hotkey_mid;
        CLib::KeyCombo hotkey_low;
    };
} // namespace

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
