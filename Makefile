PROJDIR = .
SRCDIR = $(PROJDIR)/src
INCDIR = $(PROJDIR)/include

SRC_CFILES = $(shell find $(PROJDIR) -type f -name "*.c")
SRC_SFILES = $(shell find $(PROJDIR) -type f -name "*.s")
OBJ_CFILES = $(patsubst %.c, %.o, $(SRC_CFILES))
OBJ_SFILES = $(patsubst %.s, %.o, $(SRC_SFILES))

.PHONY: all clean run

WARNINGS = -Wall -Wextra

CFLAGS  = -std=gnu99 -ffreestanding -O0 $(WARNINGS) -I$(INCDIR)
ASFLAGS = -f elf32

LD = i686-elf-gcc
CC = i686-elf-gcc
AS = nasm

LDFILE  = $(SRCDIR)/linker.ld
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

OBJ_FILES = $(OBJ_CFILES) $(OBJ_SFILES)

OPERATING_SYSTEM = tarediiran-os.bin

VIRTUAL_MACHINE = qemu-system-i386
VIRTUAL_MACHINE_FLAGS = -kernel

all: $(OPERATING_SYSTEM)

run: all
	$(VIRTUAL_MACHINE) $(VIRTUAL_MACHINE_FLAGS) $(OPERATING_SYSTEM)

$(OPERATING_SYSTEM): $(OBJ_FILES)
	$(LD) -T $(LDFILE) -o $(OPERATING_SYSTEM) $(LDFLAGS) $(OBJ_FILES)

%.o: %.s
	$(AS) $< $(ASFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(OPERATING_SYSTEM)

