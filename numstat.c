#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Configuration structure
typedef struct {
    int json_output;
    int precision;
    char *input_file;
} Config;

// Statistics structure
typedef struct {
    size_t count;
    double sum;
    double mean;
    double min;
    double max;
    double range;
    double median;
    double q1;
    double q3;
    double variance;
    double stddev;
} Stats;

// Function prototypes
void print_help(const char *program_name);
void parse_args(int argc, char *argv[], Config *config);
double* read_numbers(FILE *file, size_t *count);
int compare_double(const void *a, const void *b);
void calculate_stats(double *values, size_t count, Stats *stats);
double get_percentile(double *sorted_values, size_t count, double percentile);
void print_stats_text(Stats *stats, int precision);
void print_stats_json(Stats *stats, int precision);

int main(int argc, char *argv[]) {
    Config config = {0, 4, NULL};  // Default: text output, 4 decimals, no file

    // Parse command-line arguments
    parse_args(argc, argv, &config);

    // Determine input source
    FILE *file;
    if (config.input_file) {
        file = fopen(config.input_file, "r");
        if (!file) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", config.input_file);
            return 1;
        }
    } else {
        file = stdin;
    }

    // Read numbers from input
    size_t count = 0;
    double *values = read_numbers(file, &count);

    if (config.input_file) {
        fclose(file);
    }

    if (!values) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    if (count == 0) {
        fprintf(stderr, "Error: No valid numbers found in input\n");
        free(values);
        return 1;
    }

    // Calculate statistics
    Stats stats;
    calculate_stats(values, count, &stats);

    // Print results
    if (config.json_output) {
        print_stats_json(&stats, config.precision);
    } else {
        print_stats_text(&stats, config.precision);
    }

    free(values);
    return 0;
}

void print_help(const char *program_name) {
    printf("numstat - Calculate statistics for numerical data\n\n");
    printf("Usage: %s [OPTIONS] [FILE]\n\n", program_name);
    printf("Options:\n");
    printf("  -j, --json         Output in JSON format\n");
    printf("  -p, --precision N  Set decimal precision (default: 4)\n");
    printf("  -h, --help         Show this help message\n\n");
    printf("Input:\n");
    printf("  If FILE is provided, reads numbers from file\n");
    printf("  If no FILE is given, reads from stdin\n\n");
    printf("Statistics calculated:\n");
    printf("  - Count, Sum, Mean, Median\n");
    printf("  - Minimum, Maximum, Range\n");
    printf("  - Q1 (25th percentile), Q3 (75th percentile)\n");
    printf("  - Standard Deviation (population)\n\n");
    printf("Examples:\n");
    printf("  %s data.txt              # Read from file\n", program_name);
    printf("  cat data.txt | %s        # Read from stdin\n", program_name);
    printf("  echo \"1 2 3\" | %s       # Quick calculation\n", program_name);
    printf("  %s -j data.txt           # JSON output\n", program_name);
    printf("  %s -p 2 data.txt         # 2 decimal places\n", program_name);
}

void parse_args(int argc, char *argv[], Config *config) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "-j") == 0 || strcmp(argv[i], "--json") == 0) {
            config->json_output = 1;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--precision") == 0) {
            if (i + 1 < argc) {
                config->precision = atoi(argv[++i]);
                if (config->precision < 0 || config->precision > 10) {
                    fprintf(stderr, "Warning: Precision should be between 0 and 10. Using default (4).\n");
                    config->precision = 4;
                }
            } else {
                fprintf(stderr, "Error: -p requires a number argument\n");
                exit(1);
            }
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            fprintf(stderr, "Use -h or --help for usage information\n");
            exit(1);
        } else {
            // Assume it's the input file
            if (config->input_file) {
                fprintf(stderr, "Error: Multiple input files specified\n");
                exit(1);
            }
            config->input_file = argv[i];
        }
    }
}

