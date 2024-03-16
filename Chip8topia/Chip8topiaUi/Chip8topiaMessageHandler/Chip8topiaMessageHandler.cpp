#include "Chip8topiaMessageHandler.h"

#include <utility>
#include <IconsFontAwesome6.h>

#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Chip8topiaMessageHandler::Chip8topiaMessageHandler()
{
    Chip8topiaInputHandler::getInstance().m_SuccessEvent.subscribe(this, &Chip8topiaMessageHandler::triggerSuccess);
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.subscribe(this, &Chip8topiaMessageHandler::triggerError);
    Chip8topiaInputHandler::getInstance().m_WarningEvent.subscribe(this, &Chip8topiaMessageHandler::triggerWarning);
    Chip8topiaInputHandler::getInstance().m_InfoEvent.subscribe(this, &Chip8topiaMessageHandler::triggerInfo);
}

Chip8topiaMessageHandler::~Chip8topiaMessageHandler()
{
    Chip8topiaInputHandler::getInstance().m_SuccessEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerSuccess);
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerError);
    Chip8topiaInputHandler::getInstance().m_WarningEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerWarning);
    Chip8topiaInputHandler::getInstance().m_InfoEvent.unsubscribe(this, &Chip8topiaMessageHandler::triggerInfo);
}

void Chip8topiaMessageHandler::triggerError(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_EXCLAMATION " Error", message, std::move(callback));
}

void Chip8topiaMessageHandler::triggerWarning(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_TRIANGLE_EXCLAMATION " Warning", message, std::move(callback));
}

void Chip8topiaMessageHandler::triggerInfo(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_INFO " Info", message, std::move(callback));
}

void Chip8topiaMessageHandler::triggerSuccess(const std::string& message, std::function<void()> callback)
{
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_CHECK " Success", message, std::move(callback));
}

void Chip8topiaMessageHandler::showMessages()
{
    m_messageQueue.showMessage();
    Chip8topiaInputHandler::getInstance().setInputEnabled(m_messageQueue.empty());
}

void Chip8topiaMessageHandler::clearMessageQueue()
{
    m_messageQueue.clearMessageQueue();
}
