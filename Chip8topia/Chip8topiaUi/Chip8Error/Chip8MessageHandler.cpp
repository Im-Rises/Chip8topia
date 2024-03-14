#include "Chip8MessageHandler.h"

#include <utility>
#include <IconsFontAwesome6.h>

#include "../../Chip8topiaInputHandler/Chip8topiaInputHandler.h"

Chip8MessageHandler::Chip8MessageHandler() {
    Chip8topiaInputHandler::getInstance().m_SuccessEvent.subscribe(this, &Chip8MessageHandler::triggerSuccess);
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.subscribe(this, &Chip8MessageHandler::triggerError);
    Chip8topiaInputHandler::getInstance().m_WarningEvent.subscribe(this, &Chip8MessageHandler::triggerWarning);
    Chip8topiaInputHandler::getInstance().m_InfoEvent.subscribe(this, &Chip8MessageHandler::triggerInfo);
}

Chip8MessageHandler::~Chip8MessageHandler() {
    Chip8topiaInputHandler::getInstance().m_SuccessEvent.unsubscribe(this, &Chip8MessageHandler::triggerSuccess);
    Chip8topiaInputHandler::getInstance().m_ErrorEvent.unsubscribe(this, &Chip8MessageHandler::triggerError);
    Chip8topiaInputHandler::getInstance().m_WarningEvent.unsubscribe(this, &Chip8MessageHandler::triggerWarning);
    Chip8topiaInputHandler::getInstance().m_InfoEvent.unsubscribe(this, &Chip8MessageHandler::triggerInfo);
}

void Chip8MessageHandler::triggerError(const std::string& message, std::function<void()> callback) {
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_EXCLAMATION " Error", message, std::move(callback));
}

void Chip8MessageHandler::triggerWarning(const std::string& message, std::function<void()> callback) {
    m_messageQueue.pushMessage(ICON_FA_TRIANGLE_EXCLAMATION " Warning", message, std::move(callback));
}

void Chip8MessageHandler::triggerInfo(const std::string& message, std::function<void()> callback) {
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_INFO " Info", message, std::move(callback));
}

void Chip8MessageHandler::triggerSuccess(const std::string& message, std::function<void()> callback) {
    m_messageQueue.pushMessage(ICON_FA_CIRCLE_CHECK " Success", message, std::move(callback));
}

void Chip8MessageHandler::showMessages() {
    m_messageQueue.showMessage();
}

void Chip8MessageHandler::clearMessageQueue() {
    m_messageQueue.clearMessageQueue();
}
