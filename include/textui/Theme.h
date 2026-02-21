#ifndef TEXTUI_THEME_H
#define TEXTUI_THEME_H

#include "../graphics/Colors.h"
#include "../graphics/Chars.h"
#include <string>
#include <unordered_map>

namespace ui {

/**
 * @brief Типы тем оформления
 */
enum class ThemeType {
    BIOS,           // Классический BIOS Setup
    NortonCommander, // NC style
    Terminal,       // Современный терминал
    Mono,           // Монохромная
    Custom          // Пользовательская
};

/**
 * @brief Цветовая схема для виджета
 */
struct WidgetColors {
    ColorAttr normal;
    ColorAttr focused;
    ColorAttr disabled;
    ColorAttr highlight;
    ColorAttr text;
    ColorAttr background;

    WidgetColors() 
        : normal(ColorAttr::normal())
        , focused(ColorAttr::highlight())
        , disabled(ColorAttr(Color8::Gray, Color8::Black))
        , highlight(ColorAttr::highlight())
        , text(ColorAttr::normal())
        , background(ColorAttr(Color8::Black, Color8::Black)) {}
};

/**
 * @brief Тема оформления
 */
class Theme {
private:
    std::string name_;
    ThemeType type_;
    
    // Цвета для различных элементов
    WidgetColors windowColors_;
    WidgetColors buttonColors_;
    WidgetColors labelColors_;
    WidgetColors textBoxColors_;
    WidgetColors menuColors_;
    WidgetColors listBoxColors_;
    WidgetColors checkBoxColors_;
    WidgetColors radioColors_;
    WidgetColors tabColors_;
    WidgetColors statusBarColors_;
    WidgetColors dialogColors_;
    WidgetColors errorColors_;

    // Стили рамок
    BoxStyle boxStyle_;

    // Цвет фона экрана
    ColorAttr screenBackground_;

public:
    Theme(const std::string& name = "Default", ThemeType type = ThemeType::Custom)
        : name_(name)
        , type_(type)
        , boxStyle_(BoxStyles::thin())
        , screenBackground_(ColorAttr(Color8::Black, Color8::Black)) {
    }

    // Геттеры
    const std::string& getName() const { return name_; }
    ThemeType getType() const { return type_; }

    const WidgetColors& getWindowColors() const { return windowColors_; }
    const WidgetColors& getButtonColors() const { return buttonColors_; }
    const WidgetColors& getLabelColors() const { return labelColors_; }
    const WidgetColors& getTextBoxColors() const { return textBoxColors_; }
    const WidgetColors& getMenuColors() const { return menuColors_; }
    const WidgetColors& getListBoxColors() const { return listBoxColors_; }
    const WidgetColors& getCheckBoxColors() const { return checkBoxColors_; }
    const WidgetColors& getRadioColors() const { return radioColors_; }
    const WidgetColors& getTabColors() const { return tabColors_; }
    const WidgetColors& getStatusBarColors() const { return statusBarColors_; }
    const WidgetColors& getDialogColors() const { return dialogColors_; }
    const WidgetColors& getErrorColors() const { return errorColors_; }
    
    const BoxStyle& getBoxStyle() const { return boxStyle_; }
    const ColorAttr& getScreenBackground() const { return screenBackground_; }

    // Сеттеры
    void setWindowColors(const WidgetColors& c) { windowColors_ = c; }
    void setButtonColors(const WidgetColors& c) { buttonColors_ = c; }
    void setLabelColors(const WidgetColors& c) { labelColors_ = c; }
    void setTextBoxColors(const WidgetColors& c) { textBoxColors_ = c; }
    void setMenuColors(const WidgetColors& c) { menuColors_ = c; }
    void setListBoxColors(const WidgetColors& c) { listBoxColors_ = c; }
    void setCheckBoxColors(const WidgetColors& c) { checkBoxColors_ = c; }
    void setRadioColors(const WidgetColors& c) { radioColors_ = c; }
    void setTabColors(const WidgetColors& c) { tabColors_ = c; }
    void setStatusBarColors(const WidgetColors& c) { statusBarColors_ = c; }
    void setDialogColors(const WidgetColors& c) { dialogColors_ = c; }
    void setErrorColors(const WidgetColors& c) { errorColors_ = c; }
    
