#include<stdio.h>
#include<stdlib.h>
#include"heys.h"

void print_titre(char*);
void next(void);
void print_titre_next(char*);
void print_next(char*, ...);
void print_sbox(sbox_t *sbox);
void print_newlines(int);
void separation(void);
void print_proceeding(int iterations, int sleep);
void clear(void);

void get_int(int*);
void get_string(char*);
void get_char(char*);

void system_call(const char cmd[]);

void read_bytes(int fd, void *buf, size_t count);
void write_bytes(int fd, const void *buf, size_t count);
