#ifndef TEXTUI_TEXTBOX_H
#define TEXTUI_TEXTBOX_H

#include "Widget.h"
#include "../core/Screen.h"
#include <string>
#include <functional>
#include <vector>
#include <algorithm>

namespace ui {

/**
 * @brief Р РµР¶РёРјС‹ РІРІРѕРґР° TextBox
 */
enum class TextBoxMode {
    Normal,       // РћР±С‹С‡РЅС‹Р№ РІРІРѕРґ
    Password,     // РџР°СЂРѕР»СЊ (СЃРёРјРІРѕР»С‹ *)
    Integer,      // РўРѕР»СЊРєРѕ С†РµР»С‹Рµ С‡РёСЃР»Р°
    Float,        // РўРѕР»СЊРєРѕ С‡РёСЃР»Р° СЃ РїР»Р°РІР°СЋС‰РµР№ С‚РѕС‡РєРѕР№
    IPAddress,    // IP Р°РґСЂРµСЃ (xxx.xxx.xxx.xxx)
    ReadOnly      // РўРѕР»СЊРєРѕ С‡С‚РµРЅРёРµ
};

/**
 * @brief РџРѕР»Рµ РІРІРѕРґР° С‚РµРєСЃС‚Р° СЃ СЂР°СЃС€РёСЂРµРЅРЅС‹РјРё РІРѕР·РјРѕР¶РЅРѕСЃС‚СЏРјРё
 */
class TextBox : public Widget {
private:
    std::string text_;
    std::string placeholder_;
    size_t cursorPos_ = 0;
    size_t scrollOffset_ = 0;
    bool hasFocus_ = false;
    TextBoxMode mode_ = TextBoxMode::Normal;
    std::string mask_;  // РњР°СЃРєР° РІРІРѕРґР° (РЅР°РїСЂРёРјРµСЂ "AAA-000")
    std::function<void()> onChange_;
    std::function<void()> onEnter_;
    int maxLength_ = 256;
    bool modified_ = false;
    
    // РСЃС‚РѕСЂРёСЏ РІРІРѕРґР°
    std::vector<std::string> history_;
    int historyIndex_ = -1;
    bool useHistory_ = false;

    // РџРѕР»СѓС‡РµРЅРёРµ РѕС‚РѕР±СЂР°Р¶Р°РµРјРѕРіРѕ С‚РµРєСЃС‚Р°
    std::string getDisplayText() const {
        if (mode_ == TextBoxMode::Password) {
            return std::string(text_.length(), '*');
        }
        return text_;
    }

    // РџСЂРѕРІРµСЂРєР° СЃРёРјРІРѕР»Р° РЅР° РґРѕРїСѓСЃС‚РёРјРѕСЃС‚СЊ
    bool isValidChar(char ch) const {
        switch (mode_) {
            case TextBoxMode::Integer:
                return (ch >= '0' && ch <= '9') || ch == '-';
            case TextBoxMode::Float:
                return (ch >= '0' && ch <= '9') || ch == '.' || ch == '-';
            case TextBoxMode::IPAddress:
                return (ch >= '0' && ch <= '9') || ch == '.';
            case TextBoxMode::ReadOnly:
                return false;
            default:
                return true;
        }
    }

    // РџСЂРѕРІРµСЂРєР° РјР°СЃРєРё
    bool matchesMask(const std::string& str) const {
        if (mask_.empty()) return true;
        if (str.length() != mask_.length()) return false;
        
        for (size_t i = 0; i < mask_.length(); i++) {
            char m = mask_[i];
            char c = str[i];
            
            switch (m) {
                case '0':  // Р¦РёС„СЂР°
                    if (c < '0' || c > '9') return false;
                    break;
                case 'A':  // Р‘СѓРєРІР° (РІРµСЂС…РЅРёР№ СЂРµРіРёСЃС‚СЂ)
                    if (c < 'A' || c > 'Z') return false;
                    break;
                case 'a':  // Р‘СѓРєРІР° (РЅРёР¶РЅРёР№ СЂРµРіРёСЃС‚СЂ)
                    if (c < 'a' || c > 'z') return false;
                    break;
                case '*':  // Р›СЋР±РѕР№ СЃРёРјРІРѕР»
                    break;
                default:   // РўРѕС‡РЅРѕРµ СЃРѕРІРїР°РґРµРЅРёРµ
                    if (c != m) return false;
                    break;
            }
        }
        return true;
    }

public:
    TextBox(int x, int y, int width, const std::string& text = "")
        : Widget(x, y, width, 3) {
        text_ = text;
        cursorPos_ = text.length();
        canFocus_ = true;
    }

