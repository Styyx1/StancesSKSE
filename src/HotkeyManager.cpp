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
              cycle_key(settings->cycleKey)
        {
        }

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed()) {
                auto key = CLib::ParseKey(a_button->GetIDCode(), a_button->GetDevice());

                hotkey_high.UpdatePressed(key);
                hotkey_mid.UpdatePressed(key);
                hotkey_low.UpdatePressed(key);

                if (a_button->IsDown()) {
                    hotkey_high.UpdateDown(key);
                    hotkey_mid.UpdateDown(key);
                    hotkey_low.UpdateDown(key);
                    cycle_key.Update(key);
                }
            }
        }

        void Finalize(EventManager* input)
        {
            auto settings = Settings::GetSingleton();
            auto player   = Cache::GetPlayerSingleton();

            for (std::uint32_t count = 2; count > 0; --count) {
                bool done = false;
                if (hotkey_mid.IsActive() && !settings->useCycle) {
                    input->ApplyStance(settings->MidStanceSpell);
                    logger::debug("don't use cycling");
                    done = true;
                }
                if (hotkey_low.IsActive() && !settings->useCycle) {
                    input->ApplyStance(settings->LowStanceSpell);
                    logger::debug("don't use cycling");
                    done = true;
                }
                if (hotkey_high.IsActive() && !settings->useCycle) {
                    input->ApplyStance(settings->HighStanceSpell);
                    logger::debug("don't use cycling");
                    done = true;
                }
                if (settings->useCycle && cycle_key.IsActive()) {
                    if (done) {
                        logger::debug("break during cycle key");
                        break;
                    }
                    logger::debug("cycle key is active");
                    if (player->HasSpell(settings->LowStanceSpell)) {
                        logger::debug("Low Stance active");
                        player->RemoveSpell(settings->LowStanceSpell);
                        input->ApplyStance(settings->MidStanceSpell);
                        done = true;
                    }

                    else if (player->HasSpell(settings->MidStanceSpell)) {
                        logger::debug("Mid Stance active");
                        player->RemoveSpell(settings->MidStanceSpell);
                        input->ApplyStance(settings->HighStanceSpell);
                        done = true;
                    }
                    else if (player->HasSpell(settings->HighStanceSpell)) {
                        logger::debug("High Stance active");
                        player->RemoveSpell(settings->HighStanceSpell);
                        input->ApplyStance(settings->LowStanceSpell);
                        done = true;
                    }
                }
                if (done) {
                    logger::debug("break after cycle key bool checks");
                    break;
                }
            }
        }

    private:
        CLib::KeyCombo hotkey_high;
        CLib::KeyCombo hotkey_mid;
        CLib::KeyCombo hotkey_low;
        CLib::Key      cycle_key;
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
