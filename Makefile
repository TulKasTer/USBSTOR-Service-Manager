# ─────────────────────────────────────────────
# Makefile for Windows (MinGW)
# ─────────────────────────────────────────────

CC      = gcc
OUTPUT  = USBSTORServiceManager.exe
SRCS    = USBSTORServiceManager.c
CFLAGS  = -Wall -Wextra -O2
LIBS    = -ladvapi32

.PHONY: all clean test

all: $(OUTPUT)

$(OUTPUT): $(SRCS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRCS) $(LIBS)
	@echo Successfully compiled: $(OUTPUT)

# clean using Windows commands
clean:
	@if exist $(OUTPUT) del /f /q $(OUTPUT)
	@if exist *.o del /f /q *.o
	@echo Cleanup complete

# basic test — verifies the executable exists after build
test:
	@echo Running tests...
	@if not exist $(OUTPUT) ( echo ERROR: $(OUTPUT) not found && exit 1 )
	@echo Test OK: $(OUTPUT) compiled successfully