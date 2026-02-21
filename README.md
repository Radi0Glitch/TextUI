# TextUI Framework v3.0

Кроссплатформенный фреймворк для создания текстовых интерфейсов в стиле **BIOS Setup** и **Norton Commander**.

## Особенности

- **16-цветная палитра BIOS** - полная поддержка CGA/EGA/VGA цветов
- **Система тем** - BIOS, Norton Commander, Terminal, Mono + кастомные
- **Горячие клавиши** - поддержка Alt+X, Ctrl+X, F1-F12
- **Псевдографика** - рамки из ASCII и Box Drawing символов
- **Двойная буферизация** - отрисовка без мигания
- **Только клавиатура** - управление без мыши
- **Статическая сборка** - один exe файл без DLL

## Виджеты

| Виджет | Описание |
|--------|----------|
| Window | Окно с заголовком и рамкой |
| Button | Кнопка с callback и горячей клавишей |
| Label | Текстовая метка с выравниванием |
| TextBox | Поле ввода (пароль, маска, история) |
| CheckBox | Чекбокс с горячей клавишей |
| RadioButton | Радио-кнопка с группой |
| ProgressBar | Индикатор прогресса (4 стиля) |
| ListBox | Список с прокруткой и скроллбаром |
| Menu | Вертикальное меню с разделителями |
| TabControl | Вкладки с переключением |
| StatusBar | Строка состояния с F-клавишами |
| DropDown | Выпадающий список |
| MessageBox | Диалоговые окна (Info, Warning, Error) |

## Управление

| Клавиша | Действие |
|---------|----------|
| Tab | Переключение фокуса |
| Enter/Space | Активация (кнопка, чекбокс) |
| Стрелки ↑↓←→ | Навигация |
| F1-F12 | Функциональные клавиши |
| A-Z | Горячие клавиши |
| Escape | Выход / отмена |
| Q | Выход |

## Быстрый старт

```cpp
#include "core/App.h"

int main() {
    ui::App app;

    if (!app.init()) return 1;

    // Создаем окно
    auto* window = app.createWindow(10, 2, 60, 18, "Demo");

    // Добавляем виджеты
    app.addLabel(window, 2, 1, "=== TextUI v3.0 ===");

    auto* textbox = app.addTextBox(window, 2, 3, 20, "Type here...");
    textbox->setHotkey('T');  // T - фокус

    app.addButton(window, 2, 10, 10, "Exit", [&app]() {
        app.exit();
    })->setHotkey('X');  // X - активация

    app.run();
    return 0;
}
```

## Примеры использования виджетов

### RadioButton с группой
```cpp
auto* group = std::make_unique<ui::RadioButtonGroup>();

auto* radio1 = window->addChild<ui::RadioButton>(2, 4, "Option A", true);
auto* radio2 = window->addChild<ui::RadioButton>(2, 5, "Option B", false);
auto* radio3 = window->addChild<ui::RadioButton>(2, 6, "Option C", false);

group->add(radio1);
group->add(radio2);
group->add(radio3);

group->setOnChange([](int index) {
    std::cout << "Selected: " << index << std::endl;
});
```

### TabControl с вкладками
```cpp
auto* tabs = app.addTabControl(window, 1, 4, 68, 17);
tabs->setHotkey('T');

// Страница 1
ui::TabPage* page1 = tabs->addTab("Main");
page1->setHotkey('M');
page1->addWidget<ui::Label>(2, 1, "Main Page");

// Страница 2
ui::TabPage* page2 = tabs->addTab("Advanced");
page2->setHotkey('A');
page2->addWidget<ui::Label>(2, 1, "Advanced Page");
```

### DropDown список
```cpp
auto* drop = app.addDropDown(window, 2, 5, 20);
drop->setHotkey('C');
drop->addItem("Auto");
drop->addItem("Enabled");
drop->addItem("Disabled");
drop->setOnSelect([](int index) {
    std::cout << "Selected: " << index << std::endl;
});
```

### TextBox с маской и паролем
```cpp
// Пароль
auto* pass = app.addTextBox(window, 2, 3, 20);
pass->setMode(ui::TextBoxMode::Password);

// Целое число
auto* num = app.addTextBox(window, 2, 5, 10);
num->setMode(ui::TextBoxMode::Integer);
num->setIntValue(42);

// Маска (AAA-000)
auto* masked = app.addTextBox(window, 2, 7, 10);
masked->setMask("AAA-000");

// История ввода
auto* history = app.addTextBox(window, 2, 9, 30);
history->enableHistory(10);
```

