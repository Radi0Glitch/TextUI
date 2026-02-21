#ifndef TEXTUI_MESSAGEBOX_H
#define TEXTUI_MESSAGEBOX_H

#include "Widget.h"
#include "../core/Screen.h"
#include "../widgets/Button.h"
#include "../widgets/Label.h"
#include <string>
#include <functional>
#include <vector>

namespace ui {

/**
 * @brief РўРёРїС‹ РёРєРѕРЅРѕРє РґР»СЏ MessageBox
 */
enum class MessageBoxIcon {
    None,
    Info,
    Warning,
    Error,
    Question
};

/**
 * @brief РўРёРїС‹ РєРЅРѕРїРѕРє РґР»СЏ MessageBox
 */
enum class MessageBoxButtons {
    OK,
    OKCancel,
    YesNo,
    YesNoCancel,
    AbortRetryIgnore
};

/**
 * @brief Р РµР·СѓР»СЊС‚Р°С‚С‹ MessageBox
 */
enum class MessageBoxResult {
    None = 0,
    OK = 1,
    Cancel = 2,
    Yes = 3,
    No = 4,
    Abort = 5,
    Retry = 6,
    Ignore = 7
};

/**
 * @brief Р”РёР°Р»РѕРіРѕРІРѕРµ РѕРєРЅРѕ СЃРѕРѕР±С‰РµРЅРёСЏ
 */
class MessageBox : public Widget {
private:
    std::string title_;
    std::string message_;
    MessageBoxIcon icon_;
    MessageBoxButtons buttons_;
    MessageBoxResult result_;
    int selectedButton_ = 0;
    std::vector<std::string> buttonLabels_;
    std::function<void(MessageBoxResult)> onResult_;
    bool centered_ = true;

    // РРєРѕРЅРєРё (ASCII)
    static const char* iconInfo() { return "[i]"; }
    static const char* iconWarning() { return "[!]"; }
    static const char* iconError() { return "[X]"; }
    static const char* iconQuestion() { return "[?]"; }

    // РџРѕР»СѓС‡РµРЅРёРµ РєРЅРѕРїРѕРє
    std::vector<std::string> getButtonLabels() const {
        switch (buttons_) {
            case MessageBoxButtons::OK:
                return {"OK"};
            case MessageBoxButtons::OKCancel:
                return {"OK", "Cancel"};
            case MessageBoxButtons::YesNo:
                return {"Yes", "No"};
            case MessageBoxButtons::YesNoCancel:
                return {"Yes", "No", "Cancel"};
            case MessageBoxButtons::AbortRetryIgnore:
                return {"Abort", "Retry", "Ignore"};
            default:
                return {"OK"};
        }
    }

    int getButtonCount() const {
        return static_cast<int>(buttonLabels_.size());
    }

public:
    MessageBox(int x, int y, int w, const std::string& title, const std::string& message,
               MessageBoxIcon icon = MessageBoxIcon::None,
               MessageBoxButtons buttons = MessageBoxButtons::OK)
        : Widget(x, y, w, 1)
        , title_(title)
        , message_(message)
        , icon_(icon)
        , buttons_(buttons)
        , result_(MessageBoxResult::None) {
        buttonLabels_ = getButtonLabels();
        calculateHeight();
        canFocus_ = true;
        modal_ = true;
    }

    // РЎС‚Р°С‚РёС‡РµСЃРєРёР№ РјРµС‚РѕРґ РґР»СЏ СЃРѕР·РґР°РЅРёСЏ С†РµРЅС‚СЂРёСЂРѕРІР°РЅРЅРѕРіРѕ MessageBox
    static MessageBox createCentered(Screen& screen, 
                                     const std::string& title, 
                                     const std::string& message,
                                     MessageBoxIcon icon = MessageBoxIcon::None,
                                     MessageBoxButtons buttons = MessageBoxButtons::OK) {
        int screenWidth = screen.getWidth();
        int msgWidth = 50;
        
        // РЈС‡РёС‚С‹РІР°РµРј РґР»РёРЅСѓ СЃРѕРѕР±С‰РµРЅРёСЏ
        for (const auto& line : splitLines(message, msgWidth - 4)) {
            if (static_cast<int>(line.length()) + 4 > msgWidth) {
                msgWidth = static_cast<int>(line.length()) + 4;
            }
        }
        if (msgWidth > screenWidth - 4) msgWidth = screenWidth - 4;
        
        int x = (screenWidth - msgWidth) / 2;
        int y = screen.getHeight() / 2 - 3;
        
        MessageBox box(x, y, msgWidth, title, message, icon, buttons);
        box.centered_ = true;
        return box;
    }

    // Р Р°Р·Р±РёРµРЅРёРµ С‚РµРєСЃС‚Р° РЅР° СЃС‚СЂРѕРєРё
    static std::vector<std::string> splitLines(const std::string& text, int maxWidth) {
        std::vector<std::string> lines;
        std::string current;
        
        for (char c : text) {
            if (c == '\n') {
                lines.push_back(current);
                current.clear();
            } else {
                current += c;
                if (static_cast<int>(current.length()) >= maxWidth) {
                    lines.push_back(current);
                    current.clear();
                }
            }
        }
        if (!current.empty()) {
            lines.push_back(current);
        }
        return lines;
    }

    void calculateHeight() {
        int textWidth = width_ - 6;  // РЈС‡РёС‚С‹РІР°РµРј СЂР°РјРєРё
        auto lines = splitLines(message_, textWidth);
        int textHeight = static_cast<int>(lines.size());
        
        // 1 (Р·Р°РіРѕР»РѕРІРѕРє) + 1 (РІРµСЂС…РЅСЏСЏ СЂР°РјРєР°) + icon(1) + text + buttons(1) + 1 (РЅРёР¶РЅСЏСЏ СЂР°РјРєР°)
        height_ = 3 + textHeight + 1 + 2;
    }

