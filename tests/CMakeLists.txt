include(${PROJECT_ROOT}/cmake/FetchGtest.cmake)

add_executable(${PROJECT_NAME}_tests algo_test.cpp)
target_link_libraries(${PROJECT_NAME}_tests
  PRIVATE
    ${PROJECT_NAME}_objs
    GTest::gtest_main
)

target_include_directories(${PROJECT_NAME}_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

enable_sanitizers(${PROJECT_NAME}_tests)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)