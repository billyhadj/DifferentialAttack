#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "heys.h"



//Fonction qui retourne l'indice de la valeur max d'un tableau de blocks donné. renvoie un int.
int max_of_block (block_t *b_r)
{
	block_t m=b_r[0];
	int r=0;
	for(int i=0 ;i<256;i++)
	{
		if(b_r[i]>m)
		{
			m=b_r[i];
			r=i;
		}
	}
	return r;
}


ckey_t part_key(int f, sbox_t the_box)
{
	char rep = 'd';
	while (rep != 'y' && rep != 'n'){
		printf("Voulez vous afficher les clefs au fur et a mesure ? y/n ");
		scanf("%c",&rep);
	}
	
	uint16_t keys[256] = {0}; // Nous déclarons un tableau de clés
	block_t b_r[256] = {0}; // Nous prenons un tableau de 256 blocks initialisé à 0
	block_t a1,b1, a2, b2; // Nous déclarons 4 variables temporaires
	if (rep == 'y'){
		for (int j=0; j<256; j++)
		{	
			uint16_t k = (j & 0xf);
			k ^= ((j & 0xf0) <<4);

			if (lseek(f, 0 ,SEEK_SET) < 0) printf("Le fichier ne peut pas être réinisialisé \n");

			for (int i=0 ;i<5000;i++)
			{	
				read(f,&a1,sizeof(a1));
				read(f,&b1,sizeof(b1));

				a1 = a1 ^ k;
				b1 = b1 ^ k;

				a2=heys_subst(a1 ,the_box);
				b2=heys_subst(b1 ,the_box);

				if((a2 ^ b2)==0x0606) {b_r[j]++;	printf("%04x : %d \n", k, b_r[j]);}
			}	
			keys[j] = k;
		}
	}
	if (rep == 'n'){
		for (int j=0; j<256; j++)
                {
                        uint16_t k = (j & 0xf);
                        k ^= ((j & 0xf0) <<4);

                        if (lseek(f, 0 ,SEEK_SET) < 0) printf("Le fichier ne peut pas être réinisialisé \n");

                        for (int i=0 ;i<5000;i++)
                        {
                                read(f,&a1,sizeof(a1));
                                read(f,&b1,sizeof(b1));

                                a1 = a1 ^ k;
                                b1 = b1 ^ k;

                                a2=heys_subst(a1 ,the_box);
                                b2=heys_subst(b1 ,the_box);

                                if((a2 ^ b2)==0x0606) {b_r[j]++;}
                        }
                        keys[j] = k;
                }
        }

	return keys[max_of_block(b_r)];
}
