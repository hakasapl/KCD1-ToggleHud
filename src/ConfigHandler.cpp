#include "ConfigHandler.hpp"
#include "HudElements.hpp"
#include "KeyMap.hpp"

#include <SimpleIni.h>

#include <filesystem>
#include <sstream>

using namespace ToggleHud;

auto ConfigHandler::init() -> bool
{
    CSimpleIniA ini;
    ini.SetUnicode();

    // Load the configuration file
    const auto confPath = std::filesystem::path(REL::Module::get().game_root()) / "KCSE" / "Plugins" / "ToggleHud.ini";
    SI_Error rc = ini.LoadFile(confPath.wstring().c_str());
    if (rc != SI_OK) {
        return false;
    }

    // Parse ToggleKey
    const char* toggleKeyStr = ini.GetValue("ToggleHud", "ToggleKey");
    if (toggleKeyStr != nullptr) {
        m_config.toggleKey = ToggleHud::getKeyFromStr(std::string(toggleKeyStr));
    }

    // Parse HudElements
    const char* hudElementsStr = ini.GetValue("ToggleHud", "HudElements");
    if (hudElementsStr != nullptr) {
        std::stringstream ss(hudElementsStr);
        std::string element;
        while (std::getline(ss, element, ',')) {
            trim(element);
            if (!element.empty()) {
                auto hudElem = HudElements::getHudElemFromStr(element);
                if (hudElem != HudElements::KCD1Elements::UNKNOWN) {
                    m_config.hudElements.push_back(hudElem);
                }
            }
        }
    }

    return true;
}

auto ConfigHandler::getConfig() const -> Config { return m_config; }

void ConfigHandler::trim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}