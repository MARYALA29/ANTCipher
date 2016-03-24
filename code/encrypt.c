#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

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
int* converttoText(bitString b);
int convertbintoDec(int a[]);

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
/*bitString randomness(char* in, char* key){
    bitString b;
    int i=0;
    for(i=0;i<32;i++){
        if(in[i]==key[i])
            b.a[i]=0;
        else
            b.a[i]=1;
    }
    return b;
}*/
bitString randomness(bitString in,bitString key)
{
    bitString b;
    int i=0;
    for(i=0;i<32;i++)
    {
        if(in.a[i]==key.a[i])
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

//Function used for applying substitution function using sboxes
bitString substitution(bitString k,bitString a){
    int i;
    int sbox1[16] = {0,3,5,8,6,9,12,7,13,10,14,4,1,15,11,2};
    int sbox2[16] = {0,3,5,8,6,12,11,7,9,14,10,13,15,2,1,4};
    int sbox3[16] = {0,3,5,8,6,10,15,4,14,13,9,2,1,7,12,11};
    int sbox4[16] = {0,3,5,8,6,12,11,7,10,4,9,14,15,1,2,13};
    
    int sbox5[16] = {0,15,11,8,12,9,6,3,13,1,2,4,10,7,5,14};
    //int sbox5[16] = {1,15,8,3,12,0,11,6,2,5,4,10,9,14,7,13};

    data m[8];int e[8];data d[8];
    for(i=0;i<8;i++){
        m[i]= bitStringToData(k,4*i);
        e[i]= sbox5[converttoint(m[i])];
        d[i]= convertToData(e[i]);
    }

    //BitString used to select the correct combination of sboxes    
    bitString b = intToBitstring(e[0],e[1],e[2],e[3],e[4],e[5],e[6],e[7]);

    data initial[8];//Divide the input bitstring into 8 parts of 4 bits each
    int middle[8];  //Pass the each 4-bit box into corresponding sbox and get the new output
    data final[8];  //Converting the integer to bitstring format

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

//Function to permutate using the statndard permutation for DES
bitString permutation(bitString b){
    int perm[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
    bitString ret;
    int i;
    for(i=0;i<32;i++){
        ret.a[perm[i]-1]= b.a[i];
    }
    return ret;
} 
bitString expansion(int main_key[],int round)
{
    bitString key;
    int r1,r2;
    int i;
    int order1[16] = {1,7,8,9,6,10,19,11,23,22,4,23,14,15,5,0};
    int order2[16] = {7,6,0,21,1,19,20,2,11,12,3,13,4,14,5,15};
    int order3[16] = {10,20,11,21,12,13,22,14,23,4,5,16,17,19,15,18};
    int order4[16] = {20,23,5,18,6,19,12,7,13,10,14,4,1,15,11,22};
    int order5[16] = {20,22,21,4,23,17,16,5,19,18,11,20,23,15,12,14};
    int order6[16] = {3,0,1,5,4,6,22,9,8,10,7,11,15,13,14,12};
    int order7[16] = {17,16,21,2,5,4,23,0,15,13,14,12,10,11,9,8};
    int order8[16] = {23,0,6,5,2,1,7,3,18,22,19,8,21,20,14,13};
    for(i=0;i<16;i++)
    {
        if(round==0)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order2[i]];
        }
        if(round==1)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order3[i]];
        }
        if(round==2)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order4[i]];
        }
        if(round==3)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order5[i]];
        }
        if(round==4)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order6[i]];
        }
        if(round==5)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order7[i]];
        }
        if(round==6)
        {
            key.a[i]=main_key[order1[i]];
            key.a[16+i]=main_key[24+order8[i]];
        }
        if(round==7)
        {
            key.a[i]=main_key[order2[i]];
            key.a[16+i]=main_key[24+order3[i]];
        }
        if(round==8)
        {
            key.a[i]=main_key[order2[i]];
            key.a[16+i]=main_key[24+order4[i]];
        }
        if(round==9)
        {
            key.a[i]=main_key[order2[i]];
            key.a[16+i]=main_key[24+order5[i]];
        }
        if(round==10)
        {
            key.a[i]=main_key[order2[i]];
            key.a[16+i]=main_key[24+order6[i]];
        }
        if(round==11)
        {
            key.a[i]=main_key[order2[i]];
            key.a[16+i]=main_key[24+order7[i]];
        }
        if(round==12)
        {
            key.a[i]=main_key[order2[i]];
            key.a[16+i]=main_key[24+order8[i]];
        }
        if(round==13)
        {
            key.a[i]=main_key[order3[i]];
            key.a[16+i]=main_key[24+order4[i]];
        }
        if(round==14)
        {
            key.a[i]=main_key[order3[i]];
            key.a[16+i]=main_key[24+order5[i]];
        }
        if(round==15)
        {
            key.a[i]=main_key[order3[i]];
            key.a[16+i]=main_key[24+order6[i]];
        }
        if(round==16)
        {
            key.a[i]=main_key[order3[i]];
            key.a[16+i]=main_key[24+order7[i]];
        }
        if(round==17)
        {
            key.a[i]=main_key[order3[i]];
            key.a[16+i]=main_key[24+order8[i]];
        }
        if(round==18)
        {
            key.a[i]=main_key[order4[i]];
            key.a[16+i]=main_key[24+order5[i]];
        }
        if(round==19)
        {
            key.a[i]=main_key[order4[i]];
            key.a[16+i]=main_key[24+order6[i]];
        }
        if(round==20)
        {
            key.a[i]=main_key[order4[i]];
            key.a[16+i]=main_key[24+order7[i]];
        }
    }
    return key;
}
void printkey(bitString key)
{
    int i;
    for(i=0;i<32;i++)
    {
        printf("%d",key.a[i]);
    }
    printf("\n");
}
bitString encrypt(bitString b,int k[])
{
    bitString a,c;
    bitString key;
    int i;
    i=1;
    //printf("KEY\n");
    key=expansion(k,i);
    //printkey(key);
    c=randomness(b,key);
    c=substitution(key,c);
    c=permutation(c);
    printf("ROUND%d\n",i);
    printkey(c);
    for(i=2;i<21;i++)
    {   
        key=expansion(k,i);
        //printkey(key);
        c=randomness(c,key);
        c=substitution(key,c);
        c=permutation(c);
        printf("ROUND%d\n",i);
        printkey(c);

    }
    return c;
}
bitString formSet(char *buffer,int index,int length)
{
    int array[8];
    int i,j;
    bitString new;
    int array_index=1;
    for(j=index;j<index+4;j++)
    {   
        int num;
        if(j<length)
        {   
            num=buffer[j];
            for (i = 0; i < 8; ++i) 
            {  
                array[i] = num & (1 << i) ? 1 : 0;
                new.a[(array_index*8)-1-i]=array[i];
            }
        }
        else
        {
            for(i=0;i<8;i++)
            {
                new.a[(array_index*8)-1-i]=0;
            }
        }
        array_index++;
    }
    
    for(j=0;j<32;j++)
    {
        printf("%d",new.a[j]);
    }
    printf("\n");
    return new;
}
void cbcEncrypt(int key[])
{   

    FILE *fileptr;
    char *buffer;
    long filelen;
    fileptr = fopen("try.txt", "rb"); 
    fseek(fileptr, 0, SEEK_END);          
    filelen = ftell(fileptr);            
    rewind(fileptr);                      

    buffer = (char *)malloc((filelen)*sizeof(char)); 
    fread(buffer, filelen, 1, fileptr); 
    fclose(fileptr);
    printf("length=%ld\n",filelen);
    bitString initialVector;
    int i;
    for(i=0;i<32;i++)
    {
        initialVector.a[i]=0;
    }
    int *res;

    bitString temp,encrypted,beforeEnc;
    int j;
    FILE *f;
    f = fopen("output3.txt", "w");

    for(i=0;i<filelen;i=i+4)
    {
        temp=formSet(buffer,i,filelen);
        if(i==0)
        {
            beforeEnc=randomness(temp,initialVector);
        }
        else
        {
            beforeEnc=randomness(temp,encrypted);
        }
        encrypted=encrypt(beforeEnc,key);
        res=converttoText(encrypted);
        for(j=0;j<4;j++)
        {
            printf("%d\n",res[j]);
            char tempchar;
            tempchar=(char)res[j];
            fputc(tempchar,f);
        }
    }
    printf("\n\n");
    for(i=0;i<32;i++)
    {
        printf("%d",encrypted.a[i]);
    }
    printf("\n\n");
    fclose(f);
}
int* converttoText(bitString b)
{
    int i,j;
    int* res=malloc(4*sizeof(int));
    int array[8];
    for(i=0;i<4;i++)
    {   
        int k,l;
        k=i*8;
        l=0;
        for(j=k;j<k+8;j++)
        {
            array[l]=b.a[j];
            l++;
        }
        res[i]=convertbintoDec(array);
    }
    return res;
}
int convertbintoDec(int a[])
{
    int i;
    int res=0;
    for(i=0;i<8;i++)
    {      
        if(a[7-i]==1)
        {
            res=res+pow(2,i);
        }
    }
    return res;
}


void main()
{
    int initialVector[48];
    int i;
    for(i=0;i<48;i++)
    {
        initialVector[i]=1;
    }
    cbcEncrypt(initialVector);
    
}
