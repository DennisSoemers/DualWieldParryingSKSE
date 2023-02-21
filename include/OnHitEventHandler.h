#pragma once

#include <RE/Skyrim.h>

namespace OnHitEvents {
#pragma warning(push)
#pragma warning(disable : 4251)

    class __declspec(dllexport) OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent> {
    public:

        virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event,
                                                      RE::BSTEventSource<RE::TESHitEvent>* a_eventSource);

        [[nodiscard]] static OnHitEventHandler& GetSingleton() noexcept;

    private:
        OnHitEventHandler() = default;
        OnHitEventHandler(const OnHitEventHandler&) = delete;
        OnHitEventHandler(OnHitEventHandler&&) = delete;
        ~OnHitEventHandler() = default;

        OnHitEventHandler& operator=(const OnHitEventHandler&) = delete;
        OnHitEventHandler& operator=(OnHitEventHandler&&) = delete;
    };
#pragma warning(pop)
}  // namespace OnHitEvents