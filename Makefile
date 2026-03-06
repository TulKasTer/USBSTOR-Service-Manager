CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS := -ladvapi32

TARGET := USBSTORServiceManager.exe
SRCS := USBSTORServiceManager.c
OBJS := $(SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
RUN_CMD := .\$(TARGET)
else
RUN_CMD := ./$(TARGET)
endif

.PHONY: all build clean run help

all: build

build: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	-rm -f $(TARGET) $(OBJS)

run: build
	$(RUN_CMD)

help:
	@echo "Targets:"
	@echo "  make        (build - default)"
	@echo "  make build  (build binary)"
	@echo "  make run    (build and run)"
	@echo "  make clean  (remove artifacts)"