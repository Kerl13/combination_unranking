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

# ---
# libcombunrank static library
# ---

$(BUILD)libcombunrank.a: $(BUILD)recmeth.o
$(BUILD)libcombunrank.a: $(BUILD)factoradics.o
	ar rc $@ $?
	ranlib $@

$(BUILD)%.o: algorithms/%.c combunrank.h
	@[ -e "$(BUILD)" ] || mkdir "$(BUILD)"
	$(CC) $(CFLAGS) -o $@ -c $< $(LDLIBS)

# ---
# Test suite
# ---

test: $(BUILD)tests/exhaustive.done

$(BUILD)tests/%.done: $(BUILD)tests/% $(BUILD)tests
	./$<
	touch $@

$(BUILD)tests/%: tests/%.c $(BUILD)libcombunrank.a
	@[ -e "$(BUILD)test" ] || mkdir -p "$(BUILD)tests"
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS) -L$(BUILD) -lcombunrank

# ---
# Benchmarks
# ---

bench: $(BUILD)

$(BUILD)bench/%: bench/%.c $(BUILD)libcombunrank.a
	@[ -e "$(BUILD)bench" ] || mkdir -p "$(BUILD)bench"
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS) -L$(BUILD) -lcombunrank

$(BUILD)bench/%.dat: $(BUILD)bench/k_varies
	$< $* > $@

$(BUILD)bench/k_varies.svg: $(BUILD)bench/factoradics.dat
$(BUILD)bench/k_varies.svg: $(BUILD)bench/recmeth.dat
	echo "$^" | gnuplot -d plot.gpi > $@
