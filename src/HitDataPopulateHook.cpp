#include <SKSE/SKSE.h>
#include "HitDataPopulateHook.h"

using namespace Hooks;

HitDataPopulateHook& HitDataPopulateHook::GetSingleton() noexcept {
    static HitDataPopulateHook instance;
    return instance;
}

void HitDataPopulateHook::InstallHooks() {
    SKSE::AllocTrampoline(1 << 4);

    REL::Relocation<std::uintptr_t> HitActor{REL::RelocationID(37673, 38627)};
    //REL::Relocation<std::uintptr_t> CalculateCurrentHitTargetForWeaponSwing{REL::RelocationID(37674, 38628)};
    //REL::Relocation<std::uintptr_t> PopulateHitData{REL::RelocationID(42832, 44001)};
    auto& trampoline = SKSE::GetTrampoline();

    // Thanks for addresses to: https://github.com/ersh1/Precision
    _HitData_Populate1 = trampoline.write_call<5>(HitActor.address() + REL::Relocate(0x1B7, 0x1C6), HitData_Populate1);

    /*
    _HitData_Populate2 = trampoline.write_call<5>(
        CalculateCurrentHitTargetForWeaponSwing.address() + REL::Relocate(0xEB, 0x110), HitData_Populate2);
    _HitActor_GetAttackData =
        trampoline.write_call<5>(HitActor.address() + REL::Relocate(0xB3, 0xC2), HitActor_GetAttackData);
    _HitData_GetAttackData =
        trampoline.write_call<5>(PopulateHitData.address() + REL::Relocate(0xD3, 0xDD), HitData_GetAttackData);
    //*/
}

void HitDataPopulateHook::HitData_Populate1(RE::HitData* a_this, RE::TESObjectREFR* a_source,
    RE::TESObjectREFR* a_target, RE::InventoryEntryData* a_weapon,
    bool a_bIsOffhand) {

    if (a_source) {
        if (a_source->IsPlayerRef() && RE::PlayerCamera::GetSingleton()->IsInFirstPerson()) {
            auto player = a_source->As<RE::Actor>();
            const auto playerState = player->AsActorState();
            bool isBashing;
            player->GetGraphVariableBool("IsBashing", isBashing);
            if (isBashing && playerState) {
                if (!a_this->flags.any(RE::HitData::Flag::kBash) &&
                    playerState->actorState1.meleeAttackState != RE::ATTACK_STATE_ENUM::kBash) {
                    // Set the bash flag and attacking state
                    a_this->flags.set(RE::HitData::Flag::kBash);
                    playerState->actorState1.meleeAttackState = RE::ATTACK_STATE_ENUM::kBash;
                    GetSingleton().patchedPlayerAttackStatus = true;
                } 
            }
        } 
    }

    // Finally call the original game's function
    _HitData_Populate1(a_this, a_source, a_target, a_weapon, a_bIsOffhand);
}

/*
void HitDataPopulateHook::HitData_Populate2(RE::HitData* a_this, RE::TESObjectREFR* a_source,
                                            RE::TESObjectREFR* a_target, RE::InventoryEntryData* a_weapon,
                                            bool a_bIsOffhand) {
    // First call the original game's function
    _HitData_Populate2(a_this, a_source, a_target, a_weapon, a_bIsOffhand);

    if (a_source) {
        logger::info("HitData_Populate2 called!");
        if (a_source->IsPlayerRef()) {
            logger::info("source is player");
            auto player = a_source->As<RE::Actor>();
            const auto playerState = player->AsActorState();
            if (playerState && playerState->GetAttackState() == RE::ATTACK_STATE_ENUM::kBash) {
                if (!a_this->flags.any(RE::HitData::Flag::kBash)) {
                    logger::info("trying to set the bash flag");
                    a_this->flags.set(RE::HitData::Flag::kBash);
                } else {
                    logger::info("bash flag was already set");
                }
            } else {
                logger::info("player is not bashing");
            }
        } else {
            logger::info("source is not player");
        }
    }
}

RE::NiPointer<RE::BGSAttackData>& HitDataPopulateHook::HitActor_GetAttackData(RE::AIProcess* a_source) {
    // First call the original game's function
    auto& attackData = _HitActor_GetAttackData(a_source);

    if (a_source && attackData) {
        auto aggressor = a_source->GetUserData();
        if (aggressor && aggressor->IsPlayerRef()) {
            logger::info("aggressor is player!");
            const auto playerState = aggressor->AsActorState();
            if (playerState && playerState->GetAttackState() == RE::ATTACK_STATE_ENUM::kBash) {
                if (!attackData->data.flags.any(RE::AttackData::AttackFlag::kBashAttack)) {
                    auto race = aggressor->GetRace();
                    if (race) {
                        logger::info("trying to change attack data to bash");
                        auto attackDataMap = race->attackDataMap;
                        if (attackDataMap) {
                            for (auto entry : attackDataMap->attackDataMap) {
                                if (entry.first == "bashStart") {
                                    return entry.second;
                                }
                            }
                            logger::info("did not find bashStart");
                        }
                    }
                } else {
                    logger::info("bash flag was already set");
                }
            } else {
                logger::info("player didn't bash");
            }
        } else if (aggressor) {
            logger::info("aggressor = {}", aggressor->GetFormID());
        } else {
            logger::info("aggressor == null!");
        }
    }

    return attackData;
}

RE::NiPointer<RE::BGSAttackData>& HitDataPopulateHook::HitData_GetAttackData(RE::Actor* a_source) {
    // First call the original game's function
    auto& attackData = _HitData_GetAttackData(a_source);

    if (a_source && attackData) {
        if (a_source->IsPlayerRef()) {
            logger::info("aggressor is player!");
            const auto playerState = a_source->AsActorState();
            if (playerState && playerState->GetAttackState() == RE::ATTACK_STATE_ENUM::kBash) {
                if (!attackData->data.flags.any(RE::AttackData::AttackFlag::kBashAttack)) {
                    auto race = a_source->GetRace();
                    if (race) {
                        logger::info("trying to change attack data to bash");
                        auto attackDataMap = race->attackDataMap;
                        if (attackDataMap) {
                            for (auto entry : attackDataMap->attackDataMap) {
                                if (entry.first == "bashStart") {
                                    return entry.second;
                                }
                            }
                            logger::info("did not find bashStart");
                        }
                    }
                } else {
                    logger::info("bash flag was already set");
                }
            } else {
                logger::info("player didn't bash");
            }
        } else {
            logger::info("aggressor = {}", a_source->GetFormID());
        }
    }

    return attackData;
}
*/