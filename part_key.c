#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "heys.h"
#include "inputs.h"
#include "interface.h"

extern bool enable_interactif_io;
extern inputs_t *inputs;

//Fonction qui retourne l'indice de la valeur max d'un tableau de blocks donné. renvoie un int.
int max_of_block (block_t *b_r)
{
  block_t m = b_r[0];
  int r = 0;
  for(int i=0 ; i < 256; i++) {
    if( b_r[i] > m) {
      m = b_r[i];
      r = i;
    }
  }
  return r;
}

bool is_printed_key_step_by_step(void) {

  char answer = 'd';
  if (enable_interactif_io == false)
    answer = inputs->print_key_step_by_step;
  else {
    while (answer != 'y' && answer != 'n'){
      printf("Voulez vous afficher les clefs au fur et a mesure ? y/n ");
      get_char(&answer);
    }
  }
  return (answer == 'y') ? true : false;
}

ckey_t part_key(int f, sbox_t the_box)
{
  bool print_step = is_printed_key_step_by_step();
  
  uint16_t keys[256] = {0}; // Nous déclarons un tableau de clés
  block_t b_r[256] = {0}; // Nous prenons un tableau de 256 blocks initialisé à 0
  block_t a1, b1, a2, b2; // Nous déclarons 4 variables temporaires

  for (int j = 0; j < 256; j++) {	
    uint16_t k = (j & 0xf);
    k ^= ((j & 0xf0) << 4);
      
    if (lseek(f, 0 ,SEEK_SET) < 0) printf("Le fichier ne peut pas être réinisialisé \n");
	
    for (int i=0; i < 5000; i++) {	
      read_bytes(f, &a1, sizeof(a1));
      read_bytes(f, &b1, sizeof(b1));

      a1 = a1 ^ k;
      b1 = b1 ^ k;

      a2 = heys_subst(a1 ,the_box);
      b2 = heys_subst(b1 ,the_box);

      if( (a2 ^ b2) == 0x0606) {
	b_r[j]++;
	if (print_step) printf("%04x : %d \n", k, b_r[j]);
      }
    }	
    keys[j] = k;
  }
  
  return keys[max_of_block(b_r)];
}


ckey_t part_key_2(int fd, sbox_t box) {
  
  bool print_step = is_printed_key_step_by_step();
  
  uint16_t keys[256] = {0}; // Nous déclarons un tableau de clés
  block_t b_r[256] = {0}; // Nous prenons un tableau de 256 blocks initialisé à 0
  block_t a1, b1, a2, b2; // Nous déclarons 4 variables temporaires

  block_t a_tab[5000];
  block_t b_tab[5000];

  for (int i = 0; i < 5000; i++) {
    read_bytes(fd, &(a_tab[i]), sizeof(block_t));
    read_bytes(fd, &(b_tab[i]), sizeof(block_t));
  }
  
  for (int j = 0; j < 256; j++) {	
    uint16_t k = (j & 0xf);
    k ^= ((j & 0xf0) << 4);
    
    for (int i=0; i < 5000; i++) {	

      a1 = a_tab[i] ^ k;
      b1 = b_tab[i] ^ k;

      a2 = heys_subst(a1 ,box);
      b2 = heys_subst(b1 ,box);

      if( (a2 ^ b2) == 0x0606 ) {
	b_r[j]++;
	if (print_step) printf("%04x : %d \n", k, b_r[j]);
      }
    }	
    keys[j] = k;
  }
  
  return keys[max_of_block(b_r)];

}
