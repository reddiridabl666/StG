cmake_minimum_required(VERSION 3.16)

project(SFMLTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(ERROR_FLAGS -Wall -Wextra -Werror -Wpedantic)
add_compile_options(${ERROR_FLAGS})
# add_compile_options(-DDEBUG)

# add memcheck
add_custom_target(memcheck COMMAND valgrind --tool=memcheck --leak-check=full --suppressions=/lib/valgrind/sfml.supp ./main)
# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Add main
add_executable(main main.cpp)

# Add lib
set(SOURCES src/GameObject.cpp src/Window.cpp src/Player.cpp src/Hitbox.cpp src/DynamicObject.cpp src/Wall.cpp)
add_library(GameLib SHARED ${SOURCES})

# Add include dirs
target_include_directories(main PRIVATE include)
target_include_directories(GameLib PRIVATE include)

# Link libraries
target_link_libraries(GameLib sfml-graphics sfml-audio)
target_link_libraries(main sfml-graphics sfml-audio)
target_link_libraries(main GameLib)
