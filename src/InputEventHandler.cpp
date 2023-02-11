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
            for (auto ev = *a_event; ev != nullptr; ev = ev->next) {
                if (ev && ev->eventType == RE::INPUT_EVENT_TYPE::kButton) {
                    const auto buttonEvent = static_cast<RE::ButtonEvent*>(ev);
                    if (buttonEvent) {
                        const auto keyCode = buttonEvent->GetIDCode();
                        
                        if (keyCode == Settings::GetSingleton()->dualWieldParryingSettings.parryKey) {
                            // Event for correct key
                            const auto controlMap = RE::ControlMap::GetSingleton();
                            const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
                            const auto playerControls = RE::PlayerControls::GetSingleton();

                            if (controlMap && playerCharacter && playerControls && playerControls->attackBlockHandler) {
                                if (controlMap->IsFightingControlsEnabled() &&
                                    playerControls->attackBlockHandler->inputEventHandlingEnabled) {

                                    const auto playerState = playerCharacter->AsActorState();
                                    
                                    if (playerState && 
                                        playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
                                        playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                                        playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                                        playerState->GetFlyState() == RE::FLY_STATE::kNone &&
                                        !playerCharacter->IsInKillMove()) {

                                        bool isBlocking = false;
                                        if (playerCharacter->GetGraphVariableBool("IsBlocking", isBlocking)) {
                                            // We managed to successfully read the graph variable
                                            if (buttonEvent->IsHeld()) {
                                                // Player wants to block
                                                if (!isBlocking) {
                                                    playerCharacter->NotifyAnimationGraph("blockStart");
                                                }
                                            } else if (buttonEvent->IsUp()) {
                                                // Player wants to stop blocking
                                                if (isBlocking) {
                                                    playerCharacter->NotifyAnimationGraph("blockStop");
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
