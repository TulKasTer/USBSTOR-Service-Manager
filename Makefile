# ─────────────────────────────────────────────
# Makefile — USBSTORServiceManager
# Targets: Windows (cross), Linux (native), macOS (native)
# ─────────────────────────────────────────────

PROG_BASE ?= USBSTORServiceManager
SRC        = USBSTORServiceManager.c
CFLAGS     = -Wall -Wextra -O2

# Compilers
CC_WINDOWS ?= x86_64-w64-mingw32-gcc
CC_LINUX   ?= gcc
CC_MACOS   ?= clang

# Output names
OUT_WINDOWS = $(PROG_BASE)-windows-amd64.exe
OUT_LINUX   = $(PROG_BASE)-linux-amd64
OUT_MACOS   = $(PROG_BASE)-macos-universal

# Libraries
LIBS_WINDOWS = -ladvapi32
LIBS_LINUX   =
LIBS_MACOS   =

.PHONY: all build-windows build-linux build-macos package clean test

all:
ifeq ($(OS),Windows_NT)
	$(MAKE) build-windows
else
	$(MAKE) build-linux
endif

# ── Windows ───────────────────────────────────
build-windows: $(OUT_WINDOWS)

$(OUT_WINDOWS): $(SRC)
	$(CC_WINDOWS) $(CFLAGS) -o $@ $< $(LIBS_WINDOWS)
	@echo "[ok] Compiled: $@"

# ── Linux ─────────────────────────────────────
build-linux: $(OUT_LINUX)

$(OUT_LINUX): $(SRC)
	$(CC_LINUX) $(CFLAGS) -o $@ $< $(LIBS_LINUX)
	@echo "[ok] Compiled: $@"

# ── macOS (universal binary) ──────────────────
build-macos: $(OUT_MACOS)

$(OUT_MACOS): $(SRC)
	clang -target x86_64-apple-macos10.15 -o $(PROG_BASE)-x86   $(SRC)
	clang -target arm64-apple-macos11     -o $(PROG_BASE)-arm64  $(SRC)
	lipo -create -output $(OUT_MACOS) $(PROG_BASE)-x86 $(PROG_BASE)-arm64
	@rm $(PROG_BASE)-x86 $(PROG_BASE)-arm64
	@echo "[ok] Compiled: $@"

# ── Package ───────────────────────────────────
package: build-windows build-linux build-macos
	@mkdir -p dist
	@cp $(OUT_WINDOWS) dist/ 2>/dev/null || true
	@cp $(OUT_LINUX)   dist/ 2>/dev/null || true
	@cp $(OUT_MACOS)   dist/ 2>/dev/null || true
	@echo "[ok] Artifacts in dist/"

# ── Test ──────────────────────────────────────
test: package
	@echo "Running artifact checks..."
	@test -f dist/$(OUT_WINDOWS) || (echo "ERROR: Windows artifact missing" && exit 1)
	@test -f dist/$(OUT_LINUX)   || (echo "ERROR: Linux artifact missing"   && exit 1)
	@test -f dist/$(OUT_MACOS)   || (echo "ERROR: macOS artifact missing"   && exit 1)
	@echo "[ok] All artifacts present."

# ── Clean ─────────────────────────────────────
clean:
	-rm -f $(OUT_WINDOWS) $(OUT_LINUX) $(OUT_MACOS) *.o
	-rm -rf dist
	@echo "[ok] Cleanup complete."