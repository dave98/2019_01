# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.13

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\ASUS\Desktop\FANN-2.2.0-Source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Dave\code\2019_01\P_Neuronal_Network\Fann

# Include any dependencies generated for this target.
include src/CMakeFiles/fixedfann.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/fixedfann.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/fixedfann.dir/flags.make

src/CMakeFiles/fixedfann.dir/fixedfann.c.obj: src/CMakeFiles/fixedfann.dir/flags.make
src/CMakeFiles/fixedfann.dir/fixedfann.c.obj: src/CMakeFiles/fixedfann.dir/includes_C.rsp
src/CMakeFiles/fixedfann.dir/fixedfann.c.obj: C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/fixedfann.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Dave\code\2019_01\P_Neuronal_Network\Fann\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/fixedfann.dir/fixedfann.c.obj"
	cd /d D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\fixedfann.dir\fixedfann.c.obj   -c C:\Users\ASUS\Desktop\FANN-2.2.0-Source\src\fixedfann.c

src/CMakeFiles/fixedfann.dir/fixedfann.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fixedfann.dir/fixedfann.c.i"
	cd /d D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ASUS\Desktop\FANN-2.2.0-Source\src\fixedfann.c > CMakeFiles\fixedfann.dir\fixedfann.c.i

src/CMakeFiles/fixedfann.dir/fixedfann.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fixedfann.dir/fixedfann.c.s"
	cd /d D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ASUS\Desktop\FANN-2.2.0-Source\src\fixedfann.c -o CMakeFiles\fixedfann.dir\fixedfann.c.s

# Object files for target fixedfann
fixedfann_OBJECTS = \
"CMakeFiles/fixedfann.dir/fixedfann.c.obj"

# External object files for target fixedfann
fixedfann_EXTERNAL_OBJECTS =

src/libfixedfann.dll: src/CMakeFiles/fixedfann.dir/fixedfann.c.obj
src/libfixedfann.dll: src/CMakeFiles/fixedfann.dir/build.make
src/libfixedfann.dll: src/CMakeFiles/fixedfann.dir/linklibs.rsp
src/libfixedfann.dll: src/CMakeFiles/fixedfann.dir/objects1.rsp
src/libfixedfann.dll: src/CMakeFiles/fixedfann.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Dave\code\2019_01\P_Neuronal_Network\Fann\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libfixedfann.dll"
	cd /d D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\fixedfann.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/fixedfann.dir/build: src/libfixedfann.dll

.PHONY : src/CMakeFiles/fixedfann.dir/build

src/CMakeFiles/fixedfann.dir/clean:
	cd /d D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src && $(CMAKE_COMMAND) -P CMakeFiles\fixedfann.dir\cmake_clean.cmake
.PHONY : src/CMakeFiles/fixedfann.dir/clean

src/CMakeFiles/fixedfann.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\ASUS\Desktop\FANN-2.2.0-Source C:\Users\ASUS\Desktop\FANN-2.2.0-Source\src D:\Dave\code\2019_01\P_Neuronal_Network\Fann D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src D:\Dave\code\2019_01\P_Neuronal_Network\Fann\src\CMakeFiles\fixedfann.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/fixedfann.dir/depend

