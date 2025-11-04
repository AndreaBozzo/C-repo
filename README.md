# numstat

Calculate statistics for numerical data from the command line, making it available system-wide.

My first attempt on C.

## Build

```bash
# Using Makefile (recommended)
make

# Or manually
gcc -o numstat numstat.c -lm
```

## Usage

```bash
numstat [OPTIONS] [FILE]
```

### Options

- `-j, --json` - Output in JSON format
- `-p N, --precision N` - Set decimal precision (default: 4)
- `-h, --help` - Show help message

## Examples

### Read from file

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

### Read from stdin

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

### JSON output

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

### Custom precision

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

### Pipeline usage

```bash
# Analyze log file numbers
grep -oP '\d+\.\d+' logfile.txt | numstat -j

# Quick calculation
seq 1 100 | numstat

# With other tools
cat measurements.csv | cut -d',' -f2 | numstat -p 3
```

## Statistics Calculated

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

## Installation

```bash
# Install to /usr/local/bin
sudo make install

# Uninstall
sudo make uninstall
```

## License

MIT License - see [LICENSE](LICENSE) file for details.
