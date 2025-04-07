cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(VCLIB_ALLOW_SYSTEM_QT OFF)
add_subdirectory(${LIBS_DIR}/vclib)

set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")
file(GLOB_RECURSE ABSOLUTE_SHADERS_FILES "${SHADERS_DIR}/*.sc" "${SHADERS_DIR}/*.sh")
set(RELATIVE_SHADERS_FILES "")

foreach(file ${ABSOLUTE_SHADERS_FILES})
    file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} ${file})
    list(APPEND RELATIVE_SHADERS_FILES ${relative_file})
endforeach()

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include)
target_link_libraries(main PUBLIC vclib)
target_add_bgfx_shaders(
    main
    ${RELATIVE_SHADERS_FILES}
)