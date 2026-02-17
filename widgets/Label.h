#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

class Label : public Widget {
public:
    Label(int x, int y, const std::string& text)
        : Widget(x, y, static_cast<int>(text.length()), 1) {
        this->text = text;
    }

    void render() override {
        if (!visible) return;
        Renderer::drawText(x, y, text, colorStyle);
    }

    void renderToBuffer(RenderBuffer& buffer) override {
        if (!visible || !needsRedraw) return;

        for (size_t i = 0; i < text.length() && x + static_cast<int>(i) < buffer.getWidth(); i++) {
            buffer.setStyledChar(x + static_cast<int>(i), y, text[i],
                static_cast<int>(colorStyle.foreground),
                static_cast<int>(colorStyle.background),
                colorStyle.bold, colorStyle.italic, colorStyle.underline);
        }
        markClean();
    }
};

#endif // LABEL_H
