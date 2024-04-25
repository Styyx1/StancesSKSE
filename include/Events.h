#pragma once
#include "Settings.h"
#include <Xinput.h>

namespace Events
{
    class InputSink : public RE::BSTEventSink<RE::InputEvent*>
    {
        InputSink()                            = default;
        InputSink(const InputSink&)            = delete;
        InputSink(InputSink&&)                 = delete;
        InputSink& operator=(const InputSink&) = delete;
        InputSink& operator=(InputSink&&)      = delete;

        static InputSink* GetSingleton()
        {
            static InputSink singleton;
            return &singleton;
        }

    public:
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* eventPtr, RE::BSTEventSource<RE::InputEvent*>*)
        {
            if (!eventPtr)
                return RE::BSEventNotifyControl::kContinue;

            // Do stuff
            auto* event = *eventPtr;
            if (!event)
                return RE::BSEventNotifyControl::kContinue;

            for (RE::InputEvent* e = *eventPtr; e; e = e->next) {
                switch (e->eventType.get()) {
                case RE::INPUT_EVENT_TYPE::kButton:
                    auto             settings = Settings::GetSingleton();
                    RE::ButtonEvent* a_event  = e->AsButtonEvent();
                    uint32_t         keyMask  = a_event->idCode;
                    uint32_t         keyCode;

                    // Mouse
                    if (a_event->device.get() == RE::INPUT_DEVICE::kMouse) {
                        keyCode = kMacro_NumKeyboardKeys + keyMask;
                    }
                    // Gamepad
                    else if (a_event->device.get() == RE::INPUT_DEVICE::kGamepad) {
                        keyCode = GamepadMaskToKeycode(keyMask);
                    }
                    // Keyboard
                    else
                        keyCode = keyMask;

                    // Valid scancode?
                    if (keyCode >= kMaxMacros)
                        continue;

                    float timer  = a_event->HeldDuration();
                    bool  isDown = a_event->Value() != 0 && timer == 0.0;
                    bool  isHeld = a_event->Value() != 0 && timer > 0;
                    bool  isUp   = a_event->Value() == 0 && timer != 0;
                    modifierKey  = settings->mod_key;
                    auto player  = RE::PlayerCharacter::GetSingleton();

                    if (keyCode == modifierKey && isDown)
                        keyComboPressed = true;
                    if (keyCode == modifierKey && isUp)
                        keyComboPressed = false;

                    if (keyCode == settings->high_key) {
                        if (isDown) {
                            if (modifierKey >= 2) {
                                if (keyComboPressed) {
                                    ApplySpell(player, player, settings->HighStanceSpell);
                                }
                            }
                            else {
                                ApplySpell(player, player, settings->HighStanceSpell);
                            }
                        }
                    }
                    if (keyCode == settings->mid_key) {
                        if (isDown) {
                            if (modifierKey >= 2) {
                                if (keyComboPressed) {
                                    ApplySpell(player, player, settings->MidStanceSpell);
                                    logger::info("mid stance key pressed with modifier");
                                }
                            }
                            else {
                                ApplySpell(player, player, settings->MidStanceSpell);
                                logger::info("mid stance key pressed without modifier");
                            }
                        }
                    }
                    if (keyCode == settings->low_key) {
                        if (isDown) {
                            if (modifierKey >= 2) {
                                if (keyComboPressed) {
                                    ApplySpell(player, player, settings->LowStanceSpell);
                                }
                            }
                            else {
                                ApplySpell(player, player, settings->LowStanceSpell);
                            }
                        }
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        };

        static void Register()
        {
            RE::BSInputDeviceManager::GetSingleton()->AddEventSink(InputSink::GetSingleton());
            logger::info("Registered Input Event");
        }

        static bool IsPermanent(RE::MagicItem* item)
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
        }

        inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
        {
            if (IsPermanent(spell)) {
                target->AddSpell(spell);
            }
            else {
                caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
            }
        }

        enum
        {
            // first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down, then 16 gamepad buttons
            kMacro_KeyboardOffset  = 0, // not actually used, just for self-documentation
            kMacro_NumKeyboardKeys = 256,

            kMacro_MouseButtonOffset = kMacro_NumKeyboardKeys, // 256
            kMacro_NumMouseButtons   = 8,

            kMacro_MouseWheelOffset     = kMacro_MouseButtonOffset + kMacro_NumMouseButtons, // 264
            kMacro_MouseWheelDirections = 2,

            kMacro_GamepadOffset     = kMacro_MouseWheelOffset + kMacro_MouseWheelDirections, // 266
            kMacro_NumGamepadButtons = 16,

            kMaxMacros = kMacro_GamepadOffset + kMacro_NumGamepadButtons // 282
        };

        enum
        {
            kGamepadButtonOffset_DPAD_UP = kMacro_GamepadOffset, // 266
            kGamepadButtonOffset_DPAD_DOWN,
            kGamepadButtonOffset_DPAD_LEFT,
            kGamepadButtonOffset_DPAD_RIGHT,
            kGamepadButtonOffset_START,
            kGamepadButtonOffset_BACK,
            kGamepadButtonOffset_LEFT_THUMB,
            kGamepadButtonOffset_RIGHT_THUMB,
            kGamepadButtonOffset_LEFT_SHOULDER,
            kGamepadButtonOffset_RIGHT_SHOULDER,
            kGamepadButtonOffset_A,
            kGamepadButtonOffset_B,
            kGamepadButtonOffset_X,
            kGamepadButtonOffset_Y,
            kGamepadButtonOffset_LT,
            kGamepadButtonOffset_RT // 281
        };

        uint32_t GamepadMaskToKeycode(uint32_t keyMask)
        {
            switch (keyMask) {
            case XINPUT_GAMEPAD_DPAD_UP:
                return kGamepadButtonOffset_DPAD_UP;
            case XINPUT_GAMEPAD_DPAD_DOWN:
                return kGamepadButtonOffset_DPAD_DOWN;
            case XINPUT_GAMEPAD_DPAD_LEFT:
                return kGamepadButtonOffset_DPAD_LEFT;
            case XINPUT_GAMEPAD_DPAD_RIGHT:
                return kGamepadButtonOffset_DPAD_RIGHT;
            case XINPUT_GAMEPAD_START:
                return kGamepadButtonOffset_START;
            case XINPUT_GAMEPAD_BACK:
                return kGamepadButtonOffset_BACK;
            case XINPUT_GAMEPAD_LEFT_THUMB:
                return kGamepadButtonOffset_LEFT_THUMB;
            case XINPUT_GAMEPAD_RIGHT_THUMB:
                return kGamepadButtonOffset_RIGHT_THUMB;
            case XINPUT_GAMEPAD_LEFT_SHOULDER:
                return kGamepadButtonOffset_LEFT_SHOULDER;
            case XINPUT_GAMEPAD_RIGHT_SHOULDER:
                return kGamepadButtonOffset_RIGHT_SHOULDER;
            case XINPUT_GAMEPAD_A:
                return kGamepadButtonOffset_A;
            case XINPUT_GAMEPAD_B:
                return kGamepadButtonOffset_B;
            case XINPUT_GAMEPAD_X:
                return kGamepadButtonOffset_X;
            case XINPUT_GAMEPAD_Y:
                return kGamepadButtonOffset_Y;
            case 0x9:
                return kGamepadButtonOffset_LT;
            case 0xA:
                return kGamepadButtonOffset_RT;
            default:
                return kMaxMacros; // Invalid
            }
        }

        inline static bool keyComboPressed{ false };
        uint32_t           modifierKey{ 0 };
    };
} // namespace Events
