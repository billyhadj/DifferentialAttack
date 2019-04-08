#ifndef CARACTERISTIQUE_H
#define CARACTERISTIQUE_H

#include "heys.h"
#include "utility.h"

#define NB_CARACTERISTICS 10

typedef block_t(*fct_perm)(block_t);


#define MORE_INFO 0
#if MORE_INFO == 0
#define PATH_INFO
typedef struct {
        block_t input_diff;
        block_t output_diff;
        unsigned long long int proba;
        block_t output_diff_round_1;
        block_t input_diff_round_2;
        block_t output_diff_round_2;
        block_t input_diff_round_3;
        block_t output_diff_round_3;
} differential_caracterictic_t;
#else
typedef struct {
        block_t input_diff;
        block_t output_diff;
        unsigned long long int proba;
} differential_caracterictic_t;
#endif

typedef differential_caracterictic_t diff_carac_tab_t[NB_CARACTERISTICS + 1];

void differential_caracteristic(diff_carac_tab_t, fct_perm, byte_t[16][16]);

#endif /* CARACTERISTIQUE_H */
