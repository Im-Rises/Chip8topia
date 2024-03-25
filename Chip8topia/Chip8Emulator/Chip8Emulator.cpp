#include "Chip8Emulator.h"

#include <ImGuiNotify.hpp>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "ChipCores/Chip8Core/Chip8Core.h"
#include "ChipCores/SChip11Core/SChip11Core.h"
#include "ChipCores/SchipCCore/SChipCCore.h"
#include "ChipCores/XoChipCore/XoChipCore.h"

#if defined(BUILD_DEBUG)
// Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<SChipCCore>(DEFAULT_FREQUENCY))
// Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<SChip11Core>(DEFAULT_FREQUENCY))
// Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<Chip8Core>(DEFAULT_FREQUENCY))
Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<XoChipCore>(DEFAULT_FREQUENCY))
#else
Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<XoChipCore>(DEFAULT_FREQUENCY))
#endif
{
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();
    inputHandler.m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
    inputHandler.m_TogglePauseEmulationEvent.subscribe(this, &Chip8Emulator::toggleBreakEmulation);
    inputHandler.m_RestartEmulationEvent.subscribe(this, &Chip8Emulator::restart);

    inputHandler.m_BreakEmulationEvent.subscribe(this, &Chip8Emulator::breakEmulation);
    inputHandler.m_StepEmulationEvent.subscribe(this, &Chip8Emulator::stepEmulation);
    inputHandler.m_RunEmulationEvent.subscribe(this, &Chip8Emulator::runEmulation);
    inputHandler.m_ClearBreakpointsEvent.subscribe(this, &Chip8Emulator::clearBreakpoints);

    resetColorPalette();
}

Chip8Emulator::~Chip8Emulator()
{
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();
    inputHandler.m_GameInput.unsubscribe(this, &Chip8Emulator::OnInput);
    inputHandler.m_TogglePauseEmulationEvent.unsubscribe(this, &Chip8Emulator::toggleBreakEmulation);
    inputHandler.m_RestartEmulationEvent.unsubscribe(this, &Chip8Emulator::restart);

    inputHandler.m_BreakEmulationEvent.unsubscribe(this, &Chip8Emulator::breakEmulation);
    inputHandler.m_StepEmulationEvent.unsubscribe(this, &Chip8Emulator::stepEmulation);
    inputHandler.m_RunEmulationEvent.unsubscribe(this, &Chip8Emulator::runEmulation);
    inputHandler.m_ClearBreakpointsEvent.unsubscribe(this, &Chip8Emulator::clearBreakpoints);
}

void Chip8Emulator::resetColorPalette()
{
    //    switch (m_core->getType())
    //    {
    //    case Chip8CoreType::Chip8:
    //    case Chip8CoreType::SChip11Legacy:
    //    case Chip8CoreType::SChip11Modern:
    //    case Chip8CoreType::SChipC:
    //        m_videoEmulation.resetToBWColors();
    //        break;
    //    case Chip8CoreType::XoChip:
    //        m_videoEmulation.resetToColorColors();
    //        break;
    //    }

    m_videoEmulation.resetToColorColors();
}

void Chip8Emulator::restart()
{
    m_videoEmulation.reset();
    m_core->reset();
    m_accumulator = 0.0F;
    // TODO: Restart is disabled for now, it causes issue on XoChip games
    //  Reload rom here !
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.trigger("Restart is disabled for now, it causes issue on XoChip games", nullptr);
    m_isRomLoaded = false;
}

void Chip8Emulator::loadRom(const std::vector<uint8_t>& romData)
{
    m_videoEmulation.reset();
    m_core->reset();
    m_core->readRom(romData);
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float deltaTime)
{
    if (!m_isRomLoaded)
    {
        return;
    }

    if (m_isBreak)
    {
        if (m_step)
        {
            m_step = false;
            m_core->clock();
        }
    }
    else
    {
        m_accumulator += deltaTime;

        if (m_isTurboMode || m_accumulator >= 1.0F / Chip8Core::SCREEN_AND_TIMERS_FREQUENCY)
        {
            m_accumulator = 0.0F;
            bool screenUpdated = false;
            while (!screenUpdated && !m_isBreak)
            {
                screenUpdated = m_core->clock();
                if (m_breakpoints.find(m_core->getCpu()->getPc()) != m_breakpoints.end() && m_canBreak)
                {
                    m_isBreak = true;
                }
            }
        }
    }
}

