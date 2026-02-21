#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "textui::textui_shared" for configuration "Release"
set_property(TARGET textui::textui_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(textui::textui_shared PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libtextui.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libtextui.dll"
  )

list(APPEND _cmake_import_check_targets textui::textui_shared )
list(APPEND _cmake_import_check_files_for_textui::textui_shared "${_IMPORT_PREFIX}/lib/libtextui.dll.a" "${_IMPORT_PREFIX}/bin/libtextui.dll" )

# Import target "textui::textui_static" for configuration "Release"
set_property(TARGET textui::textui_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(textui::textui_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtextui-static.a"
  )

list(APPEND _cmake_import_check_targets textui::textui_static )
list(APPEND _cmake_import_check_files_for_textui::textui_static "${_IMPORT_PREFIX}/lib/libtextui-static.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
