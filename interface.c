#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "interface.h"

#ifdef PERF
#define SKIP_NEXT 1
#endif

void print_titre(char* g){
  clear();
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
#ifdef SKIP_NEXT
  return;
#else
  char enter = 0;
  while (enter != '\r' && enter != '\n') { enter = getchar(); }
#endif
}

void separation(void){
  printf("***************\n***************\n***************\n");
}

void print_next(char *fmt, ...) {
  va_list args;
  printf(fmt, args);
  next();
}

void print_titre_next(char *g) {
  print_titre(g);
  next();
}

void print_sbox(sbox_t *sbox) {
  printf("Sbox : \n");

  for (int i = 0; i <= sizeof(sbox_t); i++) {
    printf("%x ",i);
  }
  printf("\n");

  for (int i = 0; i <= sizeof(sbox_t); i++) {
    printf("%x ",*sbox[i]);
  }
  printf("\n");

}

void print_newlines(int n) {
  assert(n >= 0);
  for (int i = 0; i < n; i++)
    printf("\n");
}

void print_proceeding(int iterations, int sleep_duration) {
  assert(iterations >= 0);

  const char *msg[] = {"echo Proceeding.", "echo Proceeding..", "echo Proceeding..."};
  
  clear();  
  for(int i = 0 ;  i < 3*iterations ; i++){
    system_call(msg[i%3]);
    sleep(sleep_duration);
    clear();
  }
  
}

void clear(void) {
#ifdef DEBUG
  return;
#else
  system("clear");
#endif
}

#ifdef DEBUG
void get_int(int *d) {
  int input_read = scanf("%d", d);
  if (input_read == EOF) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}
#else
void get_int(int *d) {
  scanf("%d", d);
}
#endif

#ifdef DEBUG
void get_string(char *str) {
  int input_read = scanf("%s", str);
  if (input_read == EOF) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}
#else
void get_string(char *str) {
  scanf("%s", str);
}
#endif

#ifdef DEBUG
void get_char(char *c) {
  int input_read = scanf("%c", c);
  if (input_read == EOF) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}
#else
inline void get_char(char *c) {
  scanf("%c", c);
}
#endif

#ifdef DEBUG
void system_call(const char cmd[]) {
  if (cmd == NULL) {
    fprintf(stderr, "Error with system call: Command is NULL\n");
    exit(EXIT_FAILURE);
  }

  int status = system(cmd);
  fprintf(stderr, "%s\n", cmd);
  
  if (status == -1 ) {
    fprintf(stderr, "Error with system call: Child process cannot be created\n");
    exit(EXIT_FAILURE);
  } else if (status == 127) {
    fprintf(stderr, "Error with system call: Shell cannot executed %s in child process\n", cmd);
    exit(EXIT_FAILURE);
  }

}
#else
inline void system_call(const char cmd[]) {
  system(cmd);
}
#endif

void print_filename_of_fd(int fd) {
  char *get_filename_cmd = (char*)malloc(128*sizeof(char));
  sprintf(get_filename_cmd, "ls -la /proc/%d/fd | cut -d' ' -f12", fd);
  system_call(get_filename_cmd);  
}

#ifdef DEBUG
void read_bytes(int fd, void *buf, size_t count) {
  int bytes_read = read(fd, buf, count);
  /* Only check error status but the number of bytes read could */
  /* be lower than requested  */
  if (bytes_read < -1) {
    fprintf(stderr, "Error while reading:\n");
    print_filename_of_fd(fd);
    fprintf(stderr, "%s\n", strerror(errno));
  }
}
#else
inline void read_bytes(int fd, void *buf, size_t count) {
  read(fd, buf, count);
}
#endif

#ifdef DEBUG
void write_bytes(int fd, const void *buf, size_t count) {
  int bytes_written = write(fd, buf, count);
  /* Only check error status but the number of bytes written could */
  /* be lower than requested  */
  if (bytes_written < -1) {
    fprintf(stderr, "Error while writting:\n");
    print_filename_of_fd(fd);
    fprintf(stderr, "%s\n", strerror(errno));
  }
}
#else
inline void write_bytes(int fd, const void *buf, size_t count) {
  write(fd, buf, count);
}
#endif
