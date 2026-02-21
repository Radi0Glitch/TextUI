#ifndef TEXTUI_TABCONTROL_H
#define TEXTUI_TABCONTROL_H

#include "Widget.h"
#include "../core/Screen.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>

namespace ui {

/**
 * @brief Р’РєР»Р°РґРєР° (СЃС‚СЂР°РЅРёС†Р°) РІ TabControl
 */
class TabPage {
private:
    std::string name_;
    std::vector<std::unique_ptr<Widget>> widgets_;
    char hotkey_ = '\0';
    bool visible_ = false;

public:
    TabPage(const std::string& name) : name_(name) {}

    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    char getHotkey() const { return hotkey_; }
    void setHotkey(char key) { hotkey_ = key; }
    
    bool isVisible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }

    // Р”РѕР±Р°РІР»РµРЅРёРµ РІРёРґР¶РµС‚Р° РЅР° СЃС‚СЂР°РЅРёС†Сѓ
    template<typename T, typename... Args>
    T* addWidget(int x, int y, Args&&... args) {
        auto widget = std::make_unique<T>(x, y, std::forward<Args>(args)...);
        T* ptr = widget.get();
        widgets_.push_back(std::move(widget));
        return ptr;
    }

    // РћС‚СЂРёСЃРѕРІРєР° РІСЃРµС… РІРёРґР¶РµС‚РѕРІ СЃС‚СЂР°РЅРёС†С‹
    void draw(Screen& screen) {
        if (!visible_) return;
        for (auto& widget : widgets_) {
            if (widget->visible()) {
                widget->draw(screen);
            }
        }
    }

    // РћР±СЂР°Р±РѕС‚РєР° РєР»Р°РІРёР°С‚СѓСЂС‹
    bool handleKey(Key key) {
        if (!visible_) return false;
        for (auto& widget : widgets_) {
            if (widget->visible() && widget->focused()) {
                if (widget->handleKey(key)) {
                    return true;
                }
            }
        }
        return false;
    }

    // РћР±СЂР°Р±РѕС‚РєР° РіРѕСЂСЏС‡РёС… РєР»Р°РІРёС€
    bool handleHotkey(char key) {
        if (!visible_) return false;
        for (auto& widget : widgets_) {
            if (widget->visible() && widget->handleHotkey(key)) {
                return true;
            }
        }
        return false;
    }

    // РџРµСЂРµРєР»СЋС‡РµРЅРёРµ С„РѕРєСѓСЃР° РјРµР¶РґСѓ РІРёРґР¶РµС‚Р°РјРё
    void focusNext() {
        if (widgets_.empty()) return;
        
        int current = -1;
        for (size_t i = 0; i < widgets_.size(); i++) {
            if (widgets_[i]->focused() && widgets_[i]->canFocus()) {
                current = static_cast<int>(i);
                widgets_[i]->setFocused(false);
                break;
            }
        }
        
        int next = (current + 1) % static_cast<int>(widgets_.size());
        for (int i = 0; i < static_cast<int>(widgets_.size()); i++) {
            int idx = (next + i) % static_cast<int>(widgets_.size());
            if (widgets_[idx]->canFocus()) {
                widgets_[idx]->setFocused(true);
                break;
            }
        }
    }

    void focusPrev() {
        if (widgets_.empty()) return;
        
        int current = -1;
        for (size_t i = 0; i < widgets_.size(); i++) {
            if (widgets_[i]->focused() && widgets_[i]->canFocus()) {
                current = static_cast<int>(i);
                widgets_[i]->setFocused(false);
                break;
            }
        }
        
        int prev = (current - 1);
        if (prev < 0) prev = static_cast<int>(widgets_.size()) - 1;
        for (int i = 0; i < static_cast<int>(widgets_.size()); i++) {
            int idx = (prev - i + static_cast<int>(widgets_.size())) % static_cast<int>(widgets_.size());
            if (widgets_[idx]->canFocus()) {
                widgets_[idx]->setFocused(true);
                break;
            }
        }
    }

    // РЎРЅСЏС‚СЊ С„РѕРєСѓСЃ СЃРѕ РІСЃРµС… РІРёРґР¶РµС‚РѕРІ
    void clearFocus() {
        for (auto& widget : widgets_) {
            widget->setFocused(false);
        }
    }

    const std::vector<std::unique_ptr<Widget>>& getWidgets() const { return widgets_; }
};

/**
 * @brief РљРѕРЅС‚СЂРѕР» СЃ РІРєР»Р°РґРєР°РјРё
 */
class TabControl : public Widget {
private:
    std::vector<std::unique_ptr<TabPage>> tabs_;
    int selectedIndex_ = 0;
    bool hasFocus_ = false;
    std::function<void(int)> onTabChange_;

public:
    TabControl(int x, int y, int width, int height)
        : Widget(x, y, width, height) {
        canFocus_ = true;
    }

    // Р”РѕР±Р°РІР»РµРЅРёРµ РІРєР»Р°РґРєРё
    TabPage* addTab(const std::string& name) {
        auto tab = std::make_unique<TabPage>(name);
        TabPage* ptr = tab.get();
        tabs_.push_back(std::move(tab));
        if (tabs_.size() == 1) {
            ptr->setVisible(true);
        }
        return ptr;
    }

    int getSelectedIndex() const { return selectedIndex_; }

