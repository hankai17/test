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

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/test/dns/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/test/dns/src

# Include any dependencies generated for this target.
include CMakeFiles/fakecli.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fakecli.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fakecli.dir/flags.make

CMakeFiles/fakecli.dir/buffer.cpp.o: CMakeFiles/fakecli.dir/flags.make
CMakeFiles/fakecli.dir/buffer.cpp.o: buffer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/test/dns/src/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/fakecli.dir/buffer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fakecli.dir/buffer.cpp.o -c /root/test/dns/src/buffer.cpp

CMakeFiles/fakecli.dir/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fakecli.dir/buffer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/test/dns/src/buffer.cpp > CMakeFiles/fakecli.dir/buffer.cpp.i

CMakeFiles/fakecli.dir/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fakecli.dir/buffer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/test/dns/src/buffer.cpp -o CMakeFiles/fakecli.dir/buffer.cpp.s

CMakeFiles/fakecli.dir/buffer.cpp.o.requires:
.PHONY : CMakeFiles/fakecli.dir/buffer.cpp.o.requires

CMakeFiles/fakecli.dir/buffer.cpp.o.provides: CMakeFiles/fakecli.dir/buffer.cpp.o.requires
	$(MAKE) -f CMakeFiles/fakecli.dir/build.make CMakeFiles/fakecli.dir/buffer.cpp.o.provides.build
.PHONY : CMakeFiles/fakecli.dir/buffer.cpp.o.provides

CMakeFiles/fakecli.dir/buffer.cpp.o.provides.build: CMakeFiles/fakecli.dir/buffer.cpp.o

CMakeFiles/fakecli.dir/message.cpp.o: CMakeFiles/fakecli.dir/flags.make
CMakeFiles/fakecli.dir/message.cpp.o: message.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/test/dns/src/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/fakecli.dir/message.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fakecli.dir/message.cpp.o -c /root/test/dns/src/message.cpp

CMakeFiles/fakecli.dir/message.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fakecli.dir/message.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/test/dns/src/message.cpp > CMakeFiles/fakecli.dir/message.cpp.i

CMakeFiles/fakecli.dir/message.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fakecli.dir/message.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/test/dns/src/message.cpp -o CMakeFiles/fakecli.dir/message.cpp.s

CMakeFiles/fakecli.dir/message.cpp.o.requires:
.PHONY : CMakeFiles/fakecli.dir/message.cpp.o.requires

CMakeFiles/fakecli.dir/message.cpp.o.provides: CMakeFiles/fakecli.dir/message.cpp.o.requires
	$(MAKE) -f CMakeFiles/fakecli.dir/build.make CMakeFiles/fakecli.dir/message.cpp.o.provides.build
.PHONY : CMakeFiles/fakecli.dir/message.cpp.o.provides

CMakeFiles/fakecli.dir/message.cpp.o.provides.build: CMakeFiles/fakecli.dir/message.cpp.o

CMakeFiles/fakecli.dir/rr.cpp.o: CMakeFiles/fakecli.dir/flags.make
CMakeFiles/fakecli.dir/rr.cpp.o: rr.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/test/dns/src/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/fakecli.dir/rr.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fakecli.dir/rr.cpp.o -c /root/test/dns/src/rr.cpp

CMakeFiles/fakecli.dir/rr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fakecli.dir/rr.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/test/dns/src/rr.cpp > CMakeFiles/fakecli.dir/rr.cpp.i

CMakeFiles/fakecli.dir/rr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fakecli.dir/rr.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/test/dns/src/rr.cpp -o CMakeFiles/fakecli.dir/rr.cpp.s

CMakeFiles/fakecli.dir/rr.cpp.o.requires:
.PHONY : CMakeFiles/fakecli.dir/rr.cpp.o.requires

CMakeFiles/fakecli.dir/rr.cpp.o.provides: CMakeFiles/fakecli.dir/rr.cpp.o.requires
	$(MAKE) -f CMakeFiles/fakecli.dir/build.make CMakeFiles/fakecli.dir/rr.cpp.o.provides.build
.PHONY : CMakeFiles/fakecli.dir/rr.cpp.o.provides

CMakeFiles/fakecli.dir/rr.cpp.o.provides.build: CMakeFiles/fakecli.dir/rr.cpp.o

CMakeFiles/fakecli.dir/qs.cpp.o: CMakeFiles/fakecli.dir/flags.make
CMakeFiles/fakecli.dir/qs.cpp.o: qs.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/test/dns/src/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/fakecli.dir/qs.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fakecli.dir/qs.cpp.o -c /root/test/dns/src/qs.cpp

