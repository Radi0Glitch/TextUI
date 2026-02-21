#ifndef TEXTUI_INPUT_H
#define TEXTUI_INPUT_H

#include <cstdint>
#include <string>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace ui {

/**
 * @brief Коды клавиш с поддержкой модификаторов
 * 
 * Формат: 0xMMKK где MM - модификаторы, KK - код клавиши
 * Модификаторы: Alt=0x10, Ctrl=0x20, Shift=0x40
 */
enum class Key : int {
    None = 0,
    Escape = 27,
    Enter = 13,
    Tab = 9,
    Backspace = 8,
    Space = 32,

    // Стрелки
    Up = 256,
    Down = 257,
    Left = 258,
    Right = 259,

    // Специальные
    Home = 260,
    End = 261,
    PageUp = 262,
    PageDown = 263,
    Insert = 264,
    Delete = 265,
    Menu = 266,

    // Функциональные клавиши
    F1 = 270,
    F2 = 271,
    F3 = 272,
    F4 = 273,
    F5 = 274,
    F6 = 275,
    F7 = 276,
    F8 = 277,
    F9 = 278,
    F10 = 279,
    F11 = 280,
    F12 = 281,

    // Цифры
    _0 = 48, _1, _2, _3, _4, _5, _6, _7, _8, _9,

    // Буквы (нижний регистр)
    A = 97, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Модификаторы (битовые флаги)
    AltMask = 0x1000,
    CtrlMask = 0x2000,
    ShiftMask = 0x4000,
};

// Вспомогательные функции для работы с модификаторами
inline Key operator|(Key k, int mask) {
    return static_cast<Key>(static_cast<int>(k) | mask);
}

inline bool hasAlt(Key k) {
    return (static_cast<int>(k) & static_cast<int>(Key::AltMask)) != 0;
}

inline bool hasCtrl(Key k) {
    return (static_cast<int>(k) & static_cast<int>(Key::CtrlMask)) != 0;
}

inline bool hasShift(Key k) {
    return (static_cast<int>(k) & static_cast<int>(Key::ShiftMask)) != 0;
}

inline Key getBaseKey(Key k) {
    return static_cast<Key>(static_cast<int>(k) & 0x0FFF);
}

inline Key withAlt(Key k) { return k | static_cast<int>(Key::AltMask); }
inline Key withCtrl(Key k) { return k | static_cast<int>(Key::CtrlMask); }
inline Key withShift(Key k) { return k | static_cast<int>(Key::ShiftMask); }

// Создание комбинированных клавиш
inline Key Alt(char c) {
    return static_cast<Key>((static_cast<int>(Key::AltMask)) | (toupper(c)));
}

inline Key Ctrl(char c) {
    return static_cast<Key>((static_cast<int>(Key::CtrlMask)) | (toupper(c)));
}

/**
 * @brief Событие ввода
 */
struct InputEvent {
    Key key;
    char ch;           // ASCII символ (если есть)
    bool isRepeat;     // Повтор нажатия
    uint32_t timestamp; // Время нажатия

    InputEvent() : key(Key::None), ch(0), isRepeat(false), timestamp(0) {}

    bool isPrintable() const {
        return ch >= 32 && ch < 127;
    }

    std::string toString() const {
        std::string result;
        if (hasAlt(key)) result += "Alt+";
        if (hasCtrl(key)) result += "Ctrl+";
        if (hasShift(key)) result += "Shift+";

        Key base = getBaseKey(key);
        if (isPrintable()) {
            result += ch;
        } else {
            switch (base) {
                case Key::Enter: result += "Enter"; break;
                case Key::Escape: result += "Escape"; break;
                case Key::Tab: result += "Tab"; break;
                case Key::Backspace: result += "Backspace"; break;
                case Key::Up: result += "Up"; break;
                case Key::Down: result += "Down"; break;
                case Key::Left: result += "Left"; break;
                case Key::Right: result += "Right"; break;
                case Key::Home: result += "Home"; break;
                case Key::End: result += "End"; break;
                case Key::PageUp: result += "PageUp"; break;
                case Key::PageDown: result += "PageDown"; break;
                case Key::Insert: result += "Insert"; break;
                case Key::Delete: result += "Delete"; break;
                case Key::F1: result += "F1"; break;
                case Key::F2: result += "F2"; break;
                case Key::F3: result += "F3"; break;
                case Key::F4: result += "F4"; break;
                case Key::F5: result += "F5"; break;
                case Key::F6: result += "F6"; break;
                case Key::F7: result += "F7"; break;
                case Key::F8: result += "F8"; break;
                case Key::F9: result += "F9"; break;
                case Key::F10: result += "F10"; break;
                case Key::F11: result += "F11"; break;
                case Key::F12: result += "F12"; break;
                default: result += "?"; break;
            }
        }
        return result;
    }
};

