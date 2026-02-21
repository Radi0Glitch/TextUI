#ifndef TEXTUI_CHARS_H
#define TEXTUI_CHARS_H

namespace ui {

/**
 * @brief Символы для рисования рамок (ASCII Box Drawing)
 * 
 * Коды символов из Code Page 437 (176-223)
 */
namespace BoxChars {
    // Тонкие линии
    inline const char* hLine = "\xC4";  // 196 - горизонтальная
    inline const char* vLine = "\xB3";  // 179 - вертикальная
    
    // Углы
    inline const char* topLeft = "\xDA";     // 218
    inline const char* topRight = "\xBF";    // 191
    inline const char* bottomLeft = "\xC0";  // 192
    inline const char* bottomRight = "\xD9"; // 217
    
    // T-образные соединения
    inline const char* teeDown = "\xC2";     // 194
    inline const char* teeUp = "\xC1";       // 193
    inline const char* teeRight = "\xC3";    // 195
    inline const char* teeLeft = "\xB4";     // 180
    
    // Перекрёсток
    inline const char* cross = "\xC5";       // 197
    
    // Жирные линии
    inline const char* hLineBold = "\xCD";   // 205
    inline const char* vLineBold = "\xBA";   // 186
    
    // Жирные углы
    inline const char* topLeftBold = "\xCE";     // 206
    inline const char* topRightBold = "\xBB";    // 187
    inline const char* bottomLeftBold = "\xBC";  // 188
    inline const char* bottomRightBold = "\xBD"; // 189
    
    // Двойные линии
    inline const char* hLineDouble = "\xCD";   // 205
    inline const char* vLineDouble = "\xBA";   // 186
    
    // Смешанные (жирные углы, тонкие линии)
    inline const char* topLeftMixed = "\xD9";      // 217
    inline const char* topRightMixed = "\xBF";     // 191
    inline const char* bottomLeftMixed = "\xC0";   // 192
    inline const char* bottomRightMixed = "\xDA";  // 218
}

/**
 * @brief Предопределённые стили рамок
 */
struct BoxStyle {
    const char* horizontal;
    const char* vertical;
    const char* top_left;
    const char* top_right;
    const char* bottom_left;
    const char* bottom_right;

    BoxStyle()
        : horizontal(BoxChars::hLine)
        , vertical(BoxChars::vLine)
        , top_left(BoxChars::topLeft)
        , top_right(BoxChars::topRight)
        , bottom_left(BoxChars::bottomLeft)
        , bottom_right(BoxChars::bottomRight) {}

    BoxStyle(const char* h, const char* v, const char* tl, const char* tr, const char* bl, const char* br)
        : horizontal(h), vertical(v), top_left(tl), top_right(tr), 
          bottom_left(bl), bottom_right(br) {}
};

namespace BoxStyles {
    // Тонкая рамка
    inline BoxStyle thin() {
        return BoxStyle(BoxChars::hLine, BoxChars::vLine,
                       BoxChars::topLeft, BoxChars::topRight,
                       BoxChars::bottomLeft, BoxChars::bottomRight);
    }
    
    // Жирная рамка
    inline BoxStyle bold() {
        return BoxStyle(BoxChars::hLineBold, BoxChars::vLineBold,
                       BoxChars::topLeftBold, BoxChars::topRightBold,
                       BoxChars::bottomLeftBold, BoxChars::bottomRightBold);
    }
    
    // Двойная рамка
    inline BoxStyle doubleLine() {
        return BoxStyle(BoxChars::hLineDouble, BoxChars::vLineDouble,
                       BoxChars::topLeftBold, BoxChars::topRightBold,
                       BoxChars::bottomLeftBold, BoxChars::bottomRightBold);
    }
    
    // Смешанная (жирные углы, тонкие линии)
    inline BoxStyle mixed() {
        return BoxStyle(BoxChars::hLine, BoxChars::vLine,
                       BoxChars::topLeftMixed, BoxChars::topRightMixed,
                       BoxChars::bottomLeftMixed, BoxChars::bottomRightMixed);
    }
    
    // ASCII рамка (для совместимости)
    inline BoxStyle ascii() {
        return BoxStyle("-", "|", "+", "+", "+", "+");
    }
}

/**
 * @brief Специальные символы интерфейса
 */
namespace Symbols {
    // Навигация
    inline const char* arrowRight = "\xBA";  // 186
    inline const char* arrowLeft = "\xC9";   // 201
    inline const char* arrowUp = "\x1E";     // 30
    inline const char* arrowDown = "\x1F";   // 31
    
    // Маркеры
    inline const char* bullet = "\x07";      // 7
    inline const char* bulletSmall = "\xB7"; // 183
    
    // Чекбоксы и радио
    inline const char* checkboxOn = "[\xDB]";   // 219 - полный блок
    inline const char* checkboxOff = "[\xB0]";  // 176 - лёгкая штриховка
    inline const char* radioOn = "(\xDB)";
    inline const char* radioOff = "(\xB0)";
    
    // Прогресс бар
    inline const char* progressFull = "\xDB";   // 219 - полный блок
    inline const char* progressHalf = "\xB1";   // 177 - средняя штриховка
    inline const char* progressEmpty = "\xB0";  // 176 - лёгкая штриховка
    
    // Разделители
    inline const char* separatorH = "\xC4";     // 196
    inline const char* separatorV = "\xB3";     // 179
    inline const char* separatorCross = "\xC5"; // 197
    
    // Курсоры
    inline const char* cursorBlock = "\xDB";    // 219
    inline const char* cursorUnderline = "\xDF"; // 223
    
    // Скроллбар
    inline const char* scrollUp = "\x1E";       // 30
    inline const char* scrollDown = "\x1F";     // 31
    inline const char* scrollThumb = "\xDB";    // 219
    
    // ASCII альтернативы
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

} // namespace ui

#endif // TEXTUI_CHARS_H
