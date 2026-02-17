#ifndef BOX_STYLE_H
#define BOX_STYLE_H

/**
 * @brief Стиль рамок для окон и виджетов
 */
struct BoxStyle {
    const char* horizontal;
    const char* vertical;
    const char* top_left;
    const char* top_right;
    const char* bottom_left;
    const char* bottom_right;

    BoxStyle()
        : horizontal("-")
        , vertical("|")
        , top_left("+")
        , top_right("+")
        , bottom_left("+")
        , bottom_right("+") {}

    BoxStyle(const char* h, const char* v, const char* tl, const char* tr, const char* bl, const char* br)
        : horizontal(h)
        , vertical(v)
        , top_left(tl)
        , top_right(tr)
        , bottom_left(bl)
        , bottom_right(br) {}

    // Предопределённые стили
    static BoxStyle simple() {
        return BoxStyle("-", "|", "+", "+", "+", "+");
    }

    static BoxStyle doubleLine() {
        return BoxStyle("\xE2\x95\x90", "\xE2\x95\x91", "\xE2\x95\x94", "\xE2\x95\x97", "\xE2\x95\x9A", "\xE2\x95\x9D");
    }

    static BoxStyle singleLine() {
        return BoxStyle("\xE2\x94\x80", "\xE2\x94\x82", "\xE2\x94\x8C", "\xE2\x94\x90", "\xE2\x94\x94", "\xE2\x94\x98");
    }

    static BoxStyle rounded() {
        return BoxStyle("\xE2\x94\x80", "\xE2\x94\x82", "\xE2\x95\xAD", "\xE2\x95\xAE", "\xE2\x95\xB0", "\xE2\x95\xAF");
    }

    static BoxStyle bold() {
        return BoxStyle("\xE2\x94\x81", "\xE2\x94\x83", "\xE2\x94\x8F", "\xE2\x94\x93", "\xE2\x94\x97", "\xE2\x94\x9B");
    }

    static BoxStyle dotted() {
        return BoxStyle("\xE2\x95\x8C", "\xE2\x95\x8E", "\xE2\x94\x8C", "\xE2\x94\x90", "\xE2\x94\x94", "\xE2\x94\x98");
    }

    static BoxStyle dashed() {
        return BoxStyle("\xE2\x94\x84", "\xE2\x94\x86", "\xE2\x94\x8C", "\xE2\x94\x90", "\xE2\x94\x94", "\xE2\x94\x98");
    }
};

#endif // BOX_STYLE_H