    void setOnResult(std::function<void(MessageBoxResult)> callback) {
        onResult_ = callback;
    }

    MessageBoxResult getResult() const { return result_; }

    bool handleKey(Key key) override {
        if (!visible_) return false;

        switch (key) {
            case Key::Left:
            case Key::Up:
                selectedButton_--;
                if (selectedButton_ < 0) selectedButton_ = getButtonCount() - 1;
                return true;

            case Key::Right:
            case Key::Down:
                selectedButton_++;
                if (selectedButton_ >= getButtonCount()) selectedButton_ = 0;
                return true;

            case Key::Enter:
            case Key::Space:
                activateSelected();
                return true;

            case Key::Escape:
                // РћС‚РјРµРЅР°, РµСЃР»Рё РµСЃС‚СЊ РєРЅРѕРїРєР° Cancel
                for (int i = 0; i < getButtonCount(); i++) {
                    if (buttonLabels_[i] == "Cancel") {
                        selectedButton_ = i;
                        activateSelected();
                        return true;
                    }
                }
                return true;

            default:
                return false;
        }
    }

    void activateSelected() {
        if (selectedButton_ < 0 || selectedButton_ >= getButtonCount()) return;

        const std::string& label = buttonLabels_[selectedButton_];
        
        if (label == "OK") result_ = MessageBoxResult::OK;
        else if (label == "Cancel") result_ = MessageBoxResult::Cancel;
        else if (label == "Yes") result_ = MessageBoxResult::Yes;
        else if (label == "No") result_ = MessageBoxResult::No;
        else if (label == "Abort") result_ = MessageBoxResult::Abort;
        else if (label == "Retry") result_ = MessageBoxResult::Retry;
        else if (label == "Ignore") result_ = MessageBoxResult::Ignore;

        if (onResult_) onResult_(result_);
        visible_ = false;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        // Р¤РѕРЅ РґРёР°Р»РѕРіР°
        ColorAttr bgColor = ColorAttr(Color8::White, Color8::Blue);
        ColorAttr textColor = ColorAttr(Color8::BrightWhite, Color8::Blue);
        ColorAttr buttonColor = ColorAttr(Color8::Black, Color8::Gray);
        ColorAttr buttonSelectedColor = ColorAttr(Color8::BrightWhite, Color8::Blue);

        // Р РёСЃСѓРµРј СЂР°РјРєСѓ
        screen.drawBox(x_, y_, width_, height_, BoxStyles::doubleLine(), textColor);

        // Р—Р°РіРѕР»РѕРІРѕРє
        std::string title = " " + title_ + " ";
        screen.putString(x_ + 2, y_, title.c_str(), TextStyle::biosTitle());

        // РРєРѕРЅРєР°
        const char* iconStr = "";
        int iconX = x_ + 2;
        int textStartX = x_ + 2;
        
        switch (icon_) {
            case MessageBoxIcon::Info:
                iconStr = iconInfo();
                screen.putString(iconX, y_ + 2, iconStr, ColorAttr(Color8::BrightCyan, Color8::Blue));
                textStartX = iconX + 5;
                break;
            case MessageBoxIcon::Warning:
                iconStr = iconWarning();
                screen.putString(iconX, y_ + 2, iconStr, ColorAttr(Color8::BrightYellow, Color8::Blue));
                textStartX = iconX + 5;
                break;
            case MessageBoxIcon::Error:
                iconStr = iconError();
                screen.putString(iconX, y_ + 2, iconStr, ColorAttr(Color8::BrightRed, Color8::Blue));
                textStartX = iconX + 5;
                break;
            case MessageBoxIcon::Question:
                iconStr = iconQuestion();
                screen.putString(iconX, y_ + 2, iconStr, ColorAttr(Color8::BrightCyan, Color8::Blue));
                textStartX = iconX + 5;
                break;
            default:
                break;
        }

        // РўРµРєСЃС‚ СЃРѕРѕР±С‰РµРЅРёСЏ
        auto lines = splitLines(message_, width_ - 6);
        int textY = y_ + 2;
        if (icon_ != MessageBoxIcon::None) textY = y_ + 2;
        
        for (size_t i = 0; i < lines.size(); i++) {
            screen.putString(x_ + 2, textY + static_cast<int>(i), lines[i].c_str(), textColor);
        }

        // РљРЅРѕРїРєРё
        int buttonY = y_ + height_ - 2;
        int totalButtonsWidth = 0;
        for (const auto& label : buttonLabels_) {
            totalButtonsWidth += static_cast<int>(label.length()) + 4;  // [ Label ]
        }
        int buttonX = x_ + (width_ - totalButtonsWidth) / 2;

        for (int i = 0; i < getButtonCount(); i++) {
            const std::string& label = buttonLabels_[i];
            bool isSelected = (i == selectedButton_);
            
            std::string btnText = " " + label + " ";
            ColorAttr btnColor = isSelected ? buttonSelectedColor : buttonColor;
            
            screen.drawBox(buttonX, buttonY, static_cast<int>(btnText.length()) + 2, 3, 
                          BoxStyles::thin(), btnColor);
            screen.putString(buttonX + 1, buttonY + 1, btnText.c_str(), btnColor);

            buttonX += static_cast<int>(btnText.length()) + 4;
        }
    }

private:
    bool modal_ = false;
};

} // namespace ui

#endif // TEXTUI_MESSAGEBOX_H
