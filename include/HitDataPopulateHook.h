#pragma once

#include <RE/Skyrim.h>

namespace Hooks {
#pragma warning(push)
#pragma warning(disable : 4251)

    class __declspec(dllexport) HitDataPopulateHook {
    public:
        [[nodiscard]] static HitDataPopulateHook& GetSingleton() noexcept;

        static void InstallHooks();

        /** Did we set the player's attack status to patch the "double attack bug"? */
        bool patchedPlayerAttackStatus = false;

    private:
        HitDataPopulateHook() = default;
        HitDataPopulateHook(const HitDataPopulateHook&) = delete;
        HitDataPopulateHook(HitDataPopulateHook&&) = delete;
        ~HitDataPopulateHook() = default;

        HitDataPopulateHook& operator=(const HitDataPopulateHook&) = delete;
        HitDataPopulateHook& operator=(HitDataPopulateHook&&) = delete;

        /** Our new functions */
        static void HitData_Populate1(RE::HitData* a_this, RE::TESObjectREFR* a_source, RE::TESObjectREFR* a_target,
                                      RE::InventoryEntryData* a_weapon, bool a_bIsOffhand);

        /*
        static void HitData_Populate2(RE::HitData* a_this, RE::TESObjectREFR* a_source, RE::TESObjectREFR* a_target,
                                      RE::InventoryEntryData* a_weapon, bool a_bIsOffhand);
        static RE::NiPointer<RE::BGSAttackData>& HitActor_GetAttackData(RE::AIProcess* a_source);
        static RE::NiPointer<RE::BGSAttackData>& HitData_GetAttackData(RE::Actor* a_source);
        */

        /** The original functions */
        static inline REL::Relocation<decltype(HitData_Populate1)> _HitData_Populate1;

        /*
        static inline REL::Relocation<decltype(HitData_Populate2)> _HitData_Populate2;
        static inline REL::Relocation<decltype(HitActor_GetAttackData)> _HitActor_GetAttackData;
        static inline REL::Relocation<decltype(HitData_GetAttackData)> _HitData_GetAttackData;
        */
    };

#pragma warning(pop)
}  // namespace Hooks