# Makefile for UBSSTOR-Services-Manager
# Usage: make            (build)
#        make CC=mingw32-gcc (override compiler)

CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2
LDFLAGS ?= -ladvapi32

# Executable base name (without OS suffix)
PROG_BASE ?= UBSSTOR
SRCS := USBSTORServiceManager.c
PROG := $(PROG_BASE).exe

.PHONY: all build clean test run package help

all: build

build: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

test:
	@echo "No tests defined. Add a 'test' target if you have test cases."

run: build
	@echo "Running $(PROG)"
	@$(PROG)

package: build
	@mkdir -p dist
	@cp $(PROG) dist/$(PROG_BASE)-windows-amd64.exe 2> /dev/null || copy /Y $(PROG) dist\$(PROG_BASE)-windows-amd64.exe >NUL 2>NUL || true
	@echo "Packaged into dist/"

clean:
	-rm -f $(PROG)
	-rm -rf dist

help:
	@echo "Available targets:"
	@echo "  make         (build - default)"
	@echo "  make build   (compile $(PROG))"
	@echo "  make run     (build and run)"
	@echo "  make test    (run tests - none by default)"
	@echo "  make package (create dist/ and copy binary)"
	@echo "  make clean   (remove artifacts)"
