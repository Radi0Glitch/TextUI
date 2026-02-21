#ifndef TEXTUI_RADIOBUTTON_H
#define TEXTUI_RADIOBUTTON_H

#include "Widget.h"
#include "../core/Screen.h"
#include <functional>
#include <vector>
#include <memory>

namespace ui {

/**
 * @brief Группа радио-кнопок (только одна может быть выбрана)
 */
class RadioButtonGroup {
private:
    std::vector<class RadioButton*> buttons_;
    int selectedIndex_ = -1;
    std::function<void(int)> onChange_;

public:
    RadioButtonGroup() = default;

    void add(class RadioButton* btn);

    int getSelectedIndex() const { return selectedIndex_; }

    void setSelected(int index);

    void setOnChange(std::function<void(int)> callback) {
        onChange_ = callback;
    }

    int getCount() const { return static_cast<int>(buttons_.size()); }
    class RadioButton* getButton(int index) { return (index >= 0 && index < static_cast<int>(buttons_.size())) ? buttons_[index] : nullptr; }
};

/**
 * @brief Радио-кнопка (переключатель)
 */
class RadioButton : public Widget {
private:
    bool checked_ = false;
    bool hasFocus_ = false;
    std::function<void()> onChange_;
    RadioButtonGroup* group_ = nullptr;
    int groupIndex_ = -1;

public:
    RadioButton(int x, int y, const std::string& text, bool checked = false)
        : Widget(x, y, static_cast<int>(text.length()) + 4, 1) {
        text_ = text;
        checked_ = checked;
        canFocus_ = true;
    }

    void setGroup(RadioButtonGroup* group, int index) {
        group_ = group;
        groupIndex_ = index;
    }

    void setOnChange(std::function<void()> callback) {
        onChange_ = callback;
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
        
        if (group_) {
            group_->setSelected(groupIndex_);
        } else {
            checked_ = !checked_;
        }
        if (onChange_) onChange_();
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_) return false;

        if (key == Key::Space || key == Key::Enter) {
            toggle();
            return true;
        }
        
        // Навигация стрелками в группе
        if (group_ && key == Key::Down) {
            int next = groupIndex_ + 1;
            if (next >= group_->getCount()) next = 0;
            group_->setSelected(next);
            return true;
        }
        if (group_ && key == Key::Up) {
            int prev = groupIndex_ - 1;
            if (prev < 0) prev = group_->getCount() - 1;
            group_->setSelected(prev);
            return true;
        }
        
        return false;
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;
        if (hotkey_ != '\0' && (key == hotkey_ || key == static_cast<char>(toupper(hotkey_)))) {
            setFocused(true);
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

        // Рисуем ( ) или (•)
        const char* radioStr = checked_ ? Symbols::radioOn : Symbols::radioOff;
        
        // Горячая клавиша
        if (hotkey_ != '\0' && !hasFocus_) {
            std::string hotkeyText = "[";
            hotkeyText += static_cast<char>(toupper(hotkey_));
            hotkeyText += "]";
            screen.putString(x_, y_, hotkeyText.c_str(), TextStyle::biosHotkey());
            screen.putString(x_ + 3, y_, radioStr, color);
            screen.putString(x_ + 7, y_, text_.c_str(), color);
        } else {
            screen.putString(x_, y_, radioStr, color);
            screen.putString(x_ + 4, y_, text_.c_str(), color);
        }

        // Индикатор фокуса
        if (hasFocus_) {
            screen.putString(x_ - 1, y_, Symbols::arrowRight, color);
        }
    }
    
    friend class RadioButtonGroup;
};

// Реализация методов RadioButtonGroup
inline void RadioButtonGroup::add(RadioButton* btn) {
    int index = static_cast<int>(buttons_.size());
    buttons_.push_back(btn);
    btn->group_ = this;
    btn->groupIndex_ = index;
    btn->setOnChange([this, index]() {
        setSelected(index);
    });
}

inline void RadioButtonGroup::setSelected(int index) {
    if (index < 0 || index >= static_cast<int>(buttons_.size())) return;
    if (selectedIndex_ == index) return;

    selectedIndex_ = index;
    for (size_t i = 0; i < buttons_.size(); i++) {
        buttons_[i]->checked_ = (i == static_cast<size_t>(index));
    }
    if (onChange_) onChange_(index);
}

} // namespace ui

#endif // TEXTUI_RADIOBUTTON_H
