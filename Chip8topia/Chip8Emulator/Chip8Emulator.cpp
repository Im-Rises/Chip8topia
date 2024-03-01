#include "Chip8Emulator.h"

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

// TODO: Change this to create right instance of the core
#include "ChipCores/Chip8Core/Chip8Core.h"
#include "ChipCores/SChip11Core/SChip11Core.h"
// TODO: Remove the line below
#include "Chip8CoreBase/Core/CpuBase.h"

// Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<SChip11Core>()) {
Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<Chip8Core>()) {
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();
    inputHandler.m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
    //    Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.subscribe(this, &Chip8Emulator::togglePause);
    inputHandler.m_RestartEmulationEvent.subscribe(this, &Chip8Emulator::restart);

    inputHandler.m_BreakEmulationEvent.subscribe(this, &Chip8Emulator::breakEmulation);
    inputHandler.m_StepEmulationEvent.subscribe(this, &Chip8Emulator::stepEmulation);
    inputHandler.m_RunEmulationEvent.subscribe(this, &Chip8Emulator::runEmulation);
}

Chip8Emulator::~Chip8Emulator() {
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();
    inputHandler.m_GameInput.unsubscribe(this, &Chip8Emulator::OnInput);
    //    Chip8topiaInputHandler::getInstance().m_PauseEmulationEvent.unsubscribe(this, &Chip8Emulator::togglePause);
    inputHandler.m_RestartEmulationEvent.unsubscribe(this, &Chip8Emulator::restart);

    inputHandler.m_BreakEmulationEvent.unsubscribe(this, &Chip8Emulator::breakEmulation);
    inputHandler.m_StepEmulationEvent.unsubscribe(this, &Chip8Emulator::stepEmulation);
    inputHandler.m_RunEmulationEvent.unsubscribe(this, &Chip8Emulator::runEmulation);
}

void Chip8Emulator::restart() {
    m_core->reset();
    m_videoEmulation.reset();
    m_accumulator = 0.0F;
}

void Chip8Emulator::loadRom(const std::vector<uint8_t>& romData) {
    m_core->reset();
    m_core->readRom(romData);
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float deltaTime) {
    if (!m_isRomLoaded)
    {
        return;
    }

    if (m_isBreak)
    {
        if (m_stepNextFrame)
        {
            m_stepNextFrame = false;
            m_core->clock(); // TODO: Correct, it is not a cpu clock, it does several !
            //        m_soundEmulation.update(deltaTime);
        }
    }
    else
    {
        m_accumulator += deltaTime;

        if (m_isTurboMode || m_accumulator >= 1.0F / Chip8Core::SCREEN_AND_TIMERS_FREQUENCY)
        {
            m_accumulator = 0.0F;
            while (!m_core->clock())
            {}
            //        m_soundEmulation.update(deltaTime);
        }
    }
}

void Chip8Emulator::render() {
    // Another way to do this would be to use a trap of the opcode (check if the opcode is render and if not then use the switch case to compute the opcode)
    // But because there is no real vsync, its no use
    m_videoEmulation.updateTexture(m_core);
    m_videoEmulation.update(m_core);
}

// void Chip8Emulator::togglePause() {
//     m_isPaused = !m_isPaused;
// }

void Chip8Emulator::setIsTurboMode(const bool isTurboMode) {
    m_isTurboMode = isTurboMode;
}

// auto Chip8Emulator::getIsPaused() const -> bool {
//     return m_isPaused;
// }

auto Chip8Emulator::getChip8Core() -> Chip8CoreBase* {
    return m_core.get();
}

auto Chip8Emulator::getChip8VideoEmulation() -> Chip8VideoEmulation& {
    return m_videoEmulation;
}

void Chip8Emulator::switchCore(const Chip8CoreType coreType) {
    switch (coreType)
    {
    case Chip8CoreType::Chip8:
        m_core = std::make_unique<Chip8Core>();
        break;
    case Chip8CoreType::SChip11:
        m_core = std::make_unique<SChip11Core>();
        break;
    case Chip8CoreType::XoChip:
        break;
    }

    m_isRomLoaded = false;
    //    m_isPaused = false;
}

auto Chip8Emulator::getCoreType() const -> Chip8CoreType {
    return m_core->getType();
}

void Chip8Emulator::switchFrequency(const Chip8Frequency frequency) {
    switch (frequency)
    {
    case Chip8Frequency::FREQ_600_HZ:
        break;
    }
}

auto Chip8Emulator::getFrequency() const -> Chip8Frequency {
    return Chip8Frequency::FREQ_600_HZ;
}

void Chip8Emulator::OnInput(const uint8 key, const bool isPressed) {
    m_core->updateKey(key, isPressed);
}
