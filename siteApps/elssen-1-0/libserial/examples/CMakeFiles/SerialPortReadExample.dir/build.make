# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ctrluser/GitHub/libserial

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ctrluser/GitHub/libserial

# Include any dependencies generated for this target.
include examples/CMakeFiles/SerialPortReadExample.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/SerialPortReadExample.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/SerialPortReadExample.dir/flags.make

examples/CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.o: examples/CMakeFiles/SerialPortReadExample.dir/flags.make
examples/CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.o: examples/serial_port_read.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ctrluser/GitHub/libserial/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.o"
	cd /home/ctrluser/GitHub/libserial/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.o -c /home/ctrluser/GitHub/libserial/examples/serial_port_read.cpp

examples/CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.i"
	cd /home/ctrluser/GitHub/libserial/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ctrluser/GitHub/libserial/examples/serial_port_read.cpp > CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.i

examples/CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.s"
	cd /home/ctrluser/GitHub/libserial/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ctrluser/GitHub/libserial/examples/serial_port_read.cpp -o CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.s

# Object files for target SerialPortReadExample
SerialPortReadExample_OBJECTS = \
"CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.o"

# External object files for target SerialPortReadExample
SerialPortReadExample_EXTERNAL_OBJECTS =

bin/SerialPortReadExample: examples/CMakeFiles/SerialPortReadExample.dir/serial_port_read.cpp.o
bin/SerialPortReadExample: examples/CMakeFiles/SerialPortReadExample.dir/build.make
bin/SerialPortReadExample: lib/libserial.a
bin/SerialPortReadExample: examples/CMakeFiles/SerialPortReadExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ctrluser/GitHub/libserial/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/SerialPortReadExample"
	cd /home/ctrluser/GitHub/libserial/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SerialPortReadExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/SerialPortReadExample.dir/build: bin/SerialPortReadExample

.PHONY : examples/CMakeFiles/SerialPortReadExample.dir/build

examples/CMakeFiles/SerialPortReadExample.dir/clean:
	cd /home/ctrluser/GitHub/libserial/examples && $(CMAKE_COMMAND) -P CMakeFiles/SerialPortReadExample.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/SerialPortReadExample.dir/clean

examples/CMakeFiles/SerialPortReadExample.dir/depend:
	cd /home/ctrluser/GitHub/libserial && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ctrluser/GitHub/libserial /home/ctrluser/GitHub/libserial/examples /home/ctrluser/GitHub/libserial /home/ctrluser/GitHub/libserial/examples /home/ctrluser/GitHub/libserial/examples/CMakeFiles/SerialPortReadExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/SerialPortReadExample.dir/depend
