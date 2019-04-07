#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "heys.h"
#include <stdbool.h>

int max_of_block(block_t*);

ckey_t part_key(int, sbox_t);
ckey_t part_key_2(int, sbox_t);
