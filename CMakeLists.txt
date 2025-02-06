cmake_minimum_required(VERSION 3.7)

set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(VCLIB_ALLOW_SYSTEM_QT OFF)
add_subdirectory(libs/vclib)
add_executable(main ${SRC_DIR}/main.cpp)
target_link_libraries(main PUBLIC vclib)
