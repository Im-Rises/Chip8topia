#include "Chip8topiaMessageHandler.h"

#include <IconsFontAwesome6.h>
#include <consoleLogger/consoleLogger.h>

#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Chip8topiaMessageHandler::Chip8topiaMessageHandler()
{
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();

    inputHandler.m_SuccessEvent.subscribe(this, &Chip8topiaMessageHandler::triggerSuccess);
    inputHandler.m_ErrorEvent.subscribe(this, &Chip8topiaMessageHandler::triggerError);
    inputHandler.m_WarningEvent.subscribe(this, &Chip8topiaMessageHandler::triggerWarning);
    inputHandler.m_InfoEvent.subscribe(this, &Chip8topiaMessageHandler::triggerInfo);

    inputHandler.m_EmulationError.subscribe(this, &Chip8topiaMessageHandler::triggerEmulationError);
}

Chip8topiaMessageHandler::~Chip8topiaMessageHandler()
{
    Chip8topiaInputHandler& inputHandler = Chip8topiaInputHandler::getInstance();

    inputHandler.m_SuccessEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerSuccess);
    inputHandler.m_ErrorEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerError);
    inputHandler.m_WarningEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerWarning);
    inputHandler.m_InfoEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerInfo);

    inputHandler.m_EmulationError.unsubscribe(this, &Chip8topiaMessageHandler::triggerEmulationError);
}

void Chip8topiaMessageHandler::triggerError(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_EXCLAMATION " Error", message, std::move(callback));
    LOG_ERROR(message);
}

void Chip8topiaMessageHandler::triggerWarning(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_TRIANGLE_EXCLAMATION " Warning", message, std::move(callback));
    LOG_WARNING(message);
}

void Chip8topiaMessageHandler::triggerInfo(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_INFO " Info", message, std::move(callback));
    LOG_INFO(message);
}

void Chip8topiaMessageHandler::triggerSuccess(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_CHECK " Success", message, std::move(callback));
    LOG_SUCCESS(message);
}

void Chip8topiaMessageHandler::triggerEmulationError(const std::string& message)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_EXCLAMATION " Emulation Error", message);
    LOG_ERROR(message);
}

void Chip8topiaMessageHandler::showMessages()
{
    m_messageQueue.showMessages();
    Chip8topiaInputHandler::getInstance().setInputEnabled(m_messageQueue.empty());
}

void Chip8topiaMessageHandler::clearMessageQueue()
{
    m_messageQueue.clearMessageQueue();
}
