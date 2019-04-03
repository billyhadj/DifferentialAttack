CC = gcc
CFLAGS = -w -O3

.SUFFIXES: .c .o

PRGS = test_crypt_diff

OBJS=test_crypt_diff.o choose_rand.o brute_force.o part_key.o heys.o interface.o

all:\
	$(PRGS)

test_crypt_diff:\
	$(OBJS)
	$(CC) $(OBJS) -o $@

.c.o: \
	$*.h $*.c
	@echo $*
	$(CC) $(CFLAGS) -c $< 

clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name '*~' -exec rm -f {} ';'
	find . -name '#*#' -exec rm -f {} ';'
	find . -name '.#*' -exec rm -f {} ';'
	find . -name 'core' -exec rm -f {} ';'
	find . -name '*.core' -exec rm -f {} ';'
	-rm -rf $(PRGS)
