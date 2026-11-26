CC      = gcc
# Warnings required by the task (-Werror: every warning stops the build).
# -Iinclude: also look for headers in include/
# -MMD -MP: write .d files so a header change rebuilds the .c files that use it
CFLAGS  = -Wall -Wextra -Werror -Iinclude -MMD -MP
# Libraries to link: libmosquitto.so and libcjson.so
LDLIBS  = -lmosquitto -lcjson

TARGET  = iot_monitor
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:src/%.c=build/%.o)
DEP     = $(OBJ:.o=.d)

# First rule = default: plain "make" builds everything
all: $(TARGET)

# Link: combine all object files and the libraries into the program
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDLIBS)

# Compile: each src/X.c becomes build/X.o (only if X.c or its headers changed)
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build folder if it does not exist yet
build:
	mkdir -p build

# Remove all build output
clean:
	rm -rf build $(TARGET)

# Load the header dependency files written by -MMD (silently skip if missing)
-include $(DEP)

# all and clean are actions, not files
.PHONY: all clean
