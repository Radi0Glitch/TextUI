#ifndef TEXTUI_EXPORT_H
#define TEXTUI_EXPORT_H

// Макрос экспорта для Windows DLL
#if defined(_WIN32) || defined(_WIN64)
    #ifdef TEXTUI_EXPORTS
        #define TEXTUI_API __declspec(dllexport)
    #elif defined(TEXTUI_SHARED)
        #define TEXTUI_API __declspec(dllimport)
    #else
        #define TEXTUI_API
    #endif
#else
    // Linux/macOS - символы экспортируются по умолчанию
    #define TEXTUI_API __attribute__((visibility("default")))
#endif

// Отключение предупреждений
#ifdef _MSC_VER
    #pragma warning(disable: 4251) // DLL interface for class members
    #pragma warning(disable: 4275) // Non-DLL interface for base class
#endif

#endif // TEXTUI_EXPORT_H
