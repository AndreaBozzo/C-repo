# Makefile for C-repo - C Programming Learning Repository
# Automatically builds all .c files into separate executables
# Resilient to addition of new examples

# ============================================================================
# CONFIGURATION
# ============================================================================

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O2
LDFLAGS := -lm

# Debug settings
DEBUG_CFLAGS := -g -O0 -DDEBUG
SANITIZE_CFLAGS := -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

# Directories
SRC_DIR := .
BUILD_DIR := build
BIN_DIR := bin
INSTALL_PREFIX := /usr/local

# ============================================================================
# AUTO-DISCOVERY OF SOURCE FILES
# ============================================================================

# Find all .c files in the source directory (excluding build directories)
SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Generate executable names (without .c extension)
PROGRAMS := $(patsubst $(SRC_DIR)/%.c,%,$(SOURCES))

# Generate targets for build directory
BUILD_TARGETS := $(patsubst %,$(BIN_DIR)/%,$(PROGRAMS))
DEBUG_TARGETS := $(patsubst %,$(BIN_DIR)/%-debug,$(PROGRAMS))
SANITIZE_TARGETS := $(patsubst %,$(BIN_DIR)/%-sanitize,$(PROGRAMS))

# ============================================================================
# PHONY TARGETS
# ============================================================================

.PHONY: all debug sanitize clean install uninstall test help list

# Default target: build all programs
all: $(BUILD_TARGETS)

# Debug builds: compile with debug symbols and no optimization
debug: $(DEBUG_TARGETS)

# Sanitize builds: compile with AddressSanitizer and UndefinedBehaviorSanitizer
sanitize: $(SANITIZE_TARGETS)

# ============================================================================
# BUILD RULES
# ============================================================================

# Create bin directory if it doesn't exist
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Standard build rule: compile each .c file into an executable
$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	@echo "Building $* (release)..."
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
	@echo "✓ $@ built successfully"

# Debug build rule: compile with debug symbols
$(BIN_DIR)/%-debug: $(SRC_DIR)/%.c | $(BIN_DIR)
	@echo "Building $* (debug)..."
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $< -o $@ $(LDFLAGS)
	@echo "✓ $@ built successfully"

# Sanitize build rule: compile with sanitizers
$(BIN_DIR)/%-sanitize: $(SRC_DIR)/%.c | $(BIN_DIR)
	@echo "Building $* (sanitize)..."
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $(SANITIZE_CFLAGS) $< -o $@ $(LDFLAGS)
	@echo "✓ $@ built successfully"

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BIN_DIR)
	@echo "✓ Clean complete"

# Install programs to system
install: all
	@echo "Installing programs to $(INSTALL_PREFIX)/bin..."
	@mkdir -p $(INSTALL_PREFIX)/bin
	@for prog in $(PROGRAMS); do \
		install -m 755 $(BIN_DIR)/$$prog $(INSTALL_PREFIX)/bin/$$prog; \
		echo "  Installed $$prog"; \
	done
	@echo "✓ Installation complete"

# Uninstall programs from system
uninstall:
	@echo "Uninstalling programs from $(INSTALL_PREFIX)/bin..."
	@for prog in $(PROGRAMS); do \
		rm -f $(INSTALL_PREFIX)/bin/$$prog; \
		echo "  Removed $$prog"; \
	done
	@echo "✓ Uninstallation complete"

# Run basic tests on all programs
test: all
	@echo "Running basic tests..."
	@echo ""
	@echo "=== Testing numstat ==="
	@if [ -f $(BIN_DIR)/numstat ]; then \
		echo "Test 1: Basic statistics"; \
		echo "1 2 3 4 5" | $(BIN_DIR)/numstat || exit 1; \
		echo ""; \
		echo "Test 2: Help output"; \
		$(BIN_DIR)/numstat -h || exit 1; \
	fi
	@echo ""
	@echo "=== Testing memmap ==="
	@if [ -f $(BIN_DIR)/memmap ]; then \
		$(BIN_DIR)/memmap || exit 1; \
	fi
	@echo ""
	@echo "=== Testing mem_errors ==="
	@if [ -f $(BIN_DIR)/mem_errors ]; then \
		echo "Note: mem_errors contains intentional unsafe code."; \
		echo "Running without sanitizers..."; \
		$(BIN_DIR)/mem_errors || exit 1; \
	fi
	@echo ""
	@echo "✓ All tests passed"

# Run Valgrind memory checks on all programs
valgrind: all
	@echo "Running Valgrind memory checks..."
	@for prog in $(PROGRAMS); do \
		echo ""; \
		echo "=== Checking $$prog ==="; \
		if [ "$$prog" = "mem_errors" ]; then \
			echo "Skipping mem_errors (contains intentional memory errors)"; \
		elif [ "$$prog" = "numstat" ]; then \
			echo "1 2 3 4 5" | valgrind --leak-check=full --error-exitcode=1 $(BIN_DIR)/$$prog; \
		else \
			valgrind --leak-check=full --error-exitcode=1 $(BIN_DIR)/$$prog; \
		fi \
	done
	@echo ""
	@echo "✓ Valgrind checks complete"

# List all discovered programs
list:
	@echo "Discovered programs:"
	@for prog in $(PROGRAMS); do \
		echo "  - $$prog"; \
	done

# Show help
help:
	@echo "C-repo Makefile - Available targets:"
	@echo ""
	@echo "  make [all]      - Build all programs (default)"
	@echo "  make debug      - Build all programs with debug symbols"
	@echo "  make sanitize   - Build all programs with sanitizers"
	@echo "  make clean      - Remove all build artifacts"
	@echo "  make test       - Run basic tests on all programs"
	@echo "  make valgrind   - Run Valgrind memory checks"
	@echo "  make install    - Install programs to $(INSTALL_PREFIX)/bin"
	@echo "  make uninstall  - Remove programs from $(INSTALL_PREFIX)/bin"
	@echo "  make list       - List all discovered programs"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Build variants:"
	@echo "  bin/<program>           - Release build (optimized)"
	@echo "  bin/<program>-debug     - Debug build (symbols, no optimization)"
	@echo "  bin/<program>-sanitize  - Sanitize build (with AddressSanitizer)"
	@echo ""
	@echo "Compiler flags:"
	@echo "  CFLAGS = $(CFLAGS)"
	@echo "  DEBUG_CFLAGS = $(DEBUG_CFLAGS)"
	@echo "  SANITIZE_CFLAGS = $(SANITIZE_CFLAGS)"
	@echo ""
	@echo "Discovered programs: $(PROGRAMS)"
