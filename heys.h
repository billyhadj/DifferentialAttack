#ifndef HEYS_H
#define HEYS_H

#define VERBOSE 0

#include <stdio.h>
#include <stdlib.h>
/* déclarations posix pour les types d'entiers */
#include <stdint.h>
#include<time.h>
#include<fcntl.h>
#include<unistd.h>
// Le chiffrement

typedef uint8_t byte_t;
typedef uint16_t block_t;
typedef uint16_t rkey_t;
//typedef uint32_t ckey_t;
typedef uint64_t ckey_t;

typedef byte_t sbox_t[16];

#define NROUNDS 4
#define NKEYS 5
typedef rkey_t skey_t[NKEYS];

// #define CIPHERKEYSIZE 32
#define CIPHERKEYSIZE 40
#define ROUNDKEYSIZE (8 * sizeof(rkey_t))
#define BLOCKSIZE (8 * sizeof(block_t))

sbox_t sbox;
sbox_t isbox;
block_t heys_perm(block_t);
block_t heys_subst(block_t, sbox_t);
int heys_key_schedule(ckey_t, skey_t);
block_t heys_encrypt(block_t, ckey_t);
block_t heys_decrypt(block_t, ckey_t);
void genere(int [5000][2], int, char*);


/*
int  heys_encrypts(char *data, char *data, ckey_t);
block_t heys_decrypts(char *data, char *data, ckey_t);
*/



#endif /* HEYS_H */
