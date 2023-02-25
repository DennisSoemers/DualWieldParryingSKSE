#include <SKSE/SKSE.h>

#include "InputEventHandler.h"

using namespace DualWieldParryingSKSE;

InputEventHandler& InputEventHandler::GetSingleton() noexcept {
    static InputEventHandler instance;
    return instance;
}

RE::BSEventNotifyControl InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event,
    RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {

    if (a_event) {
        const auto ui = RE::UI::GetSingleton();

        if (ui && !ui->GameIsPaused()) {
            const auto controlMap = RE::ControlMap::GetSingleton();
            const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
            const auto playerControls = RE::PlayerControls::GetSingleton();

            if (controlMap && playerCharacter && playerControls && playerControls->attackBlockHandler) {
                if (controlMap->IsFightingControlsEnabled() &&
                    playerControls->attackBlockHandler->inputEventHandlingEnabled) {
                    const auto playerState = playerCharacter->AsActorState();
                    auto playerAI = playerCharacter->GetActorRuntimeData().currentProcess;

                    if (playerState && playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
                        playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                        playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                        playerState->GetFlyState() == RE::FLY_STATE::kNone && 
                        !playerCharacter->IsInKillMove() &&
                        playerAI) {

                        // Bow / crossbow checks necessary to stop the block button from zooming in
                        // when we have the perk for that
                        auto playerEquippedLeft = playerAI->GetEquippedLeftHand();
                        auto playerLeftWeap =
                            playerEquippedLeft == nullptr ? nullptr : playerEquippedLeft->As<RE::TESObjectWEAP>();

                        if (playerLeftWeap == nullptr || (playerLeftWeap->GetWeaponType() != RE::WEAPON_TYPE::kBow &&
                                                          playerLeftWeap->GetWeaponType() != RE::WEAPON_TYPE::kCrossbow)) {
                            bool isBlocking = false;
                            if (playerCharacter->GetGraphVariableBool("IsBlocking", isBlocking)) {
                                // We managed to successfully read the graph variable
                                const auto parryKey = Settings::GetSingleton()->dualWieldParryingSettings.parryKey;

                                for (auto ev = *a_event; ev != nullptr; ev = ev->next) {
                                    if (ev && ev->eventType == RE::INPUT_EVENT_TYPE::kButton) {
                                        const auto buttonEvent = static_cast<RE::ButtonEvent*>(ev);
                                        if (buttonEvent) {
                                            auto keyCode = buttonEvent->GetIDCode();

                                            if (buttonEvent->device.get() == RE::INPUT_DEVICE::kMouse) {
                                                keyCode += 257;
                                            } else if (buttonEvent->device.get() == RE::INPUT_DEVICE::kGamepad) {
                                                RE::BSWin32GamepadDevice::Key gamepadKey =
                                                    static_cast<RE::BSWin32GamepadDevice::Key>(keyCode);
                                                switch (gamepadKey) {
                                                    case RE::BSWin32GamepadDevice::Key::kUp:
                                                        keyCode = 266;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kDown:
                                                        keyCode = 267;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kLeft:
                                                        keyCode = 268;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kRight:
                                                        keyCode = 269;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kStart:
                                                        keyCode = 270;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kBack:
                                                        keyCode = 271;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kLeftThumb:
                                                        keyCode = 272;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kRightThumb:
                                                        keyCode = 273;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kLeftShoulder:
                                                        keyCode = 274;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kRightShoulder:
                                                        keyCode = 275;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kA:
                                                        keyCode = 276;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kB:
                                                        keyCode = 277;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kX:
                                                        keyCode = 278;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kY:
                                                        keyCode = 279;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kLeftTrigger:
                                                        keyCode = 280;
                                                        break;
                                                    case RE::BSWin32GamepadDevice::Key::kRightTrigger:
                                                        keyCode = 281;
                                                        break;
                                                    default:
                                                        keyCode = static_cast<uint32_t>(-1);
                                                        break;
                                                }
                                            }

                                            if (keyCode == parryKey) {
                                                // Event for parry key
                                                if (buttonEvent->IsHeld()) {
                                                    // Player wants to block
                                                    //playerCharacter->SetGraphVariableInt("iWantBlock", 1);
                                                    playerState->actorState2.wantBlocking = 1;
                                                    if (!isBlocking) {
                                                        playerCharacter->NotifyAnimationGraph("blockStart");
                                                    }
                                                } else if (buttonEvent->IsUp()) {
                                                    // Player wants to stop blocking
                                                    playerState->actorState2.wantBlocking = 0;
                                                    if (isBlocking) {
                                                        playerCharacter->NotifyAnimationGraph("blockStop");
                                                    //    playerCharacter->SetGraphVariableInt("iWantBlock", 0);
                                                    //    playerState->actorState2.wantBlocking = 0;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Let the game continue handling this event
    return RE::BSEventNotifyControl::kContinue;
}
