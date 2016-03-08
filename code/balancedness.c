#include <stdio.h>
#include <math.h>

typedef struct _data data;
typedef struct _sbox sbox;

struct _data
{
	int a0;
	int a1;
	int a2;
	int a3;
};

struct _sbox
{
	data results[16];
};

data convert(int number)
{
	data d;
	d.a0 = number%2;
	d.a1 = (int)(number/2)%2;
	d.a2 = (int)(number/4)%2;
	d.a3 = (int)(number/8)%2;
	return d;
}

sbox converttosbox(int* numbers)
{
	sbox s;
	data d;
	int i=0;
	for (i=0; i<16; i++)
	{
		d = convert(numbers[i]);
		s.results[i] = d;
	}
	return s;
}

void printdata(data d)
{
	printf("%d%d%d%d", d.a3, d.a2, d.a1, d.a0);
	return;
}

void printsbox(sbox s)
{
	printf("Input\t\t\tOutput\n");
	int i=0;
	for (i=0; i<16; i++)
	{
		printdata(convert(i));
		printf("\t\t\t");
		printdata(s.results[i]);
		printf("\n");
	}
}

void balancedness(sbox s)
{
	int num_of_zeros[4] = {0,0,0,0};
	
	data d;
	int i=0;
	for (i=0; i<16; i++)
	{
		d = s.results[i];
		if (d.a0 == 0)
			num_of_zeros[0]++;
		if (d.a1 == 0)
			num_of_zeros[1]++;
		if (d.a2 == 0)
			num_of_zeros[2]++;
		if (d.a3 == 0)
			num_of_zeros[3]++;
	}
	printf("The bias factors are %d, %d, %d, %d \n", num_of_zeros[3]-8, num_of_zeros[2]-8, num_of_zeros[1]-8, num_of_zeros[0]-8);
	printf("\n");
}

data exor(data d1, data d2)
{
	data d;
	
	if (d1.a0 == d2.a0)
		d.a0 = 0;
	else
		d.a0 = 1;
	if (d1.a1 == d2.a1)
		d.a1 = 0;
	else
		d.a1 = 1;
	if (d1.a2 == d2.a2)
		d.a2 = 0;
	else
		d.a2 = 1;
	if (d1.a3 == d2.a3)
		d.a3 = 0;
	else
		d.a3 = 1;

	return d;
}

int converttoint(data d)
{
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

void sac_alphawise(sbox s, data alpha)
{
	sbox s1;
	data temp1, temp2;
	int numtemp;
	
	int i=0;
	for (i=0; i<16; i++)
	{
		temp1 = convert(i);
		temp2 = exor(temp1, alpha);
		numtemp = converttoint(temp2);

		temp1 = exor(s.results[i], s.results[numtemp]);
		s1.results[i] = temp1;
	}
	balancedness(s1);
	return;
}

void sac(sbox s)
{
	data alpha;
	alpha.a1 = 0;
	alpha.a2 = 0;
	alpha.a3 = 0;
	alpha.a0 = 1;
	printf("alpha is: ");
	printdata(alpha);
	printf("\n");
	sac_alphawise(s, alpha);

	alpha.a0 = 0;
	alpha.a1 = 1;
	printf("alpha is: ");
	printdata(alpha);
	printf("\n");
	sac_alphawise(s, alpha);

	alpha.a1 = 0;
	alpha.a2 = 1;
	printf("alpha is: ");
	printdata(alpha);
	printf("\n");
	sac_alphawise(s, alpha);

	alpha.a2 = 0;
	alpha.a3 = 1;
	printf("alpha is: ");
	printdata(alpha);
	printf("\n");
	sac_alphawise(s, alpha);

	return;
}

void main()
{
	//int numbers[16] = {13,0,2,12,5,6,9,11,8,15,10,1,7,14,3,4};
//	int numbers[16] = {0,15,11,8,12,9,6,3,13,1,2,4,10,7,5,14};
	//int numbers[16] = {14,6,12,10,8,7,2,15,11,1,4,0,5,9,13,3};
	//int numbers[16] = {0,3,5,8,6,12,11,7,9,14,10,13,15,2,1,4};
	//int numbers[16] = {0,3,5,8,6,9,12,7,13,10,14,4,1,15,11,2};
	//int numbers[16] = {0,3,5,8,6,10,15,4,14,13,9,2,1,7,12,11};

	//int numbers[16]= {3,8,15,1,10,6,5,11,14,13,4,2,7,0,9,12};
	//int numbers[16] = {15,12,2,7,9,0,5,10,1,11,14,8,6,13,3,4};
	//int numbers[16]= {8,6,7,9,3,12,10,15,13,1,14,4,0,11,5,2};
	//int numbers[16] = {1,15,8,3,12,0,11,6,2,5,4,10,9,14,7,13};
    int numbers[16] = {0,3,5,8,6,9,12,7,13,10,14,4,1,15,11,2};
	sbox s;
	s = converttosbox(numbers);
	printsbox(s);

	balancedness(s);
	sac(s);
	/*data a;
	a = convert(14);
	printf("%d%d%d%d\n", a.a0,a.a1,a.a2,a.a3);*/
}