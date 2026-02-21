#ifndef TEXTUI_PROGRESSBAR_H
#define TEXTUI_PROGRESSBAR_H

#include "Widget.h"
#include "../core/Screen.h"

namespace ui {

/**
 * @brief РЎС‚РёР»СЊ РїСЂРѕРіСЂРµСЃСЃ-Р±Р°СЂР°
 */
enum class ProgressBarStyle {
    Solid,      // РЎРїР»РѕС€РЅР°СЏ Р·Р°РїРѕР»РЅРµРЅРёРµ
    Block,      // Р‘Р»РѕС‡РЅРѕРµ Р·Р°РїРѕР»РЅРµРЅРёРµ
    Dashed,     // РџСѓРЅРєС‚РёСЂРЅРѕРµ
    Animated    // РђРЅРёРјРёСЂРѕРІР°РЅРЅРѕРµ
};

/**
 * @brief РРЅРґРёРєР°С‚РѕСЂ РїСЂРѕРіСЂРµСЃСЃР°
 */
class ProgressBar : public Widget {
private:
    float progress_ = 0.0f;  // 0.0 - 1.0
    ProgressBarStyle style_ = ProgressBarStyle::Solid;
    bool showPercent_ = true;
    int animationFrame_ = 0;

public:
    ProgressBar(int x, int y, int width, float progress = 0.0f)
        : Widget(x, y, width, 3), progress_(progress) {}

    void setStyle(ProgressBarStyle style) { style_ = style; }
    ProgressBarStyle getStyle() const { return style_; }

    void setShowPercent(bool show) { showPercent_ = show; }

    float getProgress() const { return progress_; }

    void setProgress(float progress) {
        if (progress < 0.0f) progress = 0.0f;
        if (progress > 1.0f) progress = 1.0f;
        progress_ = progress;
    }

    void increment(float amount) {
        setProgress(progress_ + amount);
    }

    void setAnimationFrame(int frame) {
        animationFrame_ = frame;
    }

    void draw(Screen& screen) override {
        if (!visible_) return;

        ColorAttr normalColor = ColorAttr::normal();
        ColorAttr fillColor = ColorAttr(Color8::Black, Color8::BrightCyan);

        // Р Р°РјРєР°
        screen.drawBox(x_, y_, width_, height_, BoxStyles::ascii(), normalColor);

        // Р’РЅСѓС‚СЂРµРЅРЅСЏСЏ РѕР±Р»Р°СЃС‚СЊ
        int innerWidth = width_ - 2;
        int innerX = x_ + 1;
        int innerY = y_ + 1;

        // РћС‡РёС‰Р°РµРј РІРЅСѓС‚СЂРµРЅРЅРѕСЃС‚СЊ
        screen.fillRect(innerX, innerY, innerWidth, 1, ' ', ColorAttr::normal());

        // Р РёСЃСѓРµРј Р·Р°РїРѕР»РЅРµРЅРёРµ РІ Р·Р°РІРёСЃРёРјРѕСЃС‚Рё РѕС‚ СЃС‚РёР»СЏ
        int fillWidth = static_cast<int>(innerWidth * progress_);
        
        switch (style_) {
            case ProgressBarStyle::Solid:
                for (int i = 0; i < fillWidth; i++) {
                    screen.putString(innerX + i, innerY, Symbols::progressBlock, fillColor);
                }
                break;

            case ProgressBarStyle::Block:
                for (int i = 0; i < fillWidth; i++) {
                    screen.putString(innerX + i, innerY, Symbols::progressFull, fillColor);
                }
                break;

            case ProgressBarStyle::Dashed:
                for (int i = 0; i < fillWidth; i++) {
                    const char* ch = (i % 2 == 0) ? Symbols::progressFull : " ";
                    screen.putString(innerX + i, innerY, ch, fillColor);
                }
                break;

            case ProgressBarStyle::Animated:
                // РђРЅРёРјРёСЂРѕРІР°РЅРЅС‹Р№ РїСЂРѕРіСЂРµСЃСЃ
                for (int i = 0; i < fillWidth; i++) {
                    int phase = (i + animationFrame_) % 4;
                    const char* chars[] = {Symbols::progressHalf, Symbols::progressFull, 
                                          Symbols::progressHalf, " "};
                    screen.putString(innerX + i, innerY, chars[phase], fillColor);
                }
                break;
        }

        // РџСЂРѕС†РµРЅС‚
        if (showPercent_) {
            std::string percent = std::to_string(static_cast<int>(progress_ * 100)) + "%";
            int percentX = x_ + (width_ - static_cast<int>(percent.length())) / 2;
            screen.putString(percentX, innerY, percent.c_str(), TextStyle::bold());
        }
    }
};

} // namespace ui

#endif // TEXTUI_PROGRESSBAR_H
