#include "caracteristic.h"

static void
permute(diff_carac_tab_t res) {
        unsigned int cpt = NB_CARACTERISTICS;
        differential_caracterictic_t tmp;
        while (cpt != 0 && res[cpt].proba > res[cpt-1].proba) {
                tmp = res[cpt];
                res[cpt] = res[cpt-1];
                res[cpt-1] = tmp;
                cpt--;
        }
}

void
differential_caracteristic(diff_carac_tab_t res, fct_perm f, difference_table_t *diff_table) {
        const byte_t max = highest_value_difference_table(diff_table);
        const unsigned int proba_min_round = max*max*max*max;

        unsigned long long int proba;

        unsigned long long int proba_i, proba_j, proba_k;
        for (block_t i = 1; i != 0; i++) {
                for (block_t i2 = 1; i2 != 0; i2++) {
                        proba_i = (*diff_table)[(i & 0xf)*SBOX_SIZE + (i2 & 0xf)];
                        proba_i *= (*diff_table)[((i & 0xf0) >> 4)*SBOX_SIZE + ((i2 & 0xf0) >> 4)];
                        proba_i *= (*diff_table)[((i & 0xf00) >> 8)*SBOX_SIZE + ((i2 & 0xf00) >> 8)];
                        proba_i *= (*diff_table)[((i & 0xf000) >> 12)*SBOX_SIZE + ((i2 & 0xf000) >> 12)];
                        if (proba_i == 0 || proba_i < proba_min_round) {
                                continue;
                        }
                        block_t j = f(i2);
                        for (block_t j2 = 1; j2 != 0; j2++) {
                                proba_j = (*diff_table)[(j & 0xf)*SBOX_SIZE + (j2 & 0xf)];
                                proba_j *= (*diff_table)[((j & 0xf0) >> 4)*SBOX_SIZE + ((j2 & 0xf0) >> 4)];
                                proba_j *= (*diff_table)[((j & 0xf00) >> 8)*SBOX_SIZE + ((j2 & 0xf00) >> 8)];
                                proba_j *= (*diff_table)[((j & 0xf000) >> 12)*SBOX_SIZE + ((j2 & 0xf000) >> 12)];
                                if (proba_j == 0 || proba_j < proba_min_round) {
                                        continue;
                                }
                                block_t k = f(j2);
                                for (block_t k2 = 1; k2 != 0; k2++) {
                                        proba_k = (*diff_table)[(k & 0xf)*SBOX_SIZE + (k2 & 0xf)];
                                        proba_k *= (*diff_table)[((k & 0xf0) >> 4)*SBOX_SIZE + ((k2 & 0xf0) >> 4)];
                                        proba_k *= (*diff_table)[((k & 0xf00) >> 8)*SBOX_SIZE + ((k2 & 0xf00) >> 8)];
                                        proba_k *= (*diff_table)[((k & 0xf000) >> 12)*SBOX_SIZE + ((k2 & 0xf000) >> 12)];
                                        if (proba_k == 0 || proba_k < proba_min_round) {
                                                continue;
                                        }
                                        proba = proba_i * proba_j * proba_k;
                                        if (proba > res[NB_CARACTERISTICS].proba) {
                                                res[NB_CARACTERISTICS].input_diff = i;
                                                res[NB_CARACTERISTICS].output_diff = f(k2);
                                                res[NB_CARACTERISTICS].proba = proba;
                                                #ifdef PATH_INFO
                                                        res[NB_CARACTERISTICS].output_diff_round_1 = i2;
                                                        res[NB_CARACTERISTICS].input_diff_round_2 = j;
                                                        res[NB_CARACTERISTICS].output_diff_round_2 = j2;
                                                        res[NB_CARACTERISTICS].input_diff_round_3 = k;
                                                        res[NB_CARACTERISTICS].output_diff_round_3 = k2;
                                                #endif
                                                permute(res);
                                        }
                                }
                        }
                }
        }
}
