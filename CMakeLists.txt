cmake_minimum_required(VERSION 3.26)
project(chip-8-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(chip-8-emulator
        src/kernel.cpp
        src/memory.cpp
        src/display.cpp
        src/main.cpp
        src/cartridge.cpp
        src/speakers.cpp)

target_link_libraries(chip-8-emulator PRIVATE sfml-graphics sfml-audio)
target_compile_features(chip-8-emulator PRIVATE cxx_std_20)

install(TARGETS chip-8-emulator)
