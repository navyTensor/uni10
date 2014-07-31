# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yundasie/TensorNetwork/guni10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yundasie/TensorNetwork/guni10/build

# Include any dependencies generated for this target.
include CMakeFiles/uni10.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/uni10.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/uni10.dir/flags.make

# Object files for target uni10
uni10_OBJECTS =

# External object files for target uni10
uni10_EXTERNAL_OBJECTS = \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/datatype/lib/CMakeFiles/uni10-datatype.dir/Qnum.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/data-structure/lib/CMakeFiles/uni10-data-structure.dir/Block.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/data-structure/lib/CMakeFiles/uni10-data-structure.dir/Bond.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/tools/lib/CMakeFiles/uni10-tools.dir/uni10_tools.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/Network.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/UniTensor.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/UniTensor_elem.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/Matrix.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/operate.cpp.o" \
"/home/yundasie/TensorNetwork/guni10/build/src/uni10/numeric/lib/CMakeFiles/uni10-numeric.dir/uni10_lapack.cpp.o"

lib/libuni10.so: src/uni10/datatype/lib/CMakeFiles/uni10-datatype.dir/Qnum.cpp.o
lib/libuni10.so: src/uni10/data-structure/lib/CMakeFiles/uni10-data-structure.dir/Block.cpp.o
lib/libuni10.so: src/uni10/data-structure/lib/CMakeFiles/uni10-data-structure.dir/Bond.cpp.o
lib/libuni10.so: src/uni10/tools/lib/CMakeFiles/uni10-tools.dir/uni10_tools.cpp.o
lib/libuni10.so: src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/Network.cpp.o
lib/libuni10.so: src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/UniTensor.cpp.o
lib/libuni10.so: src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/UniTensor_elem.cpp.o
lib/libuni10.so: src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/Matrix.cpp.o
lib/libuni10.so: src/uni10/tensor-network/lib/CMakeFiles/uni10-tensor-network.dir/operate.cpp.o
lib/libuni10.so: src/uni10/numeric/lib/CMakeFiles/uni10-numeric.dir/uni10_lapack.cpp.o
lib/libuni10.so: CMakeFiles/uni10.dir/build.make
lib/libuni10.so: /usr/lib64/liblapack.so
lib/libuni10.so: /usr/lib64/libblas.so
lib/libuni10.so: CMakeFiles/uni10.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library lib/libuni10.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/uni10.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/uni10.dir/build: lib/libuni10.so
.PHONY : CMakeFiles/uni10.dir/build

CMakeFiles/uni10.dir/requires:
.PHONY : CMakeFiles/uni10.dir/requires

CMakeFiles/uni10.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/uni10.dir/cmake_clean.cmake
.PHONY : CMakeFiles/uni10.dir/clean

CMakeFiles/uni10.dir/depend:
	cd /home/yundasie/TensorNetwork/guni10/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yundasie/TensorNetwork/guni10 /home/yundasie/TensorNetwork/guni10 /home/yundasie/TensorNetwork/guni10/build /home/yundasie/TensorNetwork/guni10/build /home/yundasie/TensorNetwork/guni10/build/CMakeFiles/uni10.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/uni10.dir/depend

