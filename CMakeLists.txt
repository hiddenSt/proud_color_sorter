cmake_minimum_required(VERSION 3.15)

project(proud_color_sorter
        VERSION 1.0.0
        DESCRIPTION "Who am I?"
        LANGUAGES CXX)

include(GNUInstallDirs)

#--------------------------------------------------------------------
# Settings an options
#--------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED PROUD_SORTER_WARNINGS)
    if (MSVC)
      set(PROUD_SORTER_WARNINGS "/Wall")
    else()
      set(PROUD_SORTER_WARNINGS "-Wall -Wextra -Wconversion -Wpedantic")
    endif()
endif()

#--------------------------------------------------------------------
# Dependencies
#--------------------------------------------------------------------

find_package(Threads REQUIRED)

include(cmake/Sanitizers.cmake)

#--------------------------------------------------------------------
# Sources
#--------------------------------------------------------------------
## source_group ???
#--------------------------------------------------------------------
# Targets
#--------------------------------------------------------------------

add_library(${PROJECT_NAME}_objs OBJECT)
target_include_directories(${PROJECT_NAME}_objs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}_objs PUBLIC Threads::Threads)
target_sources(${PROJECT_NAME}_objs 
  PRIVATE
    src/obj.hpp
    src/obj.cpp
)

separate_arguments(WARNING_FLAGS NATIVE_COMMAND ${PROUD_SORTER_WARNINGS})
target_compile_options(${PROJECT_NAME}_objs PUBLIC ${WARNING_FLAGS})
enable_sanitizers(${PROJECT_NAME}_objs)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_NAME}_objs
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_sanitizers(${PROJECT_NAME})

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