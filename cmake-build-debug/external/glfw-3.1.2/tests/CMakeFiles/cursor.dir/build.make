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
include external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/depend.make

# Include the progress variables for this target.
include external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/flags.make

external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/cursor.c.o: external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/flags.make
external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/cursor.c.o: ../external/glfw-3.1.2/tests/cursor.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/cursor.c.o"
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cursor.dir/cursor.c.o   -c /Users/jacobsalvi/Desktop/BachelorProject/Project/code/external/glfw-3.1.2/tests/cursor.c

external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/cursor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cursor.dir/cursor.c.i"
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jacobsalvi/Desktop/BachelorProject/Project/code/external/glfw-3.1.2/tests/cursor.c > CMakeFiles/cursor.dir/cursor.c.i

external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/cursor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cursor.dir/cursor.c.s"
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jacobsalvi/Desktop/BachelorProject/Project/code/external/glfw-3.1.2/tests/cursor.c -o CMakeFiles/cursor.dir/cursor.c.s

# Object files for target cursor
cursor_OBJECTS = \
"CMakeFiles/cursor.dir/cursor.c.o"

# External object files for target cursor
cursor_EXTERNAL_OBJECTS =

external/glfw-3.1.2/tests/cursor: external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/cursor.c.o
external/glfw-3.1.2/tests/cursor: external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/build.make
external/glfw-3.1.2/tests/cursor: external/glfw-3.1.2/src/libglfw3.a
external/glfw-3.1.2/tests/cursor: external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cursor"
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cursor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/build: external/glfw-3.1.2/tests/cursor

.PHONY : external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/build

external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/clean:
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -P CMakeFiles/cursor.dir/cmake_clean.cmake
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/clean

external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/depend:
	cd /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jacobsalvi/Desktop/BachelorProject/Project/code /Users/jacobsalvi/Desktop/BachelorProject/Project/code/external/glfw-3.1.2/tests /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests /Users/jacobsalvi/Desktop/BachelorProject/Project/code/cmake-build-debug/external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/cursor.dir/depend

