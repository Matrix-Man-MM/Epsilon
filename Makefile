.POSIX:
.PHONY: all clean

CC = clang -target x86_64-unknown-windows -fuse-ld=lld-link -Wl,-subsystem:efi_application -Wl,-entry:efi_main
CCFLAGS = -std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib

SRC = src/efi.c
TGT = bin/BOOTX64.EFI

all: mkbin $(TGT)

$(TGT): $(SRC)
	$(CC) $(CCFLAGS) -o $@ $<
	cp $(TGT) img

mkbin:
	mkdir -p bin

clean:
	rm -rf bin
