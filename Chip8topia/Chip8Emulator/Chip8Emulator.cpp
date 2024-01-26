#include "Chip8Emulator.h"

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Chip8Emulator::Chip8Emulator() {
    Chip8topiaInputHandler::getInstance().m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
}

Chip8Emulator::~Chip8Emulator() {
    Chip8topiaInputHandler::getInstance().m_GameInput.unsubscribe(this, &Chip8Emulator::OnInput);
}

void Chip8Emulator::readRom(const uint8* rom, const size_t romSize) {
    m_core.readRom(rom, romSize);
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float deltaTime) {
    m_core.clock();
    m_videoEmulation.update(deltaTime);
    m_soundEmulation.update(deltaTime);
}

void Chip8Emulator::OnInput(const uint8 key, const bool isPressed) {
    m_core.updateKey(key, isPressed);
}
