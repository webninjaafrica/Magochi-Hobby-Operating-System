# Makefile for Magochi v20 GUI OS

CC = gcc
AS = nasm
LD = ld
CFLAGS = -m32 -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -I. -Ikernel
AFLAGS = -f elf32
LDFLAGS = -T boot/linker.ld -nostdlib -melf_i386

OBJS = boot/boot.o \
       kernel/main.o \
       kernel/framebuffer.o \
       kernel/keyboard.o \
       kernel/mouse.o \
       kernel/login.o \
       kernel/desktop.o \
       kernel/string.o

all: magochi_v20.iso

magochi_v20.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "✓ Binary created: $@"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(AFLAGS) $< -o $@

magochi_v20.iso: magochi_v20.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/magochi_v20.bin
	echo 'set timeout=2' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Magochi v20 GUI OS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/magochi_v20.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso 2>/dev/null || xorriso -as mkisofs -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o $@ iso
	@echo "✓ ISO created: $@"

clean:
	rm -rf $(OBJS) magochi_v20.bin magochi_v20.iso iso
	@echo "✓ Cleaned build files"

run: magochi_v20.iso
	qemu-system-x86_64 -cdrom $@ -m 256M

.PHONY: all clean run
