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
CMAKE_SOURCE_DIR = /home/user/work/mon-service/build/_deps/ubox-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/work/mon-service/build/_deps/ubox-subbuild

# Utility rule file for ubox-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/ubox-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ubox-populate.dir/progress.make

CMakeFiles/ubox-populate: CMakeFiles/ubox-populate-complete

CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-install
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-mkdir
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-configure
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-build
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-install
CMakeFiles/ubox-populate-complete: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'ubox-populate'"
	/usr/bin/cmake -E make_directory /home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles/ubox-populate-complete
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-done

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-build: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'ubox-populate'"
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-build

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-configure: ubox-populate-prefix/tmp/ubox-populate-cfgcmd.txt
ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-configure: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'ubox-populate'"
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-configure

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download: ubox-populate-prefix/src/ubox-populate-stamp/download-ubox-populate.cmake
ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-urlinfo.txt
ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (download, verify and extract) for 'ubox-populate'"
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -P /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/download-ubox-populate.cmake
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -P /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/verify-ubox-populate.cmake
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -P /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/extract-ubox-populate.cmake
	cd /home/user/work/mon-service/build/_deps && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-install: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'ubox-populate'"
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-install

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'ubox-populate'"
	/usr/bin/cmake -Dcfgdir= -P /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/tmp/ubox-populate-mkdirs.cmake
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-mkdir

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch-info.txt
ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'ubox-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-test: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'ubox-populate'"
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E echo_append
	cd /home/user/work/mon-service/build/_deps/ubox-build && /usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-test

ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update-info.txt
ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No update step for 'ubox-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/user/work/mon-service/build/_deps/ubox-subbuild/ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update

ubox-populate: CMakeFiles/ubox-populate
ubox-populate: CMakeFiles/ubox-populate-complete
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-build
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-configure
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-download
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-install
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-mkdir
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-patch
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-test
ubox-populate: ubox-populate-prefix/src/ubox-populate-stamp/ubox-populate-update
ubox-populate: CMakeFiles/ubox-populate.dir/build.make
.PHONY : ubox-populate

# Rule to build all files generated by this target.
CMakeFiles/ubox-populate.dir/build: ubox-populate
.PHONY : CMakeFiles/ubox-populate.dir/build

CMakeFiles/ubox-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ubox-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ubox-populate.dir/clean

CMakeFiles/ubox-populate.dir/depend:
	cd /home/user/work/mon-service/build/_deps/ubox-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/work/mon-service/build/_deps/ubox-subbuild /home/user/work/mon-service/build/_deps/ubox-subbuild /home/user/work/mon-service/build/_deps/ubox-subbuild /home/user/work/mon-service/build/_deps/ubox-subbuild /home/user/work/mon-service/build/_deps/ubox-subbuild/CMakeFiles/ubox-populate.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ubox-populate.dir/depend

