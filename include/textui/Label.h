#ifndef TEXTUI_LABEL_H
#define TEXTUI_LABEL_H

#include "Widget.h"
#include "../core/Screen.h"

namespace ui {

/**
 * @brief Р’С‹СЂР°РІРЅРёРІР°РЅРёРµ С‚РµРєСЃС‚Р°
 */
enum class TextAlign {
    Left,
    Center,
    Right
};

/**
 * @brief РўРµРєСЃС‚РѕРІР°СЏ РјРµС‚РєР°
 */
class Label : public Widget {
private:
    TextAlign align_ = TextAlign::Left;
    bool autoWidth_ = true;

public:
    Label(int x, int y, const std::string& text, const TextStyle& style = TextStyle::normal())
        : Widget(x, y, static_cast<int>(text.length()), 1) {
        text_ = text;
        style_ = style;
        canFocus_ = false;
    }

    Label(int x, int y, int width, const std::string& text, TextAlign align = TextAlign::Left)
        : Widget(x, y, width, 1) {
        text_ = text;
        align_ = align;
        autoWidth_ = false;
        canFocus_ = false;
    }

    Label(int x, int y, const std::string& text, TextAlign align)
        : Widget(x, y, 1, 1) {
        text_ = text;
        align_ = align;
        autoWidth_ = false;
        canFocus_ = false;
    }

    void setText(const std::string& text) {
        text_ = text;
        if (autoWidth_) {
            width_ = static_cast<int>(text.length());
        }
    }

    void setAlign(TextAlign align) { align_ = align; }
    TextAlign getAlign() const { return align_; }

    void setAutoWidth(bool autoW) {
        autoWidth_ = autoW;
        if (autoWidth_) {
            width_ = static_cast<int>(text_.length());
        }
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        int drawX = x_;
        
        // Р’С‹СЂР°РІРЅРёРІР°РЅРёРµ
        if (align_ == TextAlign::Center) {
            drawX = x_ + (width_ - static_cast<int>(text_.length())) / 2;
        } else if (align_ == TextAlign::Right) {
            drawX = x_ + width_ - static_cast<int>(text_.length());
        }

        // РџСЂРёРјРµРЅСЏРµРј СЃС‚РёР»СЊ
        TextStyle drawStyle = style_;
        if (!enabled_) {
            drawStyle = TextStyle::biosDisabled();
        }

        screen.putString(drawX, y_, text_.c_str(), drawStyle);
    }
};

} // namespace ui

#endif // TEXTUI_LABEL_H
