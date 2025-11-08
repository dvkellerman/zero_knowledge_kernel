# Debug Guide for Zero Knowledge Kernel

This guide explains how to use the debug functionality in the kernel.

## Debug Features

The kernel includes several debugging features:

1. **Serial Port Output (COM1)** - Output to serial port for logging
2. **Unified Debug Logging** - Log messages to both VGA and serial port
3. **Panic/Halt Functions** - Handle critical errors gracefully
4. **GDB Support** - Debug the kernel with GDB

## Serial Port Output

The kernel outputs debug messages to COM1 (serial port at 0x3F8). QEMU can redirect this output to:

### Output to stdout
```bash
make run
```
This will show serial port output in your terminal.

### Output to file
```bash
make run-log
```
This will save serial port output to `serial.log`.

### Custom serial output
You can also specify custom serial output when running QEMU:
```bash
qemu-system-i386 -cdrom kernel.iso -serial file:my_log.txt
qemu-system-i386 -cdrom kernel.iso -serial tcp:localhost:1234
```

## Debug Logging Functions

The kernel provides several debug logging functions:

- `debug_debug(message)` - Debug level messages (only to serial)
- `debug_info(message)` - Info level messages (only to serial)
- `debug_warn(message)` - Warning messages (VGA + serial)
- `debug_error(message)` - Error messages (VGA + serial)
- `panic(message)` - Critical error, halts the system

### Example Usage

```c
debug_info("Kernel starting...");
debug_warn("Memory information not available");
debug_error("Failed to initialize device");
panic("Critical error occurred!");
```

## GDB Debugging

### Step 1: Build the kernel with debug symbols

Debug symbols are automatically included when you build:
```bash
make
```

### Step 2: Start QEMU with GDB server

In one terminal:
```bash
make debug
```

This starts QEMU with:
- GDB server on port 1234
- CPU frozen at startup (waits for GDB to connect)
- Serial port output to stdout

### Step 3: Connect GDB

In another terminal, use the helper script:
```bash
gdb -x debug.gdb
```

Or manually:
```bash
gdb -ex 'target remote localhost:1234' -ex 'symbol-file build/kernel.bin'
```

### Useful GDB Commands

Once connected to GDB:

```gdb
# Set breakpoints
break kernel_main
break _start
break serial_init

# Start execution
continue
# or
c

# Step through code
step        # Step into function
next        # Step over function

# Inspect state
info registers           # Show all CPU registers
print variable_name      # Print variable value
x/10i $pc               # Disassemble 10 instructions at PC
x/10x $esp              # Show 10 words on stack

# Examine memory
x/10x 0x100000          # Show 10 words at address 0x100000
x/10c 0xB8000           # Show 10 characters at VGA buffer

# Continue execution
continue
```

### Example GDB Session

```bash
# Terminal 1
$ make debug

# Terminal 2
$ gdb -x debug.gdb
(gdb) break kernel_main
(gdb) continue
(gdb) print magic
(gdb) info registers
(gdb) continue
```

## Debugging Tips

1. **Use serial port for early debugging** - Serial port works even before VGA is initialized
2. **Set log level** - Use `debug_set_level()` to filter messages
3. **Use breakpoints strategically** - Set breakpoints at key initialization points
4. **Inspect registers** - Use `info registers` to see CPU state
5. **Check memory** - Use `x` command to examine memory contents

## Troubleshooting

### GDB can't find symbols

Make sure you loaded the symbol file:
```gdb
symbol-file build/kernel.bin
```

### Serial port not working

Check that QEMU is configured with `-serial stdio` or similar.

### Breakpoints not working

Make sure:
1. Debug symbols are included (`-g` flag in Makefile)
2. You've loaded the symbol file in GDB
3. The kernel hasn't already passed the breakpoint

## Next Steps

- Add more sophisticated logging with printf-style formatting
- Add stack trace on panic
- Add register dump on panic
- Add memory dump utilities

