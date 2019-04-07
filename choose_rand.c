#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "choose_rand.h"
#include "interface.h"


void rand_2 (int difference, int nombre)
{
  int s = open("/dev/urandom", O_RDONLY);
  int s2 = open("random_file",O_CREAT | O_WRONLY, 00666);	

  block_t a ;
  block_t b ;

  for(int i=0 ; i<nombre;i++){	
    read_bytes(s,&a,sizeof(a));
    b=a^difference; 
    write_bytes(s2,&a, sizeof(a));
    write_bytes(s2,&b,sizeof(b));
  }
  close(s);
}


void rand_2_2 (int difference, int nombre)
{
  int s = open("/dev/urandom", O_RDONLY);
  int s2 = open("random_file", O_CREAT | O_WRONLY, 00666);	

  block_t a[nombre];
  block_t b[nombre];

  for (int i=0; i < nombre; i++) {
    read_bytes(s, &(a[i]), sizeof(block_t));
  }

  for (int i=0; i < nombre; i++) {
    b[i] = a[i]^difference;
  }

  for (int i=0; i < nombre; i++) {
    write_bytes(s2, &(a[i]), sizeof(block_t));
    write_bytes(s2, &(b[i]), sizeof(block_t));
  }
  
  close(s);
}

