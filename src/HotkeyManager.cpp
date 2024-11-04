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
            : hotkey_high(settings->high_key, settings->mod_key_high), hotkey_mid(settings->mid_key, settings->mod_key_mid), hotkey_low(settings->low_key, settings->mod_key_low),
              hotkey_neutral(settings->neutral_stance_key, settings->neutral_stance_modifier)
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
                hotkey_neutral.UpdatePressed(key);

                if (a_button->IsDown()) {
                    hotkey_low.UpdateDown(key);
                    hotkey_mid.UpdateDown(key);
                    hotkey_high.UpdateDown(key);
                    hotkey_neutral.UpdateDown(key);
                }
            }
        }

        void Finalize(EventManager* input)
        {
            auto                       settings   = Settings::GetSingleton();
            RE::PlayerCharacter* const player     = Cache::GetPlayerSingleton();
            RE::UI*                    ui         = RE::UI::GetSingleton();
            std::atomic<bool>          inprogress = false;
            std::shared_mutex          working;
            // vector with key-stance pairs for easy access in the cycle function and in the regular function
            std::vector<std::pair<CLib::KeyCombo, RE::SpellItem*>> keySpellCombo = {
                { hotkey_low,  settings->LowStanceSpell},
                { hotkey_mid,  settings->MidStanceSpell},
                {hotkey_high, settings->HighStanceSpell}
            };
            if (hotkey_neutral.IsActive() && !input->IsInMenu(settings, ui)) {
                player->RemoveSpell(settings->HighStanceSpell);
                player->RemoveSpell(settings->MidStanceSpell);
                player->RemoveSpell(settings->LowStanceSpell);
                logger::debug("neutral stance active, no stance applied");
            }
            if (player->Is3DLoaded()) {
                for (std::uint32_t count = 2; count > 0; --count) {
                    bool done = false;
                    if (settings->useCycle) {
                        // Handle cycle mode
                        if (hotkey_mid.IsActive() && !input->IsInMenu(settings, ui)) {
                            for (std::size_t i = 0; i < keySpellCombo.size(); ++i) {
                                if (player->HasSpell(keySpellCombo[i].second)) {
                                    inprogress = true;
                                    player->RemoveSpell(
                                        keySpellCombo[i].second); // needed because the spell is the condition for applying the other spell. does not work without the spell condition
                                    input->ApplyStance(keySpellCombo[(i + 1) % keySpellCombo.size()].second); // Activate the next stance in cycle
                                    logger::debug("Exiting loop after stance application (cycle mode)");
                                    inprogress = false;
                                    break;
                                }
                                else if (!EventManager::HasAnyStance() && settings->neutral_stance_key == 0 && !inprogress) {
                                    input->ApplyStance(keySpellCombo[1].second);
                                    logger::debug("no stance prior detected. Applied {}", keySpellCombo[1].second->GetName());
                                    break;
                                }
                            }
                        }
                    }
                    else {
                        // Vector
                        if (!EventManager::HasAnyStance() && settings->neutral_stance_key == 0) {
                            logger::debug("neutral stance disabled, apply default stance");
                            input->ApplyStance(settings->MidStanceSpell);
                        }
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
            
        }

    private:
        CLib::KeyCombo hotkey_high;
        CLib::KeyCombo hotkey_mid;
        CLib::KeyCombo hotkey_low;
        CLib::KeyCombo hotkey_neutral;
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
