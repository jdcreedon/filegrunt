# BUILD INSTRUCTIONS AND REQUIREMENTS

FileGrunt requires Qt,OpenCV as well as SQLite. For more information on Qt please consult http://www.qt.io and for SQLite please see https://sqlite.org/.

# This file is under construction.

# Useful Links

https://cmake.org/


# Linux
# Forking & Branch Build
When you fork and create a branch and import into CLION right click on CMakeLists.txt and select Load/Reload CMake Project.
This will create the default debug build and include the source files in the project.

If the Reload CMake isn't an option do a file invalidate caches and this fixes the issue.

# Windows

Cross compiling for Windows
These are instructions to cross compile within a Linux system a Windows binary and installer.

Requirements:

mxe cross compile environment → http://mxe.cc
cmake
filegrunt sources

- Check the requirements https://mxe.cc/#requirements
- Follow the tutorial https://mxe.cc/#tutorial

Get the following mxe packages:

$ make gcc sqlite qt5 opencv
After successful compilation go into your mxedir/usr/bin and add 3 symlinks:

$ ln -s i686-pc-mingw32-windres windres
$ ln -s i686-pc-mingw32-makensis makensis
$ ln -s /usr/bin/lrelease

Before compiling we have to add the mxe/usr/bin directory to the PATH (so windres and makensis can be found):
$ export PATH=/path to mxe/usr/bin:$PATH

Now cd into your filegrunt source directory and create a build directory for the windows binary and create the correct makefiles:

$ mkdir build-win in the filegrunt directory
$ cd build-win
$ i686-w64-mingw32.static-cmake ...

Now compile:

$ make

#TODO:
If you additionally want an NSIS installer:

$ make package
done.