    void setSelectedIndex(int index) {
        if (index < 0 || index >= static_cast<int>(tabs_.size())) return;
        if (selectedIndex_ == index) return;

        tabs_[selectedIndex_]->setVisible(false);
        tabs_[selectedIndex_]->clearFocus();
        selectedIndex_ = index;
        tabs_[selectedIndex_]->setVisible(true);
        
        if (onTabChange_) onTabChange_(index);
    }

    TabPage* getSelectedTab() {
        return (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(tabs_.size())) 
               ? tabs_[selectedIndex_].get() 
               : nullptr;
    }

    TabPage* getTab(int index) {
        return (index >= 0 && index < static_cast<int>(tabs_.size())) 
               ? tabs_[index].get() 
               : nullptr;
    }

    void setOnTabChange(std::function<void(int)> callback) {
        onTabChange_ = callback;
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_) return false;

        // РџРµСЂРµРґР°С‘Рј РІРІРѕРґ С‚РµРєСѓС‰РµР№ РІРєР»Р°РґРєРµ
        if (hasFocus_ && tabs_[selectedIndex_]->handleKey(key)) {
            return true;
        }

        // РќР°РІРёРіР°С†РёСЏ РјРµР¶РґСѓ РІРєР»Р°РґРєР°РјРё
        if (key == Key::Left || key == Key::Up) {
            int prev = selectedIndex_ - 1;
            if (prev < 0) prev = static_cast<int>(tabs_.size()) - 1;
            setSelectedIndex(prev);
            return true;
        }
        if (key == Key::Right || key == Key::Down) {
            int next = selectedIndex_ + 1;
            if (next >= static_cast<int>(tabs_.size())) next = 0;
            setSelectedIndex(next);
            return true;
        }

        // Tab РІРЅСѓС‚СЂРё РІРєР»Р°РґРєРё
        if (key == Key::Tab && hasFocus_) {
            tabs_[selectedIndex_]->focusNext();
            return true;
        }

        return false;
    }

    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;

        // РџСЂРѕРІРµСЂРєР° РіРѕСЂСЏС‡РёС… РєР»Р°РІРёС€ РІРєР»Р°РґРѕРє
        for (size_t i = 0; i < tabs_.size(); i++) {
            if (tabs_[i]->getHotkey() != '\0') {
                char hk = tabs_[i]->getHotkey();
                if (key == hk || key == static_cast<char>(toupper(hk))) {
                    setSelectedIndex(static_cast<int>(i));
                    return true;
                }
            }
        }

        // Р“РѕСЂСЏС‡РёРµ РєР»Р°РІРёС€Рё РІРЅСѓС‚СЂРё РІРєР»Р°РґРєРё
        if (hasFocus_ && tabs_[selectedIndex_]->handleHotkey(key)) {
            return true;
        }

        return false;
    }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
        if (focus && !tabs_.empty() && !tabs_[selectedIndex_]->getWidgets().empty()) {
            // Р¤РѕРєСѓСЃ РЅР° РїРµСЂРІС‹Р№ РІРёРґР¶РµС‚ С‚РµРєСѓС‰РµР№ РІРєР»Р°РґРєРё
            tabs_[selectedIndex_]->focusNext();
        }
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        // Р РёСЃСѓРµРј Р·Р°РіРѕР»РѕРІРєРё РІРєР»Р°РґРѕРє
        int tabX = x_ + 1;
        int tabY = y_;
        
        for (size_t i = 0; i < tabs_.size(); i++) {
            bool isSelected = (static_cast<int>(i) == selectedIndex_);
            const std::string& name = tabs_[i]->getName();
            char hotkey = tabs_[i]->getHotkey();

            std::string tabText;
            if (hotkey != '\0') {
                tabText = "[";
                tabText += static_cast<char>(toupper(hotkey));
                tabText += "]";
                tabText += name;
            } else {
                tabText = " " + name + " ";
            }

            ColorAttr color;
            if (isSelected) {
                color = ColorAttr::biosSelected();
                // Р РёСЃСѓРµРј Р°РєС‚РёРІРЅСѓСЋ РІРєР»Р°РґРєСѓ
                screen.fillRect(tabX - 1, tabY, static_cast<int>(tabText.length()) + 2, 1, ' ', color);
                screen.putString(tabX, tabY, tabText.c_str(), color);
            } else {
                color = enabled_ ? ColorAttr::biosMenu() : ColorAttr::biosDisabled();
                screen.putString(tabX, tabY, tabText.c_str(), color);
            }

            tabX += static_cast<int>(tabText.length()) + 1;
        }

        // Р РёСЃСѓРµРј СЂР°Р·РґРµР»РёС‚РµР»СЊ РїРѕРґ РІРєР»Р°РґРєР°РјРё
        screen.drawHLine(x_, y_ + 1, width_, Symbols::separatorH, ColorAttr::normal());

        // Р РёСЃСѓРµРј СЃРѕРґРµСЂР¶РёРјРѕРµ С‚РµРєСѓС‰РµР№ РІРєР»Р°РґРєРё
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(tabs_.size())) {
            tabs_[selectedIndex_]->draw(screen);
        }

        // Р Р°РјРєР° РІРѕРєСЂСѓРі СЃРѕРґРµСЂР¶РёРјРѕРіРѕ
        screen.drawBox(x_, y_ + 1, width_, height_ - 1, BoxStyles::thin(), ColorAttr::normal());
    }
};

} // namespace ui

#endif // TEXTUI_TABCONTROL_H
