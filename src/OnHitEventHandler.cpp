#include "OnHitEventHandler.h"
#include "HitDataPopulateHook.h"

#include <SKSE/SKSE.h>

using namespace OnHitEvents;
using namespace SKSE;

OnHitEventHandler& OnHitEventHandler::GetSingleton() noexcept {
    static OnHitEventHandler instance;
    return instance;
}

RE::BSEventNotifyControl OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event,
                                                         RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
    if (a_event) {
        const auto aggressor = a_event->cause.get();
        if (aggressor && aggressor->IsPlayerRef()) {
            auto& hook = Hooks::HitDataPopulateHook::GetSingleton();
            if (hook.patchedPlayerAttackStatus) {
                // Clear the player's attack status
                hook.patchedPlayerAttackStatus = false;
                auto player = aggressor->As<RE::Actor>();
                const auto playerState = player->AsActorState();

                if (playerState) {
                    if (playerState->actorState1.meleeAttackState == RE::ATTACK_STATE_ENUM::kBash) {
                        playerState->actorState1.meleeAttackState = RE::ATTACK_STATE_ENUM::kNone;
                    }
                }
            }
        }
    }

    // Let other code process the same event next
    return RE::BSEventNotifyControl::kContinue;
}