void Chip8Emulator::render(const float screenWidth, const float screenHeight)
{
    m_videoEmulation.updateTexture(m_core);
    m_videoEmulation.update(m_core, screenWidth, screenHeight, CHIP8_ASPECT_RATIO);
}

auto Chip8Emulator::getChip8Core() -> Chip8CoreBase*
{
    return m_core.get();
}

auto Chip8Emulator::getChip8VideoEmulation() -> Chip8VideoEmulation&
{
    return m_videoEmulation;
}

[[nodiscard]] auto Chip8Emulator::getRomName() const -> std::string
{
    return m_romName;
}

[[nodiscard]] auto Chip8Emulator::getConsoleName() -> std::string
{
    return m_core->getConsoleName();
}

auto Chip8Emulator::getIsBreak() const -> bool
{
    return m_isBreak;
}

[[nodiscard]] auto Chip8Emulator::getIsRomLoaded() const -> bool
{
    return m_isRomLoaded;
}

auto Chip8Emulator::getCanBreak() -> bool*
{
    return &m_canBreak;
}

auto Chip8Emulator::getBreakpoints() -> std::set<uint16>&
{
    return m_breakpoints;
}

auto Chip8Emulator::getCoreType() const -> Chip8CoreType
{
    return m_core->getType();
}

auto Chip8Emulator::getFrequency() const -> Chip8Frequency
{
    return m_core->getFrequency();
}

void Chip8Emulator::stop()
{
    m_isRomLoaded = false;
    ImGui::InsertNotification({ ImGuiToastType::Info, "Emulation stopped", "The emulation has been stopped. Please load a ROM to continue." });
}

void Chip8Emulator::setIsTurboMode(const bool isTurboMode)
{
    m_isTurboMode = isTurboMode;
}

void Chip8Emulator::setRomName(const std::string& romName)
{
    m_romName = romName;
}

void Chip8Emulator::switchCoreFrequency(const Chip8CoreType coreType, const Chip8Frequency frequency)
{
    switch (coreType)
    {
    case Chip8CoreType::Chip8:
        m_core = std::make_unique<Chip8Core>(frequency);
        break;
    case Chip8CoreType::SChip11Legacy:
        m_core = std::make_unique<SChip11Core>(frequency, false);
        break;
    case Chip8CoreType::SChip11Modern:
        m_core = std::make_unique<SChip11Core>(frequency, true);
        break;
    case Chip8CoreType::SChipC:
        m_core = std::make_unique<SChipCCore>(frequency);
        break;
    case Chip8CoreType::XoChip:
        m_core = std::make_unique<XoChipCore>(frequency);
        break;
    }

    resetColorPalette();

    m_isRomLoaded = false;

    ImGui::InsertNotification({ ImGuiToastType::Info, "Core and frequency changed", "The core and frequency have been changed. Please load a ROM to continue." });
}

void Chip8Emulator::clearBreakpoints()
{
    m_breakpoints.clear();
}

void Chip8Emulator::stepEmulation()
{
    m_isBreak = true;
    m_step = true;
}
void Chip8Emulator::runEmulation()
{
    m_isBreak = false;
}
void Chip8Emulator::breakEmulation()
{
    m_isBreak = true;
}
void Chip8Emulator::toggleBreakEmulation()
{
    m_isBreak = !m_isBreak;
}

void Chip8Emulator::OnInput(const uint8 key, const bool isPressed)
{
    m_core->updateKey(key, isPressed);
}
