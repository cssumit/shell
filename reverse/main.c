//Name: Sumit Mishra
//Branch: M.Tech CSE
//Roll No.: 201305600
#include<stdio.h>
#include"functions.h"
int main(int argc,char* argv[])
{
	if(argc!=2)
	  printf("You have to give one text file\n");//check the proper command line argument
	else
		enterIntoProgram(argv[1]);
	return 0;
}
