# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/user/work/mon-service/build/_deps/static_string-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/work/mon-service/build/_deps/static_string-subbuild

# Utility rule file for static_string-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/static_string-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/static_string-populate.dir/progress.make

CMakeFiles/static_string-populate: CMakeFiles/static_string-populate-complete

CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-install
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-mkdir
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-configure
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-build
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-install
CMakeFiles/static_string-populate-complete: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'static_string-populate'"
	/usr/bin/cmake -E make_directory /home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles/static_string-populate-complete
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-done

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-build: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'static_string-populate'"
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-build

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-configure: static_string-populate-prefix/tmp/static_string-populate-cfgcmd.txt
static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-configure: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'static_string-populate'"
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-configure

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download: static_string-populate-prefix/src/static_string-populate-stamp/download-static_string-populate.cmake
static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-urlinfo.txt
static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (download, verify and extract) for 'static_string-populate'"
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -P /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/download-static_string-populate.cmake
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -P /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/verify-static_string-populate.cmake
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -P /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/extract-static_string-populate.cmake
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-install: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'static_string-populate'"
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-install

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'static_string-populate'"
	/usr/bin/cmake -Dcfgdir= -P /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/tmp/static_string-populate-mkdirs.cmake
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-mkdir

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch-info.txt
static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'static_string-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-test: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'static_string-populate'"
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/static_string-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-test

static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update-info.txt
static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No update step for 'static_string-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/static_string-subbuild/static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update

static_string-populate: CMakeFiles/static_string-populate
static_string-populate: CMakeFiles/static_string-populate-complete
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-build
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-configure
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-download
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-install
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-mkdir
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-patch
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-test
static_string-populate: static_string-populate-prefix/src/static_string-populate-stamp/static_string-populate-update
static_string-populate: CMakeFiles/static_string-populate.dir/build.make
.PHONY : static_string-populate

# Rule to build all files generated by this target.
CMakeFiles/static_string-populate.dir/build: static_string-populate
.PHONY : CMakeFiles/static_string-populate.dir/build

CMakeFiles/static_string-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/static_string-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/static_string-populate.dir/clean

CMakeFiles/static_string-populate.dir/depend:
	cd /home/user/work/mon-service/build/_deps/static_string-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/work/mon-service/build/_deps/static_string-subbuild /home/user/work/mon-service/build/_deps/static_string-subbuild /home/user/work/mon-service/build/_deps/static_string-subbuild /home/user/work/mon-service/build/_deps/static_string-subbuild /home/user/work/mon-service/build/_deps/static_string-subbuild/CMakeFiles/static_string-populate.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/static_string-populate.dir/depend

