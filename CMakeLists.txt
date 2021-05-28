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
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug)
#set_target_properties(tutorial09_vbo_indexing PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
#create_target_launcher(tutorial09_vbo_indexing WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(projectPleaseWork
        main.cpp
        shaders/shader.hpp
        shaders/shader.cpp
        cmake-build-debug/shaders/vertexShader.vertexshader
        cmake-build-debug/shaders/fragmentShader.fragmentshader
        cmake-build-debug/shaders/uvtemplate.DDS
        shaders/texture.hpp shaders/texture.cpp shaders/controls.hpp shaders/controls.cpp
        Bodies/Deformable/spring.hpp Bodies/Deformable/spring.cpp Bodies/Deformable/net.cpp Bodies/Deformable/net.h Bodies/Deformable/particle.cpp
        Bodies/Deformable/particle.h
        imgui/imgui_internal.h imgui/imgui.h imgui/imconfig.h imgui/imgui_impl_glfw_gl3.h
        imgui/stb_rect_pack.h imgui/stb_textedit.h imgui/stb_truetype.h
        imgui/imgui_draw.cpp imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_impl_glfw_gl3.cpp Bodies/Solid/collidable.cpp
        Bodies/Solid/collidable.h Bodies/Solid/Sphere.cpp Bodies/Solid/Sphere.h Bodies/Solid/cube.cpp Bodies/Solid/cube.h
        Bodies/Solid/plane.cpp Bodies/Solid/plane.h
        utilities/helperFunctions.cpp shaders/helperStruct.h utilities/helperFunctions.h Bodies/Solid/mouseIntersectStruct.h
        BVH/clothBVH.cpp BVH/clothBVH.h utilities/multiThreading.h utilities/multiThreading.cpp
        utilities/skybox.cpp utilities/skybox.h utilities/stb_image.h Bodies/Deformable/deformableSphere.cpp
        Bodies/Deformable/deformableSphere.h Bodies/Deformable/deformableObjects.cpp Bodies/Deformable/deformableObjects.h
        BVH/defSphereBVH.cpp BVH/defSphereBVH.h BVH/BVH.cpp BVH/BVH.h Bodies/Deformable/texturedCloth.cpp Bodies/Deformable/texturedCloth.h
        Bodies/Deformable/deformableCube.cpp Bodies/Deformable/deformableCube.h BVH/defCubeBVH.cpp BVH/defCubeBVH.h Bodies/Deformable/importedModels.cpp Bodies/Deformable/importedModels.h)

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
# -O3/-O2 for optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")


#add_executable(code main.cpp)