/**
 * @brief Ввод с клавиатуры с поддержкой модификаторов
 */
class Input {
private:
#ifdef _WIN32
    HANDLE hIn = INVALID_HANDLE_VALUE;
    DWORD originalInMode = 0;
    
public:
    HANDLE getHIn() const { return hIn; }
    
private:
#else
    struct termios originalTermios;
    bool termiosSaved = false;
#endif
    bool initialized = false;
    bool rawModeEnabled = false;
    
    // Состояние модификаторов
    bool altPressed_ = false;
    bool ctrlPressed_ = false;
    bool shiftPressed_ = false;

public:
    Input() = default;
    ~Input() { shutdown(); }

    // Инициализация
    bool init() {
        if (initialized) return true;

#ifdef _WIN32
        hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE) return false;

        // Сохраняем оригинальный режим
        GetConsoleMode(hIn, &originalInMode);
        
        // Очищаем буфер ввода
        FlushConsoleInputBuffer(hIn);
#else
        // Сохраняем терминал
        tcgetattr(STDIN_FILENO, &originalTermios);
        termiosSaved = true;
#endif
        initialized = true;
        return true;
    }

    // Завершение
    void shutdown() {
        if (!initialized) return;

        disableRawMode();

#ifdef _WIN32
        SetConsoleMode(hIn, originalInMode);
#else
        if (termiosSaved) {
            tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
        }
#endif
        initialized = false;
    }

    // Включение raw mode
    void enableRawMode() {
        if (rawModeEnabled) return;

#ifdef _WIN32
        SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS);
#else
        if (termiosSaved) {
            struct termios raw = originalTermios;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 1;
            raw.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        }
#endif
        rawModeEnabled = true;
    }

    // Выключение raw mode
    void disableRawMode() {
        if (!rawModeEnabled) return;

#ifdef _WIN32
        SetConsoleMode(hIn, originalInMode | ENABLE_EXTENDED_FLAGS);
#else
        if (termiosSaved) {
            tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
        }
#endif
        rawModeEnabled = false;
    }

    // Чтение клавиши (возвращает Key с модификаторами)
    Key readKey() {
#ifdef _WIN32
        if (!_kbhit()) return Key::None;

        int ch = _getch();
        Key key = Key::None;

        // Проверка на модификаторы (для Windows)
        if (GetAsyncKeyState(VK_MENU) & 0x8000) altPressed_ = true;
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000) ctrlPressed_ = true;
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000) shiftPressed_ = true;

        // Специальные клавиши (первый байт 0 или 224)
        if (ch == 0 || ch == 224) {
            ch = _getch();  // Второй байт
            switch (ch) {
                case 72: key = Key::Up; break;
                case 80: key = Key::Down; break;
                case 75: key = Key::Left; break;
                case 77: key = Key::Right; break;
                case 71: key = Key::Home; break;
                case 79: key = Key::End; break;
                case 73: key = Key::PageUp; break;
                case 81: key = Key::PageDown; break;
                case 82: key = Key::Insert; break;
                case 83: key = Key::Delete; break;
                case 59: key = Key::F1; break;
                case 60: key = Key::F2; break;
                case 61: key = Key::F3; break;
                case 62: key = Key::F4; break;
                case 63: key = Key::F5; break;
                case 64: key = Key::F6; break;
                case 65: key = Key::F7; break;
                case 66: key = Key::F8; break;
                case 67: key = Key::F9; break;
                case 68: key = Key::F10; break;
                case 87: key = Key::F11; break;
                case 88: key = Key::F12; break;
            }
        } else if (ch == 13) {
            key = Key::Enter;
        } else if (ch == 27) {
            key = Key::Escape;
        } else if (ch == 9) {
            key = Key::Tab;
        } else if (ch == 8) {
            key = Key::Backspace;
        } else if (ch == 32) {
            key = Key::Space;
        } else if (ch >= 32 && ch < 127) {
            // Обычный символ
            key = static_cast<Key>(ch);
        }

        // Применяем модификаторы
        if (altPressed_) {
            key = static_cast<Key>(static_cast<int>(key) | static_cast<int>(Key::AltMask));
            altPressed_ = false;
        }
        if (ctrlPressed_) {
            key = static_cast<Key>(static_cast<int>(key) | static_cast<int>(Key::CtrlMask));
            ctrlPressed_ = false;
        }

        return key;

