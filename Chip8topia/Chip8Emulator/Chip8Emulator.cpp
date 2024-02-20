#include "Chip8Emulator.h"

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Chip8Emulator::Chip8Emulator() {
    Chip8topiaInputHandler::getInstance().m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
    Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.subscribe(this, &Chip8Emulator::togglePause);
    Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.subscribe(this, &Chip8Emulator::restart);
}

Chip8Emulator::~Chip8Emulator() {
    Chip8topiaInputHandler::getInstance().m_GameInput.unsubscribe(this, &Chip8Emulator::OnInput);
    Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.unsubscribe(this, &Chip8Emulator::togglePause);
    Chip8topiaInputHandler::getInstance().m_RestartEmulationEvent.unsubscribe(this, &Chip8Emulator::restart);
}

void Chip8Emulator::restart() {
    m_core.reset();
    m_videoEmulation.reset();
    m_accumulator = 0.0F;
}

void Chip8Emulator::loadRom(const std::string& romPath) {
    try
    {
        m_core.reset();
        m_core.readRom(Chip8RomLoader::loadRom(romPath));
        m_isRomLoaded = true;
    }
    catch (const std::exception& e)
    {
        m_isRomLoaded = false;
        throw e;
    }
}

void Chip8Emulator::loadRom(const std::vector<uint8_t>& romData) {
    m_core.reset();
    m_core.readRom(romData);
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float deltaTime) {
    if (!m_isRomLoaded || m_isPaused)
    {
        return;
    }

    m_accumulator += deltaTime;

    if (m_isTurboMode || m_accumulator >= 1.0F / Chip8Core::SCREEN_AND_TIMERS_FREQUENCY)
    {
        m_accumulator = 0.0F;
        m_core.clock();
        //        m_soundEmulation.update(deltaTime);
    }
}

void Chip8Emulator::render() {
    // Another way to do this would be to use a trap of the opcode (check if the opcode is render and if not then use the switch case to compute the opcode)
    m_videoEmulation.updateTexture(m_core.getPpu()->getVideoMemory());
    m_videoEmulation.update();
}

void Chip8Emulator::togglePause() {
    m_isPaused = !m_isPaused;
}

void Chip8Emulator::setIsTurboMode(const bool isTurboMode) {
    m_isTurboMode = isTurboMode;
}

auto Chip8Emulator::getIsPaused() const -> bool {
    return m_isPaused;
}

auto Chip8Emulator::getChip8Core() -> Chip8Core* {
    return &m_core;
}

auto Chip8Emulator::getChip8VideoEmulation() -> Chip8VideoEmulation& {
    return m_videoEmulation;
}

void Chip8Emulator::OnInput(const uint8 key, const bool isPressed) {
    m_core.updateKey(key, isPressed);
}
