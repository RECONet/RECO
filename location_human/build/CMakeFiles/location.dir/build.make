# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/wirelab/Downloads/location_human

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wirelab/Downloads/location_human/build

# Include any dependencies generated for this target.
include CMakeFiles/location.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/location.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/location.dir/flags.make

generated/test.pb.h:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating generated/test.pb.h, generated/test.pb.cc, generated/test.grpc.pb.h, generated/test.grpc.pb.cc"
	/usr/local/bin/protoc --proto_path=/home/wirelab/Downloads/location_human/protos --cpp_out=/home/wirelab/Downloads/location_human/build/generated /home/wirelab/Downloads/location_human/protos/test.proto
	/usr/local/bin/protoc --proto_path=/home/wirelab/Downloads/location_human/protos --grpc_out=/home/wirelab/Downloads/location_human/build/generated --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin /home/wirelab/Downloads/location_human/protos/test.proto

generated/test.pb.cc: generated/test.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate generated/test.pb.cc

generated/test.grpc.pb.h: generated/test.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate generated/test.grpc.pb.h

generated/test.grpc.pb.cc: generated/test.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate generated/test.grpc.pb.cc

CMakeFiles/location.dir/location.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/location.cpp.o: ../location.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/location.dir/location.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/location.cpp.o -c /home/wirelab/Downloads/location_human/location.cpp

CMakeFiles/location.dir/location.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/location.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/location.cpp > CMakeFiles/location.dir/location.cpp.i

CMakeFiles/location.dir/location.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/location.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/location.cpp -o CMakeFiles/location.dir/location.cpp.s

CMakeFiles/location.dir/location.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/location.cpp.o.requires

CMakeFiles/location.dir/location.cpp.o.provides: CMakeFiles/location.dir/location.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/location.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/location.cpp.o.provides

CMakeFiles/location.dir/location.cpp.o.provides.build: CMakeFiles/location.dir/location.cpp.o


CMakeFiles/location.dir/generated/test.pb.cc.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/generated/test.pb.cc.o: generated/test.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/location.dir/generated/test.pb.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/generated/test.pb.cc.o -c /home/wirelab/Downloads/location_human/build/generated/test.pb.cc

CMakeFiles/location.dir/generated/test.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/generated/test.pb.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/build/generated/test.pb.cc > CMakeFiles/location.dir/generated/test.pb.cc.i

CMakeFiles/location.dir/generated/test.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/generated/test.pb.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/build/generated/test.pb.cc -o CMakeFiles/location.dir/generated/test.pb.cc.s

CMakeFiles/location.dir/generated/test.pb.cc.o.requires:

.PHONY : CMakeFiles/location.dir/generated/test.pb.cc.o.requires

CMakeFiles/location.dir/generated/test.pb.cc.o.provides: CMakeFiles/location.dir/generated/test.pb.cc.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/generated/test.pb.cc.o.provides.build
.PHONY : CMakeFiles/location.dir/generated/test.pb.cc.o.provides

CMakeFiles/location.dir/generated/test.pb.cc.o.provides.build: CMakeFiles/location.dir/generated/test.pb.cc.o


CMakeFiles/location.dir/generated/test.grpc.pb.cc.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/generated/test.grpc.pb.cc.o: generated/test.grpc.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/location.dir/generated/test.grpc.pb.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/generated/test.grpc.pb.cc.o -c /home/wirelab/Downloads/location_human/build/generated/test.grpc.pb.cc

CMakeFiles/location.dir/generated/test.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/generated/test.grpc.pb.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/build/generated/test.grpc.pb.cc > CMakeFiles/location.dir/generated/test.grpc.pb.cc.i

CMakeFiles/location.dir/generated/test.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/generated/test.grpc.pb.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/build/generated/test.grpc.pb.cc -o CMakeFiles/location.dir/generated/test.grpc.pb.cc.s

CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.requires:

.PHONY : CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.requires

CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.provides: CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.provides.build
.PHONY : CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.provides

CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.provides.build: CMakeFiles/location.dir/generated/test.grpc.pb.cc.o


