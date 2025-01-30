SRC_F = src
OBJ_F = obj

CPP_SRC = $(wildcard $(SRC_F)/*.cpp)
ASM_SRC = $(wildcard $(SRC_F)/*.s)

CPP_OBJ = $(patsubst $(SRC_F)/%.cpp, $(OBJ_F)/%.o, $(CPP_SRC))
ASM_OBJ = $(patsubst $(SRC_F)/%.s, $(OBJ_F)/%.o, $(ASM_SRC))

OBJ = $(CPP_OBJ) $(ASM_OBJ)

CC = i686-elf-g++
AS = nasm -felf32
CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -g

LINKER = i686-elf-gcc
LDFLAGS = -T $(SRC_F)/linker.ld -ffreestanding -O2 -nostdlib -lgcc

TARGET = os.bin

all: build

build: $(TARGET) bootable_image

$(TARGET): $(OBJ)
	$(LINKER) $(LDFLAGS) -o $@ $^ #$@Target $^ all prerequisites

$(OBJ_F)/%.o: $(SRC_F)/%.cpp | $(OBJ_F)
	$(CC) $(CFLAGS) -c $< -o $@ # $< prerequisit

$(OBJ_F)/%.o: $(SRC_F)/%.s | $(OBJ_F)
	$(AS) $< -o $@

$(OBJ_F):
	mkdir -p $(OBJ_F)

bootable_image:
	mkdir -p isodir/boot/grub
	cp os.bin isodir/boot/os.bin
	cp $(SRC_F)/grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o os.iso isodir

run:
	qemu-system-i386 -cdrom os.iso

clean:
	rm -rf $(OBJ_F)/*.o $(TARGET)
	rm -rf $(OBJ_F)
	rm -rf isodir

fclean: clean
	rm -f os.iso

re: fclean all

.PHONY: all clean re
