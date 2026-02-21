#ifndef TEXTUI_CHECKBOX_H
#define TEXTUI_CHECKBOX_H

#include "Widget.h"
#include "../core/Screen.h"
#include <functional>

namespace ui {

/**
 * @brief Р§РµРєР±РѕРєСЃ (С„Р»Р°Р¶РѕРє)
 */
class CheckBox : public Widget {
private:
    bool checked_ = false;
    bool hasFocus_ = false;
    std::function<void()> onToggle_;

public:
    CheckBox(int x, int y, const std::string& text, bool checked = false)
        : Widget(x, y, static_cast<int>(text.length()) + 4, 1) {
        text_ = text;
        checked_ = checked;
        canFocus_ = true;
    }

    void setOnToggle(std::function<void()> callback) {
        onToggle_ = callback;
    }

    bool isChecked() const { return checked_; }

    void setChecked(bool checked) {
        checked_ = checked;
    }

    bool hasFocus() const { return hasFocus_; }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
    }

    void toggle() {
        if (!enabled_) return;
        checked_ = !checked_;
        if (onToggle_) onToggle_();
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_) return false;

        if (key == Key::Space || key == Key::Enter) {
            toggle();
            return true;
        }
        return false;
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;
        if (hotkey_ != '\0' && (key == hotkey_ || key == static_cast<char>(toupper(hotkey_)))) {
            setFocus(true);
            toggle();
            return true;
        }
        return false;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        ColorAttr normalColor = enabled_ ? ColorAttr::normal() : ColorAttr::biosDisabled();
        ColorAttr focusColor = enabled_ ? ColorAttr::highlight() : ColorAttr::biosDisabled();
        ColorAttr color = hasFocus_ ? focusColor : normalColor;

        // Р РёСЃСѓРµРј [X] РёР»Рё [ ]
        const char* checkStr = checked_ ? Symbols::checkboxOn : Symbols::checkboxOff;
        
        // Р“РѕСЂСЏС‡Р°СЏ РєР»Р°РІРёС€Р°
        if (hotkey_ != '\0' && !hasFocus_) {
            std::string hotkeyText = "[";
            hotkeyText += static_cast<char>(toupper(hotkey_));
            hotkeyText += "]";
            screen.putString(x_, y_, hotkeyText.c_str(), TextStyle::biosHotkey());
            screen.putString(x_ + 3, y_, checkStr, color);
            screen.putString(x_ + 7, y_, text_.c_str(), color);
        } else {
            screen.putString(x_, y_, checkStr, color);
            screen.putString(x_ + 4, y_, text_.c_str(), color);
        }

        // РРЅРґРёРєР°С‚РѕСЂ С„РѕРєСѓСЃР°
        if (hasFocus_) {
            screen.putString(x_ - 1, y_, Symbols::arrowRight, color);
        }
    }
};

} // namespace ui

#endif // TEXTUI_CHECKBOX_H
