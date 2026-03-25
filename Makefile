# ─────────────────────────────────────────────
# Makefile — USBSTORServiceManager
# Targets: Windows (cross), Linux (native), MacOS (native)
# ─────────────────────────────────────────────

# compiler and flags
PROG_BASE ?= USBSTORServiceManager
SRC = USBSTORServiceManager.c
CFLAGS = -Wall -Wextra -O2

# Compilers
CC_WINDOWS ?= x86_64-w64-mingw32-gcc
CC_LINUX ?= gcc
CC_MACOS ?= clang

# Output names
OUT_WINDOWS = $(PROG_BASE)-windows-amd64.exe
OUT_LINUX = $(PROG_BASE)-linux-amd64
OUT_MACOS = $(PROG_BASE)-macos-universal

# Libraries
LIBS_WINDOWS = -ladvapi32
LIBS_LINUX =
LIBS_MACOS =

.PHONY: all build-windows build-linux build-macos package clean test

all: 
ifneq ($(OS),Windows_NT)
	$(MAKE) build-windows

else
	UNAME := $(shell uname -s)

	ifeq ($(UNAME), Linux)
		$(MAKE) build-linux

		else ifeq ($(UNAME), Darwin)
			$(MAKE) build-macos

		endif
endif

# Build targets

# Windows cross-compilation
build-windows: $(WINDOWS_OUT)

$(WINDOWS_OUT): $(SRC)
	$(CC_WINDOWS) $(CFLAGS) -o $@ $< $(LIBS_WINDOWS)
	@echo Successfully compiled: $@

# Linux native compilation
build-linux: $(LINUX_OUT)

$(LINUX_OUT): $(SRC)
	gcc -o $@ $< $(LIBS_LINUX)
	@echo Successfully compiled: $@

# MacOs native compilation
build-macos: $(MACOS_OUT)

$(MACOS_OUT): $(SRC)
	clang -target x86_64-apple-macos10.15 -o $(PROG_BASE)-x86   $(SRC)
	clang -target arm64-apple-macos11     -o $(PROG_BASE)-arm64  $(SRC)
	lipo -create -output $(OUT_MACOS) $(PROG_BASE)-x86 $(PROG_BASE)-arm64
	@rm $(PROG_BASE)-x86 $(PROG_BASE)-arm64
	@echo Successfully compiled: $@

# Package all built artifacts into a dist/ directory
package: build-windows build-linux build-macos
	@mkdir -p dist
	@cp $(WINDOWS_OUT) dist/$(PROG_BASE)-windows-amd64.exe 2>/dev/null || true
	@cp $(LINUX_OUT) dist/$(PROG_BASE)-linux-amd64 2>/dev/null || true
	@cp $(MACOS_OUT) dist/$(PROG_BASE)-macos-universal 2>/dev/null || true
	@echo Packaged artifacts in dist/

# Test that the expected artifacts are present in the dist/ directory after packaging
test: package
	@echo Running tests...
	@test -f dist/$(PROG_BASE)-windows-amd64.exe || (echo "ERROR: windows artifact missing" && exit 1)
	@test -f dist/$(PROG_BASE)-linux-amd64 || (echo "ERROR: linux artifact missing" && exit 1)
	@test -f dist/$(PROG_BASE)-macos-universal || (echo "ERROR: macos artifact missing" && exit 1)
	@echo "Test OK: artifacts present in dist/"

# Cleanup build artifacts
clean:
	-rm -f $(WINDOWS_OUT) $(LINUX_OUT) $(MACOS_OUT) *.o
	-rm -rf dist
	@echo Cleanup complete