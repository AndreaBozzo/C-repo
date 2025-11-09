// Common Memory Errors in C - Educational Demonstration
//
// This program demonstrates dangerous memory errors that commonly occur in C programming:
// 1. Buffer Overflow: Writing beyond allocated memory boundaries
// 2. Use-After-Free: Accessing memory after it has been freed
// 3. Dangling Pointer: Dereferencing pointers to freed memory
// 4. Memory Leak: Allocating memory without freeing it
// 5. Double Free: Freeing the same pointer twice
//
// WARNING: These examples intentionally show UNSAFE code patterns!
//          Do NOT use these patterns in production code.
//          Each unsafe example is paired with a safe alternative.
//
// Compilation:
//   Normal:     gcc -Wall -Wextra -o mem_errors mem_errors.c
//   Debug:      gcc -g -fsanitize=address -o mem_errors mem_errors.c
//   Valgrind:   gcc -g -o mem_errors mem_errors.c && valgrind ./mem_errors
//
// Learning objectives:
// - Understand common memory errors in C
// - Learn to recognize unsafe patterns
// - Practice safe memory management techniques
// - Use debugging tools (AddressSanitizer, Valgrind)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes - Unsafe examples
void buffer_overflow_example(void);
void use_after_free_example(void);
void dangling_pointer_example(void);
void memory_leak_example(void);
void double_free_example(void);

// Function prototypes - Safe alternatives
void safe_buffer_copy(void);
void safe_memory_usage(void);
void safe_pointer_handling(void);
void safe_memory_allocation(void);
void safe_free_pattern(void);

// Helper functions
void print_separator(const char *title);

// =============================================================================
// UNSAFE EXAMPLES - These demonstrate common memory errors
// =============================================================================

void buffer_overflow_example(void) {
    printf("\n  UNSAFE: strcpy() doesn't check buffer size\n");
    char buffer[8];  // Only 8 bytes allocated

    printf("  Buffer size: 8 bytes\n");
    printf("  Attempting to copy 20 characters...\n");

    // DANGER: strcpy writes beyond buffer bounds!
    // This corrupts adjacent memory (stack smashing)
    strcpy(buffer, "AAAAAAAAAAAAAAAAAAAA");  // 20 'A's + null terminator = 21 bytes

    printf("  Buffer content: %s\n", buffer);
    printf("  ⚠️  Memory corruption occurred! Stack may be smashed.\n");
}

void use_after_free_example(void) {
    printf("\n  UNSAFE: Accessing freed memory\n");
    int *ptr = (int *)malloc(sizeof(int) * 5);

    if (ptr == NULL) {
        fprintf(stderr, "  Allocation failed\n");
        return;
    }

    // Initialize array
    for (int i = 0; i < 5; i++) {
        ptr[i] = i * 10;
    }
    printf("  Allocated and initialized array: [0, 10, 20, 30, 40]\n");

    free(ptr);
    printf("  Memory freed\n");

    // DANGER: Using memory after it's been freed!
    // The pointer still contains the old address, but memory is no longer ours
    printf("  Accessing freed memory: %d\n", ptr[0]);
    printf("  ⚠️  Undefined behavior! Value may be corrupted or cause crash.\n");
}

