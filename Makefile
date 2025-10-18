CC = gcc
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
LIBDIR = lib

OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin
DEPDIR = $(BUILDDIR)/deps

TOML_DIR = $(LIBDIR)/tomlc17
TOML_SRC = $(TOML_DIR)/src/tomlc17.c
TOML_INC = $(TOML_DIR)/src
TOML_DEP = $(DEPDIR)/tomlc17.d
TOML_OBJ = $(OBJDIR)/tomlc17.o

TESTDIR = tests
TESTBINDIR = $(BUILDDIR)/test-bin

UNITY_DIR = $(LIBDIR)/Unity
UNITY_SRC = $(UNITY_DIR)/src/unity.c
UNITY_INC = $(UNITY_DIR)/src

# trie data structure
SILLY_DIR = $(LIBDIR)/silly-string
SILLY_SRC = $(SILLY_DIR)/src/silly-string.c
SILLY_INC = $(SILLY_DIR)/src
SILLY_DEP = $(DEPDIR)/silly-string.d
SILLY_OBJ = $(OBJDIR)/silly-string.o

# doubly linked list data structure
DUB_LL_DIR = $(LIBDIR)/doubly-linked-list
DUB_LL_SRC = $(DUB_LL_DIR)/src/gd-dll.c
DUB_LL_INC = $(DUB_LL_DIR)/src
DUB_LL_DEP = $(DEPDIR)/gd-dll.d
DUB_LL_OBJ = $(OBJDIR)/gd-dll.o

# #####################
# # find source files #
# #####################
# find: recursively finds all .c files under SRCDIR
SOURCES = $(shell find $(SRCDIR) -name "*.c")
# pattern substitution: $(var:pattern=replacement)
# wildcard % matches any string
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
DEPS = $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SOURCES))

TEST_SOURCES = $(shell find $(TESTDIR) -name "*.c")
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(TEST_SOURCES))
TEST_DEPS = $(patsubst $(TESTDIR)/%.c,$(DEPDIR)/%.d,$(TEST_SOURCES))
TEST_BINARIES = $(patsubst $(TESTDIR)/%.c,$(TESTBINDIR)/%,$(TEST_SOURCES))

OBJECTS += $(TOML_OBJ)
OBJECTS += $(SILLY_OBJ)
OBJECTS += $(DUB_LL_OBJ)

# ##################
# # compiler flags #
# ##################
# pkg-config --cflags $(PKGS) gets compiler flags needed for specified packages
CFLAGS += -I$(INCDIR) $(shell pkg-config --cflags $(PKGS))
CFLAGS += -I$(TOML_INC)
CFLAGS += -I$(SILLY_INC)
CFLAGS += -I$(DUB_LL_INC)

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
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TESTBINDIR)/%: $(TESTDIR)/%.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) | $(TESTBINDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) -I$(UNITY_INC) $(DEPFLAGS) -o $@ $< $(UNITY_SRC) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(LDFLAGS)

$(TOML_OBJ): $(TOML_SRC) | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(TOML_DEP))
	$(CC) $(CFLAGS) -MMD -MP -MF $(TOML_DEP) -c $< -o $@

$(SILLY_OBJ): $(SILLY_SRC) | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(SILLY_DEP))
	$(CC) $(CFLAGS) -MMD -MP -MF $(SILLY_DEP) -c $< -o $@

$(DUB_LL_OBJ): $(DUB_LL_SRC) | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DUB_LL_DEP))
	$(CC) $(CFLAGS) -MMD -MP -MF $(DUB_LL_DEP) -c $< -o $@

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
-include $(TOML_DEP)

run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# standard target name for cleanup opersations
clean:
	rm -rf $(BUILDDIR)

# clean is not a file name; run 'clean' recipe even if 'clean' file exists
.PHONY: clean test test-build
