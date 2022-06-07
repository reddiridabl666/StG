cmake_minimum_required(VERSION 3.16)

project(SFMLTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(ERROR_FLAGS -Wall -Wextra -Werror -Wpedantic)
set(FLAGS -pthread)
add_compile_options(${ERROR_FLAGS} ${FLAGS})

option(DEBUG "Debug mode" OFF)

if (DEBUG)
    add_compile_options(-DDEBUG)
endif()

# add memcheck
add_custom_target(memcheck COMMAND valgrind --suppressions=/lib/valgrind/sfml.supp ./main)

# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Add main
add_executable(main main.cpp)

# Add lib
set(SOURCES src/GameObject.cpp src/Window.cpp src/SpriteObject.cpp 
            src/Player.cpp src/Hitbox.cpp src/DynamicObject.cpp 
            src/Wall.cpp src/Bullet.cpp src/Game.cpp src/FramedObject.cpp
            src/TestBoss.cpp)
add_library(GameLib SHARED ${SOURCES})

# Add include dirs
set(INCLUDE_DIRS include include/utilities include/bullets include/base_classes
                 include/final_classes include/helper_classes include/enemy)
target_include_directories(main PRIVATE ${INCLUDE_DIRS})
target_include_directories(GameLib PRIVATE ${INCLUDE_DIRS})

# Link libraries
target_link_libraries(GameLib sfml-graphics sfml-audio)
target_link_libraries(main sfml-graphics sfml-audio)
target_link_libraries(main GameLib)
