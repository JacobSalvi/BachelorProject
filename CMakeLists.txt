cmake_minimum_required(VERSION 3.17)
project(code)

find_package(OpenGL REQUIRED)

#Compile external dependencies
add_subdirectory(external)

include_directories(
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        imgui
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        )

# to make the executable work
#note for some reason the shaders go in the build folder
add_executable(projectPleaseWork
        main.cpp
        shaders/shader.hpp
        shaders/shader.cpp
        cmake-build-debug/shaders/vertexShader.vertexshader
        cmake-build-debug/shaders/fragmentShader.fragmentshader
        cmake-build-debug/shaders/uvtemplate.DDS
        shaders/texture.hpp shaders/texture.cpp shaders/controls.hpp shaders/controls.cpp
        integrators/spring.hpp integrators/spring.cpp integrators/net.cpp integrators/net.h integrators/particle.cpp
        integrators/particle.h
        imgui/imgui_internal.h imgui/imgui.h imgui/imconfig.h imgui/imgui_impl_glfw_gl3.h
        imgui/stb_rect_pack.h imgui/stb_textedit.h imgui/stb_truetype.h
        imgui/imgui_draw.cpp imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_impl_glfw_gl3.cpp)

target_link_libraries(projectPleaseWork
        ${ALL_LIBS}
        )

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#add_executable(code main.cpp)