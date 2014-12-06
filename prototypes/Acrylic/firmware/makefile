NAME := keychange

ifeq ($(OS),Windows_NT)
S := \\
else
S := /
endif

SRCDIR := src
OBJDIR := obj
BINDIR := bin

HEX := $(BINDIR)$(S)$(NAME).hex
OUT := $(OBJDIR)$(S)$(NAME).out
MAP := $(OBJDIR)$(S)$(NAME).map

SOURCES =	main.c \
		serial.c

#HEADERS := src/inc/$(wildcard *.h)
INCLUDES = -Isrc$(S)inc -Ires
OBJECTS = $(patsubst %,$(OBJDIR)$(S)%,$(SOURCES:.c=.o))
MCU := atmega32
MCU_AVRDUDE := atmega32
MCU_FREQ := 1000000UL

CC := avr-gcc
OBJCOPY := avr-objcopy
SIZE := avr-size -C --mcu=$(MCU)

CFLAGS := -Wall -pedantic -mmcu=$(MCU) -std=c99 -g -Os -DF_CPU=$(MCU_FREQ) -gstabs

all: $(HEX)

clean:
ifeq ($(OS),Windows_NT)
	del $(HEX) $(OUT) $(MAP) $(OBJECTS)
else
	rm $(HEX) $(OUT) $(MAP) $(OBJECTS)
endif

flash: $(HEX)
	sudo avrdude -y -c usbtiny -p $(MCU_AVRDUDE) -U flash:w:$(HEX)

$(HEX): $(OUT)
	$(OBJCOPY) -R .eeprom -O ihex $< $@

$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -Wl,-Map,$(MAP) $^
	@echo = = = = = = = = =
	$(SIZE) $@

$(OBJDIR)$(S)%.o: $(SRCDIR)$(S)%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	
$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: all clean flash

