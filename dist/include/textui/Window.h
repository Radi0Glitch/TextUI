#ifndef TEXTUI_WINDOW_H
#define TEXTUI_WINDOW_H

#include "Widget.h"
#include "../core/Screen.h"
#include <vector>
#include <memory>
#include <string>

namespace ui {

/**
 * @brief РћРєРЅРѕ СЃ Р·Р°РіРѕР»РѕРІРєРѕРј Рё СЂР°РјРєРѕР№
 */
class Window : public Widget {
private:
    std::string title_;
    std::vector<std::unique_ptr<Widget>> children_;
    Widget* focusedChild_ = nullptr;
    bool modal_ = false;
    bool draggable_ = false;
    bool hasCloseButton_ = false;
    ColorAttr titleColor_;
    ColorAttr borderColor_;
    BoxStyle boxStyle_;

public:
    Window(int x, int y, int w, int h, const std::string& title = "")
        : Widget(x, y, w, h)
        , title_(title)
        , titleColor_(ColorAttr::biosTitle())
        , borderColor_(ColorAttr::biosMenu())
        , boxStyle_(BoxStyles::doubleLine()) {
        canFocus_ = true;
    }

    void setTitle(const std::string& title) { title_ = title; }
    const std::string& getTitle() const { return title_; }
    
    bool isModal() const { return modal_; }
    void setModal(bool v) { modal_ = v; }
    
    void setDraggable(bool v) { draggable_ = v; }
    bool isDraggable() const { return draggable_; }
    
    void setShowCloseButton(bool v) { hasCloseButton_ = v; }
    bool hasCloseButton() const { return hasCloseButton_; }

    void setTitleColor(ColorAttr c) { titleColor_ = c; }
    void setBorderColor(ColorAttr c) { borderColor_ = c; }
    void setBoxStyle(const BoxStyle& style) { boxStyle_ = style; }

    // Р”РѕР±Р°РІР»РµРЅРёРµ РґРѕС‡РµСЂРЅРµРіРѕ РІРёРґР¶РµС‚Р°
    template<typename T, typename... Args>
    T* addChild(Args&&... args) {
        auto widget = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = widget.get();
        children_.push_back(std::move(widget));
        return ptr;
    }

    // РЈРґР°Р»РµРЅРёРµ РІСЃРµС… РґРѕС‡РµСЂРЅРёС… РІРёРґР¶РµС‚РѕРІ
    void clearChildren() {
        children_.clear();
        focusedChild_ = nullptr;
    }

    // РџРѕР»СѓС‡РµРЅРёРµ РґРѕС‡РµСЂРЅРµРіРѕ РІРёРґР¶РµС‚Р° РїРѕ РёРЅРґРµРєСЃСѓ
    Widget* getChild(size_t index) {
        if (index < children_.size()) {
            return children_[index].get();
        }
        return nullptr;
    }

    size_t getChildCount() const { return children_.size(); }

    // Р¤РѕРєСѓСЃ РЅР° РїРµСЂРІС‹Р№ РґРѕСЃС‚СѓРїРЅС‹Р№ РІРёРґР¶РµС‚
    void focusFirst() {
        for (auto& child : children_) {
            if (child->visible() && child->canFocus()) {
                child->setFocused(true);
                focusedChild_ = child.get();
                return;
            }
        }
    }

    // РџРµСЂРµРєР»СЋС‡РµРЅРёРµ С„РѕРєСѓСЃР°
    void focusNext() {
        if (children_.empty()) return;

        int current = -1;
        for (size_t i = 0; i < children_.size(); i++) {
            if (children_[i]->focused() && children_[i]->canFocus()) {
                current = static_cast<int>(i);
                children_[i]->setFocused(false);
                break;
            }
        }

        int next = (current + 1) % static_cast<int>(children_.size());
        for (int i = 0; i < static_cast<int>(children_.size()); i++) {
            int idx = (next + i) % static_cast<int>(children_.size());
            if (children_[idx]->canFocus() && children_[idx]->visible()) {
                children_[idx]->setFocused(true);
                focusedChild_ = children_[idx].get();
                return;
            }
        }
    }

    void focusPrev() {
        if (children_.empty()) return;

        int current = -1;
        for (size_t i = 0; i < children_.size(); i++) {
            if (children_[i]->focused() && children_[i]->canFocus()) {
                current = static_cast<int>(i);
                children_[i]->setFocused(false);
                break;
            }
        }

        int prev = (current - 1);
        if (prev < 0) prev = static_cast<int>(children_.size()) - 1;
        for (int i = 0; i < static_cast<int>(children_.size()); i++) {
            int idx = (prev - i + static_cast<int>(children_.size())) % static_cast<int>(children_.size());
            if (children_[idx]->canFocus() && children_[idx]->visible()) {
                children_[idx]->setFocused(true);
                focusedChild_ = children_[idx].get();
                return;
            }
        }
    }

