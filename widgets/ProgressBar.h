#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "Widget.h"
#include "../core/Renderer.h"

class ProgressBar : public Widget {
private:
    float progress; // 0.0 to 1.0

public:
    ProgressBar(int x, int y, int width, float progress = 0.0f)
        : Widget(x, y, width, 3), progress(progress) {
        if (progress < 0.0f) progress = 0.0f;
        if (progress > 1.0f) progress = 1.0f;
    }

    void setProgress(float value) {
        progress = value;
        if (progress < 0.0f) progress = 0.0f;
        if (progress > 1.0f) progress = 1.0f;
        markDirty();
    }

    float getProgress() const { return progress; }

    void render() override {
        if (!visible) return;

        Renderer::drawRectangle(x, y, width, height, boxStyle, colorStyle);

        int fillWidth = static_cast<int>((width - 2) * progress);
        for (int i = 0; i < fillWidth; i++) {
            Renderer::drawChar(x + 1 + i, y + 1, '#', colorStyle);
        }
    }

    void renderToBuffer(RenderBuffer& buffer) override {
        if (!visible || !needsRedraw) return;

        // Рисуем рамку
        for (int i = x + 1; i < x + width - 1; i++) {
            buffer.setStyledChar(i, y, boxStyle.horizontal[0],
                static_cast<int>(colorStyle.foreground),
                static_cast<int>(colorStyle.background),
                colorStyle.bold, colorStyle.italic, colorStyle.underline);
            buffer.setStyledChar(i, y + height - 1, boxStyle.horizontal[0],
                static_cast<int>(colorStyle.foreground),
                static_cast<int>(colorStyle.background),
                colorStyle.bold, colorStyle.italic, colorStyle.underline);
        }
        for (int i = y + 1; i < y + height - 1; i++) {
            buffer.setStyledChar(x, i, boxStyle.vertical[0],
                static_cast<int>(colorStyle.foreground),
                static_cast<int>(colorStyle.background),
                colorStyle.bold, colorStyle.italic, colorStyle.underline);
            buffer.setStyledChar(x + width - 1, i, boxStyle.vertical[0],
                static_cast<int>(colorStyle.foreground),
                static_cast<int>(colorStyle.background),
                colorStyle.bold, colorStyle.italic, colorStyle.underline);
        }
        // Углы
        buffer.setStyledChar(x, y, boxStyle.top_left[0],
            static_cast<int>(colorStyle.foreground),
            static_cast<int>(colorStyle.background),
            colorStyle.bold, colorStyle.italic, colorStyle.underline);
        buffer.setStyledChar(x + width - 1, y, boxStyle.top_right[0],
            static_cast<int>(colorStyle.foreground),
            static_cast<int>(colorStyle.background),
            colorStyle.bold, colorStyle.italic, colorStyle.underline);
        buffer.setStyledChar(x, y + height - 1, boxStyle.bottom_left[0],
            static_cast<int>(colorStyle.foreground),
            static_cast<int>(colorStyle.background),
            colorStyle.bold, colorStyle.italic, colorStyle.underline);
        buffer.setStyledChar(x + width - 1, y + height - 1, boxStyle.bottom_right[0],
            static_cast<int>(colorStyle.foreground),
            static_cast<int>(colorStyle.background),
            colorStyle.bold, colorStyle.italic, colorStyle.underline);

        // Рисуем заполнение
        int fillWidth = static_cast<int>((width - 2) * progress);
        for (int i = 0; i < fillWidth && x + 1 + i < x + width - 1; i++) {
            buffer.setStyledChar(x + 1 + i, y + 1, '#',
                static_cast<int>(colorStyle.foreground),
                static_cast<int>(colorStyle.background),
                colorStyle.bold, colorStyle.italic, colorStyle.underline);
        }

        markClean();
    }
};

#endif // PROGRESSBAR_H
