.POSIX:
.PHONY: all clean

TGT = create_epsilon_img
CC = gcc
CCFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2

VM = qemu-system-x86_64
VMFLAGS = -drive format=raw,file=epsilon.hdd -bios bios64.bin -m 256M -vga std -name Epsilon -machine q35 -usb -device usb-mouse -rtc base=localtime -net none

all: $(TGT) run

run:
	./create_epsilon_img
	$(VM) $(VMFLAGS)

clean:
	rm -f $(TGT) *.img *.INF *.vhd *.EFI *.hdd
