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

                    if (playerState && playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
                        playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                        playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                        playerState->GetFlyState() == RE::FLY_STATE::kNone && 
                        !playerCharacter->IsInKillMove()) {

                        bool isBlocking = false;
                        if (playerCharacter->GetGraphVariableBool("IsBlocking", isBlocking)) {
                            // We managed to successfully read the graph variable
                            const auto parryKey = Settings::GetSingleton()->dualWieldParryingSettings.parryKey;

                            for (auto ev = *a_event; ev != nullptr; ev = ev->next) {
                                if (ev && ev->eventType == RE::INPUT_EVENT_TYPE::kButton) {
                                    const auto buttonEvent = static_cast<RE::ButtonEvent*>(ev);
                                    if (buttonEvent) {
                                        const auto keyCode = buttonEvent->GetIDCode();

                                        if (keyCode == parryKey) {
                                            // Event for parry key
                                            if (buttonEvent->IsHeld()) {
                                                // Player wants to block
                                                playerCharacter->SetGraphVariableInt("iWantBlock", 1);
                                                if (!isBlocking) { 
                                                    playerCharacter->NotifyAnimationGraph("blockStart");
                                                }
                                            } else if (buttonEvent->IsUp()) {
                                                // Player wants to stop blocking
                                                if (isBlocking) {
                                                    playerCharacter->NotifyAnimationGraph("blockStop");
                                                    playerCharacter->SetGraphVariableInt("iWantBlock", 0);
                                                }
                                            }
                                        } /* else if (keyCode ==
                                                   controlMap->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack,
                                                                            buttonEvent->GetDevice())) {
                                            
                                            if (buttonEvent->IsHeld()) {
                                                logger::info("Attack key held!");

                                                int32_t iWantBlock = -1;
                                                playerCharacter->GetGraphVariableInt("iWantBlock", iWantBlock);
                                                logger::info("iWantBlock = {}", iWantBlock);

                                                bool isBashing = false;
                                                playerCharacter->GetGraphVariableBool("IsBashing", isBashing);
                                                logger::info("IsBashing = {}", isBashing);

                                                logger::info("IsBlocking = {}", isBlocking);

                                                logger::info("attack state = {}",
                                                             static_cast<uint32_t>(playerState->GetAttackState()));
                                            } else if (buttonEvent->IsUp()) {
                                                playerCharacter->GetActorRuntimeData().currentProcess;
                                                logger::info("Attack key up!");

                                                int32_t iWantBlock = -1;
                                                playerCharacter->GetGraphVariableInt("iWantBlock", iWantBlock);
                                                logger::info("iWantBlock = {}", iWantBlock);

                                                bool isBashing = false;
                                                playerCharacter->GetGraphVariableBool("IsBashing", isBashing);
                                                logger::info("IsBashing = {}", isBashing);

                                                logger::info("IsBlocking = {}", isBlocking);

                                                logger::info("attack state = {}",
                                                             static_cast<uint32_t>(playerState->GetAttackState()));
                                            }
                                        } */
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
