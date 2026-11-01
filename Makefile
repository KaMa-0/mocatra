# Directories & Files
SRC_DIR   := src
INC_DIR   := include
DST_DIR   := build
OUT_DIR	  := out
BASELINE  := test/baseline_original.ppm
OUT       := $(OUT_DIR)/mocatra_image.ppm

SRCS      := $(wildcard $(SRC_DIR)/*.c)

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

# 2. Base Compiler & Flags
CC      := gcc
CFLAGS  := -I$(INC_DIR) -I. -std=c99 -Wunused-function
LIBS    := -lm

.PHONY: default all debug run clean format test regression baseline-update

default: all run

# 3. Build Configurations
all: CFLAGS += -O3 -march=native -DNDEBUG
all: LDFLAGS += -s
all: $(DST)

debug: CFLAGS += -O0 -g -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: $(DST)
	gdb $(DST)

# Special build variant for regression testing with the seed/macro
regress-build: CFLAGS += -O3 -march=native -DMOCATRA_REGRESSION
regress-build: $(DST)

# Core compilation recipe (shared across targets)
$(DST): $(SRCS)
	@mkdir -p $(DST_DIR)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS) $(LIBS)

# 4. Utilities & Testing
run:
	@mkdir -p $(DST_DIR) $(OUT_DIR)
	@$(DST)

format:
	clang-format -i $(SRC_DIR)/*.c $(INC_DIR)/*.h
	ctags -R --languages=C,C++ --extras=+q --fields=+iaS -f tags .

regression: all
	@mkdir -p $(OUT_DIR)
	MOCATRA_SEED=43 $(DST)
	python3 tools/compare_ppm.py $(BASELINE) $(OUT)

test: regression

baseline-update: regress-build
	@mkdir -p $(OUT_DIR)
	MOCATRA_SEED=43 $(DST)
	cp $(OUT) $(BASELINE)

clean:
	rm -rf $(DST_DIR)/* $(OUT_DIR)/* tags
