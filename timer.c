#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "timer.h"

#define STACK_SIZE_DEFAULT 1024
#define EMPTY_STACK -1ULL

FILE* output_file = NULL;

/* stringizing output file */
#ifdef TIME_PROFILING_OUTPUT
#define xstr(x) str(x)
#define str(s) #s
#define TIME_OUTPUT_FILE xstr(TIME_PROFILING_OUTPUT)
#endif

void init_timer(void) {
#ifdef TIME_PROFILING_OUTPUT
output_file = fopen(TIME_OUTPUT_FILE, "w");
#else
output_file = stdout;
#endif
}

typedef struct stack {
  uint64_t _pos;
  uint64_t _size;
  uint64_t _data[STACK_SIZE_DEFAULT];
} stack_t;

static stack_t stack = {0, STACK_SIZE_DEFAULT, {0ULL}};

uint64_t pop() {
  uint64_t elt = EMPTY_STACK;
  if (stack._pos > 0) {
    stack._pos--;
    elt = stack._data[stack._pos];
  }
  return elt;
}

void push(uint64_t elt) {
  /* 1024 should be sufficient since we only keep a small number of functions */
  /* and there is no recursive calls  */
  assert((stack._pos < stack._size) && "Stack overflow: increase size of timer stack");

  stack._data[stack._pos] = elt;
  stack._pos++;
}

#ifdef __i386
extern __inline__ uint64_t rdtsc(void) {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#elif defined __amd64
extern __inline__ uint64_t rdtsc(void) {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif

void start_timer(void) {
  push(rdtsc());
};

uint64_t end_timer(void) {
  uint64_t start_time = pop();
  uint64_t end_time = rdtsc();  
  return end_time - start_time;
};
