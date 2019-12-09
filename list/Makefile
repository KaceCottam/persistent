# I use the .cxx extension for source files that define main.

CXX            := g++## What compiler we are using
CXXFLAGS       := -std=c++17 -Wall -Wextra -Wpedantic -Werror## Compiler flags for all operations
# Directories
SRCDIR         := src## Where the source files are located
OBJDIR         := build## Where the object files will be placed
BINDIR         := bin## Where the finished executable will be placed
INCDIR         := include## Additional include directories
# File GLOBs
SRCS           := $(shell ls -R ${SRCDIR}/*.cpp 2> /dev/null)
SRC_MAINS      := $(shell ls -R ${SRCDIR}/*.cxx 2> /dev/null)
HEADS          := $(shell ls -R ${SRCDIR}/*.hpp 2> /dev/null)
HEADS          += $(shell ls -R ${SRCDIR}/*.hxx 2> /dev/null)
OBJS           := ${SRCS:${SRCDIR}/%.cpp=${OBJDIR}/%.o}
# Extra Arguments
COMPILE_ARGS   := ## Extra arguments for compilation
LINKING_ARGS   := ## Extra arguments for linking
# Misc
EXEC           := ## The output executable for the linking process. Can also automatically use files with the .cxx extension if undefined.
EXEC_MAINS     := ${SRC_MAINS:${SRCDIR}/%.cxx=${BINDIR}/%}
DEBUG_COMMAND  := ## The command to use while debugging makefile

ifeq "${DEBUG}" "1"
	COMPILE_ARGS += -g
	LINKING_ARGS += -g
endif
ifeq "${VERBOSE}" "1"
	COMPILE_ARGS += -v
	LINKING_ARGS += -v
endif

default: help

setup-project: ## Sets up a project. Needs EXEC defined.

ifeq "${EXEC}" ""
all: ${EXEC_MAINS} ## Runs all targets

setup-project: 
	@echo "Did you define EXEC?"
else
all: ${BINDIR}/${EXEC}

setup-project:
	@echo "Fetching .clang-format from gist..."
	@curl -o .clang-format https://gist.githubusercontent.com/KaceCottam/957e14380e7c5e9822141c5c00ea17e4/raw/e21e1835ee0da1c27cf66c7856008265fddfd9b1/.clang-format
	@echo "...Done"
	@echo "Making source directory..."
	@mkdir -pv ${SRCDIR}
	@echo "...Done"
	@echo "Creating main..."
	@echo "int main() {}" > ${SRCDIR}/${EXEC}.cxx
	@echo "...Done"
endif

# credit from http://blog.testdouble.com/posts/2017-04-17-makefile-usability-tips
help: ## Prints usage instructions
	@echo "usage: make [target] ... [<optional argument>=<value> ...]"
	@echo "----------------------------------------------------------"
	@echo "targets:"
	@grep -E '^[A-Za-z%%$$\{\}\_-\/\.]+:.*?##.*$$' $(MAKEFILE_LIST) | \
	 sort | \
	 awk 'BEGIN {FS = ":.*?## "}; {printf "\t%-30s%s\n", $$1, $$2}'
	@echo ""
	@echo "optional arguments:"
	@grep -E '^[A-Z_]+ +:=.*?##.*$$' $(MAKEFILE_LIST) | \
	 awk 'BEGIN {FS = ":.*?## "}; {printf "\t%-30s%s\n", $$1, $$2}'

${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	@mkdir -pv ${OBJDIR}
	${CXX} ${CXXFLAGS} ${COMPILE_ARGS} -c -o $@ $<

${BINDIR}/%: ${OBJS} ${HEADS} ${SRCDIR}/%.cxx
	@mkdir -pv ${BINDIR}
	${CXX} ${CXXFLAGS} ${LINKING_ARGS} -o $@ $(filter-out %.hxx, $(filter-out %.hpp, $^))

${BINDIR}/${EXEC}: ${OBJS} ${HEADS}
	@mkdir -pv ${BINDIR}
	${CXX} ${CXXFLAGS} ${LINKING_ARGS} -o $@ $(filter-out %.hxx, $(filter-out %.hpp, $^))

DEBUG_MAKE: ## Allows for debugging of makefile
	${DEBUG_COMMAND}

clean: ## Cleans directory to return to working state
	@echo "Cleaning Directory."
	@echo "Removing ${OBJDIR}, ${BINDIR}, ${INCDIR}..."
	@rm -rf ${OBJDIR} ${BINDIR} ${INCDIR}
	@echo "...Done"

# -------------- Testing Targets -------------- #

TESTDIR       := tests
TEST_SRCS     := $(shell ls -R ${TESTDIR}/*.cpp 2> /dev/null)
TEST_SRCS     += $(shell ls -R ${TESTDIR}/*.cxx 2> /dev/null)
TEST_BINDIR   := ${BINDIR}/${TESTDIR}## Where test binaries are placed
TEST_INCLUDES := -I${INCDIR}## Extra includes for tests                    
TEST_BINS     := ${TEST_SRCS:${TESTDIR}/%.cxx=${TEST_BINDIR}/%}

tests: ${TEST_BINS} ## Compile all tests

setup-project: setup-test-project

ifeq "${EXEC}" "" #change to ifdef
setup-test-project:
else
setup-test-project:
	@echo "Making test directory..."
	@mkdir -pv ${TESTDIR}
	@echo "...Done"
	@echo "Making test files..."
	@echo "#define CATCH_CONFIG_MAIN\n#include <catch.hpp>\nTEST_CASE(\"\") { REQUIRE(true); }" \
		> ${TESTDIR}/${EXEC}-test.cxx
	@mkdir -pv ${SRCDIR}
	@touch ${SRCDIR}/${EXEC}-test.cpp ${SRCDIR}/${EXEC}-test.hpp
	@echo "...Done"
endif

all: tests

${INCDIR}/catch.hpp: ## Download Catch2 Library
	@echo "Installing catch2 library to $@..."
	@mkdir -pv ${INCDIR}/catch/
	@curl -o $@ "https://raw.githubusercontent.com/catchorg/Catch2/v2.10.2/single_include/catch2/catch.hpp" \
        1>/dev/null 3>/dev/null
	@echo "...Done"

${OBJDIR}/test-main.o: ${TESTDIR}/test-main.cpp ${INCDIR}/catch.hpp
	@mkdir -pv ${OBJDIR}
	${CXX} ${CXXFLAGS} ${COMPILE_ARGS} ${TEST_INCLUDES} -o $@ -c $<

${TEST_BINDIR}/%: ${TESTDIR}/%.cxx ${SRCDIR}/%.hpp ${OBJDIR}/%.o ${OBJDIR}/test-main.o
	@mkdir -pv ${TEST_BINDIR}
	${CXX} ${CXXFLAGS} ${COMPILE_ARGS} ${TEST_INCLUDES} -o $@ $(filter-out %.hxx, $(filter-out %.hpp, $^))
