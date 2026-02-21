#ifndef TEXTUI_BUTTON_H
#define TEXTUI_BUTTON_H

#include "Widget.h"
#include "../core/Screen.h"
#include <functional>

namespace ui {

/**
 * @brief РљРЅРѕРїРєР° СЃ РїРѕРґРґРµСЂР¶РєРѕР№ РіРѕСЂСЏС‡РёС… РєР»Р°РІРёС€
 */
class Button : public Widget {
private:
    std::function<void()> onClick_;
    bool hasFocus_ = false;
    int minWidth_ = 0;

public:
    Button(int x, int y, int width, const std::string& text, std::function<void()> onClick = nullptr)
        : Widget(x, y, width, 3) {
        text_ = text;
        onClick_ = onClick;
        canFocus_ = true;
    }

    void setOnClick(std::function<void()> callback) {
        onClick_ = callback;
    }

    void setMinWidth(int w) { minWidth_ = w; }

    bool hasFocus() const { return hasFocus_; }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_) return false;
        
        if (key == Key::Enter || key == Key::Space) {
            if (onClick_) onClick_();
            return true;
        }
        return false;
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;
        if (hotkey_ != '\0' && (key == hotkey_ || key == static_cast<char>(toupper(hotkey_)))) {
            setFocus(true);
            if (onClick_) onClick_();
            return true;
        }
        return false;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        ColorAttr normalColor = enabled_ ? ColorAttr::normal() : ColorAttr::biosDisabled();
        ColorAttr focusColor = enabled_ ? ColorAttr::highlight() : ColorAttr::biosDisabled();
        ColorAttr color = hasFocus_ ? focusColor : normalColor;

        // Р Р°РјРєР°
        screen.drawBox(x_, y_, width_, height_, BoxStyles::thin(), color);

        // РўРµРєСЃС‚ РєРЅРѕРїРєРё
        std::string btnText;
        
        // Р“РѕСЂСЏС‡Р°СЏ РєР»Р°РІРёС€Р°
        if (hotkey_ != '\0') {
            // Р¤РѕСЂРјР°С‚: [X]Text РёР»Рё [X] Text
            std::string displayText = text_;
            size_t hotkeyPos = displayText.find(hotkey_);
            if (hotkeyPos == std::string::npos) {
                hotkeyPos = displayText.find(static_cast<char>(toupper(hotkey_)));
            }
            
            if (hotkeyPos != std::string::npos && !hasFocus_) {
                // РџРѕРєР°Р·С‹РІР°РµРј РіРѕСЂСЏС‡СѓСЋ РєР»Р°РІРёС€Сѓ РѕС‚РґРµР»СЊРЅРѕ
                screen.putString(x_ + 1, y_, "[", TextStyle::biosHotkey());
                screen.putString(x_ + 2, y_, std::string(1, static_cast<char>(toupper(hotkey_))).c_str(), TextStyle::biosHotkey());
                screen.putString(x_ + 3, y_, "]", TextStyle::biosHotkey());
                btnText = " " + text_ + " ";
            } else {
                btnText = " " + text_ + " ";
            }
        } else {
            btnText = " " + text_ + " ";
        }

        // Р¦РµРЅС‚СЂРёСЂРѕРІР°РЅРёРµ С‚РµРєСЃС‚Р°
        int textWidth = static_cast<int>(btnText.length());
        int textX = x_ + (width_ - textWidth) / 2;
        if (textX < x_ + 1) textX = x_ + 1;
        
        screen.putString(textX, y_ + 1, btnText.c_str(), color);

        // РРЅРґРёРєР°С‚РѕСЂ С„РѕРєСѓСЃР°
        if (hasFocus_) {
            screen.putString(x_ + 1, y_ + 1, Symbols::cursorBlock, color);
        }
    }
};

} // namespace ui

#endif // TEXTUI_BUTTON_H
