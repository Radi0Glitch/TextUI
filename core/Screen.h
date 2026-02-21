#ifndef TEXTUI_SCREEN_H
#define TEXTUI_SCREEN_H

#include <cstdio>
#include <cstdint>
#include <vector>
#include <cstring>
#include <string>
#include "../graphics/Colors.h"
#include "../graphics/Chars.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

namespace ui {

/**
 * @brief Ячейка экрана с полной информацией о стиле
 */
struct ScreenCell {
    char ch = ' ';
    uint8_t fg : 4;      // Цвет текста (0-15)
    uint8_t bg : 3;      // Цвет фона (0-7)
    bool blink : 1;      // Мигание
    bool bold : 1;       // Жирный
    bool inverse : 1;    // Инверсия
    bool underline : 1;  // Подчёркивание

    ScreenCell() : fg(7), bg(0), blink(false), bold(false), inverse(false), underline(false) {}

    bool operator==(const ScreenCell& other) const {
        return ch == other.ch && fg == other.fg && bg == other.bg && 
               blink == other.blink && bold == other.bold && 
               inverse == other.inverse && underline == other.underline;
    }
    
    bool operator!=(const ScreenCell& other) const {
        return !(*this == other);
    }
    
    // Создать из ColorAttr
    static ScreenCell fromColorAttr(char c, const ColorAttr& attr) {
        ScreenCell cell;
        cell.ch = c;
        cell.fg = attr.fg & 0x0F;
        cell.bg = attr.bg & 0x07;
        cell.blink = attr.blink != 0;
        return cell;
    }
};

/**
 * @brief Экран с двойной буферизацией и оптимизацией вывода
 * 
 * Использует back buffer для отрисовки, затем отправляет
 * на экран только изменённые ячейки.
 */
class Screen {
private:
#ifdef _WIN32
    HANDLE hOut = INVALID_HANDLE_VALUE;
    HANDLE hIn = INVALID_HANDLE_VALUE;
    DWORD originalOutMode = 0;
    DWORD originalInMode = 0;
    
public:
    HANDLE getHIn() const { return hIn; }
    
private:
#else
    struct termios originalTermios;
    bool termiosSaved = false;
#endif
    bool initialized = false;
    int width = 80;
    int height = 24;

    // Буферы экрана
    std::vector<ScreenCell> frontBuffer_;
    std::vector<ScreenCell> backBuffer_;
    bool bufferDirty = true;
    
    // Текущая позиция курсора для оптимизации
    int cursorX_ = 0;
    int cursorY_ = 0;

    // Получить индекс в буфере
    inline size_t index(int x, int y) const {
        return static_cast<size_t>(y) * width + x;
    }
    
    // Переместить курсор с оптимизацией
    void moveCursor(int x, int y) {
        if (x == cursorX_ && y == cursorY_) return;
        printf("\033[%d;%dH", y + 1, x + 1);
        cursorX_ = x;
        cursorY_ = y;
    }

public:
    Screen() = default;
    ~Screen() { shutdown(); }

