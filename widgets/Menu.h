#ifndef TEXTUI_MENU_H
#define TEXTUI_MENU_H

#include "Widget.h"
#include "../core/Screen.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

/**
 * @brief Р’РµСЂС‚РёРєР°Р»СЊРЅРѕРµ РјРµРЅСЋ
 */
class Menu : public Widget {
private:
    struct MenuItem {
        std::string label;
        std::function<void()> action;
        char hotkey;
        bool enabled;
        bool separator;

        MenuItem() : hotkey('\0'), enabled(true), separator(false) {}
        MenuItem(const std::string& l, std::function<void()> a = nullptr, char hk = '\0')
            : label(l), action(a), hotkey(hk), enabled(true), separator(false) {}
        
        static MenuItem createSeparator() {
            MenuItem m;
            m.separator = true;
            return m;
        }
    };

    std::vector<MenuItem> items_;
    int selectedIndex_ = -1;
    bool hasFocus_ = false;
    ColorAttr highlightColor_;
    std::function<void(int)> onSelect_;

public:
    Menu(int x, int y, int width)
        : Widget(x, y, width, 1) {
        canFocus_ = true;
        highlightColor_ = ColorAttr::biosSelected();
    }

    void setHighlightColor(ColorAttr color) {
        highlightColor_ = color;
    }

    // Р”РѕР±Р°РІР»РµРЅРёРµ СЌР»РµРјРµРЅС‚Р°
    void addItem(const std::string& label, std::function<void()> action = nullptr, char hotkey = '\0') {
        items_.push_back(MenuItem(label, action, hotkey));
        height_ = static_cast<int>(items_.size()) + 2;
        if (selectedIndex_ < 0) selectedIndex_ = 0;
    }

    // Р”РѕР±Р°РІР»РµРЅРёРµ СЂР°Р·РґРµР»РёС‚РµР»СЏ
    void addSeparator() {
        items_.push_back(MenuItem::createSeparator());
        height_ = static_cast<int>(items_.size()) + 2;
    }

    void clearItems() {
        items_.clear();
        selectedIndex_ = -1;
        height_ = 3;
    }

    int getSelectedIndex() const { return selectedIndex_; }

    void setSelectedIndex(int index) {
        if (index >= -1 && index < static_cast<int>(items_.size())) {
            // РџСЂРѕРїСѓСЃРєР°РµРј СЃРµРїР°СЂР°С‚РѕСЂС‹
            while (index >= 0 && index < static_cast<int>(items_.size()) && items_[index].separator) {
                index++;
            }
            selectedIndex_ = index;
        }
    }

    bool hasFocus() const { return hasFocus_; }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
        if (focus && selectedIndex_ == -1) {
            setSelectedIndex(0);
        }
    }

    void setOnSelect(std::function<void(int)> callback) {
        onSelect_ = callback;
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_) return false;

        switch (key) {
            case Key::Up:
                do {
                    if (selectedIndex_ > 0) {
                        selectedIndex_--;
                    } else {
                        selectedIndex_ = static_cast<int>(items_.size()) - 1;
                    }
                } while (items_[selectedIndex_].separator);
                return true;

            case Key::Down:
                do {
                    if (selectedIndex_ < static_cast<int>(items_.size()) - 1) {
                        selectedIndex_++;
                    } else {
                        selectedIndex_ = 0;
                    }
                } while (items_[selectedIndex_].separator);
                return true;

            case Key::Enter:
            case Key::Space:
                if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(items_.size())) {
                    if (!items_[selectedIndex_].separator && items_[selectedIndex_].enabled) {
                        if (items_[selectedIndex_].action) {
                            items_[selectedIndex_].action();
                        }
                        if (onSelect_) {
                            onSelect_(selectedIndex_);
                        }
                    }
                }
                return true;

            default:
                return false;
        }
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;

        // РџРѕРёСЃРє СЌР»РµРјРµРЅС‚Р° РїРѕ РіРѕСЂСЏС‡РµР№ РєР»Р°РІРёС€Рµ
        for (size_t i = 0; i < items_.size(); i++) {
            if (items_[i].separator) continue;
            char hk = items_[i].hotkey;
            if (hk != '\0' && (key == hk || key == static_cast<char>(toupper(hk)))) {
                selectedIndex_ = static_cast<int>(i);
                if (items_[i].enabled && items_[i].action) {
                    items_[i].action();
                }
                if (onSelect_) {
                    onSelect_(selectedIndex_);
                }
                return true;
            }
        }

        return false;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        ColorAttr normalColor = ColorAttr::biosMenu();
        ColorAttr disabledColor = ColorAttr::biosDisabled();
        ColorAttr color = hasFocus_ ? highlightColor_ : normalColor;

        // Р Р°РјРєР°
        screen.drawBox(x_, y_, width_, height_, BoxStyles::ascii(), normalColor);

        // Р РёСЃСѓРµРј СЌР»РµРјРµРЅС‚С‹ РјРµРЅСЋ
        for (size_t i = 0; i < items_.size(); i++) {
            const MenuItem& item = items_[i];
            bool isSelected = (static_cast<int>(i) == selectedIndex_);

            if (item.separator) {
                // Р Р°Р·РґРµР»РёС‚РµР»СЊ
                screen.drawHLine(x_ + 1, y_ + 1 + static_cast<int>(i), width_ - 2, 
                                Symbols::separatorH, ColorAttr::normal());
                continue;
            }

            // Р¤РѕСЂРјРёСЂСѓРµРј СЃС‚СЂРѕРєСѓ
            std::string line;
            
            if (item.hotkey != '\0' && !isSelected) {
                // РџРѕРєР°Р·С‹РІР°РµРј РіРѕСЂСЏС‡СѓСЋ РєР»Р°РІРёС€Сѓ
                line = "  ";
                line += static_cast<char>(toupper(item.hotkey));
                line += " ";
                line += item.label;
            } else {
                line = " " + item.label;
            }

            // РџРѕР·РёС†РёРѕРЅРёСЂСѓРµРј Рё СЂРёСЃСѓРµРј
            if (isSelected && hasFocus_) {
                // Р’С‹РґРµР»РµРЅРЅС‹Р№ СЌР»РµРјРµРЅС‚
                line = Symbols::arrowRight + line.substr(1);
                screen.fillRect(x_ + 1, y_ + 1 + static_cast<int>(i), width_ - 2, 1, ' ', highlightColor_);
                screen.putString(x_ + 1, y_ + 1 + static_cast<int>(i), line.c_str(), highlightColor_);
            } else {
                // РћР±С‹С‡РЅС‹Р№ СЌР»РµРјРµРЅС‚
                ColorAttr itemColor = item.enabled ? normalColor : disabledColor;
                screen.putString(x_ + 2, y_ + 1 + static_cast<int>(i), line.c_str(), itemColor);
            }
        }
    }
};

} // namespace ui

#endif // TEXTUI_MENU_H
