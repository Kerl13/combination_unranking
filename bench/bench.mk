BOBJ = $(BUILD)bench/utils.o
BHDR = bench/utils.h

# Main scripts

$(BUILD)bench/k_varies: $(BUILD)libcombunrank.a $(BOBJ) bench/k_varies.c
	$(CC) $(CFLAGS) -L$(BUILD) -o $@ bench/k_varies.c $(BOBJ) -lcombunrank $(LDLIBS) 

$(BUILD)bench/n_varies: $(BUILD)libcombunrank.a $(BOBJ) bench/n_varies.c
	$(CC) $(CFLAGS) -L$(BUILD) -o $@ bench/n_varies.c $(BOBJ) -lcombunrank $(LDLIBS) 

$(BUILD)bench/n_varies_memo: $(BUILD)libcombunrank.a $(BOBJ) bench/n_varies_memo.c
	$(CC) $(CFLAGS) -L$(BUILD) -o $@ bench/n_varies_memo.c $(BOBJ) -lcombunrank $(LDLIBS) 

# Auxiliary modules

$(BUILD)bench/utils.o: bench/utils.c $(BHDR)
	@mkdir -p "$(BUILD)bench"
	$(CC) $(CFLAGS) -o $@ -c bench/utils.c

# Running the benchmark

$(BUILD)bench/%.k.dat: $(BUILD)bench/k_varies
	$< $* > $@

$(BUILD)bench/%_memo.n.dat: $(BUILD)bench/n_varies_memo
	$< $*_memo > $@

$(BUILD)bench/%.n.dat: $(BUILD)bench/n_varies
	$< $* > $@
