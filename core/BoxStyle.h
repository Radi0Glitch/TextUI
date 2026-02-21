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
        return BoxStyle("=", "||", "+", "+", "+", "+");
    }

    static BoxStyle singleLine() {
        return BoxStyle("-", "|", "+", "+", "+", "+");
    }

    static BoxStyle rounded() {
        return BoxStyle("-", "|", "+", "+", "+", "+");
    }

    static BoxStyle bold() {
        return BoxStyle("=", "||", "+", "+", "+", "+");
    }

    static BoxStyle dotted() {
        return BoxStyle("...", ":", "+", "+", "+", "+");
    }

    static BoxStyle dashed() {
        return BoxStyle("--", ":", "+", "+", "+", "+");
    }
};

#endif // BOX_STYLE_H
