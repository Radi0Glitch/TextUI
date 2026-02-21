#ifndef TEXTUI_WIDGET_H
#define TEXTUI_WIDGET_H

#include "../graphics/Colors.h"
#include "../graphics/Chars.h"
#include "../core/Screen.h"
#include "../core/Input.h"  // Р’РєР»СЋС‡Р°РµРј Input.h РґР»СЏ Key
#include <string>
#include <functional>
#include <memory>

namespace ui {

/**
 * @brief Р‘Р°Р·РѕРІС‹Р№ РєР»Р°СЃСЃ РІСЃРµС… РІРёРґР¶РµС‚РѕРІ
 */
class Widget {
protected:
    int x_ = 0;
    int y_ = 0;
    int width_ = 0;
    int height_ = 0;
    bool visible_ = true;
    bool enabled_ = true;
    bool focused_ = false;
    bool canFocus_ = true;
    std::string text_;
    TextStyle style_;
    std::string tooltip_;
    char hotkey_ = '\0';  // Р“РѕСЂСЏС‡Р°СЏ РєР»Р°РІРёС€Р° (Alt+X)

public:
    Widget(int x, int y, int w, int h)
        : x_(x), y_(y), width_(w), height_(h) {}

    virtual ~Widget() = default;

    // Р“РµС‚С‚РµСЂС‹
    int x() const { return x_; }
    int y() const { return y_; }
    int width() const { return width_; }
    int height() const { return height_; }
    bool visible() const { return visible_; }
    bool enabled() const { return enabled_; }
    bool focused() const { return focused_; }
    bool canFocus() const { return canFocus_; }
    const std::string& text() const { return text_; }
    const std::string& tooltip() const { return tooltip_; }
    char hotkey() const { return hotkey_; }

    // РЎРµС‚С‚РµСЂС‹
    void setPosition(int x, int y) { x_ = x; y_ = y; }
    void setSize(int w, int h) { width_ = w; height_ = h; }
    void setVisible(bool v) { visible_ = v; }
    void setEnabled(bool v) { enabled_ = v; }
    virtual void setFocused(bool v) { focused_ = v; }
    void setFocus(bool v) { focused_ = v; }  // РђР»РёР°СЃ РґР»СЏ СЃРѕРІРјРµСЃС‚РёРјРѕСЃС‚Рё
    void setCanFocus(bool v) { canFocus_ = v; }
    void setText(const std::string& t) { text_ = t; }
    void setTooltip(const std::string& t) { tooltip_ = t; }
    void setHotkey(char key) { hotkey_ = key; }
    void setStyle(const TextStyle& s) { style_ = s; }

    // РџСЂРѕРІРµСЂРєР° РїРѕРїР°РґР°РЅРёСЏ С‚РѕС‡РєРё
    virtual bool contains(int px, int py) const {
        return visible_ && enabled_ && 
               px >= x_ && px < x_ + width_ && 
               py >= y_ && py < y_ + height_;
    }

    // РћР±СЂР°Р±РѕС‚РєР° РєР»Р°РІРёР°С‚СѓСЂС‹
    virtual bool handleKey(Key key) { 
        (void)key; 
        return false; 
    }
    
    // РћР±СЂР°Р±РѕС‚РєР° РіРѕСЂСЏС‡РёС… РєР»Р°РІРёС€
    virtual bool handleHotkey(char key) {
        (void)key;
        return false;
    }

    // РћС‚СЂРёСЃРѕРІРєР°
    virtual void draw(Screen& screen) = 0;
    
    // РџРѕР»СѓС‡РёС‚СЊ РїСЂРµРґРїРѕС‡С‚РёС‚РµР»СЊРЅСѓСЋ РІС‹СЃРѕС‚Сѓ
    virtual int getPreferredHeight() const { return height_; }
    
    // РџРѕР»СѓС‡РёС‚СЊ РїСЂРµРґРїРѕС‡С‚РёС‚РµР»СЊРЅСѓСЋ С€РёСЂРёРЅСѓ
    virtual int getPreferredWidth() const { return width_; }
};

} // namespace ui

#endif // TEXTUI_WIDGET_H
