
TESTS=player_unittest.common.exe \
	  globals_unittest.common.exe \
	  model_unittest.common.exe \
	  action_unittest.common.exe \
	  clientControllerState_unittest.client.exe \
	  serverControllerState_unittest.server.exe

TESTS_DIR=.
SRC_DIR=../src
COMMON_DIR=$(SRC_DIR)/common
CLIENT_DIR=$(SRC_DIR)/client
SERVER_DIR=$(SRC_DIR)/server
PROTOS_DIR=$(SRC_DIR)/protos

PROTOS_LIB=`pkg-config --cflags protobuf --libs protobuf`

LIBS=-pthread $(PROTOS_LIB)
COMMON_OBJS=$(COMMON_DIR)/*.o
SERVER_OBJS=$(SERVER_DIR)/*.o
CLIENT_OBJS=$(CLIENT_DIR)/*.o
PROTOS_OBJS=$(PROTOS_DIR)/*.o
CUSTOM_CPPFLAGS= -I$(COMMON_DIR) -I$(CLIENT_DIR) -I$(SERVER_DIR) -I$(PROTOS_DIR)
CUSTOM_CXXFLAGS= -Wall -Wextra -std=c++11 -ggdb
CXX=g++

# If we're on Windows, add these flags for static linking, so the executables are portable
ifeq ($(OS), Windows_NT)
	LIBS += lws_32
    CUSTOM_CXXFLAGS += -static-libgcc -static-libstdc++
endif

# Relative path to the gtest root directory
GTEST_ROOT = ../libs/gtest-1.7.0

# IT IS UNLIKELY ANYTHING WILL NEED TO BE MODIFIED BETWEEN HERE AND
# THE COMMENT '# END GTEST BUILD'



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
	rm -f *.exe gtest.a gtest_main.a *.o

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



# END GTEST BUILD

# Just add your .exe to the top, this should all be able to figure everything out itself
%.o : $(TESTS_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

%.common.exe : %.common.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(COMMON_OBJS) $(PROTOS_OBJS) $^ -o $@ $(LIBS)

%.client.exe : %.client.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CLIENT_OBJS) $(COMMON_OBJS) $(PROTOS_OBJS) $^ -o $@ $(LIBS)

%.server.exe : %.server.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SERVER_OBJS) $(COMMON_OBJS) $(PROTOS_OBJS) $^ -o $@ $(LIBS)

client_unittests.exe : gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CLIENT_OBJS) $(COMMON_OBJS) $(PROTOS_OBJS) *.client.cpp gtest_main.a -o $@ $(LIBS)

common_unittests.exe : gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(COMMON_OBJS) $(PROTOS_OBJS) *.common.cpp gtest_main.a -o $@ $(LIBS)

server_unittests.exe : gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SERVER_OBJS) $(COMMON_OBJS) $(PROTOS_OBJS) *.server.cpp gtest_main.a -o $@ $(LIBS)
