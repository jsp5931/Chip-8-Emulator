cmake_minimum_required(VERSION 3.14)
project(index)

set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(index "Chip8.cpp" "Chip8.h" "Graphics.cpp" "Graphics.h" "main.cpp")


add_definitions(-DSDL_MAIN_HANDLED)


target_compile_options(index PRIVATE
    -O2
    "SHELL:-s DISABLE_EXCEPTION_CATCHING=1"
    #-std=c++17
    #-Wall
    #-Wextra
   # -Wshadow
   # -pedantic
)

target_link_options(index PRIVATE
    "SHELL:-s WASM=1"
    "SHELL:-s USE_SDL=2"
    "SHELL:-s ASSERTIONS=2"
    "SHELL:-s SAFE_HEAP=1"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=\"['ccall', 'cwrap']\""
    "SHELL:-s ALLOW_MEMORY_GROWTH=1" 
    --no-heap-copy 
    --preload-file ../roms
    --shell-file ../shell_minimal.html
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET index PROPERTY CXX_STANDARD 17)
endif()