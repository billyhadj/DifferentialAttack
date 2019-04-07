#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "inputs.h"

inputs_t *inputs = NULL;

void parse_inputs(char *argv[]) {
  
  inputs->random_couples = atoi(argv[1]);
  strcpy(inputs->cryptosystem,argv[2]);  
  inputs->print_key_step_by_step = argv[3][0];

  /* inputs->random_couples = 100; */
  /* inputs->cryptosystem = "example"; */
  /* inputs->print_key_step_by_step = 'y'; */
  
  printf("Parsed inputs: %d\n", inputs->random_couples);
  printf("Parsed inputs: %s\n", inputs->cryptosystem);
  printf("Parsed inputs: %c\n", inputs->print_key_step_by_step);
}

void parse_inputs_file(char *inputs_file) {

  FILE *inputs_FILE = fopen(inputs_file, "r");

  if (inputs_FILE == NULL) {
    fprintf(stderr, "Error: Cannot open inputs file: %s (%s)\n", inputs_file, strerror(errno));
    exit(EXIT_FAILURE);
  }

  size_t nread;
  char *line = NULL;
  size_t len = 0, length = 0;
  char *key = NULL, *value = NULL;
  
  while ((nread = getline(&line, &len, inputs_FILE)) != -1) {
    key = strtok(line, ":"); /* first part */
    value = strtok(NULL, ":"); /* second part */
    length = strlen(value);
    value[length-1] = '\0';
    
    switch (key[0]){
    case 'r': /* random couples */
      inputs->random_couples = atoi(value);
    case 'c': /* cryptosystem */
      inputs->cryptosystem = value;
    }
  }  
}