    void setBoxStyle(const BoxStyle& style) { boxStyle_ = style; }
    void setScreenBackground(const ColorAttr& bg) { screenBackground_ = bg; }

    // Предопределённые темы
    
    /**
     * @brief Тема BIOS Setup (синий фон, белые/голубые элементы)
     */
    static Theme createBIOS() {
        Theme theme("BIOS Setup", ThemeType::BIOS);
        
        // Фон экрана - синий
        theme.setScreenBackground(ColorAttr(Color8::White, Color8::Blue));
        
        // Окна
        WidgetColors window;
        window.background = ColorAttr(Color8::White, Color8::Blue);
        window.normal = ColorAttr(Color8::BrightWhite, Color8::Blue);
        window.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        window.disabled = ColorAttr(Color8::Gray, Color8::Blue);
        window.highlight = ColorAttr(Color8::Black, Color8::BrightCyan);
        window.text = ColorAttr(Color8::BrightWhite, Color8::Blue);
        theme.setWindowColors(window);
        
        // Кнопки
        WidgetColors button;
        button.normal = ColorAttr(Color8::BrightWhite, Color8::Blue);
        button.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        button.disabled = ColorAttr(Color8::Gray, Color8::Blue);
        button.highlight = ColorAttr(Color8::Black, Color8::BrightCyan);
        theme.setButtonColors(button);
        
        // Меню
        WidgetColors menu;
        menu.normal = ColorAttr(Color8::BrightCyan, Color8::Blue);
        menu.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        menu.disabled = ColorAttr(Color8::Gray, Color8::Blue);
        menu.highlight = ColorAttr(Color8::Black, Color8::BrightCyan);
        theme.setMenuColors(menu);
        
        // TextBox
        WidgetColors textbox;
        textbox.normal = ColorAttr(Color8::BrightWhite, Color8::Black);
        textbox.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        textbox.disabled = ColorAttr(Color8::Gray, Color8::Black);
        theme.setTextBoxColors(textbox);
        
        // ListBox
        WidgetColors listbox;
        listbox.normal = ColorAttr(Color8::BrightWhite, Color8::Blue);
        listbox.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        listbox.disabled = ColorAttr(Color8::Gray, Color8::Blue);
        listbox.highlight = ColorAttr(Color8::Black, Color8::BrightCyan);
        theme.setListBoxColors(listbox);
        
        // Tab
        WidgetColors tab;
        tab.normal = ColorAttr(Color8::BrightCyan, Color8::Blue);
        tab.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        tab.disabled = ColorAttr(Color8::Gray, Color8::Blue);
        theme.setTabColors(tab);
        
        // StatusBar
        WidgetColors statusbar;
        statusbar.normal = ColorAttr(Color8::Black, Color8::Gray);
        statusbar.focused = ColorAttr(Color8::BrightWhite, Color8::Blue);
        statusbar.highlight = ColorAttr(Color8::BrightWhite, Color8::Blue);
        theme.setStatusBarColors(statusbar);
        
        // Dialog
        WidgetColors dialog;
        dialog.normal = ColorAttr(Color8::BrightWhite, Color8::Blue);
        dialog.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        dialog.background = ColorAttr(Color8::White, Color8::Blue);
        theme.setDialogColors(dialog);
        
        // Рамки - двойные
        theme.setBoxStyle(BoxStyles::doubleLine());
        
        return theme;
    }

