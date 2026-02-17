#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Widget.h"
#include "../core/Renderer.h"
#include "../platform/Platform.h"
#include <iostream>

class TextBox : public Widget {
private:
    bool hasFocus;
    size_t cursorPos;

public:
    TextBox(int x, int y, int width, const std::string& content = "")
        : Widget(x, y, width, 3), hasFocus(false), cursorPos(0) {
        text = content;
    }

    void setFocus(bool focus) { hasFocus = focus; }
    bool isFocused() const { return hasFocus; }

    void setContent(const std::string& newContent) {
        text = newContent;
        cursorPos = text.length();
        markDirty();
    }

    const std::string& getContent() const { return text; }

    void render() override {
        if (!visible) return;

        ColorStyle boxStyle = hasFocus
            ? ColorStyle(Color::BLACK, BackgroundColor::WHITE)
            : colorStyle;

        Renderer::drawRectangle(x, y, width, height, this->boxStyle, boxStyle);
        Renderer::drawText(x + 1, y + 1, text, boxStyle);

        // Рисуем курсор если фокус
        if (hasFocus && cursorPos <= text.length()) {
            Platform::moveCursor(x + 1 + static_cast<int>(cursorPos), y + 1);
            std::cout << "_";
        }
    }

    void renderToBuffer(RenderBuffer& buffer) override {
        if (!visible || !needsRedraw) return;

        ColorStyle boxStyle = hasFocus
            ? ColorStyle(Color::BLACK, BackgroundColor::WHITE)
            : this->colorStyle;

        // Рисуем рамку
        for (int i = x + 1; i < x + width - 1; i++) {
            buffer.setStyledChar(i, y, this->boxStyle.horizontal[0],
                static_cast<int>(boxStyle.foreground),
                static_cast<int>(boxStyle.background),
                boxStyle.bold, boxStyle.italic, boxStyle.underline);
            buffer.setStyledChar(i, y + height - 1, this->boxStyle.horizontal[0],
                static_cast<int>(boxStyle.foreground),
                static_cast<int>(boxStyle.background),
                boxStyle.bold, boxStyle.italic, boxStyle.underline);
        }
        for (int i = y + 1; i < y + height - 1; i++) {
            buffer.setStyledChar(x, i, this->boxStyle.vertical[0],
                static_cast<int>(boxStyle.foreground),
                static_cast<int>(boxStyle.background),
                boxStyle.bold, boxStyle.italic, boxStyle.underline);
            buffer.setStyledChar(x + width - 1, i, this->boxStyle.vertical[0],
                static_cast<int>(boxStyle.foreground),
                static_cast<int>(boxStyle.background),
                boxStyle.bold, boxStyle.italic, boxStyle.underline);
        }
        // Углы
        buffer.setStyledChar(x, y, this->boxStyle.top_left[0],
            static_cast<int>(boxStyle.foreground),
            static_cast<int>(boxStyle.background),
            boxStyle.bold, boxStyle.italic, boxStyle.underline);
        buffer.setStyledChar(x + width - 1, y, this->boxStyle.top_right[0],
            static_cast<int>(boxStyle.foreground),
            static_cast<int>(boxStyle.background),
            boxStyle.bold, boxStyle.italic, boxStyle.underline);
        buffer.setStyledChar(x, y + height - 1, this->boxStyle.bottom_left[0],
            static_cast<int>(boxStyle.foreground),
            static_cast<int>(boxStyle.background),
            boxStyle.bold, boxStyle.italic, boxStyle.underline);
        buffer.setStyledChar(x + width - 1, y + height - 1, this->boxStyle.bottom_right[0],
            static_cast<int>(boxStyle.foreground),
            static_cast<int>(boxStyle.background),
            boxStyle.bold, boxStyle.italic, boxStyle.underline);

        // Рисуем текст
        for (size_t i = 0; i < text.length() && x + 1 + static_cast<int>(i) < x + width - 1; i++) {
            buffer.setStyledChar(x + 1 + static_cast<int>(i), y + 1, text[i],
                static_cast<int>(boxStyle.foreground),
                static_cast<int>(boxStyle.background),
                boxStyle.bold, boxStyle.italic, boxStyle.underline);
        }

        markClean();
    }

    bool handleInput(char key) override {
        if (!visible || !hasFocus) return false;

        if (key == '\b' || key == 127) { // Backspace
            if (cursorPos > 0) {
                text.erase(cursorPos - 1, 1);
                cursorPos--;
                markDirty();
            }
            return true;
        } else if (key == 27) { // Escape
            hasFocus = false;
            return true;
        } else if (key >= 32 && key < 127) { // Printable characters
            text.insert(cursorPos, 1, key);
            cursorPos++;
            markDirty();
            return true;
        }
        return false;
    }

    bool handleMouse(int mouseX, int mouseY, MouseButton /*button*/, bool isPress) override {
        if (!visible) return false;

        if (isPointInside(mouseX, mouseY)) {
            if (isPress) {
                hasFocus = true;
                // Простая установка курсора
                cursorPos = static_cast<size_t>(mouseX - x - 1);
                if (cursorPos > text.length()) cursorPos = text.length();
            }
            return true;
        } else {
            hasFocus = false;
        }
        return false;
    }
};

#endif // TEXTBOX_H
