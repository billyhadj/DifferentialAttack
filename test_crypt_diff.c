#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "brute_force.h"
#include "choose_rand.h"
#include "inputs.h"
#include "interface.h"
#include "part_key.h"
#include "timer.h"
#include "utility.h"
#include "caracteristic.h"

#ifndef PROCEEDING_ITERATIONS
#define PROCEEDING_ITERATIONS 3
#endif

#define PROCEEDING_SLEEP      1

#define DEFAULT_COMMAND_ENCRYPT "./encrypt06 random_file > encrypted"

extern inputs_t *inputs;
bool enable_interactif_io = true;

difference_table_t tab_diff = {0};
byte_t diff_true_tab[16][16] = {0};
block_t input_difference;
block_t output_difference;
diff_carac_tab_t diff_carac = {0};


void diff_tab_to_diff_true_tab(void) {
  for (unsigned int i = 0; i < SBOX_SIZE; i++) {
    for (unsigned int j = 0; j < SBOX_SIZE; j++) {
      diff_true_tab[i][j] = tab_diff[i*SBOX_SIZE + j];
    }
  }
}


void print_current_sbox(void) {
  print_titre("THE CURRENT SBOX");

  print_sbox(&sbox);
  next();
  next();
  clear();
}

void compute_difference_table(void) {
  START_TIMER();

  print_titre("THE ARRAY OF DIFFERENCES BETWEEN INPUTS AND OUTPUTS FROM THE CURRENT SBOX ");

  init_difference_table(&tab_diff);
  print_newlines(8);
  print_difference_table(&tab_diff);
  next();
  clear();

  END_TIMER();
}

void print_differential_caracteristic(void) {
  START_TIMER();
  print_titre("HERE ARE THE DIFFERENTIAL CARACTERISTICS WITH HIGHEST PROBABILITY");
  printf("                  ");
  for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
    printf("%6d ", i);
  }
  printf("\nInputs  (round 1):");
  for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
    printf("0x%04x ", diff_carac[i].input_diff);
  }
  #ifdef PATH_INFO
    printf("\nOutputs (round 1):");
    for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
      printf("0x%04x ", diff_carac[i].output_diff_round_1);
    }
    printf("\nInputs  (round 2):");
    for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
      printf("0x%04x ", diff_carac[i].input_diff_round_2);
    }
    printf("\nOutputs (round 2):");
    for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
      printf("0x%04x ", diff_carac[i].output_diff_round_2);
    }
    printf("\nInputs  (round 3):");
    for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
      printf("0x%04x ", diff_carac[i].input_diff_round_3);
    }
    printf("\nOutputs (round 3):");
    for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
      printf("0x%04x ", diff_carac[i].output_diff_round_3);
    }
  #endif
  printf("\nInputs  (round 4):");
  for (unsigned int i = 0; i < NB_CARACTERISTICS; i++) {
    printf("0x%04x ", diff_carac[i].output_diff);
  }

  int nb;
  printf("\nWhich differential caracteristic do you want?\n");
  if (enable_interactif_io)
    get_int(&nb);
  else
    nb = 0;
  input_difference = diff_carac[nb].input_diff;
  output_difference = diff_carac[nb].output_diff;
  END_TIMER();
}

void compute_differential_caracteristic(void) {
  START_TIMER();

  print_titre("DIFFERENTIAL CARACTERISTIC COMPUTING");
  diff_tab_to_diff_true_tab();
  differential_caracteristic(diff_carac, heys_perm, diff_true_tab);
#ifdef DEBUG
  system_call("evince atta* &");
#endif
  print_differential_caracteristic();
  next();

  END_TIMER();
}

void get_random_couples_integer(void) {
  START_TIMER();

  print_titre("THE BINARY FILE WHICH CONTAINS RANDOM COUPLES OF INTEGER 0xb00");

  int nb;
  printf("How many couples would you generate ? \n");
  if (enable_interactif_io)
    get_int(&nb);
  else
    nb = inputs->random_couples;
  rand_2(input_difference, nb);

  END_TIMER();
}

