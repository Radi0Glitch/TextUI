#ifndef TEXTUI_DROPDOWN_H
#define TEXTUI_DROPDOWN_H

#include "Widget.h"
#include "../core/Screen.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

/**
 * @brief Р’С‹РїР°РґР°СЋС‰РёР№ СЃРїРёСЃРѕРє
 */
class DropDown : public Widget {
private:
    std::vector<std::string> items_;
    int selectedIndex_ = -1;
    bool expanded_ = false;
    bool hasFocus_ = false;
    int maxVisibleItems_ = 5;
    std::function<void(int)> onSelect_;

public:
    DropDown(int x, int y, int width)
        : Widget(x, y, width, 1) {
        canFocus_ = true;
    }

    void setMaxVisibleItems(int count) {
        maxVisibleItems_ = count;
    }

    // Р”РѕР±Р°РІР»РµРЅРёРµ СЌР»РµРјРµРЅС‚Р°
    void addItem(const std::string& item) {
        items_.push_back(item);
        if (selectedIndex_ < 0) selectedIndex_ = 0;
    }

    void clearItems() {
        items_.clear();
        selectedIndex_ = -1;
    }

    int getSelectedIndex() const { return selectedIndex_; }

    void setSelectedIndex(int index) {
        if (index >= 0 && index < static_cast<int>(items_.size())) {
            selectedIndex_ = index;
        }
    }

    const std::string& getSelectedItem() const {
        static std::string empty;
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(items_.size())) {
            return items_[selectedIndex_];
        }
        return empty;
    }

    void setOnSelect(std::function<void(int)> callback) {
        onSelect_ = callback;
    }

    bool hasFocus() const { return hasFocus_; }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
        if (!focus) {
            expanded_ = false;
        }
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_) return false;

        if (expanded_) {
            // РќР°РІРёРіР°С†РёСЏ РІ СЂР°СЃРєСЂС‹С‚РѕРј СЃРїРёСЃРєРµ
            switch (key) {
                case Key::Up:
                    if (selectedIndex_ > 0) {
                        selectedIndex_--;
                    }
                    return true;

                case Key::Down:
                    if (selectedIndex_ < static_cast<int>(items_.size()) - 1) {
                        selectedIndex_++;
                    }
                    return true;

                case Key::Enter:
                case Key::Space:
                    expanded_ = false;
                    if (onSelect_) onSelect_(selectedIndex_);
                    return true;

                case Key::Escape:
                    expanded_ = false;
                    return true;

                default:
                    return false;
            }
        } else {
            // РЎРІС‘СЂРЅСѓС‚С‹Р№ СЃРїРёСЃРѕРє
            switch (key) {
                case Key::Enter:
                case Key::Space:
                    if (!items_.empty()) {
                        expanded_ = true;
                    }
                    return true;

                case Key::Up:
                    if (selectedIndex_ > 0) {
                        selectedIndex_--;
                        if (onSelect_) onSelect_(selectedIndex_);
                    }
                    return true;

                case Key::Down:
                    if (selectedIndex_ < static_cast<int>(items_.size()) - 1) {
                        selectedIndex_++;
                        if (onSelect_) onSelect_(selectedIndex_);
                    }
                    return true;

                default:
                    return false;
            }
        }
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;
        if (hotkey_ != '\0' && (key == hotkey_ || key == static_cast<char>(toupper(hotkey_)))) {
            setFocus(true);
            if (!expanded_ && !items_.empty()) {
                expanded_ = true;
            }
            return true;
        }
        return false;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        ColorAttr normalColor = enabled_ ? ColorAttr::normal() : ColorAttr::biosDisabled();
        ColorAttr focusColor = enabled_ ? ColorAttr::highlight() : ColorAttr::biosDisabled();
        ColorAttr color = hasFocus_ ? focusColor : normalColor;

        // Р Р°РјРєР° РєРѕРЅС‚СЂРѕР»Р°
        screen.drawBox(x_, y_, width_, 3, BoxStyles::ascii(), color);

        // РўРµРєСѓС‰РµРµ Р·РЅР°С‡РµРЅРёРµ
        std::string display;
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(items_.size())) {
            display = " " + items_[selectedIndex_];
        } else {
            display = " (none)";
        }

        // РћР±СЂРµР·Р°РµРј РµСЃР»Рё РЅРµ РїРѕРјРµС‰Р°РµС‚СЃСЏ
        int maxLen = width_ - 4;  // РЈС‡РёС‚С‹РІР°РµРј СЂР°РјРєСѓ Рё СЃС‚СЂРµР»РєСѓ
        if (static_cast<int>(display.length()) > maxLen) {
            display = display.substr(0, maxLen - 1) + ">";
        }

        screen.putString(x_ + 1, y_ + 1, display.c_str(), color);

        // РЎС‚СЂРµР»РєР°
        const char* arrow = expanded_ ? Symbols::arrowUp : Symbols::arrowDown;
        screen.putString(x_ + width_ - 2, y_ + 1, arrow, color);

        // Р“РѕСЂСЏС‡Р°СЏ РєР»Р°РІРёС€Р°
        if (hotkey_ != '\0' && !hasFocus_) {
            std::string hotkeyText = "[";
            hotkeyText += static_cast<char>(toupper(hotkey_));
            hotkeyText += "]";
            screen.putString(x_, y_, hotkeyText.c_str(), TextStyle::biosHotkey());
        }

        // Р Р°СЃРєСЂС‹С‚С‹Р№ СЃРїРёСЃРѕРє
        if (expanded_) {
            int visibleItems = std::min(static_cast<int>(items_.size()), maxVisibleItems_);
            int listHeight = visibleItems + 2;
            int listY = y_ + 2;

            // Р Р°РјРєР° СЃРїРёСЃРєР°
            screen.drawBox(x_, listY, width_, listHeight, BoxStyles::ascii(), color);

            // Р­Р»РµРјРµРЅС‚С‹
            for (int i = 0; i < visibleItems; i++) {
                bool isSelected = (i == selectedIndex_);
                ColorAttr itemColor = isSelected ? focusColor : normalColor;
                
                std::string itemText = " " + items_[i];
                if (isSelected) {
                    itemText = Symbols::arrowRight + items_[i];
                }
                
                screen.putString(x_ + 1, listY + 1 + i, itemText.c_str(), itemColor);
            }
        }
    }
};

} // namespace ui

#endif // TEXTUI_DROPDOWN_H