    // Р“РµС‚С‚РµСЂС‹ Рё СЃРµС‚С‚РµСЂС‹
    const std::string& getText() const { return text_; }
    
    void setText(const std::string& text) {
        text_ = text;
        if (cursorPos_ > text_.length()) {
            cursorPos_ = text_.length();
        }
        modified_ = false;
    }

    int getIntValue() const {
        try {
            return std::stoi(text_);
        } catch (...) {
            return 0;
        }
    }

    void setIntValue(int value) {
        text_ = std::to_string(value);
        cursorPos_ = text_.length();
    }

    float getFloatValue() const {
        try {
            return std::stof(text_);
        } catch (...) {
            return 0.0f;
        }
    }

    void setFloatValue(float value) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.2f", value);
        text_ = buf;
        cursorPos_ = text_.length();
    }

    void setMode(TextBoxMode mode) { mode_ = mode; }
    TextBoxMode getMode() const { return mode_; }

    void setMask(const std::string& mask) { mask_ = mask; }
    const std::string& getMask() const { return mask_; }

    void setPlaceholder(const std::string& placeholder) { placeholder_ = placeholder; }
    const std::string& getPlaceholder() const { return placeholder_; }

    void setMaxLength(int len) { maxLength_ = len; }
    int getMaxLength() const { return maxLength_; }

    bool isModified() const { return modified_; }
    void setModified(bool m) { modified_ = m; }