CMakeFiles/fakecli.dir/qs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fakecli.dir/qs.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/test/dns/src/qs.cpp > CMakeFiles/fakecli.dir/qs.cpp.i

CMakeFiles/fakecli.dir/qs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fakecli.dir/qs.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/test/dns/src/qs.cpp -o CMakeFiles/fakecli.dir/qs.cpp.s

CMakeFiles/fakecli.dir/qs.cpp.o.requires:
.PHONY : CMakeFiles/fakecli.dir/qs.cpp.o.requires

CMakeFiles/fakecli.dir/qs.cpp.o.provides: CMakeFiles/fakecli.dir/qs.cpp.o.requires
	$(MAKE) -f CMakeFiles/fakecli.dir/build.make CMakeFiles/fakecli.dir/qs.cpp.o.provides.build
.PHONY : CMakeFiles/fakecli.dir/qs.cpp.o.provides

CMakeFiles/fakecli.dir/qs.cpp.o.provides.build: CMakeFiles/fakecli.dir/qs.cpp.o

CMakeFiles/fakecli.dir/fakecli.cpp.o: CMakeFiles/fakecli.dir/flags.make
CMakeFiles/fakecli.dir/fakecli.cpp.o: fakecli.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/test/dns/src/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/fakecli.dir/fakecli.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fakecli.dir/fakecli.cpp.o -c /root/test/dns/src/fakecli.cpp

CMakeFiles/fakecli.dir/fakecli.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fakecli.dir/fakecli.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/test/dns/src/fakecli.cpp > CMakeFiles/fakecli.dir/fakecli.cpp.i

CMakeFiles/fakecli.dir/fakecli.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fakecli.dir/fakecli.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/test/dns/src/fakecli.cpp -o CMakeFiles/fakecli.dir/fakecli.cpp.s

CMakeFiles/fakecli.dir/fakecli.cpp.o.requires:
.PHONY : CMakeFiles/fakecli.dir/fakecli.cpp.o.requires

CMakeFiles/fakecli.dir/fakecli.cpp.o.provides: CMakeFiles/fakecli.dir/fakecli.cpp.o.requires
	$(MAKE) -f CMakeFiles/fakecli.dir/build.make CMakeFiles/fakecli.dir/fakecli.cpp.o.provides.build
.PHONY : CMakeFiles/fakecli.dir/fakecli.cpp.o.provides

CMakeFiles/fakecli.dir/fakecli.cpp.o.provides.build: CMakeFiles/fakecli.dir/fakecli.cpp.o

# Object files for target fakecli
fakecli_OBJECTS = \
"CMakeFiles/fakecli.dir/buffer.cpp.o" \
"CMakeFiles/fakecli.dir/message.cpp.o" \
"CMakeFiles/fakecli.dir/rr.cpp.o" \
"CMakeFiles/fakecli.dir/qs.cpp.o" \
"CMakeFiles/fakecli.dir/fakecli.cpp.o"

# External object files for target fakecli
fakecli_EXTERNAL_OBJECTS =

fakecli: CMakeFiles/fakecli.dir/buffer.cpp.o
fakecli: CMakeFiles/fakecli.dir/message.cpp.o
fakecli: CMakeFiles/fakecli.dir/rr.cpp.o
fakecli: CMakeFiles/fakecli.dir/qs.cpp.o
fakecli: CMakeFiles/fakecli.dir/fakecli.cpp.o
fakecli: CMakeFiles/fakecli.dir/build.make
fakecli: CMakeFiles/fakecli.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable fakecli"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fakecli.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fakecli.dir/build: fakecli
.PHONY : CMakeFiles/fakecli.dir/build

CMakeFiles/fakecli.dir/requires: CMakeFiles/fakecli.dir/buffer.cpp.o.requires
CMakeFiles/fakecli.dir/requires: CMakeFiles/fakecli.dir/message.cpp.o.requires
CMakeFiles/fakecli.dir/requires: CMakeFiles/fakecli.dir/rr.cpp.o.requires
CMakeFiles/fakecli.dir/requires: CMakeFiles/fakecli.dir/qs.cpp.o.requires
CMakeFiles/fakecli.dir/requires: CMakeFiles/fakecli.dir/fakecli.cpp.o.requires
.PHONY : CMakeFiles/fakecli.dir/requires

CMakeFiles/fakecli.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fakecli.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fakecli.dir/clean

CMakeFiles/fakecli.dir/depend:
	cd /root/test/dns/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/test/dns/src /root/test/dns/src /root/test/dns/src /root/test/dns/src /root/test/dns/src/CMakeFiles/fakecli.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fakecli.dir/depend

