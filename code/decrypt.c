#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct _bitString bitString;

struct _bitString{
  int a[32];  
};

typedef struct _data data;

struct _data{
    int a0;
    int a1;
    int a2;
    int a3;
};