CMakeFiles/location.dir/s6a_task.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_task.cpp.o: ../s6a_task.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/location.dir/s6a_task.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_task.cpp.o -c /home/wirelab/Downloads/location_human/s6a_task.cpp

CMakeFiles/location.dir/s6a_task.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_task.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_task.cpp > CMakeFiles/location.dir/s6a_task.cpp.i

CMakeFiles/location.dir/s6a_task.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_task.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_task.cpp -o CMakeFiles/location.dir/s6a_task.cpp.s

CMakeFiles/location.dir/s6a_task.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_task.cpp.o.requires

CMakeFiles/location.dir/s6a_task.cpp.o.provides: CMakeFiles/location.dir/s6a_task.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_task.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_task.cpp.o.provides

CMakeFiles/location.dir/s6a_task.cpp.o.provides.build: CMakeFiles/location.dir/s6a_task.cpp.o


CMakeFiles/location.dir/s6a_up_loc.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_up_loc.cpp.o: ../s6a_up_loc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/location.dir/s6a_up_loc.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_up_loc.cpp.o -c /home/wirelab/Downloads/location_human/s6a_up_loc.cpp

CMakeFiles/location.dir/s6a_up_loc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_up_loc.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_up_loc.cpp > CMakeFiles/location.dir/s6a_up_loc.cpp.i

CMakeFiles/location.dir/s6a_up_loc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_up_loc.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_up_loc.cpp -o CMakeFiles/location.dir/s6a_up_loc.cpp.s

CMakeFiles/location.dir/s6a_up_loc.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_up_loc.cpp.o.requires

CMakeFiles/location.dir/s6a_up_loc.cpp.o.provides: CMakeFiles/location.dir/s6a_up_loc.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_up_loc.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_up_loc.cpp.o.provides

CMakeFiles/location.dir/s6a_up_loc.cpp.o.provides.build: CMakeFiles/location.dir/s6a_up_loc.cpp.o


CMakeFiles/location.dir/s6a_config.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_config.cpp.o: ../s6a_config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/location.dir/s6a_config.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_config.cpp.o -c /home/wirelab/Downloads/location_human/s6a_config.cpp

CMakeFiles/location.dir/s6a_config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_config.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_config.cpp > CMakeFiles/location.dir/s6a_config.cpp.i

CMakeFiles/location.dir/s6a_config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_config.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_config.cpp -o CMakeFiles/location.dir/s6a_config.cpp.s

CMakeFiles/location.dir/s6a_config.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_config.cpp.o.requires

CMakeFiles/location.dir/s6a_config.cpp.o.provides: CMakeFiles/location.dir/s6a_config.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_config.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_config.cpp.o.provides

CMakeFiles/location.dir/s6a_config.cpp.o.provides.build: CMakeFiles/location.dir/s6a_config.cpp.o


CMakeFiles/location.dir/s6a_dict.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_dict.cpp.o: ../s6a_dict.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/location.dir/s6a_dict.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_dict.cpp.o -c /home/wirelab/Downloads/location_human/s6a_dict.cpp

CMakeFiles/location.dir/s6a_dict.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_dict.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_dict.cpp > CMakeFiles/location.dir/s6a_dict.cpp.i

CMakeFiles/location.dir/s6a_dict.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_dict.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_dict.cpp -o CMakeFiles/location.dir/s6a_dict.cpp.s

CMakeFiles/location.dir/s6a_dict.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_dict.cpp.o.requires

CMakeFiles/location.dir/s6a_dict.cpp.o.provides: CMakeFiles/location.dir/s6a_dict.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_dict.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_dict.cpp.o.provides

CMakeFiles/location.dir/s6a_dict.cpp.o.provides.build: CMakeFiles/location.dir/s6a_dict.cpp.o


CMakeFiles/location.dir/s6a_peer.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_peer.cpp.o: ../s6a_peer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/location.dir/s6a_peer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_peer.cpp.o -c /home/wirelab/Downloads/location_human/s6a_peer.cpp

CMakeFiles/location.dir/s6a_peer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_peer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_peer.cpp > CMakeFiles/location.dir/s6a_peer.cpp.i

