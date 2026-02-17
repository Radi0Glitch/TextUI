/**
 * TextUI - Пример использования библиотеки
 * 
 * Демонстрирует создание окна с различными виджетами:
 * - Кнопки
 * - Метки
 * - Поля ввода
 * - Чекбоксы
 * - Индикатор прогресса
 * - Список
 */

#include "core/SimpleAPI.h"

using namespace ui;

int main() {
    // Создаем главное окно
    auto* window = createWindow(5, 3, 60, 20, "TextUI Demo");
    
    // Добавляем заголовок
    addLabel(window, 2, 1, "=== Добро пожаловать в TextUI! ===");
    
    // Добавляем поле ввода
    addLabel(window, 2, 3, "Имя:");
    auto* nameInput = addTextBox(window, 10, 3, 20);
    nameInput->setContent("Пользователь");
    
    // Добавляем чекбокс
    auto* checkBox = addCheckBox(window, 2, 5, "Включить уведомления");
    checkBox->setChecked(true);
    
    // Добавляем прогресс бар
    addLabel(window, 2, 7, "Загрузка:");
    auto* progressBar = addProgressBar(window, 12, 7, 30);
    progressBar->setProgress(0.75f);
    
    // Добавляем список
    addLabel(window, 2, 9, "Выберите опцию:");
    auto* listBox = addListBox(window, 2, 10, 25, 5);
    listBox->addItem("Опция 1");
    listBox->addItem("Опция 2");
    listBox->addItem("Опция 3");
    listBox->addItem("Опция 4");
    
    // Добавляем кнопки
    addButton(window, 2, 16, 12, "OK", []() {
        showMessage("Информация", "Кнопка OK нажата!");
    });
    
    addButton(window, 16, 16, 12, "Отмена", []() {
        exitApplication();
    });
    
    addButton(window, 30, 16, 12, "О программе", []() {
        showMessage("О программе", 
            "TextUI v1.0\n"
            "Кроссплатформенный\n"
            "UI фреймворк для C++");
    });
    
    // Запускаем приложение
    runApplication();
    
    return 0;
}
