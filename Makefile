CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -fopenmp

.SUFFIXES: .c .o

PRGS = test_crypt_diff

OBJS=test_crypt_diff.o choose_rand.o brute_force.o part_key.o heys.o interface.o utility.o timer.o inputs.o rbtree.o

# -freduce-all-givs -ftracer
# -funroll-loops

ifeq ($(DEBUG),1)
  CFLAGS += -DPROCEEDING_ITERATIONS=0 -DDEBUG=1 -g -Werror -O0
else
  CFLAGS += -march=native -Ofast -ffast-math -ftree-loop-vectorize -fno-strict-aliasing  \
	-fmerge-all-constants -flto -fopenmp -fsched-spec-load -fsched-spec-load-dangerous \
	-fsched2-use-traces  -frename-registers -funit-at-a-time \
	-fpeel-loops -funswitch-loops -fprefetch-loop-arrays \
	-DPERF -DPROCEEDING_ITERATIONS=0 
endif

ifeq ($(PROFILE),1)
  CFLAGS += -DTIME_PROFILING
endif

ifdef TIME_PROFILING_OUTPUT
  CFLAGS += -DTIME_PROFILING_OUTPUT="$(TIME_PROFILING_OUTPUT)"
endif

ifdef KEEP_TRACES
  CFLAGS += -DDO_NOT_CLEAN
endif

all:\
	$(PRGS)

run:$(PRGS)
	export OMP_CANCELLATION=true
	time ./$(PRGS) 1000 example y

test_crypt_diff:\
	$(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o $@

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
