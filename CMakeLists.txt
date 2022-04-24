cmake_minimum_required(VERSION 3.16)

project(SFMLTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(ERROR_FLAGS -Wall -Wextra -Werror -Wpedantic)
add_compile_options(${ERROR_FLAGS})
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DDEBUG)

# Find SFML
set(SFML_DIR "K:/lib/SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Add main
add_executable(main main.cpp)

# Add lib
set(SOURCES src/GameObject.cpp src/Window.cpp src/HitboxObject.cpp src/Player.cpp)
add_library(GameLib SHARED ${SOURCES})

# Add include dirs
target_include_directories(main PRIVATE include)
target_include_directories(GameLib PRIVATE include)

# Link libraries
target_link_libraries(GameLib sfml-graphics sfml-audio)
target_link_libraries(main sfml-graphics sfml-audio)
target_link_libraries(main GameLib)
