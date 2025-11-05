// Memory layout demonstration in C
// This program shows how different types of variables are organized in memory segments:
// - .text segment: executable code (functions)
// - .data segment: initialized global/static variables
// - .bss segment: uninitialized global/static variables
// - heap: dynamically allocated memory (malloc/free)
// - stack: local variables and function call frames
//
// Compile with: gcc -o memmap memmap.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_init_var = 42; // Initialized global variable (.data segment)
int global_uninit;         // Uninitialized global variable (.bss segment)

void print_addresses(void) {
    int local_var = 1;  // Stack variable
    int *heap = malloc(sizeof(int)); // Heap variable
    if (heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    *heap = 2;

    printf("===Memory map of Variables===\n");
    printf("Function (text segment): %p\n", (void*)print_addresses);
    printf("Global initialized: %p\n", (void*)&global_init_var);
    printf("Global uninitialized: %p\n", (void*)&global_uninit);
    printf("Local variable: %p\n", (void*)&local_var);
    printf("Heap variable: %p\n", (void*)heap);
    printf("===End of Memory map===\n");

    free(heap);
}

void recurse(int depth) {
    if (depth > 10) return; // Base case: stop after 10 levels
    int local = depth;
    printf("Depth %d - stack address: %p\n", depth, (void*)&local);
    recurse(depth + 1);
}

int main() {
    printf("System page size: %ld bytes\n\n", sysconf(_SC_PAGESIZE));
    print_addresses();
    printf("\n===Stack growth demonstration===\n");
    recurse(1);
    return 0;
}




