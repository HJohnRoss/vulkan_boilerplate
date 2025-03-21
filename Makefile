# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11 -Wall -Wextra

# Include directories
INCLUDE_DIRS = -Iinclude -IC:/VulkanSDK/1.4.309.0/include -Ilibs/glfw/include -Ilibs/cglm/include

# Library directories
LIB_DIRS = -Llib -LC:/VulkanSDK/1.4.309.0/Lib -Llibs/glfw/lib-mingw-w64

# Libraries to link
LIBS = -lglfw3 -lvulkan-1 -lgdi32

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

# Executable name
TARGET = bin/vulk.exe

# Default target
all: $(TARGET)

# Create the obj directory if it doesn't exist
obj:
	mkdir -p obj

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LIB_DIRS) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files into object files
obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf obj

.PHONY: all clean