    /**
     * @brief Тема Norton Commander (серый/синий)
     */
    static Theme createNortonCommander() {
        Theme theme("Norton Commander", ThemeType::NortonCommander);
        
        theme.setScreenBackground(ColorAttr(Color8::BrightMagenta, Color8::Blue));
        
        // Окна - серые с синей рамкой
        WidgetColors window;
        window.background = ColorAttr(Color8::White, Color8::Gray);
        window.normal = ColorAttr(Color8::Black, Color8::Gray);
        window.focused = ColorAttr(Color8::BrightWhite, Color8::Blue);
        window.disabled = ColorAttr(Color8::Gray, Color8::Gray);
        window.highlight = ColorAttr(Color8::BrightWhite, Color8::Blue);
        window.text = ColorAttr(Color8::Black, Color8::Gray);
        theme.setWindowColors(window);
        
        // Меню
        WidgetColors menu;
        menu.normal = ColorAttr(Color8::BrightWhite, Color8::Blue);
        menu.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        menu.disabled = ColorAttr(Color8::Gray, Color8::Blue);
        menu.highlight = ColorAttr(Color8::Black, Color8::BrightCyan);
        theme.setMenuColors(menu);
        
        // TextBox
        WidgetColors textbox;
        textbox.normal = ColorAttr(Color8::Black, Color8::Gray);
        textbox.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        textbox.disabled = ColorAttr(Color8::Gray, Color8::Gray);
        theme.setTextBoxColors(textbox);
        
        // ListBox
        WidgetColors listbox;
        listbox.normal = ColorAttr(Color8::Black, Color8::Gray);
        listbox.focused = ColorAttr(Color8::BrightWhite, Color8::Blue);
        listbox.disabled = ColorAttr(Color8::Gray, Color8::Gray);
        listbox.highlight = ColorAttr(Color8::BrightWhite, Color8::Blue);
        theme.setListBoxColors(listbox);
        
        // StatusBar - классическая NC
        WidgetColors statusbar;
        statusbar.normal = ColorAttr(Color8::BrightWhite, Color8::Blue);
        statusbar.focused = ColorAttr(Color8::Black, Color8::BrightCyan);
        statusbar.highlight = ColorAttr(Color8::Black, Color8::BrightCyan);
        theme.setStatusBarColors(statusbar);
        
        // Рамки - ASCII
        theme.setBoxStyle(BoxStyles::ascii());
        
        return theme;
    }

    /**
     * @brief Тема Терминал (зелёный на чёрном)
     */
    static Theme createTerminal() {
        Theme theme("Terminal", ThemeType::Terminal);
        
        theme.setScreenBackground(ColorAttr(Color8::Black, Color8::Black));
        
        // Основной цвет - зелёный
        WidgetColors window;
        window.background = ColorAttr(Color8::Black, Color8::Black);
        window.normal = ColorAttr(Color8::BrightGreen, Color8::Black);
        window.focused = ColorAttr(Color8::Black, Color8::BrightGreen);
        window.disabled = ColorAttr(Color8::Green, Color8::Black);
        window.highlight = ColorAttr(Color8::Black, Color8::BrightGreen);
        window.text = ColorAttr(Color8::BrightGreen, Color8::Black);
        theme.setWindowColors(window);
        
        // Кнопки
        WidgetColors button;
        button.normal = ColorAttr(Color8::BrightGreen, Color8::Black);
        button.focused = ColorAttr(Color8::Black, Color8::BrightGreen);
        button.disabled = ColorAttr(Color8::Green, Color8::Black);
        button.highlight = ColorAttr(Color8::Black, Color8::BrightGreen);
        theme.setButtonColors(button);
        
        // TextBox
        WidgetColors textbox;
        textbox.normal = ColorAttr(Color8::BrightGreen, Color8::Black);
        textbox.focused = ColorAttr(Color8::Black, Color8::BrightGreen);
        textbox.disabled = ColorAttr(Color8::Green, Color8::Black);
        theme.setTextBoxColors(textbox);
        
        // ListBox
        WidgetColors listbox;
        listbox.normal = ColorAttr(Color8::BrightGreen, Color8::Black);
        listbox.focused = ColorAttr(Color8::Black, Color8::BrightGreen);
        listbox.disabled = ColorAttr(Color8::Green, Color8::Black);
        listbox.highlight = ColorAttr(Color8::Black, Color8::BrightGreen);
        theme.setListBoxColors(listbox);
        
        // StatusBar
        WidgetColors statusbar;
        statusbar.normal = ColorAttr(Color8::Black, Color8::BrightGreen);
        statusbar.focused = ColorAttr(Color8::BrightGreen, Color8::Black);
        statusbar.highlight = ColorAttr(Color8::Black, Color8::BrightGreen);
        theme.setStatusBarColors(statusbar);
        
        // Рамки - ASCII
        theme.setBoxStyle(BoxStyles::ascii());
        
        return theme;
    }

