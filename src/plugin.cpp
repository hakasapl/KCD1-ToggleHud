#include "KCSE/KCSEAPI.h"

#include "ConfigHandler.hpp"
#include "Hooks.hpp"
#include "State.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <windows.h>

using namespace ToggleHud;

namespace {
auto initLogger() -> bool
{
    const auto logsFolder = std::filesystem::path(REL::Module::get().game_root()) / "KCSE" / "logs";
    if (!std::filesystem::exists(logsFolder)) {
        return false;
    }
    auto logFilePath = logsFolder / "TerrainHelper.log";
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);

    return true;
}

void init()
{
    // Initialize logger
    initLogger();
    spdlog::info("ToggleHud initializing...");

    // Build ConfigHandler
    ConfigHandler configHandler;
    if (!configHandler.init()) {
        auto* log = SSystemGlobalEnvironment::GetInstance()->pLog;
        if (log != nullptr) {
            spdlog::error("Failed to initialize ConfigHandler. Check if ToggleHud.ini exists and is valid.");
        }
        return;
    }

    // Build State
    State::getSingleton()->init(configHandler.getConfig().hudElements, configHandler.getConfig().toggleKey);

    // Register KeyListener
    KeyListener::getSingleton()->registerHook();
}

// NOLINTBEGIN
KCSE_PLUGIN_INFO("ToggleHud",
                 "hakasapl",
                 1);

KCSE_PLUGIN_LOAD(kcse)
{
    KCSE::GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) -> void {
        if (msg->type == KCSE::IMessagingInterface::kMessage_DataLoaded) {
            init();
        }
    });
    return true;
}
// NOLINTEND
}
