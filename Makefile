ifndef config
  config=debug
endif

ifeq ($(config),debug)
  COMPILECONFIG := -O0 -DDEBUG_

else ifeq ($(config),release)
  COMPILECONFIG := -O3

else
  $(error "invalid configuration $(config)")
endif

TARGETNAME := asteroids
TESTTARGETNAME := asteroidsTest

ifeq ($(OS),Windows_NT)
	TARGET := $(TARGETNAME).exe
	TESTTARGET := $(TESTTARGETNAME).exe
else
	TARGET := $(TARGETNAME)
	TESTTARGET := $(TESTTARGETNAME)
endif

DEFINEFLAGS := $(DFLAGS:%=-D%)
CXX := g++
CXXFLAGS := -g -std=c++20 -Wall -Wextra -Werror $(COMPILECONFIG) -pthread $(DEFINEFLAGS)
TESTFLAGS := -g -std=c++20 -Wextra -Werror $(COMPILECONFIG) -pthread $(DEFINEFLAGS)
TESTCOVERAGEFLAGS := $(TESTFLAGS) -fprofile-arcs -ftest-coverage

CC := gcc
CCFLAGS := -g $(DEFINEFLAGS)

INCDIR := include
RAYLIBINCDIR := raylib/include
GOOGLETESTBIN := googletestbin
GOOGLETESTDIR := ../googletest
GOOGLETESTINCDIR := $(GOOGLETESTDIR)/googletest/include
GOOGLEMOCKINCDIR := $(GOOGLETESTDIR)/googlemock/include
GOOGLETESTFLAGS := -Wall -Wextra -Werror -O3 -std=c++17 -pthread -c
OBJDIR := obj
TESTOBJDIR := tstobj
SRCDIR := src
TESTSRCDIR := test/testSrc
MOCKINCDIR := test/mockInclude
MOCKSRCDIR := test/mockSrc
LIBS := -L ./raylib/lib/ -lraylib -lgdi32 -lwinmm
TESTLIBS := $(GOOGLETESTBIN)/libgtest.a $(GOOGLETESTBIN)/libgtest_main.a $(GOOGLETESTBIN)/libgmock.a $(GOOGLETESTBIN)/libgmock_main.a -lgcov

INC := $(INCDIR) $(RAYLIBINCDIR)
INCLUDE := $(INC:%=-I%)

TESTINC := $(GOOGLETESTINCDIR) $(GOOGLEMOCKINCDIR) $(MOCKINCDIR)
TESTINCLUDE := $(TESTINC:%=-I%)

CXXSRC := $(SRCDIR)
CXXSRCS := $(wildcard $(CXXSRC)/*.cpp)

TESTSRC := $(TESTSRCDIR)
TESTSRCS := $(wildcard $(TESTSRC)/*.cpp)

MOCKSRC := $(MOCKSRCDIR)
MOCKSRCS := $(wildcard $(MOCKSRC)/*.cpp)

OBJS := $(patsubst $(CXXSRC)/%.cpp, $(OBJDIR)/%.o, $(CXXSRCS))
TESTOBJS := $(patsubst $(CXXSRC)/%.cpp, $(TESTOBJDIR)/%.o, $(CXXSRCS))
TESTSRCOBJS := $(patsubst $(TESTSRC)/%.cpp, $(TESTOBJDIR)/%.o, $(TESTSRCS))
MOCKOBJS := $(patsubst $(MOCKSRC)/%.cpp, $(TESTOBJDIR)/%.o, $(MOCKSRCS))

#parallel compilation
MAKEFLAGS += -j$(nproc)

.PHONY: all test clean help
all: $(OBJDIR) $(OBJS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(OBJS) main.cpp -o $(TARGET) $(LIBS)
	@echo make all successful

$(OBJDIR):
	@echo Creating $(OBJDIR)
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : $(CXXSRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

test: $(GOOGLETESTBIN) $(TESTOBJDIR) $(TESTOBJS) $(TESTSRCOBJS) $(MOCKOBJS)
	$(CXX) $(INCLUDE) $(TESTINCLUDE) $(TESTFLAGS) $(TESTOBJS) $(TESTSRCOBJS) $(MOCKOBJS) -o $(TESTTARGET) $(TESTLIBS) $(LIBS)
	@echo make test successful

$(GOOGLETESTBIN):
	@if [ ! -d $(GOOGLETESTBIN) ]; then \
		echo "build googletest as static library"; \
		if [ ! -d $(GOOGLETESTDIR) ]; then \
			echo "git clone googletest"; \
			git clone https://github.com/google/googletest.git $(GOOGLETESTDIR); \
		fi; \
		$(CXX) $(GOOGLETESTFLAGS) \
			-I$(GOOGLETESTDIR)/googletest/include \
			-I$(GOOGLETESTDIR)/googletest \
			-I$(GOOGLETESTDIR)/googlemock/include \
			-I$(GOOGLETESTDIR)/googlemock \
			$(GOOGLETESTDIR)/googletest/src/gtest-all.cc \
			$(GOOGLETESTDIR)/googletest/src/gtest_main.cc \
			$(GOOGLETESTDIR)/googlemock/src/gmock-all.cc \
			$(GOOGLETESTDIR)/googlemock/src/gmock_main.cc; \
		mkdir -p $(GOOGLETESTBIN); \
		mv -t $(GOOGLETESTBIN) gtest-all.o gtest_main.o gmock-all.o gmock_main.o; \
		ar -rv $(GOOGLETESTBIN)/libgtest.a $(GOOGLETESTBIN)/gtest-all.o; \
		ar -rv $(GOOGLETESTBIN)/libgtest_main.a $(GOOGLETESTBIN)/gtest_main.o; \
		ar -rv $(GOOGLETESTBIN)/libgmock.a $(GOOGLETESTBIN)/gmock-all.o; \
		ar -rv $(GOOGLETESTBIN)/libgmock_main.a $(GOOGLETESTBIN)/gmock_main.o; \
	fi

$(TESTOBJDIR):
	@echo Creating $(TESTOBJDIR)
	mkdir -p $(TESTOBJDIR)

$(TESTOBJDIR)/%.o : $(CXXSRC)/%.cpp
	$(CXX) $(TESTFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJDIR)/%.o : $(TESTSRC)/%.cpp
	$(CXX) $(TESTFLAGS) $(INCLUDE) $(TESTINCLUDE) -c $< -o $@

$(TESTOBJDIR)/%.o : $(MOCKSRC)/%.cpp
	$(CXX) $(TESTFLAGS) $(INCLUDE) $(TESTINCLUDE) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(TESTTARGET)
	rm -rf $(OBJDIR)
	rm -rf $(TESTOBJDIR)

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   test"
	@echo "   clean"
	@echo ""
