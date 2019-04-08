#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "heys.h"

/* type for encoding difference table */
/* 2D array linearized */
typedef byte_t difference_table_t[SBOX_SIZE*SBOX_SIZE];
void init_difference_table(difference_table_t *diff_table);
void print_difference_table(difference_table_t *diff_table);

#endif /* __UTILITY_H__ */
