CC = gcc
CFLAGS = -Wall -Wextra -Wno-override-init -g -std=c99
TARGET = clover
PKGS = libevdev

# directory structure
SRCDIR = src
INCDIR = include
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin
DEPDIR = $(BUILDDIR)/deps

# find source files
SOURCES = $(shell find $(SRCDIR) -name "*.c")
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
DEPS = $(SOURCES:%.c=$(DEPDIR)/%.d)

# compiler flags
CFLAGS += -I$(INCDIR) $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS))
DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$*.d

# create directories
$(shell mkdir -p $(OBJDIR) $(BINDIR) $(DEPDIR))

# main target
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# create directories
$(OBJDIR) $(BINDIR) $(DEPDIR):
	mkdir -p $@

# include dependency files
-include $(DEPS)

clean:
	rm -rf $(BUILDDIR)

.PHONY: clean 
