cmake_minimum_required(VERSION 3.28)
project(E08)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} E08.cpp)

# Add GLFW3 precompiled library
add_library(glfw SHARED IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libglfw.3.dylib")

# Headers (+ glm)
include_directories(../headers)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)


# copy the shader files to the cmake-build-debug folder
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# copy the texture files to the cmake-build-debug folder
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# copy the model files to the cmake-build-debug folder
file(COPY models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})