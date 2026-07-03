#pragma once

#include "State.hpp"

namespace ToggleHud {
class KeyListener : public Offsets::IInputEventListener {
public:
    virtual ~KeyListener() = default;
    KeyListener() = default;
    KeyListener(const KeyListener&) = delete;
    auto operator=(const KeyListener&) -> KeyListener& = delete;
    KeyListener(KeyListener&&) = delete;
    auto operator=(KeyListener&&) -> KeyListener& = delete;

    void Dtor(char /*flags*/) override { }
    auto OnInputEventUI(const void* /*unicodeEvent*/) -> bool override { return false; }
    [[nodiscard]] auto GetPriority() const -> int override { return 0x0; }
    auto OnInputEvent(const Offsets::SInputEvent& event) -> bool override
    {
        auto* state = State::getSingleton();
        if (state == nullptr) {
            return false;
        }

        return state->updateState(event);
    }

    void registerHook() { SSystemGlobalEnvironment::GetInstance()->pInput->AddEventListener(this); }

    static auto getSingleton() -> KeyListener*
    {
        static KeyListener instance;
        return &instance;
    }
};
} // namespace ToggleHud