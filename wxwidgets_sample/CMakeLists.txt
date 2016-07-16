# https://cmake.org/cmake/help/v3.0/module/FindwxWidgets.html
# https://github.com/wxWidgets/wxWidgets/releases/tag/v3.0.2
cmake_minimum_required(VERSION 3.5)
project(enumRes1 CXX)
add_definitions(-DUNICODE -D_UNICODE)
add_executable(enumRes1 enumRes1.cpp)
include_directories("C:/Program Files (x86)/Java/jdk1.8.0_91/include"
                    "C:/Program Files (x86)/Java/jdk1.8.0_91/include/win32")
#set_target_properties(enumRes1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY /usr/local/bin)
find_package(wxWidgets REQUIRED net gl core base)
message("wxWidgets_CONFIG_OPTIONS=${wxWidgets_CONFIG_OPTIONS}")
message("wxWidgets_FOUND=${wxWidgets_FOUND}")
message("wxWidgets_INCLUDE_DIRS=${wxWidgets_INCLUDE_DIRS}")
message("wxWidgets_LIBRARIES=${wxWidgets_LIBRARIES}")
message("wxWidgets_LIBRARY_DIRS=${wxWidgets_LIBRARY_DIRS}")
message("wxWidgets_DEFINITIONS=${wxWidgets_DEFINITIONS}")
message("wxWidgets_DEFINITIONS_DEBUG=${wxWidgets_DEFINITIONS_DEBUG}")
message("wxWidgets_CXX_FLAGS=${wxWidgets_CXX_FLAGS}")
message("wxWidgets_USE_FILE=${wxWidgets_USE_FILE}")
include(${wxWidgets_USE_FILE})
# and for each of your dependent executable/library targets:
target_link_libraries(enumRes1 ${wxWidgets_LIBRARIES})
add_custom_command(TARGET enumRes1 POST_BUILD
    COMMAND echo ${wxWidgets_INCLUDE_DIRS}
)

add_executable(winapp WIN32 winapp.cpp)
target_link_libraries(winapp ${wxWidgets_LIBRARIES})
