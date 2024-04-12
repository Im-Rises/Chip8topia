#include "Chip8topiaInputHandler.h"

#include "../Chip8topia.h"

void Chip8topiaInputHandler::update(Chip8topia& chip8topia, SDL_Event& event) const
{
    auto& inputHandler = Chip8topiaInputHandler::getInstance();

    if (!inputHandler.getInputEnabled())
    {
        return;
    }

    switch (event.type)
    {
    case SDL_KEYDOWN:
    {
        switch (event.key.keysym.sym)
        {
#if !defined(BUILD_RELEASE)
        case SDLK_b: m_SuccessEvent.trigger("Test error message", nullptr); break;
        case SDLK_n: m_ErrorEvent.trigger("Test error message", nullptr); break;
        case SDLK_m: m_WarningEvent.trigger("Test warning message", nullptr); break;
        case SDLK_COMMA: m_InfoEvent.trigger("Test info message", nullptr); break;
        case SDLK_F12: m_DebugRomFastLoadEvent.trigger(); break;
#endif

#if !defined(__EMSCRIPTEN__)
        case SDLK_y: m_ToggleTurboModeEvent.trigger(); break;
        case SDLK_F10: m_CenterWindowEvent.trigger(); break;
        case SDLK_F11: m_ToggleFullScreenEvent.trigger(); break;
#endif

        case SDLK_u: m_ToggleMainBarEvent.trigger(); break;
        case SDLK_i: m_CloseAllWindowsEvent.trigger(); break;
        case SDLK_p: m_TogglePauseEmulationEvent.trigger(); break;
        case SDLK_l: m_RestartEmulationEvent.trigger(); break;
        case SDLK_o: m_OpenRomExplorerEvent.trigger(); break;

        case SDLK_1: m_GameInput.trigger(0x1, true); break;
        case SDLK_2: m_GameInput.trigger(0x2, true); break;
        case SDLK_3: m_GameInput.trigger(0x3, true); break;
        case SDLK_4: m_GameInput.trigger(0xC, true); break;
        case SDLK_q: m_GameInput.trigger(0x4, true); break;
        case SDLK_w: m_GameInput.trigger(0x5, true); break;
        case SDLK_e: m_GameInput.trigger(0x6, true); break;
        case SDLK_r: m_GameInput.trigger(0xD, true); break;
        case SDLK_a: m_GameInput.trigger(0x7, true); break;
        case SDLK_s: m_GameInput.trigger(0x8, true); break;
        case SDLK_d: m_GameInput.trigger(0x9, true); break;
        case SDLK_f: m_GameInput.trigger(0xE, true); break;
        case SDLK_z: m_GameInput.trigger(0xA, true); break;
        case SDLK_x: m_GameInput.trigger(0x0, true); break;
        case SDLK_c: m_GameInput.trigger(0xB, true); break;
        case SDLK_v: m_GameInput.trigger(0xF, true); break;
        }
        break;
    }
    case SDL_KEYUP:
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_1: m_GameInput.trigger(0x1, false); break;
        case SDLK_2: m_GameInput.trigger(0x2, false); break;
        case SDLK_3: m_GameInput.trigger(0x3, false); break;
        case SDLK_4: m_GameInput.trigger(0xC, false); break;
        case SDLK_q: m_GameInput.trigger(0x4, false); break;
        case SDLK_w: m_GameInput.trigger(0x5, false); break;
        case SDLK_e: m_GameInput.trigger(0x6, false); break;
        case SDLK_r: m_GameInput.trigger(0xD, false); break;
        case SDLK_a: m_GameInput.trigger(0x7, false); break;
        case SDLK_s: m_GameInput.trigger(0x8, false); break;
        case SDLK_d: m_GameInput.trigger(0x9, false); break;
        case SDLK_f: m_GameInput.trigger(0xE, false); break;
        case SDLK_z: m_GameInput.trigger(0xA, false); break;
        case SDLK_x: m_GameInput.trigger(0x0, false); break;
        case SDLK_c: m_GameInput.trigger(0xB, false); break;
        case SDLK_v: m_GameInput.trigger(0xF, false); break;
        }
        break;
    }
    case SDL_DROPFILE:
    {
        if (event.drop.file != nullptr)
        {
            std::string romPath(event.drop.file);
            SDL_free(event.drop.file);
            chip8topia.loadRomFromPath(romPath);
        }
        else
        {
            m_ErrorEvent.trigger("Failed to load ROM by drag and drop", nullptr);
        }
        break;
    }
    }
}

auto Chip8topiaInputHandler::getInputEnabled() const -> bool
{
    return m_inputEnabled;
}

void Chip8topiaInputHandler::setInputEnabled(const bool inputEnabled)
{
    m_inputEnabled = inputEnabled;
}
