#ifndef TEXTUI_COLORS_H
#define TEXTUI_COLORS_H

#include <cstdint>

namespace ui {

/**
 * @brief 16-цветная палитра BIOS (CGA/EGA/VGA)
 * 
 * Это стандартные цвета IBM PC совместимых систем.
 * Каждый цвет имеет обычный и яркий вариант.
 */
enum class Color8 : uint8_t {
    Black       = 0,
    Blue        = 1,
    Green       = 2,
    Cyan        = 3,
    Red         = 4,
    Magenta     = 5,
    Brown       = 6,    // Тёмно-жёлтый
    White       = 7,    // Серый
    
    // Яркие цвета (8-15)
    Gray        = 8,    // Ярко-чёрный
    BrightBlue  = 9,
    BrightGreen = 10,
    BrightCyan  = 11,
    BrightRed   = 12,
    BrightMagenta = 13,
    BrightYellow = 14,   // Ярко-коричневый
    BrightWhite = 15
};

/**
 * @brief Атрибуты текста (битовые флаги)
 */
enum class TextAttr : uint8_t {
    None        = 0x00,
    Bold        = 0x01,
    Blink       = 0x02,   // Мигание
    Inverse     = 0x04,   // Инверсия (фон <-> текст)
    Underline   = 0x08
};

inline TextAttr operator|(TextAttr a, TextAttr b) {
    return static_cast<TextAttr>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline TextAttr operator&(TextAttr a, TextAttr b) {
    return static_cast<TextAttr>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline TextAttr& operator|=(TextAttr& a, TextAttr b) {
    a = a | b;
    return a;
}

/**
 * @brief Цветовой атрибут BIOS (1 байт)
 * 
 * Формат: 0bBFRGGB (B=blink, F=foreground intensity, RGGB=color)
 * Для полного атрибута (текст + фон): 2 байта
 */
struct ColorAttr {
    uint8_t fg : 4;  // Цвет текста (0-15)
    uint8_t bg : 3;  // Цвет фона (0-7, т.к. мигание отдельно)
    uint8_t blink : 1;  // Мигание

    ColorAttr() : fg(7), bg(0), blink(0) {}  // Белый на чёрном
    
    ColorAttr(Color8 foreground, Color8 background = Color8::Black, bool bl = false)
        : fg(static_cast<uint8_t>(foreground))
        , bg(static_cast<uint8_t>(background) & 0x07)
        , blink(bl ? 1 : 0) {}

    // Получить полный байт атрибута (для BIOS)
    uint8_t toBIOS() const {
        return (blink << 7) | ((bg & 0x07) << 4) | (fg & 0x0F);
    }

    // Создать из BIOS атрибута
    static ColorAttr fromBIOS(uint8_t attr) {
        ColorAttr c;
        c.blink = (attr >> 7) & 1;
        c.bg = (attr >> 4) & 0x07;
        c.fg = attr & 0x0F;
        return c;
    }

    // Предопределённые цвета
    static ColorAttr normal() { return ColorAttr(Color8::White, Color8::Black); }
    static ColorAttr highlight() { return ColorAttr(Color8::Black, Color8::White); }
    static ColorAttr error() { return ColorAttr(Color8::BrightRed, Color8::Black); }
    static ColorAttr warning() { return ColorAttr(Color8::BrightYellow, Color8::Black); }
    static ColorAttr success() { return ColorAttr(Color8::BrightGreen, Color8::Black); }
    static ColorAttr info() { return ColorAttr(Color8::BrightCyan, Color8::Black); }
    
    // BIOS Setup цвета
    static ColorAttr biosTitle() { return ColorAttr(Color8::BrightWhite, Color8::Blue); }
    static ColorAttr biosMenu() { return ColorAttr(Color8::BrightCyan, Color8::Black); }
    static ColorAttr biosSelected() { return ColorAttr(Color8::Black, Color8::BrightCyan); }
    static ColorAttr biosDisabled() { return ColorAttr(Color8::Gray, Color8::Black); }
    
    // Norton Commander цвета
    static ColorAttr ncMenu() { return ColorAttr(Color8::Black, Color8::Gray); }
    static ColorAttr ncSelected() { return ColorAttr(Color8::BrightWhite, Color8::Blue); }
    static ColorAttr ncPanel() { return ColorAttr(Color8::BrightWhite, Color8::Blue); }
};

/**
 * @brief Полный стиль текста
 */
struct TextStyle {
    ColorAttr color;
    TextAttr attr;

    TextStyle() : color(ColorAttr::normal()), attr(TextAttr::None) {}
    
    TextStyle(ColorAttr c) : color(c), attr(TextAttr::None) {}
    
    TextStyle(ColorAttr c, TextAttr a) : color(c), attr(a) {}

    // Предопределённые стили
    static TextStyle normal() { return TextStyle(ColorAttr::normal()); }
    static TextStyle bold() { 
        TextStyle t; 
        t.attr = t.attr | TextAttr::Bold; 
        return t; 
    }
    static TextStyle highlight() { return TextStyle(ColorAttr::highlight()); }
    static TextStyle error() { return TextStyle(ColorAttr::error(), TextAttr::Bold); }
    static TextStyle warning() { return TextStyle(ColorAttr::warning()); }
    static TextStyle success() { return TextStyle(ColorAttr::success()); }
    static TextStyle blink() { 
        TextStyle t; 
        t.attr = t.attr | TextAttr::Blink; 
        return t; 
    }
    static TextStyle inverse() { 
        TextStyle t; 
        t.attr = t.attr | TextAttr::Inverse; 
        return t; 
    }
    static TextStyle underline() { 
        TextStyle t; 
        t.attr = t.attr | TextAttr::Underline; 
        return t; 
    }

    // BIOS стили
    static TextStyle biosTitle() { return TextStyle(ColorAttr::biosTitle(), TextAttr::Bold); }
    static TextStyle biosMenu() { return TextStyle(ColorAttr::biosMenu()); }
    static TextStyle biosSelected() { return TextStyle(ColorAttr::biosSelected()); }
    static TextStyle biosDisabled() { return TextStyle(ColorAttr::biosDisabled()); }
    static TextStyle biosHotkey() { return TextStyle(ColorAttr(Color8::BrightYellow, Color8::Black), TextAttr::Bold); }
};

} // namespace ui

#endif // TEXTUI_COLORS_H
