project (Dragon)
cmake_minimum_required(VERSION 2.8)

find_package(GTest)
find_package(Threads)
find_package(BISON REQUIRED)

set(SOURCES
  src/comment_remover.cpp
  src/lexer.cpp
  src/manager.cpp
  src/scanner.cpp
  src/tokenizer.cpp
)

set(TEST
  test/main.cpp

  test/comment_remover.cpp
  test/manager.cpp
  test/parser.cpp
  test/prefix_tree.cpp
  test/tokenizer.cpp
)

# if DEBUG
  add_definitions(-g -Wall -DYYDEBUG)
# endif
add_definitions (-std=c++11)

set(SCANNER_HPP_FILE "${PROJECT_SOURCE_DIR}/src/scanner.hpp")
set(AST_HPP_FILE "${PROJECT_SOURCE_DIR}/src/ast.hpp")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.hpp.in"
  "${PROJECT_BINARY_DIR}/config.hpp"
)

include_directories(${PROJECT_BINARY_DIR})

bison_target(parser src/parser.y ${PROJECT_BINARY_DIR}/parser.cpp)
add_library(dragon ${SOURCES} ${BISON_parser_OUTPUTS})

if(GTEST_FOUND)
  add_executable(drg_test ${TEST})
  include_directories(${GTEST_INCLUDE_DIRS})
  target_link_libraries(drg_test dragon ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
