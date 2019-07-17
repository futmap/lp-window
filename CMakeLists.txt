cmake_minimum_required(VERSION 3.0.0)

project("Cross-Platform-Window")

if(UNIX)

    set(source "main.c" "WindowLinux.c")
    set(CMAKE_PLATFORM_INFO_NAME "\"Unix-Linux\"")

elseif(MSVC)

    set(source "main.c" "WindowWin32.c")
    set(CMAKE_PLATFORM_INFO_NAME "\"Windows\"")

endif(UNIX)


configure_file(
    ${PROJECT_SOURCE_DIR}/CMakePlatformInfo.h.in
    ${PROJECT_SOURCE_DIR}/CMakePlatformInfo.h
)

add_executable(${PROJECT_NAME} ${source})


if(UNIX)

target_link_libraries(${PROJECT_NAME} xcb)

elseif(MSVC)

target_link_libraries(${PROJECT_NAME} gdi32)

endif(UNIX)
