# GDB Debugging Script for Kernel
#
# This script helps you debug the kernel using GDB.
# 
# Usage:
#   1. In one terminal: make debug
#   2. In another terminal: gdb -x debug.gdb
#
# Or manually:
#   gdb -ex 'target remote localhost:1234' -ex 'symbol-file build/kernel.bin'

# Connect to QEMU's GDB server
target remote localhost:1234

# Load kernel symbols
symbol-file build/kernel.bin

# Set architecture to i386
set architecture i386

# Useful GDB commands:
# - break kernel_main    : Set breakpoint at kernel_main
# - break _start         : Set breakpoint at entry point
# - continue (or c)      : Continue execution
# - step (or s)          : Step into function
# - next (or n)          : Step over function
# - print variable       : Print variable value
# - info registers       : Show all CPU registers
# - x/10i $pc            : Disassemble 10 instructions at PC
# - x/10x $esp           : Show 10 words on stack

# Print a welcome message
echo \n
echo ========================================\n
echo Kernel Debugging Session Started\n
echo ========================================\n
echo \n
echo Useful commands:\n
echo   break kernel_main  - Set breakpoint at kernel_main\n
echo   continue           - Start/continue execution\n
echo   info registers     - Show CPU registers\n
echo   x/10i $pc          - Disassemble at PC\n
echo \n

b idt.c:205
c
