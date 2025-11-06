ifndef config
  config=debug_x64
endif

ifeq ($(config),debug_x64)
  sandbox_config = debug_x64

else ifeq ($(config),debug_x86)
  sandbox_config = debug_x86

else ifeq ($(config),debug_arm64)
  sandbox_config = debug_arm64

else ifeq ($(config),release_x64)
  sandbox_config = release_x64

else ifeq ($(config),release_x86)
  sandbox_config = release_x86

else ifeq ($(config),release_arm64)
  sandbox_config = release_arm64

else
  $(error "invalid configuration $(config)")
endif

TARGETNAME = asteroids

ifeq ($(OS),Windows_NT)
	TARGET = $(TARGETNAME).exe
else
	TARGET = $(TARGETNAME)
endif

DEFINEFLAGS = $(DFLAGS:%=-D%)
CXX = g++
CXXFLAGS = -g -std=c++20 $(DEFINEFLAGS)

CC = gcc
CCFLAGS = -g $(DEFINEFLAGS)

INCDIR = include
RAYLIBINCDIR = raylib/include

INC = $(INCDIR) $(RAYLIBINCDIR)
INCLUDE = $(INC:%=-I%)

OBJDIR = obj
SRCDIR = src

CXXSRC = $(SRCDIR)
CXXSRCS = $(wildcard $(CXXSRC)/*.cpp)

OBJS = $(patsubst $(CXXSRC)/%.cpp, $(OBJDIR)/%.o, $(CXXSRCS))

LIBS = -L ./raylib/lib/ -lraylib -lgdi32 -lwinmm

.PHONY: all clean help
all : $(OBJDIR) $(OBJS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

$(OBJDIR):
	@echo Creating $(OBJDIR)
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : $(CXXSRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_x64"
	@echo "  debug_x86"
	@echo "  debug_arm64"
	@echo "  release_x64"
	@echo "  release_x86"
	@echo "  release_arm64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   asteroids"
	@echo ""
