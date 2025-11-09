# C-repo - Learning Repository

A collection of C programming exercises and utilities. Started as a statistics calculator, now includes various learning projects.

My first attempts at C programming.

## Projects

### 1. numstat - Statistical Analysis Tool

A command-line tool for calculating comprehensive statistics on numerical data.

### 2. memmap - Memory Layout Demonstration

An educational program demonstrating how C organizes memory into different segments (.text, .data, .bss, heap, stack).

### 3. mem_errors - Common Memory Errors

An educational program that demonstrates dangerous memory errors in C programming. Shows both unsafe patterns and their safe alternatives for learning purposes.

## Build

All programs can be built using the Makefile:

```bash
# Build all programs (release version)
make

# Build with debug symbols
make debug

# Build with sanitizers (AddressSanitizer + UndefinedBehaviorSanitizer)
make sanitize

# List all discovered programs
make list

# Clean build artifacts
make clean
```

### Manual build commands

#### numstat
```bash
gcc -Wall -Wextra -std=c99 -O2 -o numstat numstat.c -lm
```

#### memmap
```bash
gcc -Wall -Wextra -std=c99 -O2 -o memmap memmap.c
```

#### mem_errors
```bash
# Normal build
gcc -Wall -Wextra -std=c99 -o mem_errors mem_errors.c

# Debug build with sanitizers (recommended for learning)
gcc -g -fsanitize=address -o mem_errors mem_errors.c
```

---

## numstat Usage

```bash
numstat [OPTIONS] [FILE]
```

### Options

- `-j, --json` - Output in JSON format
- `-p N, --precision N` - Set decimal precision (default: 4)
- `-h, --help` - Show help message

### Examples

#### Read from file

```bash
$ numstat data.txt
Statistics for 5 numbers:
  Sum:     16.1000
  Mean:    3.2200
  Median:  3.1000
  Minimum: 1.5000
  Maximum: 5.2000
  Range:   3.7000
  Q1:      2.3000
  Q3:      4.0000
  StdDev:  1.2921
```

#### Read from stdin

```bash
$ echo "1 2 3 4 5" | numstat
Statistics for 5 numbers:
  Sum:     15.0000
  Mean:    3.0000
  Median:  3.0000
  Minimum: 1.0000
  Maximum: 5.0000
  Range:   4.0000
  Q1:      2.0000
  Q3:      4.0000
  StdDev:  1.4142
```

#### JSON output

```bash
$ echo "1 2 3 4 5" | numstat -j
{
  "count": 5,
  "sum": 15.0000,
  "mean": 3.0000,
  "median": 3.0000,
  "min": 1.0000,
  "max": 5.0000,
  "range": 4.0000,
  "q1": 2.0000,
  "q3": 4.0000,
  "stddev": 1.4142
}
```

#### Custom precision

```bash
$ numstat -p 2 data.txt
Statistics for 5 numbers:
  Sum:     16.10
  Mean:    3.22
  Median:  3.10
  Minimum: 1.50
  Maximum: 5.20
  Range:   3.70
  Q1:      2.30
  Q3:      4.00
  StdDev:  1.29
```

#### Pipeline usage

```bash
# Analyze log file numbers
grep -oP '\d+\.\d+' logfile.txt | numstat -j

# Quick calculation
seq 1 100 | numstat

# With other tools
cat measurements.csv | cut -d',' -f2 | numstat -p 3
```

### Statistics Calculated

- **Count** - Total number of values
- **Sum** - Sum of all values
- **Mean** - Arithmetic average
- **Median** - Middle value (50th percentile)
- **Minimum** - Smallest value
- **Maximum** - Largest value
- **Range** - Difference between max and min
- **Q1** - First quartile (25th percentile)
- **Q3** - Third quartile (75th percentile)
- **StdDev** - Population standard deviation

### Installation

```bash
# Install to /usr/local/bin
sudo make install

# Uninstall
sudo make uninstall
```

---

## memmap Usage

The `memmap` program is an educational tool that demonstrates how C programs organize memory.

### What it shows

When you run `memmap`, it displays:

