#ifndef TEXTUI_CHARS_H
#define TEXTUI_CHARS_H

namespace ui {

/**
 * @brief Символы для рисования рамок
 * 
 * Поддержка ASCII и расширенной псевдографики (Box Drawing)
 */
struct BoxChars {
    const char* horizontal;
    const char* vertical;
    const char* top_left;
    const char* top_right;
    const char* bottom_left;
    const char* bottom_right;

    BoxChars()
        : horizontal("-")
        , vertical("|")
        , top_left("+")
        , top_right("+")
        , bottom_left("+")
        , bottom_right("+") {}
    
    BoxChars(const char* h, const char* v, const char* tl, const char* tr, const char* bl, const char* br)
        : horizontal(h)
        , vertical(v)
        , top_left(tl)
        , top_right(tr)
        , bottom_left(bl)
        , bottom_right(br) {}
};

/**
 * @brief Предопределённые стили рамок
 */
namespace BoxStyles {
    
    // Простая ASCII рамка (максимальная совместимость)
    inline BoxChars ascii() {
        return BoxChars("-", "|", "+", "+", "+", "+");
    }

    // Жирная рамка (ASCII эмуляция)
    inline BoxChars bold() {
        return BoxChars("=", "||", "+", "+", "+", "+");
    }

    // Двойная рамка (ASCII эмуляция)
    inline BoxChars doubleLine() {
        return BoxChars("=", "||", "+", "+", "+", "+");
    }

    // Пунктирная рамка
    inline BoxChars dotted() {
        return BoxChars(".", ":", "+", "+", "+", "+");
    }

    // Пунктирная жирная
    inline BoxChars dashed() {
        return BoxChars("--", ":", "+", "+", "+", "+");
    }
}

/**
 * @brief Специальные символы интерфейса
 */
namespace Symbols {
    // Навигация
    inline const char* arrowRight = ">";
    inline const char* arrowLeft = "<";
    inline const char* arrowUp = "^";
    inline const char* arrowDown = "v";
    
    // Маркеры
    inline const char* bullet = "\x07";        // ● (или "*")
    inline const char* bulletSmall = "\xB7";   // ·
    inline const char* check = "\xFB";         // ■ (или "✓")
    inline const char* cross = "\xD7";         // ×
    
    // Чекбоксы и радио
    inline const char* checkboxOn = "[\xDB]";   // [■]
    inline const char* checkboxOff = "[\xB0]";  // [°]
    inline const char* radioOn = "(\xDB)";      // (●)
    inline const char* radioOff = "(\xB0)";     // (°)
    
    // Прогресс бар
    inline const char* progressFull = "\xDB";   // ■
    inline const char* progressHalf = "\xB1";   // ░
    inline const char* progressEmpty = "\xB0";  // ░
    inline const char* progressBlock = "\xDB";  // █
    
    // Разделители
    inline const char* separatorH = "\xC4";     // ─
    inline const char* separatorV = "\xB3";     // │
    inline const char* separatorCross = "\xC5"; // ┼
    
    // Курсоры
    inline const char* cursorBlock = "\xDB";    // █
    inline const char* cursorUnderline = "\xCD";// ¯
    
    // Табы
    inline const char* tabActive = "\xC4";
    inline const char* tabInactive = "-";
    
    // Скроллбар
    inline const char* scrollUp = "\x1E";       // ▲
    inline const char* scrollDown = "\x1F";     // ▼
    inline const char* scrollThumb = "\xDB";    // █
    
    // ASCII альтернативы (для совместимости)
    namespace ASCII {
        inline const char* arrowRight = ">";
        inline const char* arrowLeft = "<";
        inline const char* arrowUp = "^";
        inline const char* arrowDown = "v";
        inline const char* bullet = "*";
        inline const char* checkboxOn = "[X]";
        inline const char* checkboxOff = "[ ]";
        inline const char* radioOn = "(*)";
        inline const char* radioOff = "( )";
        inline const char* progressFull = "#";
        inline const char* progressEmpty = "-";
        inline const char* cursorBlock = "#";
    }
}

/**
 * @brief Тени для окон
 */
namespace Shadows {
    // Простая тень (символы разной плотности)
    inline const char* light = "\xB0";
    inline const char* medium = "\xB1";
    inline const char* dark = "\xB2";
}

} // namespace ui

#endif // TEXTUI_CHARS_H
