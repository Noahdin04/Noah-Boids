cmake_minimum_required(VERSION 3.16)
project(Boids)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories("C:/msys64/mingw64/include")
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link directories
link_directories("C:/msys64/mingw64/lib")

# Add executable
add_executable(Boids src/main.cpp src/boid.cpp src/screen.cpp)

# Link only actual libraries
target_link_libraries(Boids PRIVATE
    glfw3
    glew32
    opengl32
)