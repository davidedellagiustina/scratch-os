# @desc		Makefile for compiling and running the entire operating system.
# @author	Davide Della Giustina
# @date		14/11/2019


C_SOURCES = $(wildcard src/kernel/*.c)
C_HEADERS = $(wildcard src/kernel/*.h)
OBJ = $(C_SOURCES:.c=.o) # Extension replacement

SH = bash
SFLAGS = -i
CC = i386-elf-gcc
CFLAGS = -g
LD = i386-elf-ld

.PHONY: all
.PHONY: run
.PHONY: vbox
.PHNOY: clean

all: run # Default target

%.bin: %.asm
	$(SH) $(SFLAGS) -c "nasm -fbin $< -o $@"

%.o: %.asm
	$(SH) $(SFLAGS) -c "nasm -felf $< -o $@"

#%.o: %.c $(C_HEADERS)
#	$(SH) $(SFLAGS) -c "$(CC) $(CFLAGS) -ffreestanding -c $< -o $@"

#src/kernel/kernel.bin: src/boot/kloader.o $(OBJ)
#	$(SH) $(SFLAGS) -c "$(LD) -Ttext 0x1000 $^ -o $@ --oformat-binary"

#out/os-image.bin: src/boot/bootsect.bin src/kernel/kernel.bin
out/os-image.bin: src/boot/bootsect.bin
	$(SH) $(SFLAGS) -c "cat $^ > $@"

run: out/os-image.bin
	qemu $<

vbox: out/os-image.bin
	$(SH) $(SFLAGS) -c "dd if=/dev/zero of=out/floppy.img ibs=1k count=1440"
	$(SH) $(SFLAGS) -c "dd if=$< of=out/floppy.img conv=notrunc"

clean:
	rm -rf src/boot/*.o src/boot/*.bin src/kernel/*.o src/kernel/*.bin
	rm -rf out/os-image.bin out/floppy.img