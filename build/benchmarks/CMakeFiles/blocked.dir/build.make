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
CMAKE_SOURCE_DIR = /home/bigmath/a3-starter-files

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bigmath/a3-starter-files/build

# Include any dependencies generated for this target.
include benchmarks/CMakeFiles/blocked.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include benchmarks/CMakeFiles/blocked.dir/compiler_depend.make

# Include the progress variables for this target.
include benchmarks/CMakeFiles/blocked.dir/progress.make

# Include the compile flags for this target's objects.
include benchmarks/CMakeFiles/blocked.dir/flags.make

benchmarks/CMakeFiles/blocked.dir/blocked.c.o: benchmarks/CMakeFiles/blocked.dir/flags.make
benchmarks/CMakeFiles/blocked.dir/blocked.c.o: ../benchmarks/blocked.c
benchmarks/CMakeFiles/blocked.dir/blocked.c.o: benchmarks/CMakeFiles/blocked.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bigmath/a3-starter-files/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object benchmarks/CMakeFiles/blocked.dir/blocked.c.o"
	cd /home/bigmath/a3-starter-files/build/benchmarks && /usr/bin/gcc-11 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT benchmarks/CMakeFiles/blocked.dir/blocked.c.o -MF CMakeFiles/blocked.dir/blocked.c.o.d -o CMakeFiles/blocked.dir/blocked.c.o -c /home/bigmath/a3-starter-files/benchmarks/blocked.c

benchmarks/CMakeFiles/blocked.dir/blocked.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/blocked.dir/blocked.c.i"
	cd /home/bigmath/a3-starter-files/build/benchmarks && /usr/bin/gcc-11 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bigmath/a3-starter-files/benchmarks/blocked.c > CMakeFiles/blocked.dir/blocked.c.i

benchmarks/CMakeFiles/blocked.dir/blocked.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/blocked.dir/blocked.c.s"
	cd /home/bigmath/a3-starter-files/build/benchmarks && /usr/bin/gcc-11 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bigmath/a3-starter-files/benchmarks/blocked.c -o CMakeFiles/blocked.dir/blocked.c.s

# Object files for target blocked
blocked_OBJECTS = \
"CMakeFiles/blocked.dir/blocked.c.o"

# External object files for target blocked
blocked_EXTERNAL_OBJECTS =

benchmarks/blocked: benchmarks/CMakeFiles/blocked.dir/blocked.c.o
benchmarks/blocked: benchmarks/CMakeFiles/blocked.dir/build.make
benchmarks/blocked: benchmarks/CMakeFiles/blocked.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bigmath/a3-starter-files/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable blocked"
	cd /home/bigmath/a3-starter-files/build/benchmarks && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blocked.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
benchmarks/CMakeFiles/blocked.dir/build: benchmarks/blocked
.PHONY : benchmarks/CMakeFiles/blocked.dir/build

benchmarks/CMakeFiles/blocked.dir/clean:
	cd /home/bigmath/a3-starter-files/build/benchmarks && $(CMAKE_COMMAND) -P CMakeFiles/blocked.dir/cmake_clean.cmake
.PHONY : benchmarks/CMakeFiles/blocked.dir/clean

benchmarks/CMakeFiles/blocked.dir/depend:
	cd /home/bigmath/a3-starter-files/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bigmath/a3-starter-files /home/bigmath/a3-starter-files/benchmarks /home/bigmath/a3-starter-files/build /home/bigmath/a3-starter-files/build/benchmarks /home/bigmath/a3-starter-files/build/benchmarks/CMakeFiles/blocked.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : benchmarks/CMakeFiles/blocked.dir/depend
