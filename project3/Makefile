BIN_NAME    := wordle
TESTER_NAME := testrunner

BIN_FILES    := $(shell find ./src -type f ! -wholename '*/unit_tests.c' ! -wholename '*/test_main.c')
TESTER_FILES := $(shell find ./src -type f ! -wholename '*/main.c')
HEADERS      := $(wildcard include/*.h)

TEST_SCRIPT := test/run_tests.py
Q ?= @

DEBUG   := -O0 -g -fsanitize=address -fsanitize=undefined
OPT     := -O3

CFLAGS  += -I include -Wall -Wextra -pedantic
LDFLAGS +=

.PHONY: all check clean init

all: init bin/$(BIN_NAME)_opt bin/$(BIN_NAME)_debug bin/$(TESTER_NAME)


bin/lib$(BIN_NAME).so: $(patsubst src/%.c, build/%.opt.o, $(BIN_FILES))
	$(Q)mkdir -p build
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -shared -o $@ $(CFLAGS) $(OPT) $+ $(LDFLAGS) -fPIC

init: 
	$(Q)echo -e "CompileFlags:\n  Add:\n    - \"--include-directory=$(PWD)/include\"" > .clangd

bin/$(BIN_NAME)_opt: $(patsubst src/%.c, build/%.opt.o, $(BIN_FILES))
	$(Q)mkdir -p build
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(OPT) $+ $(LDFLAGS)

bin/$(BIN_NAME)_debug: $(patsubst src/%.c, build/%.debug.o, $(BIN_FILES))
	$(Q)mkdir -p build
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(DEBUG) $+ $(LDFLAGS)

bin/$(TESTER_NAME): $(patsubst src/%.c, build/%.debug.o, $(TESTER_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(DEBUG) $+ $(LDFLAGS)

build/%.opt.o: src/%.c $(HEADERS)
	$(Q)mkdir -p $(@D)
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c $(CFLAGS) $(OPT) $<

build/%.debug.o: src/%.c $(HEADERS)
	$(Q)mkdir -p $(@D)
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c $(CFLAGS) $(DEBUG) $<

check: init bin/$(TESTER_NAME) bin/$(BIN_NAME)_debug bin/$(BIN_NAME)_opt
	@echo "===> CHECK"
	$(Q)mkdir -p build
	$(Q)$(TEST_SCRIPT)

clean:
	@echo "===> CLEAN"
	$(Q)rm -rf bin build

