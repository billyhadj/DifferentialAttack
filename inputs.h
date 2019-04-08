#ifndef __INPUTS_H__
#define __INPUTS_H__

#include <stdbool.h>

typedef struct inputs_struct {
  int random_couples;
  char *cryptosystem;
  char print_key_step_by_step;
} inputs_t;

void parse_inputs(char *argv[]);
void parse_inputs_file(char *inputs_file);

#endif /* __INPUTS_H__ */
