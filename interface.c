#include<stdlib.h>
#include<stdio.h>
#include"interface.h"
#include<string.h>

void print_titre(char* g){
	system("clear");
	for (int i = 0 ; i < 100 ; i++){
		(i%2==0) ? printf("*") : printf("-");
	}
	printf("\n");
	printf("                     %s                                    \n",g);
	for (int i = 0 ; i < 100 ; i++){
		(i%2==0) ? printf("*") : printf("-");
	}
	printf("\n");
}

void next(void){
	printf("Press enter to continue\n");
	char enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }
}


void separation(void){
	printf("***************\n***************\n***************\n");
}


