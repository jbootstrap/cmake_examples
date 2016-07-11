cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME mydll)
project(${TARGET_NAME})

## Unicode
add_definitions(-DUNICODE -D_UNICODE)

## Java(JNI)
include_directories("C:/Program Files (x86)/Java/jdk1.8.0_92/include"
                    "C:/Program Files (x86)/Java/jdk1.8.0_92/include/win32")

## BUILD

file(GLOB TARGET_SOURCES *(.c|*.h|.cc|*.hh|.cpp|.hpp|*.cxx|*.hxx))
add_library(${TARGET_NAME} SHARED ${TARGET_SOURCES} ${GENERATED_SOURCES})
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX _d)