CMakeFiles/location.dir/s6a_peer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_peer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_peer.cpp -o CMakeFiles/location.dir/s6a_peer.cpp.s

CMakeFiles/location.dir/s6a_peer.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_peer.cpp.o.requires

CMakeFiles/location.dir/s6a_peer.cpp.o.provides: CMakeFiles/location.dir/s6a_peer.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_peer.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_peer.cpp.o.provides

CMakeFiles/location.dir/s6a_peer.cpp.o.provides.build: CMakeFiles/location.dir/s6a_peer.cpp.o


CMakeFiles/location.dir/s6a_error.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_error.cpp.o: ../s6a_error.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/location.dir/s6a_error.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_error.cpp.o -c /home/wirelab/Downloads/location_human/s6a_error.cpp

CMakeFiles/location.dir/s6a_error.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_error.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_error.cpp > CMakeFiles/location.dir/s6a_error.cpp.i

CMakeFiles/location.dir/s6a_error.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_error.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_error.cpp -o CMakeFiles/location.dir/s6a_error.cpp.s

CMakeFiles/location.dir/s6a_error.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_error.cpp.o.requires

CMakeFiles/location.dir/s6a_error.cpp.o.provides: CMakeFiles/location.dir/s6a_error.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_error.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_error.cpp.o.provides

CMakeFiles/location.dir/s6a_error.cpp.o.provides.build: CMakeFiles/location.dir/s6a_error.cpp.o


CMakeFiles/location.dir/s6a_subscription_data.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/s6a_subscription_data.cpp.o: ../s6a_subscription_data.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/location.dir/s6a_subscription_data.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/s6a_subscription_data.cpp.o -c /home/wirelab/Downloads/location_human/s6a_subscription_data.cpp

CMakeFiles/location.dir/s6a_subscription_data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/s6a_subscription_data.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/s6a_subscription_data.cpp > CMakeFiles/location.dir/s6a_subscription_data.cpp.i

CMakeFiles/location.dir/s6a_subscription_data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/s6a_subscription_data.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/s6a_subscription_data.cpp -o CMakeFiles/location.dir/s6a_subscription_data.cpp.s

CMakeFiles/location.dir/s6a_subscription_data.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/s6a_subscription_data.cpp.o.requires

CMakeFiles/location.dir/s6a_subscription_data.cpp.o.provides: CMakeFiles/location.dir/s6a_subscription_data.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/s6a_subscription_data.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/s6a_subscription_data.cpp.o.provides

CMakeFiles/location.dir/s6a_subscription_data.cpp.o.provides.build: CMakeFiles/location.dir/s6a_subscription_data.cpp.o


CMakeFiles/location.dir/bstrlib.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/bstrlib.cpp.o: ../bstrlib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/location.dir/bstrlib.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/bstrlib.cpp.o -c /home/wirelab/Downloads/location_human/bstrlib.cpp

CMakeFiles/location.dir/bstrlib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/bstrlib.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/bstrlib.cpp > CMakeFiles/location.dir/bstrlib.cpp.i

CMakeFiles/location.dir/bstrlib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/bstrlib.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/bstrlib.cpp -o CMakeFiles/location.dir/bstrlib.cpp.s

CMakeFiles/location.dir/bstrlib.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/bstrlib.cpp.o.requires

CMakeFiles/location.dir/bstrlib.cpp.o.provides: CMakeFiles/location.dir/bstrlib.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/bstrlib.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/bstrlib.cpp.o.provides

CMakeFiles/location.dir/bstrlib.cpp.o.provides.build: CMakeFiles/location.dir/bstrlib.cpp.o


CMakeFiles/location.dir/backtrace.cpp.o: CMakeFiles/location.dir/flags.make
CMakeFiles/location.dir/backtrace.cpp.o: ../backtrace.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/location.dir/backtrace.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/location.dir/backtrace.cpp.o -c /home/wirelab/Downloads/location_human/backtrace.cpp

CMakeFiles/location.dir/backtrace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/location.dir/backtrace.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wirelab/Downloads/location_human/backtrace.cpp > CMakeFiles/location.dir/backtrace.cpp.i

