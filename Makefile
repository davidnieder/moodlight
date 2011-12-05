# platform
MCU = attiny2313
# object format
FORMAT = ihex
# target name
TARGET = moodlight
# gcc input files
SRC = $(TARGET).c
# optimization level
OPT = s
# gcc ouput file
GCCOUT = $(TARGET).elf
# opjcopy output file
OBJOUT = $(TARGET).hex

# gcc flags
CFLAGS = -O$(OPT)
CFLAGS += -mmcu=$(MCU)
CFLAGS += -Wall

# objcopy flags
OBJFLAGS = -O $(FORMAT)

# commands
CC = avr-gcc
OBJCOPY = avr-objcopy
REMOVE = rm -f


# Target all
all: build

build:
	$(CC) $(CFLAGS) $(SRC) -o $(GCCOUT)
	$(OBJCOPY) $(OBJFLAGS) $(GCCOUT) $(OBJOUT)

# Target: clean
clean: clean_list

clean_list:
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).hex

