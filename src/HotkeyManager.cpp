// #include "HotkeyManager.h"
// #include "Cache.h"
// #include "formloader.h"
// #include "Settings.h"
// #include "stance-manager.h"
//
// namespace
// {
//
//     struct HotkeyContext
//     {
//         inline static bool isDisabled{ false };
//
//         using set = Config::Settings;
//
//         explicit HotkeyContext()
//             : hotkey_bear(set::bear_stance_key.GetValue(), set::bear_stance_modifier.GetValue()),
//         hotkey_wolf(set::wolf_stance_key.GetValue(), set::wolf_stance_modifier.GetValue()),
//         hotkey_hawk(set::hawk_stance_key.GetValue(), set::hawk_stance_modifier.GetValue() ),
//         hotkey_neutral(set::neutral_stance_key.GetValue(), set::neutral_stance_modifier.GetValue())
//         {
//         }
//         void Update(const RE::ButtonEvent* a_button)
//         {
//             if (!a_button->HasIDCode()) {
//                 return;
//             }
//             if (a_button->IsPressed()) {
//                 const auto key = CLib::ParseKey(a_button->GetIDCode(), a_button->GetDevice());
//                 hotkey_bear.UpdatePressed(key);
//                 hotkey_wolf.UpdatePressed(key);
//                 hotkey_hawk.UpdatePressed(key);
//                 hotkey_neutral.UpdatePressed(key);
//
//                 if (a_button->IsDown()) {
//                     hotkey_bear.UpdateDown(key);
//                     hotkey_wolf.UpdateDown(key);
//                     hotkey_hawk.UpdateDown(key);
//                     hotkey_neutral.UpdateDown(key);
//                 }
//             }
//         }
//
//         void Finalize()
//         {
//             using st = STNG::StanceManager::available_stances;
//             const auto player = Cache::GetPlayerSingleton();
//
//             // vector with key-stance pairs for easy access in the cycle function and in the regular function
//             std::array<std::pair<CLib::KeyCombo*, st>, 4> keySpellCombo = {
//                 {
//                     { &hotkey_bear,  st::kBearStance},
//                     { &hotkey_wolf,  st::kWolfStance},
//                     {&hotkey_hawk, st::kHawkStance},
//                     {&hotkey_neutral, st::kNeutralStance}
//                 }
//             };
//
//             if (!player || !player->Is3DLoaded())
//                 return;
//
//             for (uint32_t count = 2; count > 0; --count)
//             {
//                 bool done = false;
//                 if (STNG::MISC::IsAnyMenuOpen())
//                     break;
//                 if (Config::Settings::use_cycling.GetValue())
//                 {
//                     if (hotkey_neutral.IsActive())
//                     {
//                         STNG::StanceManager::UpdateStancePlayer(STNG::StanceManager::available_stances::kNeutralStance);
//                         logs::info("neutral stance updated");
//                         break;
//                     }
//                     if (hotkey_wolf.IsActive())
//                         STNG::StanceManager::CycleStancesPlayer();
//                     break;
//                 }
//                 for (auto& [key, stance] : keySpellCombo)
//                 {
//                     if (key->Count() == count && key->IsActive())
//                     {
//                         STNG::StanceManager::UpdateStancePlayer(stance);
//                         done = true;
//                         break;
//                     }
//                 }
//                 if (done)
//                     break;
//             }
//         }
//
//     private:
//         CLib::KeyCombo hotkey_bear;
//         CLib::KeyCombo hotkey_wolf;
//         CLib::KeyCombo hotkey_hawk;
//         CLib::KeyCombo hotkey_neutral;
//     };
// } // namespace
//
// void HotkeyManager::Process(const RE::InputEvent* const* a_event)
// {
//     HotkeyContext ctx{};
//
//     for (auto event = *a_event; event; event = event->next) {
//         if (const auto button = event->AsButtonEvent()) {
//             ctx.Update(button);
//             break;
//         }
//     }
//     ctx.Finalize();
// }
