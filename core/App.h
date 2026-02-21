#ifndef TEXTUI_APP_H
#define TEXTUI_APP_H

#include "Screen.h"
#include "Input.h"
#include "../graphics/Colors.h"
#include "../graphics/Chars.h"
#include "../graphics/Theme.h"
#include "../widgets/Widget.h"
#include "../widgets/Window.h"
#include "../widgets/Button.h"
#include "../widgets/Label.h"
#include "../widgets/TextBox.h"
#include "../widgets/CheckBox.h"
#include "../widgets/RadioButton.h"
#include "../widgets/ProgressBar.h"
#include "../widgets/ListBox.h"
#include "../widgets/Menu.h"
#include "../widgets/TabControl.h"
#include "../widgets/StatusBar.h"
#include "../widgets/DropDown.h"
#include "../widgets/MessageBox.h"
#include <vector>
#include <memory>
#include <functional>
#include <chrono>

namespace ui {

/**
 * @brief Главное приложение TextUI
 * 
 * Управляет окнами, вводом, отрисовкой и темами.
 */
class App {
private:
    Screen screen_;
    Input input_;
    std::vector<std::unique_ptr<Window>> windows_;
    Window* focusedWindow_ = nullptr;
    StatusBar* statusBar_ = nullptr;
    ThemeManager themeManager_;
    Theme* currentTheme_;
    bool running_ = false;
    bool useTheme_ = true;
    
    // Таймеры
    std::chrono::steady_clock::time_point lastFrameTime_;
    int frameCount_ = 0;
    float fps_ = 0.0f;

    // Callback для глобальных горячих клавиш
    std::function<bool(Key)> globalHotkeyHandler_;

public:
    App() : currentTheme_(nullptr) {
        // По умолчанию используем BIOS тему
        currentTheme_ = themeManager_.getTheme("bios");
    }
    
    ~App() { 
        shutdown(); 
    }

    // Инициализация
    bool init() {
        if (!screen_.init()) return false;
        if (!input_.init()) return false;

        screen_.clear();
        screen_.showCursor(false);

        // Применяем текущую тему
        if (currentTheme_) {
            applyTheme(*currentTheme_);
        }

        lastFrameTime_ = std::chrono::steady_clock::now();
        return true;
    }

    // Завершение
    void shutdown() {
        screen_.shutdown();
        input_.shutdown();
    }

    // Применение темы
    void applyTheme(const Theme& theme) {
        currentTheme_ = const_cast<Theme*>(&theme);
        screen_.clear(theme.getScreenBackground());
    }

    void setTheme(const std::string& themeName) {
        Theme* theme = themeManager_.getTheme(themeName);
        if (theme) {
            applyTheme(*theme);
        }
    }

    Theme* getCurrentTheme() { return currentTheme_; }
    ThemeManager* getThemeManager() { return &themeManager_; }

    // Создание окна
    Window* createWindow(int x, int y, int w, int h, const std::string& title = "") {
        auto window = std::make_unique<Window>(x, y, w, h, title);
        Window* ptr = window.get();
        windows_.push_back(std::move(window));
        focusedWindow_ = ptr;
        return ptr;
    }

    // Создание модального окна
    Window* createModalWindow(int x, int y, int w, int h, const std::string& title = "") {
        Window* window = createWindow(x, y, w, h, title);
        window->setModal(true);
        return window;
    }

    // Создание центрированного окна
    Window* createCenteredWindow(int w, int h, const std::string& title = "") {
        int x = (screen_.getWidth() - w) / 2;
        int y = (screen_.getHeight() - h) / 2;
        return createWindow(x, y, w, h, title);
    }

    // Добавление виджетов
    Button* addButton(Window* window, int x, int y, int w, const std::string& text, 
                      std::function<void()> onClick = nullptr) {
        if (!window) return nullptr;
        return window->addChild<Button>(x, y, w, text, onClick);
    }

    Label* addLabel(Window* window, int x, int y, const std::string& text) {
        if (!window) return nullptr;
        return window->addChild<Label>(x, y, text);
    }

    Label* addLabel(Window* window, int x, int y, int w, const std::string& text, 
                    TextAlign align = TextAlign::Left) {
        if (!window) return nullptr;
        return window->addChild<Label>(x, y, w, text, align);
    }

    TextBox* addTextBox(Window* window, int x, int y, int w, const std::string& text = "") {
        if (!window) return nullptr;
        return window->addChild<TextBox>(x, y, w, text);
    }

    CheckBox* addCheckBox(Window* window, int x, int y, const std::string& text, bool checked = false) {
        if (!window) return nullptr;
        return window->addChild<CheckBox>(x, y, text, checked);
    }

    RadioButton* addRadioButton(Window* window, int x, int y, const std::string& text, bool checked = false) {
        if (!window) return nullptr;
        return window->addChild<RadioButton>(x, y, text, checked);
    }

    ProgressBar* addProgressBar(Window* window, int x, int y, int w, float progress = 0.0f) {
        if (!window) return nullptr;
        return window->addChild<ProgressBar>(x, y, w, progress);
    }

    ListBox* addListBox(Window* window, int x, int y, int w, int h) {
        if (!window) return nullptr;
        return window->addChild<ListBox>(x, y, w, h);
    }

    Menu* addMenu(Window* window, int x, int y, int width) {
        if (!window) return nullptr;
        return window->addChild<Menu>(x, y, width);
    }

    TabControl* addTabControl(Window* window, int x, int y, int w, int h) {
        if (!window) return nullptr;
        return window->addChild<TabControl>(x, y, w, h);
    }

    DropDown* addDropDown(Window* window, int x, int y, int w) {
        if (!window) return nullptr;
        return window->addChild<DropDown>(x, y, w);
    }

