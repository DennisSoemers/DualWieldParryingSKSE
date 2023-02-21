#include <stddef.h>
#include "InputEventHandler.h"
#include "HitDataPopulateHook.h"
#include "OnHitEventHandler.h"
#include "Settings.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {
    /**
     * Setup logging.
     */
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }

#ifndef NDEBUG
        const auto level = spdlog::level::trace;
#else
        const auto level = spdlog::level::info;
#endif

        log->set_level(level);
        log->flush_on(level);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }

    /**
     * Initialize the event handler.
     */
    void InitializeEventHandler() {
        log::trace("Initializing event sink...");

        auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
        if (inputDeviceManager) {
            inputDeviceManager->AddEventSink(&DualWieldParryingSKSE::InputEventHandler::GetSingleton());
            log::trace("Event sink initialized.");
        } else {
            stl::report_and_fail("Failed to initialize event sink.");
        }
    }

    /**
     * Initialize the hooks.
     */
    void InitializeHooks() {
        log::trace("Initializing hooks...");
        Hooks::HitDataPopulateHook::InstallHooks();
        log::trace("Hooks initialized.");
    }

    /**
     * Initialize the event sink, which lets us respond to events
     * sent by the game engine.
     */
    void InitializeEventSink() {
        log::trace("Initializing event sink...");
        auto scriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
        if (scriptEventSource) {
            scriptEventSource->AddEventSink(&OnHitEvents::OnHitEventHandler::GetSingleton());
            log::trace("Event sink initialized.");
        } else {
            stl::report_and_fail("Failed to initialize event sink.");
        }
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
        if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
            InitializeEventHandler();
        }
    }
}  // namespace

/**
 * This is the main callback for initializing the SKSE plugin, called just before Skyrim runs its main function.
 */
SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);

    try {
        Settings::GetSingleton()->Load();
    } catch (...) {
        logger::error("Exception caught when loading settings! Default settings will be used");
    }

    InitializeHooks();
    InitializeEventSink();
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
