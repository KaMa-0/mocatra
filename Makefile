SRC := $(wildcard ./src/*)
INC := ./include/
DST_DIR := ./bin
CONF := config.h

# 1. Detect Operating System
ifeq ($(OS),Windows_NT)
    # Windows environment (detected via environment variable)
    OS_NAME := win64
    EXE_EXT := .exe
else
    # Unix-like environments (macOS / Linux)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        OS_NAME := macos
        EXE_EXT := .app
    else
        OS_NAME := linux
        EXE_EXT :=
    endif
endif

# 2. Define the OS-specific binary destination
DST := $(DST_DIR)/mocatra_$(OS_NAME)$(EXE_EXT)

# Compiler flags
CFLAGS := -I$(INC) -I. -fsanitize=address -std=c99
LIBS := -lm

.PHONY: default all debug run clean

default: all run

all:
	@mkdir -p $(DST_DIR)
	gcc $(SRC) -o $(DST) $(CFLAGS) -s $(LIBS)

debug:
	@mkdir -p $(DST_DIR)
	gcc $(SRC) -o $(DST) $(CFLAGS) -g $(LIBS)
	gdb $(DST)

run:
	@$(DST)

clean:
	rm -f test/* $(DST_DIR)/*
