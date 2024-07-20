cmake_minimum_required(VERSION 3.28)
project(A08)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

# Add GLFW3 precompiled library
find_package(glfw3 REQUIRED)

# Headers (+ glm)
include_directories(../headers)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)

# Find GLSLC
find_program(GLSLC_EXECUTABLE NAMES glslc HINTS Vulkan::glslc)

# If GLSLC is not found, copy the shader files to the cmake-build-debug folder
if (NOT GLSLC_EXECUTABLE)
    message(WARNING "GLSLC not found, copying compiled shader files if they exist")
else ()
    message(STATUS "GLSLC found at ${GLSLC_EXECUTABLE}")
    message(STATUS "Compiling shaders")

    # Compile shaders
    function(compile_shader SOURCE TARGET)
        add_custom_command(
                OUTPUT ${TARGET}
                COMMAND ${GLSLC_EXECUTABLE} ${SOURCE} -o ${TARGET}
                MAIN_DEPENDENCY ${SOURCE}
        )
        target_sources(${PROJECT_NAME} PRIVATE ${TARGET})
    endfunction()
    message(STATUS "Compiling shaders")
    file(GLOB SHADERS "shaders/*.vert" "shaders/*.frag")
    foreach(SHADER ${SHADERS})
        # Convert SomethingShader.vert to SomethingVert.spv and SomethingShader.frag to SomethingFrag.spv
        get_filename_component(DIR ${SHADER} DIRECTORY)
        get_filename_component(FILENAME ${SHADER} NAME_WE)
        string(REPLACE "Shader" "" BASE_FILENAME ${FILENAME})
        get_filename_component(EXTENSION ${SHADER} EXT)
        if(${EXTENSION} STREQUAL ".vert")
            set(NEW_FILENAME "${BASE_FILENAME}Vert.spv")
        elseif(${EXTENSION} STREQUAL ".frag")
            set(NEW_FILENAME "${BASE_FILENAME}Frag.spv")
        endif()
        set(NEW_FILEPATH "${DIR}/${NEW_FILENAME}")
        compile_shader(${SHADER} ${NEW_FILEPATH})
    endforeach()
    message(STATUS "Shaders compiled")
endif ()

message(STATUS "Copying textures, models and shaders")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_BINARY_DIR}/models
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.spv
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/
)

# Set ADDITIONAL_CLEAN_FILES to the list of files to be cleaned adding the copied files
set_target_properties(${PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/shaders/;${CMAKE_CURRENT_BINARY_DIR}/textures/;${CMAKE_CURRENT_BINARY_DIR}/models/")
