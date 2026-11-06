# ============================ Directories & Files ============================
SRC_DIR      := src
DST_DIR      := build
OUT_DIR      := out
REG_DIR      := $(OUT_DIR)/regression
TEST_DIR     := test

# stored reference image (produced once, from the original code)
BASELINE_REF := $(TEST_DIR)/baseline_original.ppm
# paths the regression builds write to -- must match config.h
BASELINE_OUT := $(REG_DIR)/baseline.ppm
TEST_OUT     := $(REG_DIR)/test.ppm

SRCS := $(sort $(shell find $(SRC_DIR) -type f -name '*.c'))
# headers are prerequisites too: config.h changes must trigger a rebuild
HDRS := $(sort $(shell find $(SRC_DIR) -type f -name '*.h') $(wildcard *.h))

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

DST          := $(DST_DIR)/mocatra_$(OS_NAME)$(EXE_EXT)
# one binary per mode, so switching modes can never reuse a stale binary
DST_BASELINE := $(DST_DIR)/mocatra_baseline_$(OS_NAME)$(EXE_EXT)
DST_TEST     := $(DST_DIR)/mocatra_test_$(OS_NAME)$(EXE_EXT)

# 2. Base Compiler & Flags
CC      := gcc
CFLAGS  := -I$(SRC_DIR) -I. -std=c11 -Wunused-function -Wall -Wextra -Wpedantic
LIBS    := -lm

# Flags for BOTH regression builds. They must stay identical to each other
# (and to the flags that produced the stored baseline), otherwise a pixel
# difference cannot be blamed on the code.
REG_CFLAGS := $(CFLAGS) -O3 -march=native

.PHONY: default all debug run clean format test baseline-update

default: all run

# 3. Build Configurations
all: CFLAGS += -O3 -march=native -DNDEBUG
all: LDFLAGS += -s
all: $(DST)

debug: CFLAGS += -O0 -g -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: $(DST)
	gdb $(DST)

# Core compilation recipe for the normal build
$(DST): $(SRCS) $(HDRS)
	@mkdir -p $(DST_DIR)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS) $(LIBS)

# Regression builds: same sources and flags, only the mode macro differs
$(DST_BASELINE): $(SRCS) $(HDRS)
	@mkdir -p $(DST_DIR)
	$(CC) $(SRCS) -o $@ $(REG_CFLAGS) -DMOCATRA_REGRESSION $(LIBS)

$(DST_TEST): $(SRCS) $(HDRS)
	@mkdir -p $(DST_DIR)
	$(CC) $(SRCS) -o $@ $(REG_CFLAGS) -DMOCATRA_TEST $(LIBS)

# 4. Utilities & Testing
run:
	@mkdir -p $(DST_DIR) $(OUT_DIR)
	@$(DST)

format:
	find $(SRC_DIR) -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
	ctags -R --languages=C,C++ --extras=+q --fields=+iaS -f tags .

# build + run the test configuration, then compare against the stored baseline
test: $(DST_TEST)
	@mkdir -p $(REG_DIR)
	@rm -f $(TEST_OUT)
	$(DST_TEST)
	python3 tools/compare_ppm.py $(BASELINE_REF) $(TEST_OUT)

# (re)create the stored baseline. Refuses to overwrite an existing one unless
# FORCE=1, because after the first capture it is the evidence that refactoring
# preserved behavior. Only update it for an intended, explained change.
baseline-update: $(DST_BASELINE)
	@if [ -f $(BASELINE_REF) ] && [ "$(FORCE)" != "1" ]; then \
		echo "refusing to overwrite $(BASELINE_REF) (use: make baseline-update FORCE=1)"; \
		exit 1; \
	fi
	@mkdir -p $(REG_DIR) $(TEST_DIR)
	@rm -f $(BASELINE_OUT)
	$(DST_BASELINE)
	cp $(BASELINE_OUT) $(BASELINE_REF)

clean:
	rm -rf $(DST_DIR)/* $(OUT_DIR)/* tags
