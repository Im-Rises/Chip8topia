#include "Chip8Emulator.h"

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "Chip8Core/Core/Ppu.h"

Chip8Emulator::Chip8Emulator() {
    Chip8topiaInputHandler::getInstance().m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
    Chip8topiaInputHandler::getInstance().m_PKeyButtonPressedEvent.subscribe(this, &Chip8Emulator::togglePause);
    Chip8topiaInputHandler::getInstance().m_F10KeyButtonPressedEvent.subscribe(this, &Chip8Emulator::toglleTurboMode);
}

Chip8Emulator::~Chip8Emulator() {
    Chip8topiaInputHandler::getInstance().m_GameInput.unsubscribe(this, &Chip8Emulator::OnInput);
    Chip8topiaInputHandler::getInstance().m_PKeyButtonPressedEvent.unsubscribe(this, &Chip8Emulator::togglePause);
    Chip8topiaInputHandler::getInstance().m_F10KeyButtonPressedEvent.unsubscribe(this, &Chip8Emulator::toglleTurboMode);
}

void Chip8Emulator::loadRom(const std::string& romPath) {
    m_core.reset();
    m_core.readRom(m_romLoader.loadRom(romPath));
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float deltaTime) {
    if (!m_isRomLoaded || m_isPaused)
    {
        return;
    }

    m_accumulator += deltaTime;

    if (m_isTurboMode || m_accumulator >= 1.0F / Ppu::CLOCK_FREQUENCY)
    {
        m_accumulator = 0.0F;
        m_core.clock();
        //        m_videoEmulation.update(deltaTime);
        //        m_soundEmulation.update(deltaTime);
    }
}

void Chip8Emulator::toglleTurboMode() {
    m_isTurboMode = !m_isTurboMode;
}

void Chip8Emulator::togglePause() {
    m_isPaused = !m_isPaused;
}

auto Chip8Emulator::getIsTurbomode() const -> bool {
    return m_isTurboMode;
}

void Chip8Emulator::OnInput(const uint8 key, const bool isPressed) {
    m_core.updateKey(key, isPressed);
}