    // РћР±СЂР°Р±РѕС‚РєР° РєР»Р°РІРёР°С‚СѓСЂС‹
    bool handleKey(Key key) override {
        if (!visible_ || !enabled_) return false;

        // РџРµСЂРµРґР°С‘Рј С„РѕРєСѓСЃ Р°РєС‚РёРІРЅРѕРјСѓ РґРѕС‡РµСЂРЅРµРјСѓ РІРёРґР¶РµС‚Сѓ
        if (focusedChild_ && focusedChild_->handleKey(key)) {
            return true;
        }

        // РџРµСЂРµРєР»СЋС‡РµРЅРёРµ С„РѕРєСѓСЃР° РјРµР¶РґСѓ РІРёРґР¶РµС‚Р°РјРё
        if (key == Key::Tab) {
            focusNext();
            return true;
        }

        // Р—Р°РєСЂС‹С‚РёРµ РїРѕ Escape
        if (key == Key::Escape && modal_) {
            visible_ = false;
            return true;
        }

        return false;
    }

    // РћР±СЂР°Р±РѕС‚РєР° РіРѕСЂСЏС‡РёС… РєР»Р°РІРёС€
    bool handleHotkey(char key) override {
        if (!visible_ || !enabled_) return false;

        // РЎРЅР°С‡Р°Р»Р° РїС‹С‚Р°РµРјСЃСЏ РїРµСЂРµРґР°С‚СЊ РіРѕСЂСЏС‡СѓСЋ РєР»Р°РІРёС€Сѓ РґРѕС‡РµСЂРЅРёРј РІРёРґР¶РµС‚Р°Рј
        for (auto& child : children_) {
            if (child->visible() && child->handleHotkey(key)) {
                return true;
            }
        }

        return false;
    }

    void setFocused(bool focus) override {
        focused_ = focus;
        if (focus && focusedChild_ == nullptr) {
            focusFirst();
        }
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        // Р РёСЃСѓРµРј СЂР°РјРєСѓ
        screen.drawBox(x_, y_, width_, height_, boxStyle_, borderColor_);

        // Р—Р°РіРѕР»РѕРІРѕРє
        if (!title_.empty()) {
            std::string title = " " + title_ + " ";
            
            // Р¤РѕРЅ Р·Р°РіРѕР»РѕРІРєР°
            screen.fillRect(x_ + 2, y_, static_cast<int>(title.length()), 1, ' ', titleColor_);
            screen.putString(x_ + 2, y_, title.c_str(), titleColor_);
        }

        // РљРЅРѕРїРєР° Р·Р°РєСЂС‹С‚РёСЏ
        if (hasCloseButton_) {
            screen.putString(x_ + width_ - 4, y_, "[X]", borderColor_);
        }

        // Р РёСЃСѓРµРј РґРѕС‡РµСЂРЅРёРµ РІРёРґР¶РµС‚С‹
        for (auto& child : children_) {
            if (child->visible()) {
                child->draw(screen);
            }
        }
    }

    // РћС‚СЂРёСЃРѕРІРєР° СЃ СѓС‡С‘С‚РѕРј С‚РµРјС‹
    void drawWithTheme(Screen& screen, const class Theme& theme) {
        if (!visible_) return;

        const WidgetColors& colors = theme.getWindowColors();
        
        // Р¤РѕРЅ РѕРєРЅР°
        screen.fillRect(x_, y_, width_, height_, ' ', colors.background);
        
        // Р Р°РјРєР°
        screen.drawBox(x_, y_, width_, height_, theme.getBoxStyle(), colors.normal);

        // Р—Р°РіРѕР»РѕРІРѕРє
        if (!title_.empty()) {
            std::string title = " " + title_ + " ";
            screen.fillRect(x_ + 2, y_, static_cast<int>(title.length()), 1, ' ', theme.getScreenBackground());
            screen.putString(x_ + 2, y_, title.c_str(), TextStyle::biosTitle());
        }

        // Р”РѕС‡РµСЂРЅРёРµ РІРёРґР¶РµС‚С‹
        for (auto& child : children_) {
            if (child->visible()) {
                child->draw(screen);
            }
        }
    }
};

} // namespace ui

#endif // TEXTUI_WINDOW_H
