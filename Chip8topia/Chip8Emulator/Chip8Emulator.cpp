#include "Chip8Emulator.h"

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

void test(const uint8 key, const bool isPressed) {
}

Chip8Emulator::Chip8Emulator() {
    //    MethodEventVarying<Chip8Emulator, int, double> test(this, &Chip8Emulator::OnInput);
    Chip8topiaInputHandler::getInstance().m_GameInput.subscribe(&test);
    //    Chip8topiaInputHandler::getInstance().m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
    //    MethodEventVarying test2(this, &Chip8Emulator::OnInput);
    Chip8topiaInputHandler::getInstance().m_GameInput.trigger(0, true);
}

Chip8Emulator::~Chip8Emulator() {
    //    Chip8topiaInputHandler::getInstance().m_GameInput.unsubscribe(this, &Chip8Emulator::OnInput);
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
    // void Chip8Emulator::OnInput() {
    //     m_core.updateKey(key, isPressed);
}
