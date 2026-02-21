cmake_minimum_required(VERSION 3.10)

# Проект
project(TextUI
    VERSION 3.0.0
    DESCRIPTION "Cross-platform text UI framework - BIOS Style"
    LANGUAGES CXX
)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Статическая линковка для Windows
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Исходники фреймворка (заголовочные файлы)
set(TEXTUI_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Screen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Input.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core/App.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/Colors.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/Chars.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/Theme.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/Widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/Window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/Button.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/Label.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/TextBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/CheckBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/RadioButton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/ProgressBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/ListBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/Menu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/TabControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/StatusBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/DropDown.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/MessageBox.h
)

# Создаем интерфейс библиотеки (только заголовки)
add_library(textui INTERFACE)
target_include_directories(textui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Примеры
option(BUILD_EXAMPLES "Build example applications" OFF)

if(BUILD_EXAMPLES)
    message(WARNING "No examples available")
endif()

# Установка
install(TARGETS textui
    EXPORT textuiTargets
    INCLUDES DESTINATION include
)

install(DIRECTORY core graphics widgets
    DESTINATION include/textui
    FILES_MATCHING PATTERN "*.h"
)

# Вывод информации
message(STATUS "")
message(STATUS "TextUI Framework v${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Examples: ${BUILD_EXAMPLES}")
message(STATUS "")