double* read_numbers(FILE *file, size_t *count) {
    double *values = NULL;
    size_t capacity = 16;  // Start with reasonable capacity
    *count = 0;
    double num;

    values = malloc(capacity * sizeof(double));
    if (!values) {
        return NULL;
    }

    // Read numbers from file/stdin
    while (fscanf(file, "%lf", &num) == 1) {
        if (*count >= capacity) {
            capacity *= 2;
            double *new_values = realloc(values, capacity * sizeof(double));
            if (!new_values) {
                free(values);
                return NULL;
            }
            values = new_values;
        }
        values[(*count)++] = num;
    }

    return values;
}

int compare_double(const void *a, const void *b) {
    double diff = (*(double*)a - *(double*)b);
    return (diff > 0) - (diff < 0);  // Returns -1, 0, or 1
}

double get_percentile(double *sorted_values, size_t count, double percentile) {
    if (count == 0) return 0.0;
    if (count == 1) return sorted_values[0];

    double index = percentile * (count - 1);
    size_t lower = (size_t)index;
    size_t upper = lower + 1;

    if (upper >= count) {
        return sorted_values[count - 1];
    }

    double weight = index - lower;
    return sorted_values[lower] * (1 - weight) + sorted_values[upper] * weight;
}

void calculate_stats(double *values, size_t count, Stats *stats) {
    stats->count = count;
    stats->sum = 0.0;
    stats->min = values[0];
    stats->max = values[0];

    // Calculate sum, min, max
    for (size_t i = 0; i < count; i++) {
        stats->sum += values[i];
        if (values[i] < stats->min) stats->min = values[i];
        if (values[i] > stats->max) stats->max = values[i];
    }

    stats->mean = stats->sum / count;
    stats->range = stats->max - stats->min;

    // Calculate variance and standard deviation
    stats->variance = 0.0;
    for (size_t i = 0; i < count; i++) {
        double diff = values[i] - stats->mean;
        stats->variance += diff * diff;
    }
    stats->variance /= count;
    stats->stddev = sqrt(stats->variance);

    // Sort values for percentile calculations
    qsort(values, count, sizeof(double), compare_double);

    // Calculate median and quartiles
    stats->median = get_percentile(values, count, 0.50);
    stats->q1 = get_percentile(values, count, 0.25);
    stats->q3 = get_percentile(values, count, 0.75);
}

void print_stats_text(Stats *stats, int precision) {
    printf("Statistics for %zu numbers:\n", stats->count);
    printf("  Sum:     %.*f\n", precision, stats->sum);
    printf("  Mean:    %.*f\n", precision, stats->mean);
    printf("  Median:  %.*f\n", precision, stats->median);
    printf("  Minimum: %.*f\n", precision, stats->min);
    printf("  Maximum: %.*f\n", precision, stats->max);
    printf("  Range:   %.*f\n", precision, stats->range);
    printf("  Q1:      %.*f\n", precision, stats->q1);
    printf("  Q3:      %.*f\n", precision, stats->q3);
    printf("  StdDev:  %.*f\n", precision, stats->stddev);
}

void print_stats_json(Stats *stats, int precision) {
    printf("{\n");
    printf("  \"count\": %zu,\n", stats->count);
    printf("  \"sum\": %.*f,\n", precision, stats->sum);
    printf("  \"mean\": %.*f,\n", precision, stats->mean);
    printf("  \"median\": %.*f,\n", precision, stats->median);
    printf("  \"min\": %.*f,\n", precision, stats->min);
    printf("  \"max\": %.*f,\n", precision, stats->max);
    printf("  \"range\": %.*f,\n", precision, stats->range);
    printf("  \"q1\": %.*f,\n", precision, stats->q1);
    printf("  \"q3\": %.*f,\n", precision, stats->q3);
    printf("  \"stddev\": %.*f\n", precision, stats->stddev);
    printf("}\n");
}
