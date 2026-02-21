cmake_minimum_required(VERSION 3.16)

# Проект
project(TextUI
    VERSION 3.0.0
    DESCRIPTION "Cross-platform text UI framework - BIOS Style"
    LANGUAGES CXX
)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции сборки
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(TEXTUI_BUILD_SHARED "Build shared library (DLL/.so)" ON)
option(TEXTUI_BUILD_STATIC "Build static library" ON)
option(TEXTUI_INSTALL "Install targets" ON)

# Позиционно-независимый код
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Макрос экспорта для Windows DLL
set(TEXTUI_EXPORT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/textui/Export.h")

# Исходники (для статической библиотеки)
set(TEXTUI_SOURCES
    core/Screen.cpp
    core/Input.cpp
)

# Заголовочные файлы
set(TEXTUI_PUBLIC_HEADERS
    include/textui/App.h
    include/textui/Screen.h
    include/textui/Input.h
    include/textui/Colors.h
    include/textui/Chars.h
    include/textui/Theme.h
    include/textui/Widget.h
    include/textui/Window.h
    include/textui/Button.h
    include/textui/Label.h
    include/textui/TextBox.h
    include/textui/CheckBox.h
    include/textui/RadioButton.h
    include/textui/ProgressBar.h
    include/textui/ListBox.h
    include/textui/Menu.h
    include/textui/TabControl.h
    include/textui/StatusBar.h
    include/textui/DropDown.h
    include/textui/MessageBox.h
    include/textui/Export.h
)

# Функция для настройки библиотеки
function(configure_textui_target target_name target_type)
    # Исходники
    if(target_type STREQUAL "SHARED")
        target_sources(${target_name} PRIVATE ${TEXTUI_SOURCES})
    endif()
    
    # Заголовки
    target_include_directories(${target_name}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    
    # Компилятор
    target_compile_features(${target_name} PUBLIC cxx_std_17)
    
    # Платформенные настройки
    if(WIN32)
        target_compile_definitions(${target_name}
            PUBLIC
                $<$<BOOL:${BUILD_SHARED_LIBS}>:TEXTUI_SHARED>
                $<$<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<COMPILE_LANGUAGE:CXX>>:TEXTUI_EXPORTS>
        )
        
        if(MSVC)
            target_compile_options(${target_name} PRIVATE /W4 /WX-)
        elseif(MINGW)
            target_compile_options(${target_name} PRIVATE -Wall -Wextra)
            if(target_type STREQUAL "SHARED")
                target_link_options(${target_name} PRIVATE -static-libgcc -static-libstdc++)
            endif()
        endif()
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra)
        if(target_type STREQUAL "SHARED")
            target_link_options(${target_name} PRIVATE -static-libgcc -static-libstdc++)
        endif()
    endif()
    
    # Версия
    set_target_properties(${target_name} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${TEXTUI_PUBLIC_HEADERS}"
    )
endfunction()

# Создаём библиотеки
if(TEXTUI_BUILD_SHARED)
    add_library(textui_shared SHARED ${TEXTUI_SOURCES})
    configure_textui_target(textui_shared SHARED)
    set_target_properties(textui_shared PROPERTIES OUTPUT_NAME textui)
endif()

if(TEXTUI_BUILD_STATIC)
    add_library(textui_static STATIC ${TEXTUI_SOURCES})
    configure_textui_target(textui_static STATIC)
    set_target_properties(textui_static PROPERTIES OUTPUT_NAME textui-static)
    target_compile_definitions(textui_static PUBLIC TEXTUI_STATIC)
endif()

# Алиас для удобства
if(TEXTUI_BUILD_SHARED)
    add_library(textui::textui ALIAS textui_shared)
else()
    add_library(textui::textui ALIAS textui_static)
endif()

# Установка
if(TEXTUI_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
    
    # Библиотеки
    if(TEXTUI_BUILD_SHARED)
        install(TARGETS textui_shared
            EXPORT textuiTargets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/textui
        )
    endif()
    
    if(TEXTUI_BUILD_STATIC)
        install(TARGETS textui_static
            EXPORT textuiTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/textui
        )
    endif()
    
    # Заголовки
    install(DIRECTORY core/ graphics/ widgets/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/textui
        FILES_MATCHING PATTERN "*.h"
    )
    
    # CMake конфиг
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/TextUIConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TextUIConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/TextUIConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TextUI
    )
    
    install(EXPORT textuiTargets
        FILE TextUITargets.cmake
        NAMESPACE textui::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TextUI
    )
    
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TextUIConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TextUIConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TextUI
    )
endif()

# Вывод информации
message(STATUS "")
message(STATUS "TextUI Framework v${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared library: ${TEXTUI_BUILD_SHARED}")
message(STATUS "  Static library: ${TEXTUI_BUILD_STATIC}")
message(STATUS "  Install: ${TEXTUI_INSTALL}")
message(STATUS "")