### MessageBox
```cpp
// Информация
app.MessageBox("Info", "Operation completed",
               ui::MessageBoxIcon::Info);

// Подтверждение
auto result = app.MessageBox("Confirm", "Delete file?",
                             ui::MessageBoxIcon::Question,
                             ui::MessageBoxButtons::YesNo);

if (result == ui::MessageBoxResult::Yes) {
    // Удалить файл
}
```

## Темы оформления

### Предопределённые темы
```cpp
app.setTheme("bios");       // Синий фон, белые/голубые элементы
app.setTheme("nc");         // Серый/синий стиль NC
app.setTheme("terminal");   // Зелёный на чёрном
app.setTheme("mono");       // Чёрно-белая гамма
```

### Кастомная тема
```cpp
Theme createMyTheme() {
    Theme theme("MyTheme", ThemeType::Custom);
    
    // Фон экрана
    theme.setScreenBackground(ColorAttr(Color8::Black, Color8::Black));
    
    // Цвета окон
    WidgetColors window;
    window.background = ColorAttr(Color8::Black, Color8::Black);
    window.normal = ColorAttr(Color8::BrightGreen, Color8::Black);
    window.focused = ColorAttr(Color8::Black, Color8::BrightGreen);
    theme.setWindowColors(window);
    
    // Цвета кнопок
    WidgetColors button;
    button.normal = ColorAttr(Color8::BrightGreen, Color8::Black);
    button.focused = ColorAttr(Color8::Black, Color8::BrightGreen);
    theme.setButtonColors(button);
    
    return theme;
}

// Использование
app.getThemeManager()->addTheme("matrix", createMyTheme());
app.setTheme("matrix");
```

### Доступные цвета (Color8)
- `Black`, `Blue`, `Green`, `Cyan`, `Red`, `Magenta`, `Brown`, `White`
- `Gray`, `BrightBlue`, `BrightGreen`, `BrightCyan`, `BrightRed`, `BrightMagenta`, `BrightYellow`, `BrightWhite`

## Сборка

### Windows (MinGW/MSYS2)
```bash
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Windows (MSVC)
```bash
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Linux
```bash
cd build
cmake ..
make
```

## Требования

- **Windows 10+** с поддержкой ANSI
- **Linux** с любым терминалом
- **Компилятор**: C++17 (GCC 7+, Clang 5+, MSVC 2017+)

## Архитектура

```
TextUI/
├── core/
│   ├── App.h           # Главное приложение
│   ├── Screen.h        # Экран с двойной буферизацией
│   └── Input.h         # Ввод с модификаторами
├── graphics/
│   ├── Colors.h        # 16-цветная палитра BIOS
│   ├── Chars.h         # ASCII и псевдографика
│   └── Theme.h         # Система тем
├── widgets/
│   ├── Widget.h        # Базовый класс
│   ├── Window.h        # Окна
│   ├── Button.h        # Кнопки
│   ├── Label.h         # Метки
│   ├── TextBox.h       # Поле ввода
│   ├── CheckBox.h      # Чекбокс
│   ├── RadioButton.h   # Радио-кнопка
│   ├── ProgressBar.h   # Прогресс бар
│   ├── ListBox.h       # Список
│   ├── Menu.h          # Меню
│   ├── TabControl.h    # Вкладки
│   ├── StatusBar.h     # Строка состояния
│   ├── DropDown.h      # Выпадающий список
│   └── MessageBox.h    # Диалоговые окна
└── examples/
    └── simple.cpp      # Простой пример
```

## Горячие клавиши

Горячие клавиши устанавливаются через `setHotkey(char)`:

```cpp
button->setHotkey('S');   // S - активация
checkBox->setHotkey('Q'); // Q - переключение
textBox->setHotkey('T');  // T - фокус
```

В меню и TabControl горячие клавиши отображаются автоматически:
- `Menu`: "S Save" - S активирует пункт
- `TabControl`: "[M]ain" - M переключает вкладку

## Системные цвета

```cpp
// Предопределённые цвета
ColorAttr::normal()       // Белый на чёрном
ColorAttr::highlight()    // Чёрный на белом
ColorAttr::error()        // Ярко-красный
ColorAttr::warning()      // Жёлтый
ColorAttr::success()      // Ярко-зелёный
ColorAttr::info()         // Ярко-голубой
```

## Лицензия

MIT License
