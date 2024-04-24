# Makefile for ATmega328P

# Compiler
CC = avr-gcc

# Compiler flags
CFLAGS = -mmcu=atmega328p -Os -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Output file names
TARGET = $(BUILD_DIR)/firmware
HEX_TARGET = $(TARGET).hex

all: $(HEX_TARGET)

$(HEX_TARGET): $(TARGET)
	avr-objcopy -O ihex -R .eeprom $< $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(HEX_TARGET)