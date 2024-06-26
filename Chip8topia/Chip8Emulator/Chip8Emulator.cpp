#include "Chip8Emulator.h"

#include <ImGuiNotify.hpp>
#include <consoleLogger/consoleLogger.h>
#include <fmt/format.h>

#include "../Chip8topiaInputHandler/Chip8topiaInputHandler.h"
#include "ChipCores/Chip8Core/Chip8Core.h"
#include "ChipCores/SChip11Core/SChip11Core.h"
#include "ChipCores/SchipCCore/SChipCCore.h"
#include "ChipCores/XoChipCore/XoChipCore.h"

Chip8Emulator::Chip8Emulator() : m_breakpointsStates{},
#if defined(BUILD_DEBUG)
                                 // Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<SChipCCore>(DEFAULT_FREQUENCY)),
                                 // Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<SChip11Core>(DEFAULT_FREQUENCY)),
                                 // Chip8Emulator::Chip8Emulator() : m_core(std::make_unique<Chip8Core>(DEFAULT_FREQUENCY)),
                                 //                                 m_core(std::make_unique<XoChipCore>(Chip8Frequency::Freq1200000Hz)),
                                 m_core(std::make_unique<XoChipCore>(Chip8Frequency::Freq1800Hz)),
#else
                                 m_core(std::make_unique<XoChipCore>(DEFAULT_FREQUENCY)),
#endif
                                 m_isRomLoaded(false),
                                 m_isBreak(false),
                                 m_step(false),
                                 m_canBreak(true),
                                 m_errorTriggered(false)
{
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();
    inputHandler.m_GameInput.subscribe(this, &Chip8Emulator::OnInput);
    inputHandler.m_TogglePauseEmulationEvent.subscribe(this, &Chip8Emulator::toggleBreakEmulation);
    inputHandler.m_RestartEmulationEvent.subscribe(this, &Chip8Emulator::restart);

    inputHandler.m_BreakEmulationEvent.subscribe(this, &Chip8Emulator::breakEmulation);
    inputHandler.m_StepEmulationEvent.subscribe(this, &Chip8Emulator::stepEmulation);
    inputHandler.m_RunEmulationEvent.subscribe(this, &Chip8Emulator::runEmulation);
    inputHandler.m_ClearBreakpointsEvent.subscribe(this, &Chip8Emulator::clearBreakpoints);

#if defined(BUILD_PARAM_SAFE)
    inputHandler.m_EmulationError.subscribe(this, &Chip8Emulator::triggerEmulationError);
#endif

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

#if defined(BUILD_PARAM_SAFE)
    inputHandler.m_EmulationError.unsubscribe(this, &Chip8Emulator::triggerEmulationError);
#endif
}

void Chip8Emulator::resetColorPalette()
{
    m_videoEmulation.resetToColorColors();
}

void Chip8Emulator::restart()
{
    m_core->reset();
    m_videoEmulation.reset();
    m_soundEmulation.reset();
    m_errorTriggered = false;
}

void Chip8Emulator::loadRom(const std::vector<uint8_t>& romData)
{
    restart();
    m_core->readRom(romData);
    m_isRomLoaded = true;
}

void Chip8Emulator::update(const float /* deltaTime */)
{
#if defined(BUILD_PARAM_SAFE)
    try
    {
#endif
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
            bool screenUpdated = false;
            while (!screenUpdated && !m_isBreak && !m_errorTriggered)
            {
                screenUpdated = m_core->clock();

                if (m_breakpointsStates[m_core->getCpu()->getPc()])
                {
                    m_isBreak = true;
                    ImGui::InsertNotification({ ImGuiToastType::Info, TOAST_DURATION_INFO,
                        "Breakpoint hit", fmt::format("Breakpoint hit at 0x{:04X}", m_core->getCpu()->getPc()).c_str() });
                }
            }
        }
#if defined(BUILD_PARAM_SAFE)
    }
    catch (const std::exception& e)
    {
        TRIGGER_ERROR(true, "Emulation error: {}", e.what());
    }
    catch (const char* const e)
    {
        TRIGGER_ERROR(true, "Emulation error: {}", e);
    }
    catch (...)
    {
        TRIGGER_ERROR(true, "Emulation error: An unknown error occurred during emulation.");
    }
