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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/library/hungpd/np/knights-2.5.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/library/hungpd/np/knights-2.5.0/build

# Utility rule file for pofiles.

# Include the progress variables for this target.
include po/fi/CMakeFiles/pofiles.dir/progress.make

po/fi/CMakeFiles/pofiles: po/fi/knights.gmo

po/fi/knights.gmo: ../po/fi/knights.po
	$(CMAKE_COMMAND) -E cmake_progress_report /home/library/hungpd/np/knights-2.5.0/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating knights.gmo"
	cd /home/library/hungpd/np/knights-2.5.0/po/fi && /usr/bin/msgfmt -o /home/library/hungpd/np/knights-2.5.0/build/po/fi/knights.gmo /home/library/hungpd/np/knights-2.5.0/po/fi/knights.po

pofiles: po/fi/CMakeFiles/pofiles
pofiles: po/fi/knights.gmo
pofiles: po/fi/CMakeFiles/pofiles.dir/build.make
.PHONY : pofiles

# Rule to build all files generated by this target.
po/fi/CMakeFiles/pofiles.dir/build: pofiles
.PHONY : po/fi/CMakeFiles/pofiles.dir/build

po/fi/CMakeFiles/pofiles.dir/clean:
	cd /home/library/hungpd/np/knights-2.5.0/build/po/fi && $(CMAKE_COMMAND) -P CMakeFiles/pofiles.dir/cmake_clean.cmake
.PHONY : po/fi/CMakeFiles/pofiles.dir/clean

po/fi/CMakeFiles/pofiles.dir/depend:
	cd /home/library/hungpd/np/knights-2.5.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/library/hungpd/np/knights-2.5.0 /home/library/hungpd/np/knights-2.5.0/po/fi /home/library/hungpd/np/knights-2.5.0/build /home/library/hungpd/np/knights-2.5.0/build/po/fi /home/library/hungpd/np/knights-2.5.0/build/po/fi/CMakeFiles/pofiles.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : po/fi/CMakeFiles/pofiles.dir/depend
