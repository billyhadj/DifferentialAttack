#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

#include"heys.h"

ckey_t bruteforce(ckey_t P_key)
{
  block_t x, y, z, x2, y2, z2;
  int f, f2;

  if ( (f= open("./encrypted", O_RDONLY)) < 0 ) printf("le fichier ne peut être ouvert \n");

  read(f,&x,sizeof(x));
  read(f,&y,sizeof(y));
  read(f,&z,sizeof(z));
  close(f);


  if ( (f2= open("./random_file", O_RDONLY)) < 0 ) printf("le fichier ne peut être ouvert \n");

  read(f2,&x2,sizeof(x2));
  read(f2,&y2,sizeof(y2));
  read(f2,&z2,sizeof(z2));
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
  return -1;
}
