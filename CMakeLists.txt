project (Dragon)
cmake_minimum_required(VERSION 2.8)

find_package(GTest)
find_package(Threads)

set(SOURCES
  src/comment_remover.cpp
  src/grammar.cpp
  src/lexer.cpp
  src/manager.cpp
  src/parser.cpp
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
  add_definitions(-g -Wall)
# endif
add_definitions (-std=c++11)

add_library(dragon ${SOURCES})

if(GTEST_FOUND)
  add_executable(test ${TEST})
  include_directories(${GTEST_INCLUDE_DIRS})
  target_link_libraries(test dragon ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
