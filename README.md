Kace Cottam  

Persistent Implementation of Data Structures
============================================

[list](list/) -- Implementation of a forward list with a forward-only iterator.

Building
--------

Example `make` command:

```bash
make

usage: make [target] ... [<optional argument>=<value> ...]
----------------------------------------------------------
targets:
	all                           Runs all targets
	clean                         Cleans directory to return to working state
	DEBUG_MAKE                    Allows for debugging of makefile
	help                          Prints usage instructions
	${INCDIR}/catch.hpp           Download Catch2 Library
	setup-project                 Sets up a project. Needs EXEC defined.
	tests                         Compile all tests

optional arguments:
	CXX                           What compiler we are using
	CXXFLAGS                      Compiler flags for all operations
	SRCDIR                        Where the source files are located
	OBJDIR                        Where the object files will be placed
	BINDIR                        Where the finished executable will be placed
	INCDIR                        Additional include directories
	COMPILE_ARGS                  Extra arguments for compilation
	LINKING_ARGS                  Extra arguments for linking
	EXEC                          The output executable for the linking process. Can also automatically use files with the .cxx extension if undefined.
	DEBUG_COMMAND                 The command to use while debugging makefile
	TEST_BINDIR                   Where test binaries are placed
	TEST_INCLUDES                 Extra includes for tests
```


