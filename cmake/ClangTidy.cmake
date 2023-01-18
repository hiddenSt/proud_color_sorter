include_guard()

function(add_clang_tidy_check)
  find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-13 clang-tidy-14 clang-tidy-15)    

  if (CLANG_TIDY-NOT_FOUND)
    message(WARNING "clang-tidy NOT FOUND")
    return()
  endif()

  message(STATUS "Found clang-tidy executable: ${CLANG_TIDY}")

  add_custom_target(clang_tidy_check 
    COMMAND ${CLANG_TIDY} -p "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy ${ARGN}
    WORKING_DIRECTORY ${PROJECT_ROOT}
    COMMENT "Applying clang-tidy to files: ${ARGN}"
  )

endfunction()