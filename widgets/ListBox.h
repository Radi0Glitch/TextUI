#ifndef TEXTUI_LISTBOX_H
#define TEXTUI_LISTBOX_H

#include "Widget.h"
#include "../core/Screen.h"
#include <vector>
#include <functional>

namespace ui {

/**
 * @brief РЎРїРёСЃРѕРє СЃ РїСЂРѕРєСЂСѓС‚РєРѕР№
 */
class ListBox : public Widget {
private:
    std::vector<std::string> items_;
    std::vector<void*> itemData_;  // Р”Р°РЅРЅС‹Рµ РґР»СЏ РєР°Р¶РґРѕРіРѕ СЌР»РµРјРµРЅС‚Р°
    int selectedIndex_ = -1;
    int scrollOffset_ = 0;
    bool hasFocus_ = false;
    std::function<void(int)> onSelect_;
    std::function<void(int)> onDoubleClick_;
    bool showScrollBars_ = true;
    int lastClickTime_ = 0;

public:
    ListBox(int x, int y, int width, int height)
        : Widget(x, y, width, height) {
        canFocus_ = true;
    }

    void setShowScrollBars(bool show) { showScrollBars_ = show; }

    // Р”РѕР±Р°РІР»РµРЅРёРµ СЌР»РµРјРµРЅС‚Р°
    void addItem(const std::string& item, void* data = nullptr) {
        items_.push_back(item);
        itemData_.push_back(data);
        if (selectedIndex_ < 0) selectedIndex_ = 0;
    }

    void insertItem(int index, const std::string& item, void* data = nullptr) {
        if (index < 0 || index > static_cast<int>(items_.size())) return;
        items_.insert(items_.begin() + index, item);
        itemData_.insert(itemData_.begin() + index, data);
        if (selectedIndex_ >= index) selectedIndex_++;
    }

    void removeItem(int index) {
        if (index < 0 || index >= static_cast<int>(items_.size())) return;
        items_.erase(items_.begin() + index);
        itemData_.erase(itemData_.begin() + index);
        if (selectedIndex_ >= index) selectedIndex_--;
        if (selectedIndex_ < 0 && !items_.empty()) selectedIndex_ = 0;
    }

    void removeItem(const std::string& item) {
        for (size_t i = 0; i < items_.size(); i++) {
            if (items_[i] == item) {
                removeItem(static_cast<int>(i));
                return;
            }
        }
    }

    void clearItems() {
        items_.clear();
        itemData_.clear();
        selectedIndex_ = -1;
        scrollOffset_ = 0;
    }

    int getCount() const { return static_cast<int>(items_.size()); }

    int getSelectedIndex() const { return selectedIndex_; }

    void setSelectedIndex(int index) {
        if (index >= 0 && index < static_cast<int>(items_.size())) {
            selectedIndex_ = index;
            // РџСЂРѕРєСЂСѓС‚РєР° Рє РІС‹Р±СЂР°РЅРЅРѕРјСѓ СЌР»РµРјРµРЅС‚Сѓ
            if (selectedIndex_ < scrollOffset_) {
                scrollOffset_ = selectedIndex_;
            } else if (selectedIndex_ >= scrollOffset_ + getVisibleCount()) {
                scrollOffset_ = selectedIndex_ - getVisibleCount() + 1;
            }
        }
    }

    const std::string& getSelectedItem() const {
        static std::string empty;
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(items_.size())) {
            return items_[selectedIndex_];
        }
        return empty;
    }

    void* getSelectedItemData() const {
        if (selectedIndex_ >= 0 && selectedIndex_ < static_cast<int>(itemData_.size())) {
            return itemData_[selectedIndex_];
        }
        return nullptr;
    }

    const std::string& getItem(int index) const {
        static std::string empty;
        if (index >= 0 && index < static_cast<int>(items_.size())) {
            return items_[index];
        }
        return empty;
    }

    void setOnSelect(std::function<void(int)> callback) {
        onSelect_ = callback;
    }

    void setOnDoubleClick(std::function<void(int)> callback) {
        onDoubleClick_ = callback;
    }

    bool hasFocus() const { return hasFocus_; }

    void setFocused(bool focus) override {
        focused_ = focus;
        hasFocus_ = focus;
    }

    int getVisibleCount() const {
        return height_ - 2;  // РЈС‡РёС‚С‹РІР°РµРј СЂР°РјРєРё
    }

