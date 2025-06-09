cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(vulk C)

# Set C standard and compile options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/vulkan
    /usr/include/GLFW
    ${PROJECT_SOURCE_DIR}/libs/cglm/include
)

# Link directories
link_directories(
    /usr/lib
    ${PROJECT_SOURCE_DIR}/libs/glfw/lib
)

# Source files
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.c)

# Output directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Define the executable
add_executable(vulk ${SRCS})

# Link libraries
target_link_libraries(vulk glfw vulkan m)
