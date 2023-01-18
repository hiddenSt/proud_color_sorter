cmake_minimum_required(VERSION 3.15)

project(proud_color_sorter
        VERSION 1.0.0
        DESCRIPTION "Who am I?"
        LANGUAGES CXX)

#--------------------------------------------------------------------
# Settings an options
#--------------------------------------------------------------------

option(Proud_Color_Sorter_WARNINGS_AS_ERRORS "Turn all warnings into errors" OFF)
option(ENABLE_DEVELOPER_MODE "Enables analyses" OFF)

include(GNUInstallDirs)
include(cmake/Sanitizers.cmake)
include(cmake/ClangTidy.cmake)
include(cmake/CompilerWarnings.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

#--------------------------------------------------------------------
# Dependencies
#--------------------------------------------------------------------

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(CLI11 REQUIRED)

#--------------------------------------------------------------------
# Sources
#--------------------------------------------------------------------

set(sources
    src/concurrent/mpmc_queue.hpp
    src/counting_sort.cpp
    src/counting_sort.hpp
    src/order.hpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

if (ENABLE_DEVELOPER_MODE)
    add_clang_tidy_check(${sources})
endif()

#--------------------------------------------------------------------
# Targets
#--------------------------------------------------------------------

add_library(${PROJECT_NAME}_objs OBJECT)
target_include_directories(${PROJECT_NAME}_objs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}_objs 
  PUBLIC
    Threads::Threads
    CLI11::CLI11
    fmt::fmt)

target_sources(${PROJECT_NAME}_objs 
  PRIVATE
    ${sources}
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_NAME}_objs
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_project_warnings(
  ${PROJECT_NAME}_objs
  "${Proud_Color_Sorter_WARNINGS_AS_ERRORS}"
  "${Proud_Color_Sorter_MSVC_WARNINGS}"
  "${Proud_Color_Sorter_CLANG_WARNINGS}"
  "${Proud_Color_Sorter_GCC_WARNINGS}"
)

if (ENABLE_DEVELOPER_MODE)
  set(Proud_Coloer_Sorter_WARNINGS_AS_ERRORS ON)
  set(BUILD_TESTING ON)
  enable_sanitizers(${PROJECT_NAME}_objs)
  enable_sanitizers(${PROJECT_NAME})
endif()

#--------------------------------------------------------------------
# Install set
#--------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME}
)

#--------------------------------------------------------------------
# Tests
#--------------------------------------------------------------------

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
