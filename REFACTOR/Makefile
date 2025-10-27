CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99 -I$(INCDIR) -I$(GD_INC)
TARGET = clover

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

##########################
###### GD LIBRARIES ######
##########################
GD_DIR = $(LIBDIR)/c-data-structures
GD_SRC = $(GD_DIR)/src
GD_INC = $(GD_DIR)/include

##########################
####### TESTING ##########
##########################
TESTDIR = tests
TESTBINDIR = $(BUILDDIR)/test-bin

UNITY_DIR = $(LIBDIR)/Unity
UNITY_SRC = $(UNITY_DIR)/src/unity.c
UNITY_INC = $(UNITY_DIR)/src

#####################
# find source files #
#####################
SRC_SOURCES = $(shell find $(SRCDIR) -name "*.c")
GD_SOURCES = $(shell find $(GD_SRC) -name "*.c")
SOURCES = $(SRC_SOURCES) $(GD_SOURCES)

SRC_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC_SOURCES))
GD_OBJECTS = $(patsubst $(GD_SRC)/%.c,$(OBJDIR)/gd/%.o,$(GD_SOURCES))
OBJECTS = $(SRC_OBJECTS) $(GD_OBJECTS)

SRC_DEPS = $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SRC_SOURCES))
GD_DEPS = $(patsubst $(GD_SRC)/%.c,$(DEPDIR)/gd/%.d,$(GD_SOURCES))
DEPS = $(SRC_DEPS) $(GD_DEPS)

TEST_SOURCES = $(shell find $(TESTDIR) -name "*.c")
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(TEST_SOURCES))
TEST_DEPS = $(patsubst $(TESTDIR)/%.c,$(DEPDIR)/%.d,$(TEST_SOURCES))
TEST_BINARIES = $(patsubst $(TESTDIR)/%.c,$(TESTBINDIR)/%,$(TEST_SOURCES))

DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$*.d

# ######################
# # create directories #
# ######################
$(shell mkdir -p $(OBJDIR) $(BINDIR) $(DEPDIR) $(TESTBINDIR))

# ###############
# # build rules #
# ###############

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJDIR)/gd/%.o: $(GD_SRC)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/gd/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TESTBINDIR)/%: $(TESTDIR)/%.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) | $(TESTBINDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) -I$(UNITY_INC) $(DEPFLAGS) -o $@ $< $(UNITY_SRC) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(LDFLAGS)

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

run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# standard target name for cleanup opersations
clean:
	rm -rf $(BUILDDIR)

# clean is not a file name; run 'clean' recipe even if 'clean' file exists
.PHONY: clean test test-build
