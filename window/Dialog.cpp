#include "../window/Dialog.h"
#include "../core/Renderer.h"
#include "../core/SimpleAPI.h"
#include "../widgets/Label.h"
#include "../widgets/Button.h"
#include <iostream>

// Dialog implementation
Dialog::Dialog(int x, int y, int width, int height, const std::string& title)
    : Window(x, y, width, height, title) {
    modal = true;
}

void Dialog::accept() {
    if (onAccept) {
        onAccept();
    }
    close();
}

void Dialog::cancel() {
    if (onCancel) {
        onCancel();
    }
    close();
}

// MessageBox implementation
MessageBox::MessageBox(const std::string& title, const std::string& msg)
    : ModalDialog(20, 10, 40, 10, title), message(msg) {
    // Добавляем кнопку OK
    okButton = addChild<Button>(getWidth() / 2 - 4, getHeight() - 3, 8, "OK");
    okButton->setOnClick([this]() {
        if (onOk) {
            onOk();
        }
        completeWithResult();
    });
    okButton->setFocus(true);
}

void MessageBox::render() {
    Window::render();
    // Рисуем сообщение
    if (!message.empty()) {
        int textX = getX() + (getWidth() - static_cast<int>(message.length())) / 2;
        int textY = getY() + 2;
        if (textX > getX() && textX + static_cast<int>(message.length()) < getX() + getWidth()) {
            Renderer::drawText(textX, textY, message, ColorStyle(Color::WHITE));
        }
    }
}

bool MessageBox::handleInput(char key) {
    if (key == '\n' || key == '\r' || key == ' ') {
        if (onOk) {
            onOk();
        }
        completeWithResult();
        return true;
    }
    return ModalDialog::handleInput(key);
}

// Функции для показа модальных диалогов
void showMessageDialog(Window* parent, const std::string& title, const std::string& message) {
    auto dialog = std::make_unique<MessageBox>(title, message);
    if (parent) {
        int centerX = parent->getX() + parent->getWidth() / 2;
        int centerY = parent->getY() + parent->getHeight() / 2;
        dialog->setPosition(centerX - dialog->getWidth() / 2,
                           centerY - dialog->getHeight() / 2);
    }
    if (auto* terminal = ui::g_terminal.get()) {
        terminal->getWidgetManager().setModalWindow(dialog.get());
    }
    dialog->waitForResult();
}

bool showConfirmDialog(Window* parent, const std::string& title, const std::string& message) {
    (void)parent; (void)title; (void)message;
    return false;
}

std::string showInputDialog(Window* parent, const std::string& title, const std::string& message,
                           const std::string& defaultValue) {
    (void)parent; (void)title; (void)message; (void)defaultValue;
    return "";
}
