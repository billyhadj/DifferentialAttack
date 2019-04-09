#include "utility.h"

void init_difference_table(difference_table_t *diff_table) {
  byte_t col = 0, row = 0;
  for(byte_t i = 0 ; i < SBOX_SIZE ; i++){
    for (byte_t j = 0 ; j < SBOX_SIZE ; j++) {
      row = i^j;
      col = sbox[i]^sbox[j];
      (*diff_table)[row * SBOX_SIZE + col]++;
    }
  }
}

void print_difference_table(difference_table_t *diff_table) {
  for (int i = 0; i < SBOX_SIZE; i++) {
    for (int j = 0; j < SBOX_SIZE; j++) {
      printf("%02hhu ", (*diff_table)[i*SBOX_SIZE+j]);
    }
    printf("\n\033[0m");
  }
}

byte_t highest_value_difference_table(difference_table_t *diff_table) {
  byte_t res = 0;
  for (unsigned int i = 0; i < SBOX_SIZE; i++) {
    for (unsigned int j = 0; j < SBOX_SIZE; j++) {
      if ((*diff_table)[i*SBOX_SIZE + j] != SBOX_SIZE && (*diff_table)[i*SBOX_SIZE + j] > res) {
        res = (*diff_table)[i*SBOX_SIZE + j];
      }
    }
  }
  return res;
}
