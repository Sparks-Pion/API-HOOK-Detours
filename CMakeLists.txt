cmake_minimum_required(VERSION 3.23)
project(Detour_demo)
set(CMAKE_CXX_STANDARD 20)
# execution-charset: UTF-8
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
# add path to qt
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")

file(MAKE_DIRECTORY "build")

# add subdirectory
add_subdirectory(detours)
add_subdirectory(mydlls)
add_subdirectory(app)