1. **System page size** - The memory page size used by your operating system
2. **Memory layout** - Addresses of different types of variables:
   - `.text` segment (executable code)
   - `.data` segment (initialized global variables)
   - `.bss` segment (uninitialized global variables)
   - Heap (dynamically allocated memory)
   - Stack (local variables)
3. **Stack growth** - How the stack grows through recursive function calls

### Example output

```bash
$ ./memmap
System page size: 4096 bytes

===Memory map of Variables===
Function (text segment): 0x5d517d42b209
Global initialized: 0x5d517d42e010
Global uninitialized: 0x5d517d42e02c
Local variable: 0x7ffd2b1ed77c
Heap variable: 0x5d51b12d22b0
===End of Memory map===

===Stack growth demonstration===
Depth 1 - stack address: 0x7ffd2b1ed784
Depth 2 - stack address: 0x7ffd2b1ed754
Depth 3 - stack address: 0x7ffd2b1ed724
...
```

### Key observations

- Stack addresses **decrease** as depth increases (stack grows downward)
- Global variables (.data and .bss) are stored close together
- Heap memory is allocated in a separate region
- Code (.text) is isolated from data segments

---

## mem_errors Usage

The `mem_errors` program is an educational tool that demonstrates common memory management mistakes in C and their safe alternatives.

### What it demonstrates

The program covers 5 common memory errors:

1. **Buffer Overflow** - Writing beyond allocated memory boundaries
2. **Use-After-Free** - Accessing memory after it has been freed
3. **Dangling Pointer** - Dereferencing pointers to freed memory
4. **Memory Leak** - Allocating memory without freeing it
5. **Double Free** - Freeing the same pointer twice

Each error type is shown with:
- An unsafe example demonstrating the error
- Explanation of why it's dangerous
- A safe alternative showing the correct approach

### Running the program

```bash
# Standard execution
./mem_errors

# With Valgrind to detect memory issues
valgrind --leak-check=full ./mem_errors

# With AddressSanitizer (requires compilation with -fsanitize=address)
./mem_errors-sanitize
```

### Example output

```
╔═══════════════════════════════════════════════════════════════════╗
║       Common Memory Errors in C - Educational Demonstration      ║
╚═══════════════════════════════════════════════════════════════════╝

═══════════════════════════════════════════════════════════════════
  1. BUFFER OVERFLOW
═══════════════════════════════════════════════════════════════════

  UNSAFE: strcpy() doesn't check buffer size
  Buffer size: 8 bytes
  Attempting to copy 20 characters...
  Buffer content: AAAAAAAAAAAAAAAAAAAA
  ⚠️  Memory corruption occurred! Stack may be smashed.

  SAFE: Using strncpy() with size checking
  Buffer size: 8 bytes
  Source length: 20 bytes
  Buffer content: AAAAAAA
  ✓ Only copied 7 characters, properly null-terminated.
```

### Learning objectives

By studying this program, you will learn to:

- **Recognize unsafe patterns** - Identify common mistakes in C code
- **Understand consequences** - See what happens when memory is mismanaged
- **Apply safe practices** - Learn proper memory management techniques
- **Use debugging tools** - Practice with AddressSanitizer and Valgrind
- **Write secure code** - Avoid security vulnerabilities

### Key safety rules demonstrated

✓ Always check buffer sizes before copying
✓ Set pointers to NULL after freeing
✓ Check for NULL before dereferencing
✓ Always free() allocated memory
✓ Never free() the same pointer twice

### Debugging tips

```bash
# Compile with AddressSanitizer to catch errors at runtime
gcc -g -fsanitize=address -o mem_errors mem_errors.c

# Use Valgrind for comprehensive memory analysis
gcc -g -o mem_errors mem_errors.c
valgrind --leak-check=full --show-leak-kinds=all ./mem_errors

# Enable all compiler warnings
gcc -Wall -Wextra -Wpedantic -o mem_errors mem_errors.c
```

### Warning

⚠️ This program intentionally contains unsafe code for educational purposes. The `double_free_example()` is commented out by default as it will crash when compiled with sanitizers. Never use these unsafe patterns in production code!

---

## License

MIT License - see [LICENSE](LICENSE) file for details.
