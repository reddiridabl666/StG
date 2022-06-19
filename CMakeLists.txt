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

# Add memcheck
add_custom_target(memcheck COMMAND valgrind --suppressions=/lib/valgrind/sfml.supp ./main)

# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Add lib
set(SOURCES src/Resources.cpp
            src/base_classes/DrawableObject.cpp 
            src/base_classes/GameObject.cpp
            src/Window.cpp src/base_classes/SpriteObject.cpp 
            src/Hitbox.cpp src/Wall.cpp 
            src/base_classes/DynamicObject.cpp 
            src/base_classes/FramedObject.cpp src/Player.cpp 
            src/GameState.cpp src/Bullet.cpp src/TestBoss.cpp 
            src/Game.cpp src/Settings.cpp)

# Add main
add_executable(main main.cpp ${SOURCES})

# Add include dirs
set(INCLUDE_DIRS include include/utilities include/bullets include/base_classes 
                 include/final_classes include/helper_classes include/enemy include/ui)
target_include_directories(main PRIVATE ${INCLUDE_DIRS})

# Link libraries
target_link_libraries(main sfml-graphics sfml-audio)
