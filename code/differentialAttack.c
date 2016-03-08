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

//Function used to convert integer to data
data convertToData(int number){
    data d;
    d.a0 = number%2;
    d.a1 = (int)(number/2)%2;
    d.a2 = (int)(number/4)%2;
    d.a3 = (int)(number/8)%2;
    return d;
}

//Function used to convert data to integer
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

//Function used to create randomness by XOR of key and input
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

//Function used to print bitstring
void printBitString(bitString b){
    int i;
    for(i=0;i<32;i++){
        printf("%d  ",b.a[i]);
    }
    printf("\n");
}

//Function used tp convert text to bitstring
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

//Function used to convert a given bit string to data using index
data bitStringToData(bitString k, int n){
    data d;
    d.a0 = k.a[n+3];
    d.a1 = k.a[n+2];
    d.a2 = k.a[n+1];
    d.a3 = k.a[n];
    return d;
}

//Function used for converting set of 8 integers to a bitstring
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

data XORdata(data a,data b){
    data c;
    if(a.a3==b.a3)
        c.a3=0;
    else 
        c.a3=1;
        
    if(a.a2==b.a2)
        c.a2=0;
    else 
        c.a2=1;
        
    if(a.a1==b.a1)
        c.a1=0;
    else 
        c.a1=1;
        
    if(a.a0==b.a0)
        c.a0=0;
    else 
        c.a0=1;
        
    return c;
}

int XORInt(int a,int b){
    if(a==b)
        return 0;
    else 
        return 1;
}

data complement(data d){
    data b;
    if(d.a3==0)
        b.a3=1;
    else
        b.a3=0;

    if(d.a2==0)
        b.a2=1;
    else
        b.a2=0;

    if(d.a1==0)
        b.a1=1;
    else
        b.a1=0;

    if(d.a0==0)
        b.a0=1;
    else
        b.a0=0;

    return b;
}

int matrixEntry(int a, int b,data* input, data* output){
    int results[16],i,count=0;
    data temp1 = convertToData(a);

    for(i=0;i<16;i++){
        data x = input[i];
        data y = output[i];
        data x1 = XORdata(x,temp1);
        int x1Num = converttoint(x1);
        data y1 = output[x1Num];
        data res = XORdata(y,y1);
        if(converttoint(res)==b)
            count++;        
        
    }
    return count;
}

int main(){
    printf("Please give the value of the sbox output\n");
    int sbox[16];
    int i,j;
    for(i=0;i<16;i++){
        scanf("%d",&sbox[i]);
        //printf("%d\t",sbox[i]);
    }
    
    data input[16];
    data output[16];
    for(i=0;i<16;i++){
        input[i] = convertToData(i);
        output[i] = convertToData(sbox[i]);
    }
    
    int diffMatrix[16][16];
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            diffMatrix[i][j]=matrixEntry(i,j,input,output);
            printf("%d\t",diffMatrix[i][j]);          
        }
        printf("\n");
    }
    printf("\n");   
    
}