    // Инициализация
    bool init() {
        if (initialized) return true;

#ifdef _WIN32
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hIn = GetStdHandle(STD_INPUT_HANDLE);

        if (hOut == INVALID_HANDLE_VALUE || hIn == INVALID_HANDLE_VALUE) {
            return false;
        }

        // Сохраняем оригинальные режимы
        GetConsoleMode(hOut, &originalOutMode);
        GetConsoleMode(hIn, &originalInMode);

        // Включаем ANSI и обработку ввода
        SetConsoleMode(hOut, originalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
        SetConsoleMode(hIn, originalInMode | ENABLE_EXTENDED_FLAGS);

        // Фиксируем размер 80x25
        width = 80;
        height = 25;
#else
        // Сохраняем терминал
        tcgetattr(STDIN_FILENO, &originalTermios);
        termiosSaved = true;

        // Фиксируем размер 80x25
        width = 80;
        height = 25;
#endif
        // Инициализируем буферы
        size_t size = static_cast<size_t>(width) * height;
        frontBuffer_.resize(size);
        backBuffer_.resize(size);

        // Заполняем пробелами
        for (size_t i = 0; i < size; i++) {
            frontBuffer_[i] = ScreenCell{};
            backBuffer_[i] = ScreenCell{};
        }

        initialized = true;
        return true;
    }

    // Завершение работы
    void shutdown() {
        if (!initialized) return;

#ifdef _WIN32
        SetConsoleMode(hOut, originalOutMode);
        SetConsoleMode(hIn, originalInMode);
#else
        if (termiosSaved) {
            tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
        }
#endif
        showCursor(true);
        initialized = false;
    }

    // Очистка экрана
    void clear() {
        clear(ColorAttr::normal());
    }
    
    // Очистка с цветом
    void clear(const ColorAttr& color) {
        ScreenCell cell;
        cell.fg = color.fg;
        cell.bg = color.bg;
        cell.blink = color.blink;
        cell.bold = false;
        cell.inverse = false;
        cell.underline = false;
        
        for (size_t i = 0; i < frontBuffer_.size(); i++) {
            frontBuffer_[i] = cell;
            backBuffer_[i] = cell;
        }
        bufferDirty = true;
    }

    // Показать/скрыть курсор
    void showCursor(bool visible) {
        if (visible) {
            printf("\033[?25h");
        } else {
            printf("\033[?25l");
        }
        fflush(stdout);
    }

    // Установка стиля текста
    void setStyle(const TextStyle& style) {
        uint8_t fg = style.color.fg;
        uint8_t bg = style.color.bg;
        bool blink = style.color.blink;
        bool bold = (style.attr & TextAttr::Bold) != TextAttr::None;
        bool inverse = (style.attr & TextAttr::Inverse) != TextAttr::None;
        bool underline = (style.attr & TextAttr::Underline) != TextAttr::None;

        // ANSI color mapping (0-15)
        static const int fg_map[] = {30, 34, 32, 36, 31, 35, 33, 37, 
                                     90, 94, 92, 96, 91, 95, 93, 97};
        static const int bg_map[] = {40, 44, 42, 46, 41, 45, 43, 47};

        printf("\033[0");  // Reset
        
        if (bold) printf(";1");
        if (underline) printf(";4");
        if (inverse) printf(";7");
        if (blink) printf(";5");
        
        printf(";%dm", fg_map[fg & 0x0F]);
        
        if (bg < 8) {
            printf("\033[%dm", bg_map[bg]);
        }
    }

    // Сброс стиля
    static void resetStyle() {
        printf("\033[0m");
    }

    // Установка символа в буфер
    void putChar(int x, int y, char ch, const TextStyle& style) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;

        size_t idx = index(x, y);
        ScreenCell& cell = backBuffer_[idx];
        cell.ch = ch;
        cell.fg = style.color.fg;
        cell.bg = style.color.bg;
        cell.blink = style.color.blink;
        cell.bold = (style.attr & TextAttr::Bold) != TextAttr::None;
        cell.inverse = (style.attr & TextAttr::Inverse) != TextAttr::None;
        cell.underline = (style.attr & TextAttr::Underline) != TextAttr::None;
        bufferDirty = true;
    }

    // Установка символа с цветом
    void putChar(int x, int y, char ch, const ColorAttr& color) {
        TextStyle style(color);
        putChar(x, y, ch, style);
    }

    // Установка строки в буфер
    void putString(int x, int y, const char* str, const TextStyle& style) {
        if (!str) return;
        int cx = x;
        while (*str && cx < width) {
            putChar(cx++, y, *str++, style);
        }
    }
    
    // Установка строки с цветом
    void putString(int x, int y, const char* str, const ColorAttr& color) {
        TextStyle style(color);
        putString(x, y, str, style);
    }
    
