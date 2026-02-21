
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was TextUIConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

# TextUI CMake конфигурация
# Использование:
#   find_package(TextUI REQUIRED)
#   target_link_libraries(your_target PRIVATE textui::textui)

if(NOT TARGET textui::textui)
    include("${CMAKE_CURRENT_LIST_DIR}/TextUITargets.cmake")
endif()

# Проверка типа библиотеки
if(TARGET textui_shared)
    set(TEXTUI_SHARED_LIB TRUE)
elseif(TARGET textui_static)
    set(TEXTUI_STATIC_LIB TRUE)
endif()

check_required_components(TextUI)
