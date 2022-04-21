cmake_minimum_required(VERSION 3.16)

project(SFMLTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(error_flags -Wall -Wextra -Werror -Wpedantic)
add_compile_options(${error_flags})
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DDEBUG)

# Find SFML
# set(SFML_DIR "K:/lib/SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Add main
add_executable(main.out main.cpp)

# Add lib
set(SOURCES src/GameObject.cpp src/Window.cpp)
add_library(GameObject SHARED ${SOURCES})

# Add include dirs
target_include_directories(main.out PRIVATE include)
target_include_directories(GameObject PRIVATE include)

# Link libraries
target_link_libraries(main.out GameObject)
target_link_libraries(main.out sfml-graphics sfml-audio)
