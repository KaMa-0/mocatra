SRC := $(wildcard ./src/*)
INC := ./include/
DST_DIR := ./bin
CONF := config.h

# 1. Detect Operating System
ifeq ($(OS),Windows_NT)
    OS_NAME := win64
    EXE_EXT := .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        OS_NAME := macos
        EXE_EXT := .app
    else
        OS_NAME := linux
        EXE_EXT :=
    endif
endif

DST := $(DST_DIR)/mocatra_$(OS_NAME)$(EXE_EXT)

# 2. Base Compiler Flags
CFLAGS := -I$(INC) -I. -std=c99
LIBS := -lm

# 3. Target-Specific Flags 
all: CFLAGS += -O3 -march=native -DNDEBUG
all: LDFLAGS += -s

# 4. Debug Flags
debug: CFLAGS += -O0 -g -fsanitize=address
debug: LDFLAGS += -fsanitize=address

.PHONY: default all debug run clean

default: all run

all:
	@mkdir -p $(DST_DIR)
	gcc $(SRC) -o $(DST) $(CFLAGS) $(LDFLAGS) $(LIBS)

debug:
	@mkdir -p $(DST_DIR)
	gcc $(SRC) -o $(DST) $(CFLAGS) $(LDFLAGS) $(LIBS)
	gdb $(DST)

run:
	@$(DST)

clean:
	rm -f test/* $(DST_DIR)/*
