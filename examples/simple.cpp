/**
 * TextUI v3.0 - Простой пример
 */

#include "core/App.h"
#include <iostream>

int main() {
    using namespace ui;
    
    App app;

    if (!app.init()) {
        std::cerr << "Failed to initialize!" << std::endl;
        return 1;
    }

    // Создаем главное окно
    auto* window = app.createWindow(10, 2, 60, 18, "TextUI Demo");

    // Добавляем заголовок
    app.addLabel(window, 2, 1, "=== Welcome to TextUI v3.0 ===");
    app.addLabel(window, 2, 3, "Press buttons or use hotkeys");
    app.addLabel(window, 2, 5, "Hotkeys:");
    app.addLabel(window, 4, 6, "C - Click Me");
    app.addLabel(window, 4, 7, "X - Exit");
    app.addLabel(window, 4, 8, "Tab - Focus");

    // Добавляем кнопки с горячими клавишами
    app.addButton(window, 2, 11, 14, "[C] Click Me!", []() {
        std::cout << "Button clicked!" << std::endl;
    })->setHotkey('C');

    app.addButton(window, 18, 11, 14, "[X] Exit", [&app]() {
        app.exit();
    })->setHotkey('X');

    // Чекбокс
    auto* check = app.addCheckBox(window, 2, 14, "Enable feature", false);
    check->setHotkey('F');

    // Запускаем
    app.run();

    return 0;
}
