#ifndef TEXTUI_STATUSBAR_H
#define TEXTUI_STATUSBAR_H

#include "Widget.h"
#include "../core/Screen.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

/**
 * @brief Р­Р»РµРјРµРЅС‚ СЃС‚СЂРѕРєРё СЃРѕСЃС‚РѕСЏРЅРёСЏ
 */
struct StatusItem {
    std::string text;
    std::string shortcut;  // РќР°РїСЂРёРјРµСЂ "F1"
    std::function<void()> action;
    bool visible = true;

    StatusItem() = default;
    StatusItem(const std::string& t, const std::string& s = "", std::function<void()> a = nullptr)
        : text(t), shortcut(s), action(a) {}
};

/**
 * @brief РЎС‚СЂРѕРєР° СЃРѕСЃС‚РѕСЏРЅРёСЏ (РІРЅРёР·Сѓ СЌРєСЂР°РЅР°)
 */
class StatusBar : public Widget {
private:
    std::vector<StatusItem> items_;
    ColorAttr normalColor_;
    ColorAttr highlightColor_;

public:
    StatusBar(int x, int y, int width)
        : Widget(x, y, width, 1) {
        canFocus_ = false;
        normalColor_ = ColorAttr(Color8::Black, Color8::Gray);
        highlightColor_ = ColorAttr(Color8::BrightWhite, Color8::Blue);
    }

    void setColors(ColorAttr normal, ColorAttr highlight) {
        normalColor_ = normal;
        highlightColor_ = highlight;
    }

    // Р”РѕР±Р°РІР»РµРЅРёРµ СЌР»РµРјРµРЅС‚Р°
    void addItem(const std::string& text, const std::string& shortcut = "", std::function<void()> action = nullptr) {
        items_.push_back(StatusItem(text, shortcut, action));
    }

    void clearItems() {
        items_.clear();
    }

    // РћР±РЅРѕРІР»РµРЅРёРµ С‚РµРєСЃС‚Р° СЌР»РµРјРµРЅС‚Р° РїРѕ РёРЅРґРµРєСЃСѓ
    void setItemText(size_t index, const std::string& text) {
        if (index < items_.size()) {
            items_[index].text = text;
        }
    }

    // РЈСЃС‚Р°РЅРѕРІРєР° РґРµР№СЃС‚РІРёСЏ РґР»СЏ СЌР»РµРјРµРЅС‚Р°
    void setItemAction(size_t index, std::function<void()> action) {
        if (index < items_.size()) {
            items_[index].action = action;
        }
    }

    // РћР±СЂР°Р±РѕС‚РєР° РЅР°Р¶Р°С‚РёСЏ С„СѓРЅРєС†РёРѕРЅР°Р»СЊРЅРѕР№ РєР»Р°РІРёС€Рё
    bool handleFunctionKey(int fKey) {
        for (auto& item : items_) {
            if (!item.visible) continue;
            
            // РџСЂРѕРІРµСЂРєР° РЅР° F1-F12
            if (item.shortcut == "F" + std::to_string(fKey)) {
                if (item.action) {
                    item.action();
                }
                return true;
            }
        }
        return false;
    }

    bool handleKey(Key key) override {
        if (!visible_) return false;

        // РћР±СЂР°Р±РѕС‚РєР° F1-F12
        if (key >= Key::F1 && key <= Key::F12) {
            int fNum = static_cast<int>(key) - static_cast<int>(Key::F1) + 1;
            return handleFunctionKey(fNum);
        }

        return false;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        // Р¤РѕРЅ СЃС‚СЂРѕРєРё СЃРѕСЃС‚РѕСЏРЅРёСЏ
        screen.fillRect(x_, y_, width_, height_, ' ', normalColor_);

        int currentX = x_ + 1;

        for (const auto& item : items_) {
            if (!item.visible) continue;

            std::string display;
            
            if (!item.shortcut.empty()) {
                // Р¤РѕСЂРјР°С‚: " F1 Help "
                display = " " + item.shortcut + " " + item.text + " ";

                // Р РёСЃСѓРµРј shortcut РІС‹РґРµР»РµРЅРЅС‹Рј
                screen.putString(currentX, y_, (" " + item.shortcut + " ").c_str(), highlightColor_);
                screen.putString(currentX + 1 + static_cast<int>(item.shortcut.length()), y_,
                                (item.text + " ").c_str(), normalColor_);
            } else {
                display = " " + item.text + " ";
                screen.putString(currentX, y_, display.c_str(), normalColor_);
            }

            currentX += static_cast<int>(display.length());
        }

        // Р Р°РјРєР° СЃРІРµСЂС…Сѓ
        screen.drawHLine(x_, y_, width_, Symbols::separatorH, ColorAttr::normal());
    }
};

} // namespace ui

#endif // TEXTUI_STATUSBAR_H