    /**
     * @brief Монохромная тема (для старых мониторов)
     */
    static Theme createMono() {
        Theme theme("Mono", ThemeType::Mono);
        
        theme.setScreenBackground(ColorAttr(Color8::Black, Color8::Black));
        
        // Чёрно-белая гамма
        WidgetColors window;
        window.background = ColorAttr(Color8::Black, Color8::Black);
        window.normal = ColorAttr(Color8::White, Color8::Black);
        window.focused = ColorAttr(Color8::Black, Color8::White);
        window.disabled = ColorAttr(Color8::Gray, Color8::Black);
        window.highlight = ColorAttr(Color8::Black, Color8::White);
        window.text = ColorAttr(Color8::White, Color8::Black);
        theme.setWindowColors(window);
        
        // Кнопки
        WidgetColors button;
        button.normal = ColorAttr(Color8::White, Color8::Black);
        button.focused = ColorAttr(Color8::Black, Color8::White);
        button.disabled = ColorAttr(Color8::Gray, Color8::Black);
        button.highlight = ColorAttr(Color8::Black, Color8::White);
        theme.setButtonColors(button);
        
        // TextBox
        WidgetColors textbox;
        textbox.normal = ColorAttr(Color8::White, Color8::Black);
        textbox.focused = ColorAttr(Color8::Black, Color8::White);
        textbox.disabled = ColorAttr(Color8::Gray, Color8::Black);
        theme.setTextBoxColors(textbox);
        
        // ListBox
        WidgetColors listbox;
        listbox.normal = ColorAttr(Color8::White, Color8::Black);
        listbox.focused = ColorAttr(Color8::Black, Color8::White);
        listbox.disabled = ColorAttr(Color8::Gray, Color8::Black);
        listbox.highlight = ColorAttr(Color8::Black, Color8::White);
        theme.setListBoxColors(listbox);
        
        // StatusBar
        WidgetColors statusbar;
        statusbar.normal = ColorAttr(Color8::Black, Color8::White);
        statusbar.focused = ColorAttr(Color8::White, Color8::Black);
        statusbar.highlight = ColorAttr(Color8::Black, Color8::White);
        theme.setStatusBarColors(statusbar);
        
        // Рамки - ASCII
        theme.setBoxStyle(BoxStyles::ascii());
        
        return theme;
    }
};

/**
 * @brief Менеджер тем
 */
class ThemeManager {
private:
    std::unordered_map<std::string, Theme> themes_;
    Theme* currentTheme_ = nullptr;

public:
    ThemeManager() {
        // Регистрируем предопределённые темы
        themes_["bios"] = Theme::createBIOS();
        themes_["nc"] = Theme::createNortonCommander();
        themes_["terminal"] = Theme::createTerminal();
        themes_["mono"] = Theme::createMono();
        
        // По умолчанию - BIOS тема
        currentTheme_ = &themes_["bios"];
    }

    // Получить тему по имени
    Theme* getTheme(const std::string& name) {
        auto it = themes_.find(name);
        if (it != themes_.end()) {
            return &it->second;
        }
        return nullptr;
    }

    // Установить текущую тему
    bool setTheme(const std::string& name) {
        Theme* theme = getTheme(name);
        if (theme) {
            currentTheme_ = theme;
            return true;
        }
        return false;
    }

    // Получить текущую тему
    Theme* getCurrentTheme() {
        return currentTheme_;
    }

    // Добавить пользовательскую тему
    void addTheme(const std::string& name, const Theme& theme) {
        themes_[name] = theme;
    }

    // Список доступных тем
    std::vector<std::string> getThemeNames() const {
        std::vector<std::string> names;
        for (const auto& pair : themes_) {
            names.push_back(pair.first);
        }
        return names;
    }
};

} // namespace ui

#endif // TEXTUI_THEME_H
