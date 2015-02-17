TESTS_DIR=.
SRC_DIR=../src
HEADER_DIR=$(SRC_DIR)
CUSTOM_CPPFLAGS= -isystem $(HEADER_DIR)
CUSTOM_CXXFLAGS= -Wall -Wextra
CXX=g++

# If we're on Windows, add these flags for static linking, so the executables are portable
ifeq ($(OS), Windows_NT)
    CUSTOM_CXXFLAGS += -static-libgcc -static-libstdc++
endif

# The list of all tests that exist
TESTS=player_unittest.exe

# Relative path to the gtest root directory
GTEST_ROOT = ../libs/gtest-1.7.0

# IT IS UNLIKELY ANYTHING WILL NEED TO BE MODIFIED BETWEEN HERE AND LINE 59




# Required settings for gtest to work, none of this should need to be modified
GTEST_CPPFLAGS = -isystem $(GTEST_ROOT)/include

# If we're not on Windows, we need pthreads.  Otherwise it will just use Windows native threading
ifneq ($(OS), Windows_NT)
    GTEST_CXXFLAGS = -pthread
endif

GTEST_HEADERS = $(GTEST_ROOT)/include/gtest/*.h \
				$(GTEST_ROOT)/include/gtest/internal/*.h

GTEST_SRCS = $(GTEST_ROOT)/src/*.cc $(GTEST_ROOT)/src/*.h $(GTEST_HEADERS)

GTEST_H = $(GTEST_ROOT)/include/gtest/gtest.h



# Set our CPP and CXX flags
CPPFLAGS+=$(GTEST_CPPFLAGS) $(CUSTOM_CPPFLAGS)
CXXFLAGS+=$(GTEST_CXXFLAGS) $(CUSTOM_CXXFLAGS)


# Basic build rules, and rules required for gtest
all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

gtest-all.o : $(GTEST_SRCS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_ROOT) $(CXXFLAGS) -c \
		$(GTEST_ROOT)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_ROOT) $(CXXFLAGS) -c \
		$(GTEST_ROOT)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^




# ADD CUSTOM TESTS BELOW HERE AND ADD TO THE TESTS LIST AT THE TOP

player.o : $(SRC_DIR)/player.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

player_unittest.o : $(TESTS_DIR)/player_unittest.cpp \
							 $(HEADER_DIR)/player.hpp  $(GTEST_H)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

player_unittest.exe : player.o player_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
