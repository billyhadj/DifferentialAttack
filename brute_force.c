#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <omp.h>
#include <stdbool.h>

#include "heys.h"
#include "timer.h"
#include "interface.h"

ckey_t bruteforce(ckey_t P_key)
{
  START_TIMER();
  block_t x, y, z, x2, y2, z2;
  int f, f2;

  if ( (f= open("./encrypted", O_RDONLY)) < 0 ) {
    fprintf(stderr,"le fichier ne peut être ouvert %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }

  read_bytes(f,&x,sizeof(x));
  read_bytes(f,&y,sizeof(y));
  read_bytes(f,&z,sizeof(z));
  close(f);


  if ( (f2= open("./random_file", O_RDONLY)) < 0 ) printf("le fichier ne peut être ouvert \n");

  read_bytes(f2,&x2,sizeof(x2));
  read_bytes(f2,&y2,sizeof(y2));
  read_bytes(f2,&z2,sizeof(z2));
  ckey_t k;
  for(ckey_t j= 0ull ; j< (1ull<<28); j++)
  {
    for(ckey_t i= 0ull; i< (1ull<<4) ; i++)
    {
      k = P_key ^ (j<<12) ^ (i<<4);
      if(x2 == heys_decrypt(x, k) )
      {
        if(y2 == heys_decrypt(y, k) )


        {
          if(z2 == heys_decrypt(z, k) )
          {
            return k;
          }
        }
      }
    }
  }
  END_TIMER();
  return -1;
}


inline ckey_t internal_bruteforce_2(ckey_t P_key, ckey_t j, block_t x, block_t y, block_t z,
			     block_t x2, block_t y2, block_t z2) {
  return 0;
}

ckey_t bruteforce_2(ckey_t P_key)
{
  START_TIMER();
  block_t x, y, z, x2, y2, z2;
  int f, f2;

  if ( (f= open("encrypted", O_RDONLY)) < 0 ) {
    fprintf(stderr,"le fichier ne peut être ouvert: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }

  read_bytes(f, &x, sizeof(x));
  read_bytes(f, &y, sizeof(y));
  read_bytes(f, &z, sizeof(z));
  close(f);

  if ( (f2= open("random_file", O_RDONLY)) < 0 ) {
    printf("le fichier ne peut être ouvert: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }

  read_bytes(f2, &x2, sizeof(x2));
  read_bytes(f2, &y2, sizeof(y2));
  read_bytes(f2, &z2, sizeof(z2));

  const uint64_t twoTo28 = 1ull << 28;
  const uint64_t twoTo4 = 1ull << 4;

  ckey_t result;
  ckey_t k = 0;
  bool found = false;

#ifdef DEBUG
  fprintf(stderr, "OMP_CANCELLATION has been set %d\n", omp_get_cancellation());
#endif

  fprintf(stderr, "Encrypted:    x: %02hx  y: %02hx  z: %02hx\n",x,y,z);
  fprintf(stderr, "Random_file: x2: %02hx y2: %02hx z2: %02hx\n",x2,y2,z2);

#pragma omp parallel for collapse(2), firstprivate(k), shared(result, found)
  for(ckey_t j= 0ull ; j < twoTo28; j++) {
    for(ckey_t i= 0ull; i < twoTo4; i++) {
      k = P_key ^ (j<<12) ^ (i<<4);
      if (found || ((x2 == heys_decrypt_2(x, k)) &&
		    (y2 == heys_decrypt_2(y, k)) &&
		    (z2 == heys_decrypt_2(z, k)))) {

#pragma omp critical
{
	  if (found == false) {
	    result = k;
	    found = true;
	    fprintf(stderr,"FOUND! %lx\n", result);
	    END_TIMER();
	  }
}

#pragma omp cancel for
 {
   fprintf(stderr,"[%d] I am waiting for my collegues...\n",omp_get_thread_num());
 }

      }
    }
  }

  return result;

  return -1;
}

ckey_t
bruteforce_3(rkey_t part_key) {
        START_TIMER();
        printf("Bruteforcing with key %04x\n", part_key);
        block_t x, y, z, x2, y2, z2;
        int f, f2;

        if ( (f= open("encrypted", O_RDONLY)) < 0 ) {
          fprintf(stderr,"le fichier ne peut être ouvert: %s\n",strerror(errno));
          exit(EXIT_FAILURE);
        }

        read_bytes(f, &x, sizeof(x));
        read_bytes(f, &y, sizeof(y));
        read_bytes(f, &z, sizeof(z));
        close(f);

        if ( (f2= open("random_file", O_RDONLY)) < 0 ) {
          printf("le fichier ne peut être ouvert: %s\n",strerror(errno));
          exit(EXIT_FAILURE);
        }

        read_bytes(f2, &x2, sizeof(x2));
        read_bytes(f2, &y2, sizeof(y2));
        read_bytes(f2, &z2, sizeof(z2));

        ckey_t result = part_key;
        ckey_t tmp;
        unsigned int decalage1 = SBOX_SIZE, decalage2 = SBOX_SIZE*SBOX_SIZE, decalage3 = SBOX_SIZE*SBOX_SIZE*SBOX_SIZE;
        /* pour la clef totale démarrant à la clef partielle et parcourant les bits des autres tours */
        for ( ; result < (1ULL << CIPHERKEYSIZE); result += (1ULL << ROUNDKEYSIZE)) {
                for (byte_t i1 = 0; i1 < SBOX_SIZE; i1++) {
                        for (byte_t i2 = 0; i2 < SBOX_SIZE; i2++) {
                                for (byte_t i3 = 0; i3 < SBOX_SIZE; i3++) {
                                        for (byte_t i4 = 0; i4 < SBOX_SIZE; i4++) {
                                                tmp = result ^ i1 ^ (i2*decalage1) ^ (i3*decalage2) ^ (i4*decalage3);
                                                if (((x2 == heys_decrypt_2(x, tmp)) && (y2 == heys_decrypt_2(y, tmp)) && (z2 == heys_decrypt_2(z, tmp)))) {
                                                        return tmp;
                                                }
                                                /* si on a déjà identifié ce morceau de clef partielle, "no need to try" */
                                                if ((result & 0xf000) != 0) {
                                                        break;
                                                }
                                        }
                                        if ((result & 0x0f00) != 0) {
                                                break;
                                        }
                                }
                                if ((result & 0x00f0) != 0) {
                                        break;
                                }
                        }
                        if ((result & 0x000f) != 0) {
                                break;
                        }
                }
        }
        return -1;
}