void print_generated_files(char *command_encrypt) {
  START_TIMER();

  system_call("echo Generated files : && ls | grep random_file");
  printf("\n");
  next();
  system_call(command_encrypt);
  system_call("echo Ciphering done...");
  system_call("echo Generated files : && ls | grep encrypted");
  printf("\n");
  next();
  clear();

  END_TIMER();
}

void launch_brut_force(ckey_t p_part) {
  START_TIMER();

  system_call("echo Press enter to launch the brute force attack");
  next();
  clear();
  system_call("echo BRUTE FORCE PROCESSING ...");

  ckey_t P_final = bruteforce_2(p_part);

  printf("\n\n\n\n\n------------------------\nTHE FINAL KEY : %lx\n-------------------------\n ",
	 P_final);

  END_TIMER();
}

ckey_t compute_partial_key(void) {
  START_TIMER();

  print_titre(" PARTIAL KEY COMPUTING");

  int fd_rand = open("encrypted",O_RDONLY);
  ckey_t p_part = part_key_2(fd_rand, isbox, output_difference);

  next();
  system_call("echo FIN DU CACLUL ");

  printf("--------------------------------\n");
  printf("The partial key is : %lx \n", p_part);
  printf("--------------------------------\n");

  END_TIMER();
  return p_part;
}

void clean_files(void) {
#ifndef DO_NOT_CLEAN
  system_call("rm random_file encrypted && make clean && echo TEMPORARY FILES ARE DELETED SUCCESSFULY ");
#endif
}

void print_requirements(void) {
  print_next("\n \nThe cryptosystem must be in the current repository, in a binary format.\n");
  print_next("\n \nMake sure that the Sbox is not wrong.\n");
}

void get_cryptosystem(char *cryptosystem, char *command_encrypt) {
  print_titre(" DIFFERENTIAL CRYPTANALYSIS OF A HEYS'S FUNCTION : Files ");
  printf("Enter the cryptosystem that you want to cryptanalyse, or 'example' : \n");

  if (enable_interactif_io) {
    get_string(cryptosystem);
  } else {
    strcpy(cryptosystem, inputs->cryptosystem);
  }

  if (strcmp(cryptosystem,"example") != 0) {
    sprintf(command_encrypt, "./%s random_file > encrypted ", cryptosystem);
  } else {
    strcpy(command_encrypt, DEFAULT_COMMAND_ENCRYPT);
  }

  printf("Encryption command: %s\n", command_encrypt);
}


int main (int argc, char * argv []){
  INIT_TIMER();
  START_TIMER();

  print_titre_next(" DIFFERENTIAL CRYPTANALYSIS OF A HEYS'S FUNCTION : Preconditions ");
  print_requirements();

  if (argc == 2) {
    char *inputs_file = argv[1];
    parse_inputs_file(inputs_file);
    enable_interactif_io = false;
  } else if (argc == 4) {
    inputs = (inputs_t*)malloc(sizeof(inputs_t));
    inputs->cryptosystem = (char*)malloc(sizeof(char*));
    parse_inputs(argv);
    enable_interactif_io = false;
  }

  char *cryptosystem = (char*)malloc(50 * sizeof(char)) ;
  char *command_encrypt = (char*)malloc(100 * sizeof(char)) ;
  get_cryptosystem(cryptosystem, command_encrypt);

  print_proceeding(PROCEEDING_ITERATIONS, PROCEEDING_SLEEP);

  print_current_sbox();
  compute_difference_table();
  compute_differential_caracteristic();
  get_random_couples_integer();
  print_generated_files(command_encrypt);
  ckey_t p_part = compute_partial_key();
  separation();
  launch_brut_force(p_part);
  clean_files();

  END_TIMER();
  return EXIT_SUCCESS ;

}
