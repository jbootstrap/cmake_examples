# https://cmake.org/cmake/help/v3.5/module/FindJava.html
# https://cmake.org/cmake/help/v3.5/module/UseJava.html

cmake_minimum_required(VERSION 3.5)

#find_package(Java 1.8 COMPONENTS Runtime Development)
find_package(Java 1.7 COMPONENTS Runtime Development)
include(UseJava)

enable_testing()

project (HelloWorld)

set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.7" "-target" "1.7" "-encoding" "utf-8")

add_jar(HelloWorld
        ENTRY_POINT swigtest1.SwigTest1
        SOURCES swigtest1/SwigTest1.java native/x86/mydll.dll native/x64/mydll.dll)

get_target_property(_jarFile HelloWorld JAR_FILE)
get_target_property(_classDir HelloWorld CLASSDIR)

message(STATUS "Jar file ${_jarFile}")
message(STATUS "Class compiled to ${_classDir}")

add_test(NAME Test1 COMMAND ${Java_JAVA_EXECUTABLE} -jar ${_jarFile})
