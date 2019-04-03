#include<stdio.h>
#include<stdlib.h>
#include "heys.h"






sbox_t sbox = {0xa, 0x0, 0x9, 0xe, 0x6, 0x3, 0xf, 0x5, 
        0x1, 0xd, 0xc, 0x7, 0xb, 0x4, 0x2, 0x8};


int main(int argc, char * argv[]){

  int tab_diff[16][16] ;

  // Initialisation de la table des differences
  for (size_t i = 0 ; i < 16 ; i++){
    for (size_t j = 0; j < 16; j++) {
      tab_diff[i][j] = 0 ;
    }
  }
  for(byte_t i = 0 ; i < 16 ; i++){
    for (byte_t j = 0 ; j < 16 ; j++) {
      tab_diff[i^j][sbox[i]^sbox[j]]++ ;
    }
  }
  int max = 0 ;
  for (size_t i = 0 ; i < 16 ; i++){
    for (size_t j = 0; j < 16; j++) {
      printf("%d ", tab_diff[i][j]) ;
	if (max < tab_diff[i][j] && tab_diff[i][j]!= 16) {
		max = tab_diff[i][j];
        }

    }
    printf("\n");
  }
	printf("\nle maximum de probabilité est %d \n ",max);
  return 0 ;
}
