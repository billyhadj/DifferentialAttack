#include<stdio.h>
#include<stdlib.h>
#include"choose_rand.h"
#include"part_key.h"
#include"brute_force.h"
#include"interface.h"
#include<time.h>
#include<string.h>
int main (int argc, char * argv []){

        print_titre(" DIFFERENTIAL CRYPTANALYSIS OF A HEYS'S FUNCTION : Preconditions ");
	next();
	printf("\n \nThe cryptosystem must be in the current repository, in a binary format.\n");
	next();
	printf("\n \nMake sur that the Sbox is not wrong.\n");
	next();
	print_titre(" DIFFERENTIAL CRYPTANALYSIS OF A HEYS'S FUNCTION : Files ");
	printf("Enter the cryptosystem you want to cryptanalyse, or 'examle' : \n");
	char * cryptosystem = malloc(sizeof(50)) ; 
	char * command_encrypt = malloc(sizeof(100)) ;
	scanf("%s",cryptosystem);
        if (strcmp(cryptosystem,"example")!=0) {
		command_encrypt=strcat(strcat("./",cryptosystem),"random_file > encrypted ");
	}else{	
		command_encrypt="./encrypt06 random_file > encrypted";
	}
	system("clear");
	for(int i = 0 ;  i<3 ; i++){
		system("echo Proceeding.");
		sleep(1);
		system("clear");
		system("echo Proceeding..");
		sleep(1);
		system("clear");
		system("echo Proceeding...");
		sleep(1);
		system("clear");
	}
	print_titre("THE CURRENT SBOX ");
	printf("Sbox : \n");
	for (int i = 0 ; i <= 15 ; i++){
		printf("%lx ",i);
	}
	printf("\n");
	for (int i = 0 ; i <= 15 ; i++){
		printf("%lx ",sbox[i]);
	}
	printf("\n");
	next();
	next();
	system("clear");
	print_titre("THE ARRAY OF DIFFERENCES BETWEEN INPUTS AND OUTPUTS FROM THE CURRENT SBOX ");
	int tab_diff[16][16] ;
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
	printf("\n\n\n\n\n\n\n\n");
 	int max = 0 ;
 	for (size_t i = 0 ; i < 16 ; i++){
 		for (size_t j = 0; j < 16; j++){
			printf("%d ",tab_diff[i][j]);
		}	
		printf("\n\033[0m");
	}
	next();
	system("clear");
	print_titre("DIFFERENTIAL CARACTERISTIC COMPUTING");
	system("evince atta* &");
	printf("Result : 0xb00\n");
	next();
	print_titre("THE BINARY FINLE WHICHC CONTAINS RANDOM COUPLES OF INTEGER 0xb00");
	int nb  ; 
	printf("How may couples would you generate ? \n");
	scanf("%d",&nb);
	rand_2(0xb00, nb);
	system("echo Generated files : && ls | grep random_file");
	printf("\n");
	next();
	system(command_encrypt);
	system("echo Ciphering done...");
	system("echo Generated files : && ls | grep encrypted");
	printf("\n");
	next();
	system("clear");
	print_titre(" PARTIAL KEY COMPUTING");
	int fd_rand= open("encrypted",O_RDONLY);
	ckey_t p_part = part_key(fd_rand, isbox);
	next();
	system("echo FIN DU CACLUL ");
	printf("--------------------------------\n");
	printf("The partial key is : %llx \n", p_part);
	printf("--------------------------------\n");
	separation();
	system("echo Press enter to launch the brute force attack");
	next();
	system("clear");
	system("echo BRUTE FORCE PROCESSING ...");
	ckey_t P_final = bruteforce(p_part);
	printf("\n\n\n\n\n------------------------\nTHE FINAL KEY : %llx\n-------------------------\n ", P_final);
	system("rm random_file encrypted && make clean && echo TEMPORARY FILES ARE DELETED SUCCESSFULY ");
	return EXIT_SUCCESS ;

}
