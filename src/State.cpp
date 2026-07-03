#include "State.hpp"

#include "KCSE/KCSEAPI.h"

#include <spdlog/spdlog.h>

using namespace ToggleHud;

auto State::getSingleton() -> State*
{
    static State instance;
    return &instance;
}

void State::init(const std::vector<HudElements::KCD1Elements>& hudElements,
                 const Offsets::EKeyId& toggleKey)
{
    m_hudElements = hudElements;
    m_toggleKey = toggleKey;
}

auto State::updateState(const Offsets::SInputEvent& event) -> bool
{
    if (m_hudElements.empty()) {
        return false;
    }

    if ((event.state & Offsets::eIS_Pressed) == 0) {
        return false;
    }

    if (event.keyId != m_toggleKey) {
        return false;
    }

    // Our time to shine!
    spdlog::debug("Toggling HUD elements visibility.");
    m_showState = !m_showState;
    for (const auto& hudElem : m_hudElements) {
        if (hudElem == HudElements::KCD1Elements::UNKNOWN) {
            continue;
        }

        setHUDElementVisibility(static_cast<uint32_t>(hudElem), m_showState);
    }
    return true;
}

void State::setHUDElementVisibility(const uint32_t& offset,
                                    const bool& visible)
{
    auto* hudinstance = wh::guimodule::uielement::C_UIEHud::GetInstance();
    if ((hudinstance == nullptr) || (hudinstance->m_pElement == nullptr)) {
        KCSE::GetTaskInterface()->AddTask([offset, visible]() -> void { setHUDElementVisibility(offset, visible); });
        return;
    }

    if (visible) {
        static_cast<Offsets::I_HUD*>(hudinstance)->RemoveElementHideRequest(1U << offset);
    } else {
        static_cast<Offsets::I_HUD*>(hudinstance)->AddElementHideRequest(1U << offset);
    }
}