CMakeFiles/location.dir/backtrace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/location.dir/backtrace.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wirelab/Downloads/location_human/backtrace.cpp -o CMakeFiles/location.dir/backtrace.cpp.s

CMakeFiles/location.dir/backtrace.cpp.o.requires:

.PHONY : CMakeFiles/location.dir/backtrace.cpp.o.requires

CMakeFiles/location.dir/backtrace.cpp.o.provides: CMakeFiles/location.dir/backtrace.cpp.o.requires
	$(MAKE) -f CMakeFiles/location.dir/build.make CMakeFiles/location.dir/backtrace.cpp.o.provides.build
.PHONY : CMakeFiles/location.dir/backtrace.cpp.o.provides

CMakeFiles/location.dir/backtrace.cpp.o.provides.build: CMakeFiles/location.dir/backtrace.cpp.o


# Object files for target location
location_OBJECTS = \
"CMakeFiles/location.dir/location.cpp.o" \
"CMakeFiles/location.dir/generated/test.pb.cc.o" \
"CMakeFiles/location.dir/generated/test.grpc.pb.cc.o" \
"CMakeFiles/location.dir/s6a_task.cpp.o" \
"CMakeFiles/location.dir/s6a_up_loc.cpp.o" \
"CMakeFiles/location.dir/s6a_config.cpp.o" \
"CMakeFiles/location.dir/s6a_dict.cpp.o" \
"CMakeFiles/location.dir/s6a_peer.cpp.o" \
"CMakeFiles/location.dir/s6a_error.cpp.o" \
"CMakeFiles/location.dir/s6a_subscription_data.cpp.o" \
"CMakeFiles/location.dir/bstrlib.cpp.o" \
"CMakeFiles/location.dir/backtrace.cpp.o"

# External object files for target location
location_EXTERNAL_OBJECTS =

location: CMakeFiles/location.dir/location.cpp.o
location: CMakeFiles/location.dir/generated/test.pb.cc.o
location: CMakeFiles/location.dir/generated/test.grpc.pb.cc.o
location: CMakeFiles/location.dir/s6a_task.cpp.o
location: CMakeFiles/location.dir/s6a_up_loc.cpp.o
location: CMakeFiles/location.dir/s6a_config.cpp.o
location: CMakeFiles/location.dir/s6a_dict.cpp.o
location: CMakeFiles/location.dir/s6a_peer.cpp.o
location: CMakeFiles/location.dir/s6a_error.cpp.o
location: CMakeFiles/location.dir/s6a_subscription_data.cpp.o
location: CMakeFiles/location.dir/bstrlib.cpp.o
location: CMakeFiles/location.dir/backtrace.cpp.o
location: CMakeFiles/location.dir/build.make
location: /usr/local/lib/libprotobuf.so
location: CMakeFiles/location.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wirelab/Downloads/location_human/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable location"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/location.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/location.dir/build: location

.PHONY : CMakeFiles/location.dir/build

CMakeFiles/location.dir/requires: CMakeFiles/location.dir/location.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/generated/test.pb.cc.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/generated/test.grpc.pb.cc.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_task.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_up_loc.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_config.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_dict.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_peer.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_error.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/s6a_subscription_data.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/bstrlib.cpp.o.requires
CMakeFiles/location.dir/requires: CMakeFiles/location.dir/backtrace.cpp.o.requires

.PHONY : CMakeFiles/location.dir/requires

CMakeFiles/location.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/location.dir/cmake_clean.cmake
.PHONY : CMakeFiles/location.dir/clean

CMakeFiles/location.dir/depend: generated/test.pb.h
CMakeFiles/location.dir/depend: generated/test.pb.cc
CMakeFiles/location.dir/depend: generated/test.grpc.pb.h
CMakeFiles/location.dir/depend: generated/test.grpc.pb.cc
	cd /home/wirelab/Downloads/location_human/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wirelab/Downloads/location_human /home/wirelab/Downloads/location_human /home/wirelab/Downloads/location_human/build /home/wirelab/Downloads/location_human/build /home/wirelab/Downloads/location_human/build/CMakeFiles/location.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/location.dir/depend

