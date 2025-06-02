# Executable target name
TARGET_EXEC ?= cxc

# Source and build directories
SRC_DIRS := src
BUILD_DIR := build

# Source file types to include
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include directories (all subdirectories in SRC_DIRS)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compiler flags
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CXXFLAGS := -g -std=c++11 -Wall

# Final binary output
TARGET_PATH := $(BUILD_DIR)/$(TARGET_EXEC)

# Default target
all: $(TARGET_PATH)

# Link all object files into the final binary
$(TARGET_PATH): $(OBJS)
	@echo "Linking $@"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Pattern rules

# Assembly
$(BUILD_DIR)/%.s.o: %.s
	@echo "Assembling $<"
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# C source
$(BUILD_DIR)/%.c.o: %.c
	@echo "Compiling C $<"
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Compiling C++ $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Clean rule
.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILD_DIR)

# Include dependency files
-include $(DEPS)
