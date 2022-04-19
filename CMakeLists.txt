cmake_minimum_required(VERSION 3.21)

set(MAIN SFMLTest)
project(${MAIN})

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DDEBUG)

# Find SFML
set(SFML_DIR "K:/lib/SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Add main
add_executable(${MAIN} main.cpp src/GameObject.cpp)

# Link libraries
target_include_directories(${MAIN} PRIVATE include)
target_link_libraries(${MAIN} sfml-graphics sfml-audio)
