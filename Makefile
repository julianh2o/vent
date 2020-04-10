MAIN = main

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src ./tests

# Compiler to use:
CC = g++

# Compile-time flags:
#CFLAGS = -g -Wall -std=c++11
CFLAGS = -Wall -std=c++11 -O2

# Header files directories, example:
# INCLUDES = -I/home/newhall/include  -I../include
INCLUDES =

# Library paths in addition to /usr/lib, for example
#   LFLAGS = -L/home/newhall/lib  -L../lib
LFLAGS =

# Libraries to link into executable, for example,
# to link with libmylib.so and libm.so use:
# LIBS = -lmylib -lm
# -lpthread
LIBS = -lpthread

SRCS := $(shell find $(SRC_DIRS) -name *.cpp ! -name main.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#g++ -Wall -std=c++11 -O2  -o main fake_hardware_test.o test_suite.o vent_machine_test.o vent_state_test.o ../fake_hardware.o ../hardware_interface.o ../time_counter.o ../vent.o ../vent_machine.o ../vent_state.o  -lpthread

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
