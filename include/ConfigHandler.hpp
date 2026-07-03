#pragma once

#include "HudElements.hpp"

#include <vector>

namespace ToggleHud {
class ConfigHandler {
public:
    struct Config {
        Offsets::EKeyId toggleKey;
        std::vector<HudElements::KCD1Elements> hudElements;
    };

private:
    // Instance Vars
    Config m_config;

public:
    auto init() -> bool;
    [[nodiscard]] auto getConfig() const -> Config;

private:
    // Helpers
    static void trim(std::string& s);
};
} // namespace ToggleHud