    // Создание строки состояния
    StatusBar* createStatusBar(int y) {
        statusBar_ = new StatusBar(0, y, screen_.getWidth());
        statusBar_->setVisible(true);
        return statusBar_;
    }

    // Показать MessageBox
    MessageBoxResult MessageBox(const std::string& title, const std::string& message,
                                MessageBoxIcon icon = MessageBoxIcon::None,
                                MessageBoxButtons buttons = MessageBoxButtons::OK) {
        auto msgBox = MessageBox::createCentered(screen_, title, message, icon, buttons);
        msgBox.setVisible(true);
        
        // Временно добавляем как окно
        auto window = std::make_unique<Window>(msgBox.x(), msgBox.y(), msgBox.width(), msgBox.height(), title);
        window->setModal(true);
        window->setVisible(true);
        
        // Блокирующий цикл для MessageBox
        input_.enableRawMode();
        while (msgBox.visible()) {
            msgBox.draw(screen_);
            screen_.flush();
            
            Key key = input_.readKey();
            if (key != Key::None) {
                msgBox.handleKey(key);
            }
        }
        input_.disableRawMode();
        
        return msgBox.getResult();
    }

    // Выход
    void exit() {
        running_ = false;
    }

    // Геттеры
    Screen* getScreen() { return &screen_; }
    Input* getInput() { return &input_; }
    const std::vector<std::unique_ptr<Window>>& getWindows() const { return windows_; }
    Window* getFocusedWindow() const { return focusedWindow_; }
    StatusBar* getStatusBar() const { return statusBar_; }

    // Установка глобального обработчика горячих клавиш
    void setGlobalHotkeyHandler(std::function<bool(Key)> handler) {
        globalHotkeyHandler_ = handler;
    }

    // Переключение фокуса между окнами
    void focusNextWindow() {
        if (windows_.empty()) return;

        int current = -1;
        for (size_t i = 0; i < windows_.size(); i++) {
            if (windows_[i].get() == focusedWindow_) {
                current = static_cast<int>(i);
                break;
            }
        }

        int next = (current + 1) % static_cast<int>(windows_.size());
        focusedWindow_ = windows_[next].get();
        if (focusedWindow_) {
            focusedWindow_->setFocus(true);
        }
    }

    // Получение FPS
    float getFPS() const { return fps_; }

    // Главный цикл
    void run() {
        if (!running_) {
            running_ = true;
            screen_.clear();
            screen_.showCursor(false);
            input_.enableRawMode();
            
            // Задержка для очистки буфера клавиатуры
#ifdef _WIN32
            Sleep(100);
            // Очищаем буфер ввода ещё раз
            FlushConsoleInputBuffer(input_.getHIn());
#else
            usleep(100000);
            tcflush(STDIN_FILENO, TCIFLUSH);
#endif
        }

        while (running_) {
            // Обработка ввода
            Key key = input_.readKey();
            
            if (key != Key::None) {
                // Глобальные горячие клавиши
                if (globalHotkeyHandler_ && globalHotkeyHandler_(key)) {
                    continue;
                }

                // Выход по Q или Escape (если нет модальных окон)
                if ((key == Key::Q || key == Key::Escape) && !hasModalWindow()) {
                    exit();
                    continue;
                }

                // Tab - переключение фокуса между виджетами
                if (key == Key::Tab && focusedWindow_) {
                    focusedWindow_->focusNext();
                    continue;
                }

                // F-клавиши для StatusBar
                if (statusBar_ && key >= Key::F1 && key <= Key::F12) {
                    if (statusBar_->handleKey(key)) {
                        continue;
                    }
                }

                // Обработка хоткеев (буквы)
                Key baseKey = getBaseKey(key);
                if (baseKey >= Key::A && baseKey <= Key::Z) {
                    char hotkey = static_cast<char>(static_cast<int>(baseKey));
                    // Сначала активному окну
                    if (focusedWindow_ && focusedWindow_->handleHotkey(hotkey)) {
                        continue;
                    }
                    // Потом всем окнам
                    for (auto& window : windows_) {
                        if (window->visible() && window->handleHotkey(hotkey)) {
                            break;
                        }
                    }
                }

                // Передача ввода активному окну
                if (focusedWindow_ && focusedWindow_->handleKey(key)) {
                    continue;
                }
            }

            // Отрисовка
            draw();

            // Подсчёт FPS
            frameCount_++;
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration<float>(now - lastFrameTime_).count();
            if (elapsed >= 1.0f) {
                fps_ = frameCount_ / elapsed;
                frameCount_ = 0;
                lastFrameTime_ = now;
            }

            // Задержка ~30 FPS
#ifdef _WIN32
            Sleep(33);
#else
            usleep(33000);
#endif
        }

        input_.disableRawMode();
    }

    // Принудительная отрисовка
    void draw() {
        // Отрисовка всех окон
        for (auto& window : windows_) {
            if (window->visible()) {
                window->draw(screen_);
            }
        }

        // Отрисовка StatusBar
        if (statusBar_ && statusBar_->visible()) {
            statusBar_->draw(screen_);
        }

        // Отправляем изменения на экран
        screen_.flush();
    }

    // Проверка наличия модальных окон
    bool hasModalWindow() const {
        for (const auto& window : windows_) {
            if (window->visible() && window->isModal()) {
                return true;
            }
        }
        return false;
    }

    // Закрыть все окна
    void closeAllWindows() {
        for (auto& window : windows_) {
            window->setVisible(false);
        }
        focusedWindow_ = nullptr;
    }

    // Удалить все окна
    void clearWindows() {
        windows_.clear();
        focusedWindow_ = nullptr;
        statusBar_ = nullptr;
    }
};

} // namespace ui

#endif // TEXTUI_APP_H
