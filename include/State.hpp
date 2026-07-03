#pragma once

#include "HudElements.hpp"

namespace ToggleHud {
class State {
private:
    // Private Members
    bool m_showState = true;
    Offsets::EKeyId m_toggleKey = Offsets::eKI_0;
    std::vector<HudElements::KCD1Elements> m_hudElements;

public:
    static auto getSingleton() -> State*;

    void init(const std::vector<HudElements::KCD1Elements>& hudElements,
              const Offsets::EKeyId& toggleKey = Offsets::eKI_0);
    auto updateState(const Offsets::SInputEvent& event) -> bool;

private:
    // Helpers
    static void setHUDElementVisibility(const uint32_t& offset,
                                        const bool& visible);
};
} // namespace ToggleHud