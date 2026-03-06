# ─────────────────────────────────────────────
# Makefile — Cross-compile for Windows from Linux
# ─────────────────────────────────────────────

# Use MinGW cross-compiler by default for Windows builds
CC ?= x86_64-w64-mingw32-gcc
PROG_BASE ?= USBSTORServiceManager
WINDOWS_OUT = $(PROG_BASE).exe
LINUX_STUB = linux_stub.c
LINUX_OUT = $(PROG_BASE)-linux

SRCS_WINDOWS = USBSTORServiceManager.c
CFLAGS = -Wall -Wextra -O2
LIBS = -ladvapi32

.PHONY: all build-windows build-linux-stub package clean test

all: build-windows

build-windows: $(WINDOWS_OUT)

$(WINDOWS_OUT): $(SRCS_WINDOWS)
	$(CC) $(CFLAGS) -o $@ $(SRCS_WINDOWS) $(LIBS)
	@echo Successfully compiled: $@

build-linux-stub: $(LINUX_OUT)

$(LINUX_OUT): $(LINUX_STUB)
	gcc -o $@ $(LINUX_STUB)
	@echo Created Linux stub: $@

package: build-windows build-linux-stub
	@mkdir -p dist
	@cp $(WINDOWS_OUT) dist/$(PROG_BASE)-windows-amd64.exe 2>/dev/null || true
	@cp $(LINUX_OUT) dist/$(PROG_BASE)-linux-amd64 2>/dev/null || true
	@echo Packaged artifacts in dist/

clean:
	-rm -f $(WINDOWS_OUT) $(LINUX_OUT) *.o
	-rm -rf dist
	@echo Cleanup complete

# Basic test — verifies the artifacts were produced
test: package
	@echo Running tests...
	@test -f dist/$(PROG_BASE)-windows-amd64.exe || (echo "ERROR: windows artifact missing" && exit 1)
	@echo "Test OK: artifacts present in dist/"