#pragma once

#include <RE/Skyrim.h>

#include "Settings.h"

namespace DualWieldParryingSKSE {
#pragma warning(push)
#pragma warning(disable : 4251)

    class __declspec(dllexport) InputEventHandler : public RE::BSTEventSink<RE::InputEvent*> {
    public:
        virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, 
                                                      RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

        [[nodiscard]] static InputEventHandler& GetSingleton() noexcept;

    private:
        InputEventHandler() = default;
        InputEventHandler(const InputEventHandler&) = delete;
        InputEventHandler(InputEventHandler&&) = delete;
        ~InputEventHandler() = default;

        InputEventHandler& operator=(const InputEventHandler&) = delete;
        InputEventHandler& operator=(InputEventHandler&&) = delete;
    };

#pragma warning(pop)
}  // namespace DualWieldParryingSKSE