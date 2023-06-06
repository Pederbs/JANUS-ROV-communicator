# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin

# Include any dependencies generated for this target.
include CMakeFiles/test_hop_index.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_hop_index.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_hop_index.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_hop_index.dir/flags.make

CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o: CMakeFiles/test_hop_index.dir/flags.make
CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o: ../tests/c/hop_index.c
CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o: CMakeFiles/test_hop_index.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o -MF CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o.d -o CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o -c /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/tests/c/hop_index.c

CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/tests/c/hop_index.c > CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.i

CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/tests/c/hop_index.c -o CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.s

# Object files for target test_hop_index
test_hop_index_OBJECTS = \
"CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o"

# External object files for target test_hop_index
test_hop_index_EXTERNAL_OBJECTS =

test_hop_index: CMakeFiles/test_hop_index.dir/tests/c/hop_index.c.o
test_hop_index: CMakeFiles/test_hop_index.dir/build.make
test_hop_index: libjanus.a
test_hop_index: /usr/local/lib/libfftw3.a
test_hop_index: CMakeFiles/test_hop_index.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_hop_index"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_hop_index.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_hop_index.dir/build: test_hop_index
.PHONY : CMakeFiles/test_hop_index.dir/build

CMakeFiles/test_hop_index.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_hop_index.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_hop_index.dir/clean

CMakeFiles/test_hop_index.dir/depend:
	cd /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5 /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5 /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin /home/peder/GitHub/JANUS-ROV-communicator/src/nodecomx_cpp_py/include/lib/janus-c-3.0.5/bin/CMakeFiles/test_hop_index.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_hop_index.dir/depend