    bool handleKey(Key key) override {
        if (!visible_ || !enabled_ || !hasFocus_ || items_.empty()) return false;

        switch (key) {
            case Key::Up:
                if (selectedIndex_ > 0) {
                    selectedIndex_--;
                    if (selectedIndex_ < scrollOffset_) {
                        scrollOffset_ = selectedIndex_;
                    }
                    if (onSelect_) onSelect_(selectedIndex_);
                }
                return true;

            case Key::Down:
                if (selectedIndex_ < static_cast<int>(items_.size()) - 1) {
                    selectedIndex_++;
                    if (selectedIndex_ >= scrollOffset_ + getVisibleCount()) {
                        scrollOffset_ = selectedIndex_ - getVisibleCount() + 1;
                    }
                    if (onSelect_) onSelect_(selectedIndex_);
                }
                return true;

            case Key::PageUp:
                selectedIndex_ -= getVisibleCount();
                if (selectedIndex_ < 0) selectedIndex_ = 0;
                scrollOffset_ = selectedIndex_;
                if (onSelect_) onSelect_(selectedIndex_);
                return true;

            case Key::PageDown:
                selectedIndex_ += getVisibleCount();
                if (selectedIndex_ >= static_cast<int>(items_.size())) {
                    selectedIndex_ = static_cast<int>(items_.size()) - 1;
                }
                scrollOffset_ = selectedIndex_ - getVisibleCount() + 1;
                if (scrollOffset_ < 0) scrollOffset_ = 0;
                if (onSelect_) onSelect_(selectedIndex_);
                return true;

            case Key::Home:
                selectedIndex_ = 0;
                scrollOffset_ = 0;
                if (onSelect_) onSelect_(selectedIndex_);
                return true;

            case Key::End:
                selectedIndex_ = static_cast<int>(items_.size()) - 1;
                scrollOffset_ = selectedIndex_ - getVisibleCount() + 1;
                if (scrollOffset_ < 0) scrollOffset_ = 0;
                if (onSelect_) onSelect_(selectedIndex_);
                return true;

            case Key::Enter:
            case Key::Space:
                if (onSelect_) onSelect_(selectedIndex_);
                return true;

            default:
                return false;
        }
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        ColorAttr normalColor = enabled_ ? ColorAttr::normal() : ColorAttr::biosDisabled();
        ColorAttr focusColor = enabled_ ? ColorAttr::highlight() : ColorAttr::biosDisabled();

        // Р Р°РјРєР°
        screen.drawBox(x_, y_, width_, height_, BoxStyles::thin(), hasFocus_ ? focusColor : normalColor);

        // Р РёСЃСѓРµРј СЌР»РµРјРµРЅС‚С‹
        int visibleItems = getVisibleCount();
        for (int i = 0; i < visibleItems && scrollOffset_ + i < static_cast<int>(items_.size()); i++) {
            int itemIndex = scrollOffset_ + i;
            std::string display = items_[itemIndex];
            bool isSelected = (itemIndex == selectedIndex_);

            // РћР±СЂРµР·Р°РµРј, РµСЃР»Рё РЅРµ РїРѕРјРµС‰Р°РµС‚СЃСЏ
            int maxLen = width_ - 4;  // РЈС‡РёС‚С‹РІР°РµРј СЂР°РјРєСѓ Рё РјР°СЂРєРµСЂ
            if (static_cast<int>(display.length()) > maxLen) {
                display = display.substr(0, maxLen - 1) + Symbols::arrowRight;
            }

            // Р¤РѕСЂРјРёСЂСѓРµРј СЃС‚СЂРѕРєСѓ
            std::string line;
            if (isSelected && hasFocus_) {
                line = Symbols::arrowRight + display;
            } else {
                line = " " + display;
            }

            // Р¦РІРµС‚
            ColorAttr color = (isSelected && hasFocus_) ? focusColor : normalColor;
            
            screen.putString(x_ + 1, y_ + 1 + i, line.c_str(), color);
        }

        // РЎРєСЂРѕР»Р»Р±Р°СЂ (РµСЃР»Рё РІРєР»СЋС‡РµРЅ)
        if (showScrollBars_ && static_cast<int>(items_.size()) > visibleItems) {
            drawScrollBar(screen);
        }

        // РРЅРґРёРєР°С‚РѕСЂ С„РѕРєСѓСЃР°
        if (hasFocus_) {
            screen.putString(x_ - 1, y_, "#", TextStyle::biosMenu());
        }
    }

private:
    void drawScrollBar(Screen& screen) {
        int visibleItems = getVisibleCount();
        int totalItems = static_cast<int>(items_.size());
        
        // РџРѕР·РёС†РёСЏ Рё СЂР°Р·РјРµСЂ РїРѕР»Р·СѓРЅРєР°
        int thumbSize = std::max(1, (visibleItems * visibleItems) / totalItems);
        int thumbPos = (scrollOffset_ * (visibleItems - thumbSize)) / 
                       std::max(1, (totalItems - visibleItems));
        
        // Р РёСЃСѓРµРј РїРѕР»РѕСЃСѓ РїСЂРѕРєСЂСѓС‚РєРё
        int scrollX = x_ + width_ - 2;
        for (int i = 0; i < visibleItems; i++) {
            if (i == 0) {
                screen.putString(scrollX, y_ + 1 + i, Symbols::arrowUp, ColorAttr::normal());
            } else if (i == visibleItems - 1) {
                screen.putString(scrollX, y_ + 1 + i, Symbols::arrowDown, ColorAttr::normal());
            } else if (i >= thumbPos && i < thumbPos + thumbSize) {
                screen.putString(scrollX, y_ + 1 + i, Symbols::scrollThumb, ColorAttr::highlight());
            } else {
                screen.putString(scrollX, y_ + 1 + i, Symbols::separatorV, ColorAttr::normal());
            }
        }
    }
};

} // namespace ui

#endif // TEXTUI_LISTBOX_H
