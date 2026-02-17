cmake_minimum_required(VERSION 3.10)
project(TextUI VERSION 1.0.0 LANGUAGES CXX)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Позиционные аргументы в cmake
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_definitions(-DWINDOWS_PLATFORM)
else()
    add_definitions(-DUNIX_PLATFORM)
endif()

# Исходные файлы ядра
set(CORE_SOURCES
    core/RenderBuffer.cpp
    core/Renderer.cpp
    core/Terminal.cpp
    core/SimpleAPI.cpp
    core/WidgetManager.cpp
)

# Исходные файлы платформы
if(WIN32)
    set(PLATFORM_SOURCES
        platform/WindowsPlatform.cpp
    )
else()
    set(PLATFORM_SOURCES
        platform/UnixPlatform.cpp
    )
endif()

# Исходные файлы окон
set(WINDOW_SOURCES
    window/Window.cpp
    window/Dialog.cpp
    window/ModalDialog.cpp
)

# Исходные файлы ввода
set(INPUT_SOURCES
    input/Mouse.cpp
)

# Все исходные файлы библиотеки
set(TEXTUI_SOURCES
    ${CORE_SOURCES}
    ${PLATFORM_SOURCES}
    ${WINDOW_SOURCES}
    ${INPUT_SOURCES}
)

# Создаем статическую библиотеку
add_library(textui STATIC ${TEXTUI_SOURCES})

# Добавляем пути для заголовочных файлов
target_include_directories(textui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/window
)

# Пример использования
add_executable(textui_example examples/main.cpp)
target_link_libraries(textui_example PRIVATE textui)

# Установка
install(TARGETS textui
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY core widgets window input platform
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
