# @desc		Makefile for compiling and running the entire operating system
# @author	Davide Della Giustina
# @date		14/11/2019


C_SOURCES = $(wildcard src/kernel/*.c src/drivers/*.c src/cpu/*.c src/libc/*.c)
C_HEADERS = $(wildcard src/kernel/*.h src/drivers/*.h src/cpu/*.h src/libc/*.h)
OBJ = $(C_SOURCES:.c=.o src/cpu/interrupt.o) # Extension replacement

KERNEL_SIZE = $$(wc -c < 'src/kernel/kernel.bin') # Compute kernel size (in bytes)
KERNEL_SECTORS_SIZE = $$((($(KERNEL_SIZE)+511)/512)) # Compute kernel size (in sectors)

SH = bash
SFLAGS = -i
CC = i386-elf-gcc
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror
LD = i386-elf-ld

.PHONY: all
.PHONY: run
.PHONY: vbox
.PHNOY: clean

all: out/os-image.bin # Default target

%.bin: %.asm src/kernel/kernel.bin
	$(SH) $(FLAGS) -c "echo Kernel takes $(KERNEL_SECTORS_SIZE) sectors"
	$(SH) $(SFLAGS) -c "nasm -fbin -dKERNEL_SECTORS_SIZE=$(KERNEL_SECTORS_SIZE) $< -o $@"

%.o: %.asm
	$(SH) $(SFLAGS) -c "nasm -felf $< -o $@"

%.o: %.c $(C_HEADERS)
	$(SH) $(SFLAGS) -c "$(CC) $(CFLAGS) -ffreestanding -c $< -o $@"

src/kernel/kernel.bin: src/kernel/kernel_entry.o $(OBJ)
	$(SH) $(SFLAGS) -c "$(LD) -e kmain -Ttext 0x1000 $^ -o $@ --oformat binary"

out/os-image.bin: src/boot/bootsect.bin src/kernel/kernel.bin
	$(SH) $(SFLAGS) -c "cat $^ > $@"

run: all
	qemu out/os-image.bin

vbox: all
	$(SH) $(SFLAGS) -c "dd if=/dev/zero of=out/floppy.img ibs=1k count=1440"
	$(SH) $(SFLAGS) -c "dd if=out/os-image.bin of=out/floppy.img conv=notrunc"

clean:
	rm -rf src/boot/*.o src/boot/*.bin src/kernel/*.o src/kernel/*.bin src/drivers/*.o src/cpu/*.o src/libc/*.o
	rm -rf out/os-image.bin out/floppy.img