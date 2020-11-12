.SUFFIXES:
.PHONY: all clean

# Build tools and compiler flags
CC     = cc
CFLAGS = -Wall -Wextra -pedantic
LDLIBS = -lgmp
AR 		 = ar
RANLIB = ranlib

# Build directory
BUILD = build

all: $(BUILD) $(BUILD)/libcombunrank.a

clean:
	rm -rf $(BUILD)

$(BUILD):
	mkdir "$@"

$(BUILD)/libcombunrank.a: $(BUILD)/recmeth.o combunrank.h
	ar rc $@ $?
	ranlib $@

$(BUILD)/%.o: algorithms/%.c combunrank.h
	$(CC) $(CFLAGS) -o $@ -c $< $(LDLIBS)
