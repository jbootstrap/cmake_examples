# CMakeの最低バージョンを記述
cmake_minimum_required(VERSION 2.8)
# ソリューション名を指定
project(opencv_sample)

## Unicode
add_definitions(-DUNICODE -D_UNICODE)

##
set(CMAKE_CXX_FLAGS_RELEASE "/MT /Z7 /wd4819")
set(CMAKE_CXX_FLAGS_DEBUG   "/MT /Z7 /wd4819")

# 実行ファイル名とソース指定
add_executable(main1 main1.cpp)
add_executable(main2 main2.cpp cvwin.cpp cvwin.hpp)
add_executable(main3 main3.cpp cvwin.cpp cvwin.hpp)

#
#set(OpenCV_STATIC ON)
# OpenCVインストールパスを指定
set(OpenCV_DIR "E:/local/opencv-3.1.0/build/install")
# OpenCVのパッケージを探す
find_package(OpenCV REQUIRED)
# OpenCVが見つかった場合のみ設定を行う
if(OpenCV_FOUND)
# インクルードパスを指定
include_directories(${OpenCV_INCLUDE_DIRS})
# リンクするライブラリ指定
target_link_libraries(main1 ${OpenCV_LIBS})
target_link_libraries(main2 ${OpenCV_LIBS})
target_link_libraries(main3 ${OpenCV_LIBS})
endif(OpenCV_FOUND)
