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

data convertToData(int number){
    data d;
    d.a0 = number%2;
    d.a1 = (int)(number/2)%2;
    d.a2 = (int)(number/4)%2;
    d.a3 = (int)(number/8)%2;
    return d;
}

int converttoint(data d){
    int number=0;

    if (d.a0 == 1)
        number+=1;
    if (d.a1 == 1)
        number+=2;
    if (d.a2 == 1)
        number+=4;
    if (d.a3 == 1)
        number+=8;
    
    return number;
}

bitString randomness(char* in, char* key){
    bitString b;
    int i=0;
    for(i=0;i<32;i++){
        if(in[i]==key[i])
            b.a[i]=0;
        else
            b.a[i]=1;
    }
    return b;
}

void printBitString(bitString b){
    int i;
    for(i=0;i<32;i++){
        printf("%d  ",b.a[i]);
    }
    printf("\n");
}

bitString convert(char* str){
    bitString b;
    int i;
    for(i=0;i<32;i++){
        if(str[i]=='1')
            b.a[i]=1;
        else
            b.a[i]=0;
    }
    return b;
}

data bitStringToData(bitString k, int n){
    data d;
    d.a0 = k.a[n+3];
    d.a1 = k.a[n+2];
    d.a2 = k.a[n+1];
    d.a3 = k.a[n];
    return d;
}

bitString intToBitstring(int e1,int e2,int e3,int e4,int e5,int e6,int e7,int e8){
    data d[8];
    d[0] = convertToData(e1);
    d[1] = convertToData(e2);
    d[2] = convertToData(e3);
    d[3] = convertToData(e4);
    d[4] = convertToData(e5);
    d[5] = convertToData(e6);
    d[6] = convertToData(e7);
    d[7] = convertToData(e8);

    bitString b;
    int i,j;
    for(i=0;i<8;i++){
        b.a[4*i]   = d[i].a3;
        b.a[4*i+1] = d[i].a2;
        b.a[4*i+2] = d[i].a1;
        b.a[4*i+3] = d[i].a0;
    }

    return b;
}

bitString substitution(bitString k,bitString a){
    int i;
    int sbox1[16] = {0,3,5,8,6,9,12,7,13,10,14,4,1,15,11,2};
    int sbox2[16] = {0,3,5,8,6,12,11,7,9,14,10,13,15,2,1,4};
    int sbox3[16] = {0,3,5,8,6,10,15,4,14,13,9,2,1,7,12,11};
    int sbox4[16] = {0,3,5,8,6,12,11,7,10,4,9,14,15,1,2,13};
    
    int sbox5[16] = {0,15,11,8,12,9,6,3,13,1,2,4,10,7,5,14};
    //    int sbox5[16] = {1,15,8,3,12,0,11,6,2,5,4,10,9,14,7,13};

    data m[8];int e[8];data d[8];
    for(i=0;i<8;i++){
        m[i]= bitStringToData(k,4*i);
        e[i]= sbox5[converttoint(m[i])];
        d[i]= convertToData(e[i]);
    }

    bitString b = intToBitstring(e[0],e[1],e[2],e[3],e[4],e[5],e[6],e[7]);
    //   printBitString(b);

    data initial[8];
    int middle[8];
    data final[8];

    for(i=0;i<8;i++){
        initial[i] = bitStringToData(a,4*i);
        if(d[i].a3==0&&d[i].a2==0)
            middle[i]=sbox1[converttoint(initial[i])];
        if(d[i].a3==0&&d[i].a2==1)
            middle[i]=sbox2[converttoint(initial[i])];
        if(d[i].a3==1&&d[i].a2==0)
            middle[i]=sbox3[converttoint(initial[i])];
        if(d[i].a3==1&&d[i].a2==1)
            middle[i]=sbox4[converttoint(initial[i])];
        final[i]= convertToData(middle[i]);
    }

    bitString ret = intToBitstring(middle[0],middle[1],middle[2],middle[3],middle[4],middle[5],middle[6],middle[7]);
    return ret;
}

bitString permutation(bitString b){
    int perm[32] = {16,7,20,21,29,12,18,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
    bitString ret;
    int i;
    for(i=0;i<32;i++){
        ret.a[perm[i]-1]= b.a[i];
    }
    return ret;
} 

void main(){
    printf("Please give the input string to encrypt\n");
    char in[32];
    scanf("%s",in);
    printf("Please give the key string\n");
    char key[32];
    scanf("%s",key);

    bitString k = convert(key);
    bitString input = convert(in);

    bitString a = randomness(in,key);
    printBitString(a);

    printf("randomness done\n");
    bitString b = substitution(k,input);
    printBitString(b);

    bitString b1;
    int i;
    for(i=0;i<19;i++){
        b1 = substitution(k,b);
        b=b1;
        printBitString(b);
    }

    printf("permutation layer done\n" );
    bitString c = permutation(b);
    printBitString(c);
}