#endif
}

void Chip8Emulator::emitSound()
{
    if (!m_isBreak && m_isRomLoaded && !m_errorTriggered)
    {
        m_soundEmulation.update(m_core);
    }
}

void Chip8Emulator::render(const float screenWidth, const float screenHeight)
{
    m_videoEmulation.updateTexture(m_core);
    m_videoEmulation.update(m_core, screenWidth, screenHeight, CHIP8_ASPECT_RATIO);
}

void Chip8Emulator::stop()
{
    if (m_isRomLoaded)
    {
        m_soundEmulation.reset();
        m_isRomLoaded = false;
        m_romName = "ROM";
        ImGui::InsertNotification({ ImGuiToastType::Info, TOAST_DURATION_INFO, "Emulation stopped", "The emulation has been stopped. Please load a ROM to continue." });
        LOG_INFO("Emulation stopped, please load a ROM to continue");
    }
    else
    {
        ImGui::InsertNotification({ ImGuiToastType::Info, TOAST_DURATION_INFO, "Emulation already stopped", "The emulation is already stopped. Please load a ROM to continue." });
        LOG_ERROR("Emulation already stopped, please load a ROM to continue");
    }
}

#if defined(BUILD_PARAM_SAFE)
void Chip8Emulator::triggerEmulationError(const std::string& /* message */)
{
    m_soundEmulation.reset();
    m_errorTriggered = true;
}
#endif

void Chip8Emulator::setRomName(const std::string& romName)
{
    m_romName = romName;
}

void Chip8Emulator::switchCoreFrequency(const Chip8CoreType coreType, const Chip8Frequency frequency)
{
    m_soundEmulation.reset();

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

    ImGui::InsertNotification({ ImGuiToastType::Info, TOAST_DURATION_INFO, "Core and frequency changed", "The core and frequency have been changed. Please load a ROM to continue." });
    LOG_INFO("Core and frequency changed, please load a ROM to continue");
}

void Chip8Emulator::clearBreakpoints()
{
    m_breakpointsStates.fill(false);
    m_breakpointsList.clear();
}

void Chip8Emulator::stepEmulation()
{
    m_soundEmulation.reset();
    m_isBreak = true;
    m_step = true;
}
void Chip8Emulator::runEmulation()
{
    m_isBreak = false;
}

void Chip8Emulator::breakEmulation()
{
    m_soundEmulation.reset();
    m_isBreak = true;
}

void Chip8Emulator::toggleBreakEmulation()
{
    m_isBreak = !m_isBreak;

    if (m_isBreak)
    {
        m_soundEmulation.reset();
    }
}

void Chip8Emulator::OnInput(const uint8 key, const bool isPressed)
{
    m_core->updateKey(key, isPressed);
}

auto Chip8Emulator::getChip8Core() -> Chip8CoreBase*
{
    return m_core.get();
}

auto Chip8Emulator::getChip8VideoEmulation() -> Chip8VideoEmulation&
{
    return m_videoEmulation;
}

auto Chip8Emulator::getChip8SoundEmulation() -> Chip8SoundEmulation&
{
    return m_soundEmulation;
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

auto Chip8Emulator::getBreakpointsStates() -> std::array<bool, CpuBase::MEMORY_SIZE>&
{
    return m_breakpointsStates;
}

auto Chip8Emulator::getBreakpointsList() -> std::set<uint16>&
{
    return m_breakpointsList;
}

auto Chip8Emulator::getCoreType() const -> Chip8CoreType
{
    return m_core->getType();
}

auto Chip8Emulator::getFrequency() const -> Chip8Frequency
{
    return m_core->getFrequency();
}
