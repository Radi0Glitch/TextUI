#ifndef LISTBOX_H
#define LISTBOX_H

#include "Widget.h"
#include "../core/Renderer.h"
#include <vector>

class ListBox : public Widget {
private:
    std::vector<std::string> items;
    int selectedIndex;
    int scrollOffset;
    bool hasFocus;

public:
    ListBox(int x, int y, int width, int height)
        : Widget(x, y, width, height), selectedIndex(-1), scrollOffset(0), hasFocus(false) {}

    void setFocus(bool focus) { hasFocus = focus; }
    bool isFocused() const { return hasFocus; }

    void addItem(const std::string& item) {
        items.push_back(item);
        markDirty();
    }

    void removeItem(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + static_cast<int>(index));
            if (selectedIndex >= static_cast<int>(items.size())) {
                selectedIndex = static_cast<int>(items.size()) - 1;
            }
            markDirty();
        }
    }

    void clear() {
        items.clear();
        selectedIndex = -1;
        scrollOffset = 0;
        markDirty();
    }

    int getSelectedIndex() const { return selectedIndex; }
    const std::string& getSelectedItem() const { return items[selectedIndex]; }

    void setSelectedIndex(int index) {
        if (index >= 0 && index < static_cast<int>(items.size())) {
            selectedIndex = index;
            // Прокрутка к выбранному элементу
            if (selectedIndex < scrollOffset) {
                scrollOffset = selectedIndex;
            } else if (selectedIndex >= scrollOffset + height - 2) {
                scrollOffset = selectedIndex - (height - 2) + 1;
            }
            markDirty();
        }
    }

    void render() override {
        if (!visible) return;

        ColorStyle style = hasFocus
            ? ColorStyle(Color::BLACK, BackgroundColor::WHITE).setBold()
            : colorStyle;

        Renderer::drawRectangle(x, y, width, height, boxStyle, style);

        int visibleItems = height - 2;
        for (int i = 0; i < visibleItems && scrollOffset + i < static_cast<int>(items.size()); i++) {
            int itemIndex = scrollOffset + i;
            std::string display = items[itemIndex];
            if (static_cast<int>(display.length()) > width - 4) {
                display = display.substr(0, width - 5) + "...";
            }

            if (itemIndex == selectedIndex) {
                display = ">" + display + "<";
            } else {
                display = " " + display + " ";
            }

            Renderer::drawText(x + 1, y + 1 + i, display, style);
        }
    }

    void renderToBuffer(RenderBuffer& buffer) override {
        if (!visible || !needsRedraw) return;

        ColorStyle style = hasFocus
            ? ColorStyle(Color::BLACK, BackgroundColor::WHITE).setBold()
            : colorStyle;

        // Рисуем рамку
        for (int i = x + 1; i < x + width - 1; i++) {
            buffer.setStyledChar(i, y, boxStyle.horizontal[0],
                static_cast<int>(style.foreground),
                static_cast<int>(style.background),
                style.bold, style.italic, style.underline);
            buffer.setStyledChar(i, y + height - 1, boxStyle.horizontal[0],
                static_cast<int>(style.foreground),
                static_cast<int>(style.background),
                style.bold, style.italic, style.underline);
        }
        for (int i = y + 1; i < y + height - 1; i++) {
            buffer.setStyledChar(x, i, boxStyle.vertical[0],
                static_cast<int>(style.foreground),
                static_cast<int>(style.background),
                style.bold, style.italic, style.underline);
            buffer.setStyledChar(x + width - 1, i, boxStyle.vertical[0],
                static_cast<int>(style.foreground),
                static_cast<int>(style.background),
                style.bold, style.italic, style.underline);
        }
        // Углы
        buffer.setStyledChar(x, y, boxStyle.top_left[0],
            static_cast<int>(style.foreground),
            static_cast<int>(style.background),
            style.bold, style.italic, style.underline);
        buffer.setStyledChar(x + width - 1, y, boxStyle.top_right[0],
            static_cast<int>(style.foreground),
            static_cast<int>(style.background),
            style.bold, style.italic, style.underline);
        buffer.setStyledChar(x, y + height - 1, boxStyle.bottom_left[0],
            static_cast<int>(style.foreground),
            static_cast<int>(style.background),
            style.bold, style.italic, style.underline);
        buffer.setStyledChar(x + width - 1, y + height - 1, boxStyle.bottom_right[0],
            static_cast<int>(style.foreground),
            static_cast<int>(style.background),
            style.bold, style.italic, style.underline);

        // Рисуем элементы
        int visibleItems = height - 2;
        for (int i = 0; i < visibleItems && scrollOffset + i < static_cast<int>(items.size()); i++) {
            int itemIndex = scrollOffset + i;
            std::string display = items[itemIndex];
            if (static_cast<int>(display.length()) > width - 4) {
                display = display.substr(0, width - 5) + "...";
            }

            if (itemIndex == selectedIndex) {
                display = ">" + display + "<";
            } else {
                display = " " + display + " ";
            }

            for (size_t j = 0; j < display.length() && x + 1 + static_cast<int>(j) < x + width - 1; j++) {
                buffer.setStyledChar(x + 1 + static_cast<int>(j), y + 1 + i, display[j],
                    static_cast<int>(style.foreground),
                    static_cast<int>(style.background),
                    style.bold, style.italic, style.underline);
            }
        }

        markClean();
    }

    bool handleInput(char key) override {
        if (!visible || !hasFocus || items.empty()) return false;

        switch (key) {
            case '\x1B': // Up arrow (simplified)
                if (selectedIndex > 0) {
                    setSelectedIndex(selectedIndex - 1);
                }
                return true;
            case '\x02': // Down arrow (simplified)
                if (selectedIndex < static_cast<int>(items.size()) - 1) {
                    setSelectedIndex(selectedIndex + 1);
                }
                return true;
            case ' ':
            case '\n':
            case '\r':
                // Выбор элемента
                return true;
        }
        return false;
    }

    bool handleMouse(int mouseX, int mouseY, MouseButton /*button*/, bool isPress) override {
        if (!visible) return false;

        if (isPointInside(mouseX, mouseY)) {
            if (isPress) {
                hasFocus = true;
                int itemIndex = mouseY - y - 1 + scrollOffset;
                if (itemIndex >= 0 && itemIndex < static_cast<int>(items.size())) {
                    setSelectedIndex(itemIndex);
                }
            }
            return true;
        }
        return false;
    }
};

#endif // LISTBOX_H
