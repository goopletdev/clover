# GNU C Compiler
CC = gcc
# C compiler flags:
# -Wall: enable all common warnings;
# -Wextra: enable extra warning flags
# -Wno-override-init: disable warnings about struct
#  	initializer overrides. 
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99
TARGET = clover

PKGS = libevdev
PKG_CHECK := $(shell pkg-config --exists $(PKGS) && echo "ok")
ifneq ($(PKG_CHECK), ok)
	$(error Required packages not found: $(PKGS). Please install libevde-dev)
endif

# #######################
# # directory structure #
# #######################
SRCDIR = src
INCDIR = include
BUILDDIR = build
# all conventional names so far. below uses 'variable expansion syntax'
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin
DEPDIR = $(BUILDDIR)/deps

TESTDIR = tests
TESTBINDIR = $(BUILDDIR)/test-bin
UNITY_DIR = lib/Unity
UNITY_SRC = $(UNITY_DIR)/src/unity.c
UNITY_INC = $(UNITY_DIR)/src

# #####################
# # find source files #
# #####################
# $(shell ...) executes a shell command and captures output
# find: recursively finds all .c files under SRCDIR
SOURCES = $(shell find $(SRCDIR) -name "*.c")
# pattern substitution: $(var:pattern=replacement)
# wildcard % matches any string
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
DEPS = $(SOURCES:%.c=$(DEPDIR)/%.d)

TEST_SOURCES = $(shell find $(TESTDIR) -name "*.c")
TEST_OBJECTS = $(TEST_SOURCES:%.c=$(OBJDIR)/%.o)
TEST_DEPS = $(TEST_SOURCES:%.c=$(DEPDIR)/%.d)
TEST_BINARIES = $(TEST_SOURCES:%.c=$(TESTBINDIR)/%)

# ##################
# # compiler flags #
# ##################
# pkg-config --cflags $(PKGS) gets compiler flags needed for specified packages
CFLAGS += -I$(INCDIR) $(shell pkg-config --cflags $(PKGS))
# conventional variable for linker flags:
# shell command gets library linking flags for packages
LDFLAGS += $(shell pkg-config --libs $(PKGS))
# -MMD: generate dependency files for user headers
# -MP: add phony targets for header files
#  	to avoid errors if headers are deleted
# -MF: specify dependency file name
# $*: automatic var representing stem of target
DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$*.d

# ######################
# # create directories #
# ######################
# executes immediately when Makefile is parsed
# -p prevents errors if dirs already exist
$(shell mkdir -p $(OBJDIR) $(BINDIR) $(DEPDIR) $(TESTBINDIR))

# ###############
# # build rules #
# ###############

# main target
# OBJECTS are prerequisites/dependencies 
# | is order-only prerequisite; directory must exist first
# following is the 'recipe' for the build;
# 	$@ auto variable for target name
# 	$^ auto variable for all prerequisites
# 	-o output file name
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# pattern rule for compiling .c files to .o files
# @mkdir suppresses echoing of mkdir;
# dir: built-in make function
# $(dir ...) extracts directory portion of file path
# $< auto variable for first prereq (.c file)
# -c: compile only; don't link
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TESTBINDIR)/%: $(TESTDIR)/%.c $(filter-out $(OBJDIR)/src/main.o, $(OBJECTS)) | $(TESTBINDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -o $@ $< $(UNITY_SRC) $(filter-out $(OBJDIR)/src/main.o, $(OBJECTS)) $(LDFLAGS)

test-build: $(TEST_BINARIES)

test: test-build
	@echo "Running tests..."
	@for test in $(TEST_BINARIES); do \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done
	@echo "All tests passed!"

# create directories; satisfies order-only prerequisites
# multiple targets charing the same rule
$(OBJDIR) $(BINDIR) $(DEPDIR) $(TESTBINDIR):
	mkdir -p $@

# Make directive; executes immediately upon makefile parsing
# include dependency files
-include $(DEPS)

# standard target name for cleanup opersations
clean:
	rm -rf $(BUILDDIR)

# clean is not a file name; run 'clean' recipe even if 'clean' file exists
.PHONY: clean test test-build
