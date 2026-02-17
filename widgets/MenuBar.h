#ifndef MENUBAR_H
#define MENUBAR_H

#include "Widget.h"
#include "../core/Renderer.h"
#include <vector>
#include <functional>

struct MenuItem {
    std::string text;
    std::function<void()> action;
};

class MenuBar : public Widget {
private:
    std::vector<MenuItem> items;
    int selectedIndex;
    bool hasFocus;

public:
    MenuBar(int x, int y, int width)
        : Widget(x, y, width, 1), selectedIndex(-1), hasFocus(false) {}

    void setFocus(bool focus) { hasFocus = focus; }
    bool isFocused() const { return hasFocus; }

    void addItem(const std::string& text, const std::function<void()>& action = nullptr) {
        items.push_back({text, action});
        markDirty();
    }

    void render() override {
        if (!visible) return;

        int currentX = x;
        for (size_t i = 0; i < items.size(); i++) {
            ColorStyle style = (static_cast<int>(i) == selectedIndex)
                ? ColorStyle(Color::BLACK, BackgroundColor::WHITE).setBold()
                : colorStyle;

            std::string display = " " + items[i].text + " ";
            Renderer::drawText(currentX, y, display, style);
            currentX += static_cast<int>(display.length());
        }
    }

    void renderToBuffer(RenderBuffer& buffer) override {
        if (!visible || !needsRedraw) return;

        int currentX = x;
        for (size_t i = 0; i < items.size(); i++) {
            ColorStyle style = (static_cast<int>(i) == selectedIndex)
                ? ColorStyle(Color::BLACK, BackgroundColor::WHITE).setBold()
                : colorStyle;

            std::string display = " " + items[i].text + " ";
            for (size_t j = 0; j < display.length() && currentX + static_cast<int>(j) < buffer.getWidth(); j++) {
                buffer.setStyledChar(currentX + static_cast<int>(j), y, display[j],
                    static_cast<int>(style.foreground),
                    static_cast<int>(style.background),
                    style.bold, style.italic, style.underline);
            }
            currentX += static_cast<int>(display.length());
        }

        markClean();
    }

    bool handleInput(char key) override {
        if (!visible || !hasFocus || items.empty()) return false;

        switch (key) {
            case '\t': // Right
                if (selectedIndex < static_cast<int>(items.size()) - 1) {
                    selectedIndex++;
                    markDirty();
                }
                return true;
            case '\x1B': // Left (simplified)
                if (selectedIndex > 0) {
                    selectedIndex--;
                    markDirty();
                }
                return true;
            case ' ':
            case '\n':
            case '\r':
                if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size()) && items[selectedIndex].action) {
                    items[selectedIndex].action();
                }
                return true;
        }
        return false;
    }

    bool handleMouse(int mouseX, int mouseY, MouseButton /*button*/, bool isPress) override {
        if (!visible || mouseY != y) return false;

        int currentX = x;
        for (size_t i = 0; i < items.size(); i++) {
            int itemWidth = static_cast<int>(items[i].text.length()) + 2;
            if (mouseX >= currentX && mouseX < currentX + itemWidth) {
                if (isPress) {
                    hasFocus = true;
                    selectedIndex = static_cast<int>(i);
                    markDirty();
                    if (items[i].action) {
                        items[i].action();
                    }
                }
                return true;
            }
            currentX += itemWidth;
        }

        return false;
    }
};

#endif // MENUBAR_H
