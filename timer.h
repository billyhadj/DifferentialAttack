#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define FMT "%s %lu\n"

extern FILE* output_file;

#ifdef TIME_PROFILING
#define INIT_TIMER() init_timer();
#define START_TIMER() start_timer();
#define END_TIMER() fprintf(output_file, FMT,__FUNCTION__, end_timer());
#else
#define INIT_TIMER() {;}
#define START_TIMER() {;}
#define END_TIMER() {;}
#endif

void init_timer();
void start_timer();
uint64_t end_timer();

#endif /* TIMER*/
