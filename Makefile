# Makefile for Columbia Optimizer on Linux
# Compiler and flags
CXX = g++
# Debug版本：-O0禁用优化，-g生成调试信息
# CXXFLAGS = -Wall -g -O0 -std=c++98 -DLINUX_PORT -DUNIQ -DIRPROP
CXXFLAGS = -Wall -g -O0 -std=c++98 -DLINUX_PORT -DUNIQ
INCLUDES = -Iheader
LDFLAGS = 

# Directories
SRCDIR = cpp
HEADERDIR = header
OBJDIR = obj
BINDIR = .

# Target executable
TARGET = $(BINDIR)/columbia

# Source files
SOURCES = $(SRCDIR)/bm.cpp \
          $(SRCDIR)/cat.cpp \
          $(SRCDIR)/cm.cpp \
          $(SRCDIR)/expr.cpp \
          $(SRCDIR)/global.cpp \
          $(SRCDIR)/group.cpp \
          $(SRCDIR)/item.cpp \
          $(SRCDIR)/logop.cpp \
          $(SRCDIR)/main.cpp \
          $(SRCDIR)/main_linux.cpp \
          $(SRCDIR)/linux_compat.cpp \
          $(SRCDIR)/mexpr.cpp \
          $(SRCDIR)/physop.cpp \
          $(SRCDIR)/query.cpp \
          $(SRCDIR)/rules.cpp \
          $(SRCDIR)/ssp.cpp \
          $(SRCDIR)/stdafx.cpp \
          $(SRCDIR)/supp.cpp \
          $(SRCDIR)/tasks.cpp

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(OBJDIR) $(TARGET)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJDIR) $(TARGET)
	rm -f colout.txt script.cove tempquery

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild
