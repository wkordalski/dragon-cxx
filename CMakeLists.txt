project (Dragon)
cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(Boost REQUIRED program_options serialization)
find_package(GTest)
find_package(LLVM REQUIRED CONFIG)
find_package(Threads)


set(SOURCES
  src/ast/source.cpp

  src/ast/builtins/integer.cpp

  src/ast/declarations/variable.cpp

  src/lexer/comment_remover.cpp
  src/lexer/lexer.cpp
  src/lexer/scanner.cpp
  src/lexer/tokenizer.cpp

  src/utils/collect.cpp
  src/utils/manager.cpp

  src/visitors/assembler.cpp
  src/visitors/exporter.cpp
  src/visitors/gc.cpp
  src/visitors/importer.cpp
  src/visitors/printer.cpp
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
add_definitions (-std=c++14)

set(SCANNER_HPP_FILE "${PROJECT_SOURCE_DIR}/src/lexer/scanner.hpp")
set(SYNTACTIC_HPP_FILE "${PROJECT_SOURCE_DIR}/src/ast/syntactic.hpp")

configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.hpp.in"
  "${PROJECT_BINARY_DIR}/config.hpp"
)

include_directories(${PROJECT_BINARY_DIR})

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${Boost_INCLUDE_DIRS})

bison_target(parser src/parser.y ${PROJECT_BINARY_DIR}/parser.cpp)
llvm_map_components_to_libnames(LLVM_LIBRARIES core codegen)
add_library(dragon SHARED ${SOURCES} ${BISON_parser_OUTPUTS})
target_link_libraries(dragon ${LLVM_LIBRARIES} ${Boost_SERIALIZATION_LIBRARY})

if(GTEST_FOUND)
  add_executable(utest ${TEST})
  include_directories(${GTEST_INCLUDE_DIRS})
  target_link_libraries(utest dragon ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()


add_executable(astdbg tools/astdbg/main.cpp)
target_link_libraries(astdbg dragon)

add_executable(drgc tools/drgc/main.cpp)
target_link_libraries(drgc dragon)

add_executable(dragonfly tools/dragonfly/main.cpp)
target_link_libraries(dragonfly dragon)
