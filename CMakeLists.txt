cmake_minimum_required(VERSION 3.20)
project(nbody-stars CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
add_executable(nbody-stars ${SOURCES})

target_link_libraries(nbody-stars PRIVATE raylib)
target_include_directories(nbody-stars PRIVATE src)

if(MSVC)
    target_compile_options(nbody-stars PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /DNDEBUG>
    )
else()
    target_compile_options(nbody-stars PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3 -DNDEBUG -march=native>
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET nbody-stars POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:nbody-stars>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:nbody-stars>/assets
    )
endif()