    bool hasFocus() const { return hasFocus_; }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
    }

    void setOnChange(std::function<void()> callback) { onChange_ = callback; }
    void setOnEnter(std::function<void()> callback) { onEnter_ = callback; }

    // РСЃС‚РѕСЂРёСЏ РІРІРѕРґР°
    void enableHistory(int maxHistory = 10) {
        history_.clear();
        historyIndex_ = -1;
        useHistory_ = true;
        maxLength_ = maxHistory;
    }

    void addToHistory() {
        if (!useHistory_ || text_.empty()) return;
        
        // РќРµ РґРѕР±Р°РІР»СЏРµРј РґСѓР±Р»РёРєР°С‚С‹
        if (!history_.empty() && history_.back() == text_) return;
        
        history_.push_back(text_);
        if (static_cast<int>(history_.size()) > maxLength_) {
            history_.erase(history_.begin());
        }
        historyIndex_ = static_cast<int>(history_.size());
    }

    void clearHistory() {
        history_.clear();
        historyIndex_ = -1;
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_) return false;

        bool changed = false;

        switch (key) {
            case Key::Backspace:
                if (cursorPos_ > 0) {
                    text_.erase(cursorPos_ - 1, 1);
                    cursorPos_--;
                    changed = true;
                }
                break;

            case Key::Delete:
                if (cursorPos_ < text_.length()) {
                    text_.erase(cursorPos_, 1);
                    changed = true;
                }
                break;

            case Key::Left:
                if (cursorPos_ > 0) {
                    cursorPos_--;
                }
                return true;

            case Key::Right:
                if (cursorPos_ < text_.length()) {
                    cursorPos_++;
                }
                return true;

            case Key::Home:
                cursorPos_ = 0;
                return true;

            case Key::End:
                cursorPos_ = text_.length();
                return true;

            case Key::Enter:
                if (onEnter_) onEnter_();
                if (useHistory_) {
                    addToHistory();
                }
                return true;

            case Key::Escape:
                hasFocus_ = false;
                return true;

            case Key::Up:
                // РСЃС‚РѕСЂРёСЏ РІРІРµСЂС…
                if (useHistory_ && historyIndex_ > 0) {
                    historyIndex_--;
                    text_ = history_[historyIndex_];
                    cursorPos_ = text_.length();
                    changed = true;
                }
                return true;

            case Key::Down:
                // РСЃС‚РѕСЂРёСЏ РІРЅРёР·
                if (useHistory_ && historyIndex_ < static_cast<int>(history_.size()) - 1) {
                    historyIndex_++;
                    text_ = history_[historyIndex_];
                    cursorPos_ = text_.length();
                    changed = true;
                } else if (useHistory_) {
                    historyIndex_ = static_cast<int>(history_.size());
                    text_.clear();
                    cursorPos_ = 0;
                    changed = true;
                }
                return true;

            case Key::Tab:
                // Р’СЃС‚Р°РІРєР° С‚Р°Р±СѓР»СЏС†РёРё (РµСЃР»Рё СЂР°Р·СЂРµС€РµРЅР°)
                if (mode_ == TextBoxMode::Normal) {
                    if (static_cast<int>(text_.length()) < maxLength_) {
                        text_.insert(cursorPos_, "    ");
                        cursorPos_ += 4;
                        changed = true;
                    }
                }
                return true;

            default:
                // Р’РІРѕРґ СЃРёРјРІРѕР»РѕРІ
                if (key >= Key::Space && key < Key::Up) {
                    char ch = static_cast<char>(static_cast<int>(key));
                    
                    if (mode_ != TextBoxMode::ReadOnly && isValidChar(ch)) {
                        if (static_cast<int>(text_.length()) < maxLength_) {
                            // РџСЂРѕРІРµСЂРєР° РјР°СЃРєРё
                            std::string newText = text_;
                            newText.insert(cursorPos_, 1, ch);
                            
                            if (matchesMask(newText)) {
                                text_.insert(cursorPos_, 1, ch);
                                cursorPos_++;
                                changed = true;
                            }
                        }
                    }
                    return true;
                }
                break;
        }

        if (changed) {
            modified_ = true;
            
            // РџСЂРѕРєСЂСѓС‚РєР° РєСѓСЂСЃРѕСЂР°
            int visibleWidth = width_ - 2;
            if (cursorPos_ < scrollOffset_) {
                scrollOffset_ = cursorPos_;
            } else if (cursorPos_ >= scrollOffset_ + static_cast<size_t>(visibleWidth)) {
                scrollOffset_ = cursorPos_ - visibleWidth + 1;
            }

            if (onChange_) onChange_();
        }

        return changed;
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;
        if (hotkey_ != '\0' && (key == hotkey_ || key == static_cast<char>(toupper(hotkey_)))) {
            setFocus(true);
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
        screen.drawBox(x_, y_, width_, height_, BoxStyles::ascii(), color);

        // РћС‚РѕР±СЂР°Р¶Р°РµРјС‹Р№ С‚РµРєСЃС‚
        std::string display = getDisplayText();
        
        // Placeholder РµСЃР»Рё РїСѓСЃС‚Рѕ
        if (display.empty() && !placeholder_.empty() && !hasFocus_) {
            display = placeholder_;
            color = ColorAttr(Color8::Gray, Color8::Black);
        }

        // РџСЂРѕРєСЂСѓС‚РєР°
        std::string visibleText;
        if (display.length() > static_cast<size_t>(width_ - 2)) {
            if (scrollOffset_ < display.length()) {
                visibleText = display.substr(scrollOffset_, width_ - 2);
            }
        } else {
            visibleText = display;
        }

        screen.putString(x_ + 1, y_ + 1, visibleText.c_str(), color);

        // РљСѓСЂСЃРѕСЂ
        if (hasFocus_ && enabled_ && mode_ != TextBoxMode::ReadOnly) {
            int cursorX = x_ + 1 + static_cast<int>(cursorPos_) - static_cast<int>(scrollOffset_);
            if (cursorX >= x_ + 1 && cursorX < x_ + width_ - 1) {
                screen.putString(cursorX, y_ + 1, Symbols::cursorBlock, focusColor);
            }
        }

        // Р“РѕСЂСЏС‡Р°СЏ РєР»Р°РІРёС€Р°
        if (hotkey_ != '\0' && !hasFocus_) {
            std::string hotkeyText = "[";
            hotkeyText += static_cast<char>(toupper(hotkey_));
            hotkeyText += "]";
            screen.putString(x_, y_, hotkeyText.c_str(), TextStyle::biosHotkey());
        }

        // РРЅРґРёРєР°С‚РѕСЂ РјРѕРґРёС„РёРєР°С†РёРё
        if (modified_ && hasFocus_) {
            screen.putString(x_ + width_ - 2, y_, "*", ColorAttr(Color8::BrightRed, Color8::Black));
        }
    }
};

} // namespace ui

#endif // TEXTUI_TEXTBOX_H
