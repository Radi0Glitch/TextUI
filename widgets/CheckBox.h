#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "Widget.h"
#include "../core/Renderer.h"

class CheckBox : public Widget {
private:
    bool checked;
    bool hasFocus;

public:
    CheckBox(int x, int y, const std::string& text, bool checked = false)
        : Widget(x, y, static_cast<int>(text.length()) + 4, 1), checked(checked), hasFocus(false) {
        this->text = text;
    }

    void setFocus(bool focus) { hasFocus = focus; }
    bool isFocused() const { return hasFocus; }

    void setChecked(bool value) {
        checked = value;
        markDirty();
    }

    bool isChecked() const { return checked; }

    void toggle() {
        checked = !checked;
        markDirty();
    }

    void render() override {
        if (!visible) return;

        ColorStyle style = hasFocus
            ? ColorStyle(Color::BLACK, BackgroundColor::WHITE).setBold()
            : colorStyle;

        // Рисуем рамку [x] или [ ]
        Renderer::drawText(x, y, checked ? "[X]" : "[ ]", style);
        // Рисуем текст
        Renderer::drawText(x + 4, y, text, style);
    }

    void renderToBuffer(RenderBuffer& buffer) override {
        if (!visible || !needsRedraw) return;

        ColorStyle style = hasFocus
            ? ColorStyle(Color::BLACK, BackgroundColor::WHITE).setBold()
            : colorStyle;

        const char* checkStr = checked ? "[X]" : "[ ]";
        for (int i = 0; i < 3 && x + i < buffer.getWidth(); i++) {
            buffer.setStyledChar(x + i, y, checkStr[i],
                static_cast<int>(style.foreground),
                static_cast<int>(style.background),
                style.bold, style.italic, style.underline);
        }

        for (size_t i = 0; i < text.length() && x + 4 + static_cast<int>(i) < buffer.getWidth(); i++) {
            buffer.setStyledChar(x + 4 + static_cast<int>(i), y, text[i],
                static_cast<int>(style.foreground),
                static_cast<int>(style.background),
                style.bold, style.italic, style.underline);
        }

        markClean();
    }

    bool handleInput(char key) override {
        if (!visible || !hasFocus) return false;

        if (key == ' ' || key == '\n' || key == '\r') {
            toggle();
            return true;
        }
        return false;
    }

    bool handleMouse(int mouseX, int mouseY, MouseButton /*button*/, bool isPress) override {
        if (!visible) return false;

        if (isPointInside(mouseX, mouseY)) {
            if (isPress) {
                hasFocus = true;
                toggle();
            }
            return true;
        }
        return false;
    }
};

#endif // CHECKBOX_H
