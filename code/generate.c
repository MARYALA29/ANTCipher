#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
void main()
{
	int i,n,x;
	n=pow(2,20);
	FILE *f;
    f = fopen("size20.txt", "w");
    char tempchar;
	for(i=0;i<n;i++)
	{	
		if(i%6==0)
		{
			tempchar='w';
		}	
		if(i%6==1)
		{
			tempchar='a';
		}
		if(i%6==2)
		{
			tempchar='s';
		}
		if(i%6==3)
		{
			tempchar='d';
		}
		if(i%6==4)
		{
			tempchar='g';
		}
		if(i%6==5)
		{
			tempchar='c';
		}
		fputc(tempchar,f);
	}
}