cmake_minimum_required(VERSION 3.20)
project(gifutil C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(stb)

add_library(stb_lib INTERFACE)
target_include_directories(stb_lib INTERFACE ${stb_SOURCE_DIR})
# math library usually needed for stb on linux/unix
if(UNIX AND NOT APPLE)
    link_libraries(m)
endif()

set(SOURCES
    main.c
    gifutil.c
    extract.c
    inspect.c
)

add_executable(gifutil ${SOURCES})
target_link_libraries(gifutil PRIVATE stb_lib)
target_include_directories(gifutil PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})