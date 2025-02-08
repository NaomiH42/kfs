# KFS

This project is a minimal, bare-metal operating system kernel written in C++ and assembly. It serves as a learning platform for low-level OS development and demonstrates key concepts such as booting via GRUB, kernel initialization, setting up the Global Descriptor Table (GDT), handling VGA text mode output, and controlling the binary layout with a custom linker script.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
  - [Boot Process](#boot-process)
  - [Kernel Initialization](#kernel-initialization)
  - [Global Descriptor Table (GDT)](#global-descriptor-table-gdt)
  - [Terminal and VGA Text Mode](#terminal-and-vga-text-mode)
  - [Linking and Binary Layout](#linking-and-binary-layout)
- [Build and Run Instructions](#build-and-run-instructions)
- [Conclusion](#conclusion)

---

## Project Overview

This operating system kernel is designed for educational purposes. It runs in 32-bit protected mode using a custom GDT for memory segmentation, and outputs text directly to the screen via VGA text mode. The kernel is compiled using a cross-compiling toolchain (e.g., `i686-elf-g++`, `i686-elf-gcc`) and assembled with NASM. GRUB is used as the bootloader, and the final binary is linked with a custom linker script to ensure proper placement in memory.

---

## Project Structure
```
├── Makefile # Automates build, ISO generation, and QEMU execution
├── src
│
├── linker.ld # Custom linker script defining memory layout
│
├── grub.cfg # GRUB configuration file for booting the OS
│
├── kernel_main.cpp # Entry point of the kernel (kernel_main)
│
├── gdt.cpp # GDT initialization functions
│
├── gdt.asm # Assembly code: multiboot header, _start, gdt_flush, gdt_test │ 
├── Terminal.cpp # Implementation of the Terminal class (VGA text mode) 
│ 
├── Terminal.hpp # Terminal class declaration 
│ 
├── vga.hpp # VGA definitions (colors, entry functions, constants) 
│ 
└── headers.hpp # Common definitions, data structures, and prototypes 
└── isodir # Temporary directory for generating the bootable ISO image
```
---

## How It Works

### Boot Process

1. **Multiboot Header:**  
   The OS kernel contains a multiboot header (defined in the assembly file) that complies with the [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html). This header provides the magic number, flags, and checksum that GRUB checks to load the kernel properly.

2. **GRUB Bootloader:**  
   GRUB loads the binary (`os.bin`) from the bootable ISO. The ISO is structured with a proper GRUB configuration (`grub.cfg`), and GRUB passes control to the kernel’s entry point.

3. **Kernel Entry Point:**  
   The assembly entry point `_start` (defined in `gdt.asm`) is the first code executed. It calls the C++ function `kernel_main`, after which it halts the CPU using a loop (`hlt`) to prevent any undefined behavior.

### Kernel Initialization

1. **GDT Setup:**  
   The kernel initializes the Global Descriptor Table (GDT) by:
   - Defining a **null segment** (always the first entry).
   - Setting up kernel segments for **code**, **data**, and **stack**.
   - Creating user segments for **code**, **data**, and **stack**.
   
   In the function `init_gdt()` (in `gdt.cpp`), an array of `GDTEntry` structures is populated with base addresses, limits, access rights, and granularity settings. A pointer structure (`GDTPointer`) is placed at a fixed address (e.g., `0x00000800`) to be passed to the GDT flush routine.

2. **GDT Flushing:**  
   After configuring the GDT, the assembly routine `gdt_flush` is called. This routine:
   - Loads the new GDT using the `lgdt` instruction.
   - Updates all the segment registers (DS, ES, FS, GS, SS) and then performs a far jump to update CS.
   
   This is crucial for ensuring the CPU uses the new segment definitions.

3. **Terminal Initialization:**  
   The `Terminal` class (implemented in `Terminal.cpp`) initializes the VGA text mode by:
   - Clearing the screen (writing blank characters with the default color).
   - Setting the default text color.
   - Providing methods (`write`, `putchar`, etc.) to output strings and hexadecimal numbers directly to the VGA buffer at address `0xB8000`.

4. **Kernel Output:**  
   In `kernel_main`, after the GDT is set up, the kernel:
   - Prints an ASCII art banner in a colorful style.
   - Calls `gdt_test()` to verify the integrity of the GDT setup.
   - Uses the `printk` function to output each GDT entry’s details (such as limits, base addresses, and flags) to the screen.

### Global Descriptor Table (GDT)

- **Purpose:**  
  The GDT is used to define the characteristics of the various memory segments in protected mode. This includes setting segment limits, base addresses, access rights, and flags.

- **Implementation:**  
  The project creates 7 GDT entries:
  - **Entry 0:** Null segment (unused).
  - **Entry 1:** Kernel code segment.
  - **Entry 2:** Kernel data segment.
  - **Entry 3:** Kernel stack segment.
  - **Entry 4:** User code segment.
  - **Entry 5:** User data segment.
  - **Entry 6:** User stack segment.

- **Flushing:**  
  The `gdt_flush` function (in assembly) ensures the CPU uses the new GDT by reloading the segment registers.

### Terminal and VGA Text Mode

- **VGA Basics:**  
  The VGA text mode buffer is located at `0xB8000` and is used to display characters on the screen. Each character cell is represented by a 16-bit value: the lower 8 bits for the ASCII character and the upper 8 bits for the color attributes.

- **Terminal Class:**  
  The `Terminal` class manages screen output:
  - It clears the screen at initialization.
  - It provides functions to write strings and hexadecimal values.
  - It supports writing characters in different colors, enabling simple visual feedback (like the ASCII art banner).

### Linking and Binary Layout

- **Custom Linker Script (`linker.ld`):**  
  The linker script is used to define the memory layout of the kernel:
  - The kernel is loaded at the 1 MB mark (`. = 1M;`).
  - Sections such as `.multiboot`, `.text`, `.rodata`, `.data`, and `.bss` are placed in specific memory regions.
  - Alignment directives (e.g., `ALIGN(4K)`) ensure that sections start at page boundaries.
  
  This controlled layout is essential because the bootloader and the CPU expect the kernel to be positioned at particular addresses.

- **Compilation and Linking:**  
  - **C++ Files:** Compiled using `i686-elf-g++` with freestanding options (disabling standard libraries).
  - **Assembly Files:** Assembled with NASM.
  - **Linking:** Object files are linked using `i686-elf-gcc` along with the custom linker script to produce the final bootable binary (`os.bin`).

---

## Build and Run Instructions

1. **Prerequisites:**  
   Ensure you have installed:
   - Cross-compilers (`i686-elf-g++` and `i686-elf-gcc`)
   - NASM assembler
   - GRUB2 (for creating the bootable ISO)
   - QEMU (for emulation)

2. **Build the Kernel:**  
   Run the following command in the project root:
```bash
make
```
   This compiles the C++ and assembly source files, links them into os.bin, and then creates a bootable ISO image (os.iso).

4. **Run the Kernel:**
   To test the kernel in an emulated environment:
```bash
make run
```
  QEMU will launch the ISO image, and you should see the ASCII art banner, GDT test, and the printed GDT entries on the screen.

4. **Clean Build Artifacts:**
  To remove compiled files and directories:
```
make clean
```
  For a complete clean-up:
```
make fclean
```