#else
        char ch = 0;
        ssize_t n = read(STDIN_FILENO, &ch, 1);

        if (n <= 0) return Key::None;

        // Escape последовательности
        if (ch == 27) {
            char seq[4] = {0};
            
            // Пробуем прочитать продолжение
            fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
            n = read(STDIN_FILENO, &seq[0], 1);
            
            if (n <= 0) {
                fcntl(STDIN_FILENO, F_SETFL, 0);
                return Key::Escape;
            }

            if (seq[0] == '[') {
                n = read(STDIN_FILENO, &seq[1], 1);
                
                if (n > 0 && seq[1] >= '0' && seq[1] <= '9') {
                    // Расширенная последовательность (F5-F8)
                    n = read(STDIN_FILENO, &seq[2], 1);
                    if (seq[2] == '~') {
                        int code = (seq[1] - '0') * 10 + (seq[2] - '0');
                        switch (code) {
                            case 15: return Key::F5;
                            case 17: return Key::F6;
                            case 18: return Key::F7;
                            case 19: return Key::F8;
                            case 20: return Key::F9;
                            case 21: return Key::F10;
                        }
                    }
                } else if (n > 0) {
                    switch (seq[1]) {
                        case 'A': return Key::Up;
                        case 'B': return Key::Down;
                        case 'C': return Key::Right;
                        case 'D': return Key::Left;
                        case 'H': return Key::Home;
                        case 'F': return Key::End;
                        case '3': return Key::Delete;
                        case '5': return Key::PageUp;
                        case '6': return Key::PageDown;
                    }
                }
            } else if (seq[0] == 'O') {
                // F1-F4
                n = read(STDIN_FILENO, &seq[1], 1);
                switch (seq[1]) {
                    case 'P': return Key::F1;
                    case 'Q': return Key::F2;
                    case 'R': return Key::F3;
                    case 'S': return Key::F4;
                }
            }

            fcntl(STDIN_FILENO, F_SETFL, 0);
            return Key::Escape;
        }

        // Обычные символы
        if (ch == 10 || ch == 13) return Key::Enter;
        if (ch == 9) return Key::Tab;
        if (ch == 8) return Key::Backspace;
        if (ch == 32) return Key::Space;

        if (ch >= 32 && ch < 127) {
            return static_cast<Key>(ch);
        }

        return Key::None;
#endif
    }

    // Чтение события ввода
    InputEvent readEvent() {
        InputEvent event;
        event.key = readKey();
        
        if (event.key != Key::None) {
            event.ch = (static_cast<int>(event.key) < 128) 
                       ? static_cast<char>(static_cast<int>(event.key)) 
                       : 0;
            event.isRepeat = false;
            event.timestamp = 0;  // Можно добавить таймер
        }
        
        return event;
    }

    // Проверка наличия ввода
    bool hasInput() {
#ifdef _WIN32
        return _kbhit() != 0;
#else
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
#endif
    }

    // Ожидание ввода с таймаутом
    Key waitKey(int timeout_ms) {
#ifdef _WIN32
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start).count() < timeout_ms) {
            if (_kbhit()) {
                return readKey();
            }
#ifdef _WIN32
            Sleep(10);
#else
            usleep(10000);
#endif
        }
        return Key::None;
#else
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        
        if (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
            return readKey();
        }
        return Key::None;
#endif
    }

    bool isInitialized() const { return initialized; }
    bool isRawModeEnabled() const { return rawModeEnabled; }
};

} // namespace ui

#endif // TEXTUI_INPUT_H
