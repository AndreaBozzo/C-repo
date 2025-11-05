# C Learning Repository (numstat)

A collection of C programming exercises and utilities. Started as a statistics calculator, now includes various learning projects.

**Note**: This repository will be renamed in the future to better reflect its purpose as a compilation of C exercises rather than a single project.

My first attempts at C programming.

## Projects

### 1. numstat - Statistical Analysis Tool

A command-line tool for calculating comprehensive statistics on numerical data.

### 2. memmap - Memory Layout Demonstration

An educational program demonstrating how C organizes memory into different segments (.text, .data, .bss, heap, stack).

## Build

### numstat
```bash
# Using Makefile (recommended)
make

# Or manually
gcc -o numstat numstat.c -lm
```

### memmap
```bash
gcc -o memmap memmap.c
./memmap
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

## License

MIT License - see [LICENSE](LICENSE) file for details.
