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
  for(int i=0 ; i < SBOX_SIZE*SBOX_SIZE*SBOX_SIZE*SBOX_SIZE; i++) {
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

ckey_t part_key(int f, sbox_t the_box, block_t delta)
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

      if( (a2 ^ b2) == delta) {
	b_r[j]++;
	if (print_step) printf("%04x : %d \n", k, b_r[j]);
      }
    }
    keys[j] = k;
  }

  return keys[max_of_block(b_r)];
}


ckey_t part_key_2(int fd, sbox_t box, block_t delta, int nb_val) {
        printf("Différence en sortie : 0x%04x\n", delta);

  bool print_step = is_printed_key_step_by_step();

  byte_t indices[4]; // Nous déclarons un tableau de 4 booléens qui enregistre si oui ou non on peut déterminer la valeur i en hexa de la clef partielle
  if ((delta & 0x000f) != 0)
    indices[0] = 1;
  else
    indices[0] = 0;
  if ( (delta & 0x00f0) != 0)
    indices[1] = 1;
  else
    indices[1] = 0;
  if ( (delta & 0x0f00) != 0)
    indices[2] = 1;
  else
    indices[2] = 0;
  if ( (delta & 0xf000) != 0)
    indices[3] = 1;
  else
    indices[3] = 0;

  rkey_t keys[SBOX_SIZE*SBOX_SIZE*SBOX_SIZE*SBOX_SIZE] = {}; // Nous déclarons un tableau de clés
  block_t b_r[SBOX_SIZE*SBOX_SIZE*SBOX_SIZE*SBOX_SIZE] = {}; // Nous prenons un tableau de 256 blocks initialisé à 0
  block_t a1, b1, a2, b2; // Nous déclarons 4 variables temporaires
  block_t a_tab[nb_val];
  block_t b_tab[nb_val];

  for (int i = 0; i < nb_val; i++) {
    read_bytes(fd, &(a_tab[i]), sizeof(block_t));
    read_bytes(fd, &(b_tab[i]), sizeof(block_t));
  }
  ckey_t k;
  unsigned int decalage1 = SBOX_SIZE, decalage2 = SBOX_SIZE*SBOX_SIZE, decalage3 = SBOX_SIZE*SBOX_SIZE*SBOX_SIZE;
  for (int j1 = 0; j1 < SBOX_SIZE; j1++) {
    for (int j2 = 0; j2 < SBOX_SIZE; j2++) {
      for (int j3 = 0; j3 < SBOX_SIZE; j3++) {
        for (int j4 = 0; j4 < SBOX_SIZE; j4++) {
          k = j1 ^ (j2*decalage1) ^ (j3*decalage2) ^ (j4*decalage3);
          keys[k] = k;
          for (int i=0; i < nb_val; i++) {

            a1 = a_tab[i] ^ k;
            b1 = b_tab[i] ^ k;

            a2 = heys_subst(a1 ,box);
            b2 = heys_subst(b1 ,box);

            if( (a2 ^ b2) == delta ) {
      	      b_r[k]++;
      	        //if (print_step) printf("%04lx : %d \n", k, b_r[j1]);
            }
          }
          if (indices[3] == 0) {
            break;
          }
        }
        if (indices[2] == 0) {
          break;
        }
      }
      if (indices[1] == 0) {
        break;
      }
    }
    if (indices[0] == 0) {
      break;
    }
    //k = (j1 & 0xf);
    //k ^= ((j1 & 0xf0) << 4);


  }

  return keys[max_of_block(b_r)];

}
