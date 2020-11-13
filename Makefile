.SUFFIXES:
.PHONY: all clean test

# Build tools and compiler flags
CC     = cc
CFLAGS = -Wall -Wextra -pedantic
LDLIBS = -lgmp
AR 		 = ar
RANLIB = ranlib

# Build directory
BUILD = build/

all: $(BUILD) $(BUILD)libcombunrank.a

clean:
	rm -rf $(BUILD)

$(BUILD):
	mkdir "$@"
	mkdir "$@/tests"

# ---
# libcombunrank static library
# ---

$(BUILD)libcombunrank.a: $(BUILD)recmeth.o
$(BUILD)libcombunrank.a: $(BUILD)factoradics.o
	ar rc $@ $?
	ranlib $@

$(BUILD)%.o: algorithms/%.c combunrank.h
	$(CC) $(CFLAGS) -o $@ -c $< $(LDLIBS)

# ---
# Test suite
# ---

test: $(BUILD)
test: $(BUILD)tests/exhaustive.done

$(BUILD)tests/%.done: $(BUILD)tests/% $(BUILD)tests
	./$<
	touch $@

$(BUILD)tests/%: tests/%.c $(BUILD)libcombunrank.a
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS) -L$(BUILD) -lcombunrank
