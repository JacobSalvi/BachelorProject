# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jacobsalvi/Desktop/BachelorProject/Project/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/projectPleaseWork.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/projectPleaseWork.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/projectPleaseWork.dir/flags.make

CMakeFiles/projectPleaseWork.dir/main.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/projectPleaseWork.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/main.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/main.cpp

CMakeFiles/projectPleaseWork.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/main.cpp > CMakeFiles/projectPleaseWork.dir/main.cpp.i

CMakeFiles/projectPleaseWork.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/main.cpp -o CMakeFiles/projectPleaseWork.dir/main.cpp.s

CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.o: ../shaders/shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/shader.cpp

CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/shader.cpp > CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.i

CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/shader.cpp -o CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.s

CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.o: ../shaders/texture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/texture.cpp

CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/texture.cpp > CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.i

CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/texture.cpp -o CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.s

CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.o: ../shaders/controls.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/controls.cpp

CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/controls.cpp > CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.i

CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/controls.cpp -o CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.s

CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.o: ../integrators/spring.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/spring.cpp

CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/spring.cpp > CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.i

CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/spring.cpp -o CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.s

CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.o: ../integrators/net.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/net.cpp

CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/net.cpp > CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.i

CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/net.cpp -o CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.s

CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.o: ../integrators/particle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/particle.cpp

CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/particle.cpp > CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.i

CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/integrators/particle.cpp -o CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.s

CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.o: ../imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_draw.cpp

CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_draw.cpp > CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.i

CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_draw.cpp -o CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.s

CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.o: ../imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui.cpp

CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui.cpp > CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.i

CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui.cpp -o CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.s

CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.o: ../imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_demo.cpp

CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_demo.cpp > CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.i

CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_demo.cpp -o CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.s

CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.o: ../imgui/imgui_impl_glfw_gl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_impl_glfw_gl3.cpp

CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_impl_glfw_gl3.cpp > CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.i

CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/imgui/imgui_impl_glfw_gl3.cpp -o CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.s

CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.o: ../Collidables/collidable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/collidable.cpp

CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/collidable.cpp > CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.i

CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/collidable.cpp -o CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.s

CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.o: ../Collidables/Sphere.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/Sphere.cpp

CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/Sphere.cpp > CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.i

CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/Sphere.cpp -o CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.s

CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.o: ../Collidables/cube.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/cube.cpp

CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/cube.cpp > CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.i

CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/cube.cpp -o CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.s

CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.o: ../Collidables/plane.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/plane.cpp

CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/plane.cpp > CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.i

CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/Collidables/plane.cpp -o CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.s

CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.o: CMakeFiles/projectPleaseWork.dir/flags.make
CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.o: ../shaders/helperFunctions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.o -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/helperFunctions.cpp

CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/helperFunctions.cpp > CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.i

CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/shaders/helperFunctions.cpp -o CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.s

# Object files for target projectPleaseWork
projectPleaseWork_OBJECTS = \
"CMakeFiles/projectPleaseWork.dir/main.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.o" \
"CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.o"

# External object files for target projectPleaseWork
projectPleaseWork_EXTERNAL_OBJECTS =

projectPleaseWork: CMakeFiles/projectPleaseWork.dir/main.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/shaders/shader.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/shaders/texture.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/shaders/controls.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/integrators/spring.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/integrators/net.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/integrators/particle.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/imgui/imgui_draw.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/imgui/imgui.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/imgui/imgui_demo.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/imgui/imgui_impl_glfw_gl3.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/Collidables/collidable.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/Collidables/Sphere.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/Collidables/cube.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/Collidables/plane.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/shaders/helperFunctions.cpp.o
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/build.make
projectPleaseWork: external/glfw-3.1.2/src/libglfw3.a
projectPleaseWork: external/libGLEW_1130.a
projectPleaseWork: CMakeFiles/projectPleaseWork.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking CXX executable projectPleaseWork"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/projectPleaseWork.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/projectPleaseWork.dir/build: projectPleaseWork

.PHONY : CMakeFiles/projectPleaseWork.dir/build

CMakeFiles/projectPleaseWork.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/projectPleaseWork.dir/cmake_clean.cmake
.PHONY : CMakeFiles/projectPleaseWork.dir/clean

CMakeFiles/projectPleaseWork.dir/depend:
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jacobsalvi/Desktop/BachelorProject/Project/code /Users/jacobsalvi/Desktop/BachelorProject/Project/code /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles/projectPleaseWork.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/projectPleaseWork.dir/depend

