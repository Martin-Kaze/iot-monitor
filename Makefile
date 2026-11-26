
CC      = gcc
# Warnings required by the task; 
CFLAGS  = -Wall -Wextra -Werror
# Libraries to link: libmosquitto.so and libcjson.so
LDLIBS  = -lmosquitto -lcjson

TARGET  = iot_monitor
SRC     = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