    // Установка строки std::string
    void putString(int x, int y, const std::string& str, const TextStyle& style) {
        putString(x, y, str.c_str(), style);
    }
    
    void putString(int x, int y, const std::string& str, const ColorAttr& color) {
        putString(x, y, str.c_str(), color);
    }

    // Рисование рамки
    void drawBox(int x, int y, int w, int h, const BoxChars& box, const TextStyle& style) {
        if (w < 2 || h < 2) return;

        // Углы
        putString(x, y, box.top_left, style);
        putString(x + w - 1, y, box.top_right, style);
        putString(x, y + h - 1, box.bottom_left, style);
        putString(x + w - 1, y + h - 1, box.bottom_right, style);

        // Горизонтальные линии
        for (int i = 1; i < w - 1; i++) {
            putString(x + i, y, box.horizontal, style);
            putString(x + i, y + h - 1, box.horizontal, style);
        }

        // Вертикальные линии
        for (int i = 1; i < h - 1; i++) {
            putString(x, y + i, box.vertical, style);
            putString(x + w - 1, y + i, box.vertical, style);
        }
    }
    
    // Рисование рамки с цветом
    void drawBox(int x, int y, int w, int h, const BoxChars& box, const ColorAttr& color) {
        TextStyle style(color);
        drawBox(x, y, w, h, box, style);
    }

    // Рисование заполненного прямоугольника
    void fillRect(int x, int y, int w, int h, char ch, const ColorAttr& color) {
        TextStyle style(color);
        for (int iy = 0; iy < h; iy++) {
            for (int ix = 0; ix < w; ix++) {
                putChar(x + ix, y + iy, ch, style);
            }
        }
    }

    // Рисование горизонтальной линии
    void drawHLine(int x, int y, int w, const char* ch, const ColorAttr& color) {
        TextStyle style(color);
        for (int i = 0; i < w; i++) {
            putString(x + i, y, ch, style);
        }
    }
    
    // Рисование вертикальной линии
    void drawVLine(int x, int y, int h, const char* ch, const ColorAttr& color) {
        TextStyle style(color);
        for (int i = 0; i < h; i++) {
            putString(x, y + i, ch, style);
        }
    }

    // Отрисовка изменений на экран
    void flush() {
        if (!bufferDirty) return;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                size_t idx = index(x, y);
                if (backBuffer_[idx] != frontBuffer_[idx]) {
                    moveCursor(x, y);

                    // Применяем стиль
                    TextStyle style;
                    style.color.fg = backBuffer_[idx].fg;
                    style.color.bg = backBuffer_[idx].bg;
                    style.color.blink = backBuffer_[idx].blink;
                    style.attr = TextAttr::None;
                    if (backBuffer_[idx].bold) style.attr = style.attr | TextAttr::Bold;
                    if (backBuffer_[idx].inverse) style.attr = style.attr | TextAttr::Inverse;
                    if (backBuffer_[idx].underline) style.attr = style.attr | TextAttr::Underline;
                    
                    setStyle(style);
                    putchar(backBuffer_[idx].ch);

                    // Копируем в front buffer
                    frontBuffer_[idx] = backBuffer_[idx];
                }
            }
        }

        resetStyle();
        fflush(stdout);
        bufferDirty = false;
    }

    // Принудительная перерисовка всего экрана
    void forceRedraw() {
        for (size_t i = 0; i < frontBuffer_.size(); i++) {
            frontBuffer_[i].ch = '\0';  // Force mismatch
        }
        bufferDirty = true;
        flush();
    }

    // Получение размера
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isInitialized() const { return initialized; }
    
    // Получить ячейку из back buffer
    const ScreenCell& getCell(int x, int y) const {
        return backBuffer_[index(x, y)];
    }
};

} // namespace ui

#endif // TEXTUI_SCREEN_H
