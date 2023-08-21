cmake_minimum_required(VERSION 3.10)
project(learn_opengl)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set the paths for GLEW and GLFW
set(CMAKE_CXX_STANDARD 17)
set(GLEW_INCLUDE_DIRS "/opt/homebrew/Cellar/glew/2.2.0_1/include")
set(GLEW_LIBRARIES "/opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.dylib")
set(GLFW_INCLUDE_DIRS "/opt/homebrew/Cellar/glfw/3.3.8/include")
set(GLFW_LIBRARIES "/opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.dylib")
set(GLAD_INCLUDE_DIRS "/Users/henryfielding/Documents/glad/include")
set(GLAD_LIBRARIES "/Users/henryfielding/Documents/glad/src")


add_executable(learn_opengl src/main.cpp src/glad.c
        src/shaders/ShaderReader.cpp
        src/shaders/ShaderReader.h
        src/shaders/Shader.cpp
        src/shaders/Shader.h
)

# Include directories
target_include_directories(learn_opengl PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${GLAD_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(learn_opengl PRIVATE
        ${SDL2_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${GLAD_LIBRARIES}
)

# On some systems, you might need to link GLEW differently
if (APPLE)
    target_link_libraries(learn_opengl PRIVATE "-framework OpenGL")
else ()
    target_link_libraries(learn_opengl PRIVATE GLEW::GLEW)
endif ()
