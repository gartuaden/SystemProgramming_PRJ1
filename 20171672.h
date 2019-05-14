#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_INPUT_SIZE 500
#define MEGA_BYTE 1024*1024
#define MEGA_BYTE_ROW 256*256
#define MAX_ARR_SIZE 100
#define HASHSIZE 20


typedef struct NODE{

  char command[10];
  struct NODE* link;
}NODE;


typedef struct HASH{
    int id,count;
    char code[10];
    char name[10];
    struct HASH* next;
}HASH;

struct HASH* hashtable[HASHSIZE];