void dangling_pointer_example(void) {
    printf("\n  UNSAFE: Dangling pointer dereference\n");
    int *ptr = (int *)malloc(sizeof(int) * 3);

    if (ptr == NULL) {
        fprintf(stderr, "  Allocation failed\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        ptr[i] = i * 100;
    }
    printf("  Created array: [0, 100, 200]\n");

    free(ptr);
    printf("  Memory freed, but pointer still holds old address\n");

    // DANGER: Dereferencing a dangling pointer!
    printf("  Dangling pointer value: %d\n", ptr[1]);
    printf("  ⚠️  This is undefined behavior - may crash or return garbage.\n");
}

void memory_leak_example(void) {
    printf("\n  UNSAFE: Memory leak - allocation without free\n");

    for (int i = 0; i < 3; i++) {
        int *leaked = (int *)malloc(sizeof(int) * 100);
        if (leaked == NULL) continue;

        leaked[0] = i;
        printf("  Iteration %d: Allocated 400 bytes, no free() called\n", i);
        // DANGER: Never calling free(leaked)!
        // Each iteration leaks 400 bytes
    }

    printf("  ⚠️  Leaked 1200 bytes total! Memory never returned to system.\n");
}

void double_free_example(void) {
    printf("\n  UNSAFE: Freeing the same pointer twice\n");
    int *ptr = (int *)malloc(sizeof(int) * 10);

    if (ptr == NULL) {
        fprintf(stderr, "  Allocation failed\n");
        return;
    }

    ptr[0] = 42;
    printf("  Allocated memory and set value: %d\n", ptr[0]);

    free(ptr);
    printf("  First free() successful\n");

    // DANGER: Freeing the same pointer again!
    // This corrupts the heap and often causes immediate crash
    free(ptr);
    printf("  ⚠️  Second free() - heap corruption! Program may crash.\n");
}

// =============================================================================
// SAFE ALTERNATIVES - These show correct memory management
// =============================================================================

void safe_buffer_copy(void) {
    printf("\n  SAFE: Using strncpy() with size checking\n");
    char buffer[8];
    const char *source = "AAAAAAAAAAAAAAAAAAAA";

    printf("  Buffer size: 8 bytes\n");
    printf("  Source length: %zu bytes\n", strlen(source));

    // SAFE: strncpy limits the number of bytes copied
    strncpy(buffer, source, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

    printf("  Buffer content: %s\n", buffer);
    printf("  ✓ Only copied 7 characters, properly null-terminated.\n");
}

void safe_memory_usage(void) {
    printf("\n  SAFE: Setting pointer to NULL after free\n");
    int *ptr = (int *)malloc(sizeof(int) * 5);

    if (ptr == NULL) {
        fprintf(stderr, "  Allocation failed\n");
        return;
    }

    for (int i = 0; i < 5; i++) {
        ptr[i] = i * 10;
    }
    printf("  Initialized array: [0, 10, 20, 30, 40]\n");

    free(ptr);
    ptr = NULL;  // IMPORTANT: Set to NULL after freeing
    printf("  Memory freed and pointer set to NULL\n");

    // Now we can safely check before using
    if (ptr != NULL) {
        printf("  Accessing: %d\n", ptr[0]);
    } else {
        printf("  ✓ Pointer is NULL, prevented use-after-free!\n");
    }
}

void safe_pointer_handling(void) {
    printf("\n  SAFE: Proper pointer lifecycle management\n");
    int *ptr = (int *)malloc(sizeof(int) * 3);

    if (ptr == NULL) {
        fprintf(stderr, "  Allocation failed\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        ptr[i] = i * 100;
    }
    printf("  Created array: [0, 100, 200]\n");
    printf("  Using value while valid: %d\n", ptr[1]);

    free(ptr);
    ptr = NULL;  // Prevent dangling pointer

    printf("  ✓ Memory freed, pointer nullified - safe!\n");
}

void safe_memory_allocation(void) {
    printf("\n  SAFE: Proper allocation and cleanup\n");

    for (int i = 0; i < 3; i++) {
        int *temp = (int *)malloc(sizeof(int) * 100);
        if (temp == NULL) {
            fprintf(stderr, "  Allocation failed\n");
            continue;
        }

        temp[0] = i;
        printf("  Iteration %d: Allocated 400 bytes\n", i);

        // IMPORTANT: Free memory when done
        free(temp);
        printf("  Iteration %d: Freed 400 bytes\n", i);
    }

    printf("  ✓ All memory properly freed - no leaks!\n");
}

void safe_free_pattern(void) {
    printf("\n  SAFE: Preventing double free with NULL check\n");
    int *ptr = (int *)malloc(sizeof(int) * 10);

    if (ptr == NULL) {
        fprintf(stderr, "  Allocation failed\n");
        return;
    }

    ptr[0] = 42;
    printf("  Allocated memory and set value: %d\n", ptr[0]);

    // Safe free pattern: free and nullify
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
        printf("  First free() successful, pointer set to NULL\n");
    }

    // Second free attempt is safe because we check for NULL
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    } else {
        printf("  ✓ Pointer is NULL, second free prevented!\n");
    }
}

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================

void print_separator(const char *title) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("  %s\n", title);
    printf("═══════════════════════════════════════════════════════════════════\n");
}

// =============================================================================
// MAIN - Demonstrates all examples
// =============================================================================

int main(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║       Common Memory Errors in C - Educational Demonstration      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");

    printf("\nThis program shows unsafe memory practices and their safe alternatives.\n");
    printf("WARNING: Some unsafe examples may crash due to runtime protections!\n");
    printf("NOTE: Buffer overflow and double free examples are commented out\n");
    printf("      to prevent crashes. Uncomment them in main() to see the crashes.\n");

    // Example 1: Buffer Overflow
    print_separator("1. BUFFER OVERFLOW");
    printf("\n⚠️  WARNING: This example will CRASH with modern gcc runtime protections!\n");
    printf("Uncomment buffer_overflow_example() in main() to see the crash.\n\n");
    // buffer_overflow_example();  // Uncomment to see crash
    safe_buffer_copy();

    // Example 2: Use-After-Free
    print_separator("2. USE-AFTER-FREE");
    use_after_free_example();
    safe_memory_usage();

    // Example 3: Dangling Pointer
    print_separator("3. DANGLING POINTER");
    dangling_pointer_example();
    safe_pointer_handling();

    // Example 4: Memory Leak
    print_separator("4. MEMORY LEAK");
    memory_leak_example();
    safe_memory_allocation();

    // Example 5: Double Free
    print_separator("5. DOUBLE FREE");
    printf("\n⚠️  WARNING: This example will likely CRASH with sanitizers enabled!\n");
    printf("Comment out this section if running with -fsanitize=address\n\n");
    // double_free_example();  // Uncomment to see crash
    printf("(Example commented out to prevent crash)\n");
    safe_free_pattern();

    // Final notes
    print_separator("SUMMARY & DEBUGGING TIPS");
    printf("\n");
    printf("  Key Takeaways:\n");
    printf("  • Always check buffer sizes before copying\n");
    printf("  • Set pointers to NULL after freeing\n");
    printf("  • Check for NULL before dereferencing\n");
    printf("  • Always free() allocated memory\n");
    printf("  • Never free() the same pointer twice\n");
    printf("\n");
    printf("  Debugging Tools:\n");
    printf("  • Compile with: gcc -g -fsanitize=address mem_errors.c\n");
    printf("  • Use Valgrind: valgrind --leak-check=full ./mem_errors\n");
    printf("  • Enable warnings: gcc -Wall -Wextra mem_errors.c\n");
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════\n\n");

    return 0;
}
