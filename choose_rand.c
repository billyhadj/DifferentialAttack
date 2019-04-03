#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "choose_rand.h"



void rand_2 (int difference, int nombre)
{
	int s= open("/dev/urandom", O_RDONLY);
	block_t a ;
	block_t b ;
        int s2 = open("random_file",O_CREAT | O_WRONLY, 00666);	
	for(int i=0 ; i<nombre;i++){
	
		read(s,&a,sizeof(a));
		b=a^difference; 
		write(s2,&a, sizeof(a));
		write(s2,&b,sizeof(b));
	}
	close(s);
}

