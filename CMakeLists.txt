cmake_minimum_required(VERSION 3.0.0)
project(poludis VERSION 0.1.0)

INCLUDE_DIRECTORIES("/usr/include/mozjs-102/")

find_package(PkgConfig REQUIRED)
pkg_check_modules(MOZJS REQUIRED mozjs-102)

add_executable(poludis src/main.cpp src/boilerplate.hpp src/boilerplate.cpp)

target_link_libraries(poludis PRIVATE ${MOZJS_LIBRARIES})
target_include_directories(poludis PRIVATE ${MOZJS_INCLUDE_DIRS})
target_link_directories(poludis PRIVATE ${MOZJS_LIBRARY_DIRS})
