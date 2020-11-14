.SUFFIXES:
.PHONY: all clean test bench

# Build tools and compiler flags
CC     = cc
CFLAGS = -Wall -Wextra -pedantic -g -O2
LDLIBS = -lgmp
AR 		 = ar
RANLIB = ranlib

# Build directory
BUILD = build/

all: $(BUILD)libcombunrank.a $(BUILD)unrank

test: $(BUILD)tests/aux
test: $(BUILD)tests/aux.done
test: $(BUILD)tests/exhaustive
test: $(BUILD)tests/exhaustive.done
test: $(BUILD)tests/permutations
test: $(BUILD)tests/permutations.done

bench: $(BUILD)bench/k_varies
bench: $(BUILD)bench/factoradics.dat
bench: $(BUILD)bench/recmeth.dat
bench: $(BUILD)bench/k_varies.svg

clean:
	rm -rf $(BUILD)

# ---
# libcombunrank static library
# ---

$(BUILD)libcombunrank.a: $(BUILD)interval_trees.o
$(BUILD)libcombunrank.a: $(BUILD)recmeth.o
$(BUILD)libcombunrank.a: $(BUILD)factoradics.o
	ar rc $@ $?
	ranlib $@

$(BUILD)%.o: src/%.c combunrank.h
	@[ -e "$(BUILD)" ] || mkdir "$(BUILD)"
	$(CC) $(CFLAGS) -o $@ -c $< $(LDLIBS)

# ---
# Cmd line tool
# ---

$(BUILD)unrank: unrank.c $(BUILD)libcombunrank.a
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS) -L$(BUILD) -lcombunrank

# ---
# Test suite
# ---

$(BUILD)tests/%.done: $(BUILD)tests/%
	./$<
	@touch $@

$(BUILD)tests/%: tests/%.c $(BUILD)libcombunrank.a
	@[ -e "$(BUILD)test" ] || mkdir -p "$(BUILD)tests"
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS) -L$(BUILD) -lcombunrank

# ---
# Benchmarks
# ---

$(BUILD)bench/%: bench/%.c $(BUILD)libcombunrank.a
	@[ -e "$(BUILD)bench" ] || mkdir -p "$(BUILD)bench"
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS) -L$(BUILD) -lcombunrank

$(BUILD)bench/%.dat: $(BUILD)bench/k_varies
	$< $* > $@

$(BUILD)bench/k_varies.svg: $(BUILD)bench/factoradics.dat
$(BUILD)bench/k_varies.svg: $(BUILD)bench/recmeth.dat
	echo "$^" | gnuplot -d plot.gpi > $@
