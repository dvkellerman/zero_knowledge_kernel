# Zero Knowledge Kernel - Development Plan

This document tracks our progress and plans for building the x86 kernel.

## ✅ Completed

### Phase 1: Basic Kernel Setup
- [x] **Multiboot Header** - Kernel can be loaded by GRUB
- [x] **Kernel Entry Point** - `_start()` function that receives bootloader parameters
- [x] **Kernel Main Function** - Basic kernel initialization
- [x] **Build System** - Makefile with proper compilation flags
- [x] **Linker Script** - Custom linker script for kernel layout
- [x] **Bootable ISO** - Can create bootable ISO image with GRUB

### Phase 2: Output and Debugging
- [x] **VGA Text Mode** - Text output to screen (80x25, 16 colors)
  - [x] `vga.c` / `vga.h` - VGA text mode implementation
  - [x] Functions: `vga_clear()`, `vga_puts()`, `vga_puthex()`, `vga_putuint()`, etc.
  
- [x] **Serial Port (COM1)** - Serial output for debugging
  - [x] `serial.c` / `serial.h` - Serial port implementation
  - [x] Functions: `serial_init()`, `serial_puts()`, `serial_puthex()`, `serial_putuint()`
  
- [x] **Debug Logging System** - Unified output interface
  - [x] `debug.c` / `debug.h` - Debug system that consumes VGA and serial
  - [x] Log levels: DEBUG, INFO, WARN, ERROR, PANIC
  - [x] General printing functions: `debug_puts()`, `debug_puthex()`, `debug_putuint()`
  - [x] Logging functions: `debug_debug()`, `debug_info()`, `debug_warn()`, `debug_error()`
  - [x] All printing now goes through `debug.h` interface

- [x] **Error Handling** - Panic and halt functions
  - [x] `panic()` - Critical error handler
  - [x] `halt()` - System halt function

- [x] **GDB Support** - Debugging with GDB
  - [x] Debug symbols in build (`-g` flag)
  - [x] QEMU GDB server support (`make debug`)
  - [x] GDB helper script (`debug.gdb`)

### Phase 3: Code Organization
- [x] **Modular Architecture** - Separated code into modules
  - [x] `boostrap.c` - Kernel entry and main function
  - [x] `vga.c` / `vga.h` - VGA text mode
  - [x] `serial.c` / `serial.h` - Serial port
  - [x] `debug.c` / `debug.h` - Debug system (unified interface)
- [x] **Clean Dependencies** - `debug.c` consumes `vga.c` and `serial.c`
- [x] **Unified Interface** - All printing goes through `debug.h`

## 🚧 Next Steps

### Phase 4: Interrupt Handling (Next Priority)
- [X] **Interrupt Descriptor Table (IDT)** - Set up IDT for handling interrupts
  - [X] Create IDT structure
  - [X] Load IDT with `lidt` instruction
  - [X] Implement interrupt handlers
  - [X] Handle exceptions (divide by zero, page fault, etc.)
  
- [X] **Programmable Interrupt Controller (PIC)** - Remap and configure PIC
  - [X] Remap IRQ 0-15 to interrupt vectors 32-47
  - [X] Enable/disable specific interrupts
  - [X] Handle IRQ interrupts
  
- [ ] **Timer Interrupt** - Set up timer for scheduling
  - [ ] Configure PIT (Programmable Interval Timer)
  - [ ] Implement timer interrupt handler
  - [ ] Basic time tracking

### Phase 5: Keyboard Input
- [ ] **Keyboard Driver** - PS/2 keyboard support
  - [ ] Handle keyboard interrupts (IRQ 1)
  - [ ] Scan code to ASCII conversion
  - [ ] Key press/release detection
  - [ ] Basic input buffer

### Phase 6: Memory Management
- [ ] **Physical Memory Management** - Track and allocate physical pages
  - [ ] Parse Multiboot memory map
  - [ ] Bitmap or linked list for free pages
  - [ ] Page allocation/deallocation functions
  
- [ ] **Paging** - Enable virtual memory
  - [ ] Set up page directory and page tables
  - [ ] Identity map first 4MB (or more)
  - [ ] Enable paging with CR0 register
  - [ ] Page fault handler
  
- [ ] **Heap Allocation** - Dynamic memory allocation
  - [ ] Implement `malloc()` / `free()`
  - [ ] Heap management (linked list or similar)
  - [ ] Memory fragmentation handling

### Phase 7: Process Management (Future)
- [ ] **Task Structure** - Process/task representation
  - [ ] Task control block (TCB)
  - [ ] Task state (running, ready, blocked)
  - [ ] Task context (registers, stack)
  
- [ ] **Task Switching** - Context switching between tasks
  - [ ] Save/restore CPU state
  - [ ] Switch stacks
  - [ ] Task scheduler
  
- [ ] **System Calls** - Interface for user programs
  - [ ] System call interface
  - [ ] System call handler
  - [ ] Basic system calls (read, write, exit, etc.)

### Phase 8: File System (Future)
- [ ] **Virtual File System (VFS)** - Abstract file system interface
- [ ] **Simple File System** - Basic file system implementation
  - [ ] Directory structure
  - [ ] File operations (open, read, write, close)
  - [ ] File metadata

### Phase 9: Advanced Features (Future)
- [ ] **Multitasking** - Multiple processes running concurrently
- [ ] **User Mode** - Separate kernel and user space
- [ ] **System Calls** - Complete system call interface
- [ ] **Device Drivers** - More hardware support
- [ ] **Networking** - Basic network stack (if needed)

## 📝 Notes

### Current Architecture
```
boostrap.c (kernel entry)
    ↓
    uses debug.h
    ↓
debug.c (unified output interface)
    ↓
    uses vga.h + serial.h
    ↓
vga.c + serial.c (hardware drivers)
```

### Design Principles
1. **Modularity** - Each component in its own file
2. **Unified Interface** - All printing through `debug.h`
3. **Separation of Concerns** - Clear boundaries between modules
4. **Educational** - Code is well-commented and explained

### Build and Run
```bash
# Build kernel
make

# Run in QEMU
make run

# Run with GDB
make debug
# In another terminal: gdb -x debug.gdb
```

## 🎯 Current Focus

**Next Priority: Interrupt Handling (Phase 4)**

We need to set up interrupt handling to:
- Handle hardware interrupts (keyboard, timer, etc.)
- Handle CPU exceptions (page faults, divide by zero, etc.)
- Enable proper kernel operation

This is the foundation for:
- Keyboard input
- Timer-based scheduling
- Memory management (page faults)
- Process management

