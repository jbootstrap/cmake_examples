cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME myapp)
project(${TARGET_NAME})

## Unicode
add_definitions(-DUNICODE -D_UNICODE)

## BUILD

file(GLOB TARGET_SOURCES *(.c|*.h|.cc|*.hh|.cpp|.hpp|*.cxx|*.hxx))
add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${GENERATED_SOURCES})
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX _d)
