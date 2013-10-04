#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"functions.h"
//#define buffer_Length (1024*1024*5) //give the buffer size 5MB
void enterIntoProgram(char* input_File) //backbone of this program.
{
	int input_File_Descriptor=open(input_File,O_RDWR);//open the input file here.
	int buffer_Length=0;
	if(input_File_Descriptor<0)//check for the proper input file opening.
	{
		printf("Error:Input file not open properly\n");
		return;
	}
	long int input_File_Length=lseek(input_File_Descriptor,0,SEEK_END);//get the input file length.
	printf("Your File Size===%ld\n",input_File_Length);
	printf("Enter the Buffer_Length [not more than your file size]===");
	scanf("%d",&buffer_Length);
	if(buffer_Length>input_File_Length)
	{
		printf("Error:Your buffer_length is greater than file size.\n");
		return;
	}
	int no_of_Buffer=(input_File_Length/buffer_Length);//this line tell how many buffer will created.
	int remaining_Char=input_File_Length%buffer_Length;// rest of the characters .
	int index=0;
	char* string_for_Buffer=malloc(buffer_Length);//string which contains buffer.
	char* string_for_Rest=malloc(remaining_Char);// sting which contains rest of the char.
	int output_File_Descriptor=open(concatFileName("reverse_",input_File),O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR);
	if(output_File_Descriptor<0)
	{
		printf("Error:Output file not open properly\n");
		return;
	}
	lseek(input_File_Descriptor,-1,SEEK_END);
	for(index=1;index<=no_of_Buffer;index++)
	{
		lseek(input_File_Descriptor,-(buffer_Length*index),SEEK_END);//set the seek to the end and catch the character from end till buffer lengths
		read(input_File_Descriptor,string_for_Buffer,buffer_Length);
		write(output_File_Descriptor,reverseTheContent(string_for_Buffer,buffer_Length),buffer_Length);// write the reverse of the string of buffer from starting of the string.
	}	
	//deal with the rest of the characters here.
	lseek(input_File_Descriptor,-(buffer_Length*(index-1))-remaining_Char,SEEK_END);
	read(input_File_Descriptor,string_for_Rest,remaining_Char);
	write(output_File_Descriptor,reverseTheContent(string_for_Rest,remaining_Char),remaining_Char);
	close(input_File_Descriptor);
	close(output_File_Descriptor);
	free(string_for_Buffer);
	free(string_for_Rest);
	return;
}
char* reverseTheContent(char* input_String,int string_Length)//this is the function to reverse the content of the string.
{
	char temp;
	int index;
	for(index=0;index<string_Length/2;index++)
	{
		temp=input_String[index];
		input_String[index]=input_String[string_Length-index-1];
		input_String[string_Length-index-1]=temp;
	}
	return input_String;
}
char* concatFileName(char* first_String,char* second_String) //this is use for convert file name input to reverse_input
{
	char* third_String=malloc(getLength(first_String)+getLength(second_String));
	int index_1=0,index_2=0;
	while(first_String[index_1]!='\0')
	{
		third_String[index_1]=first_String[index_1];
		index_1++;
	}
	while(second_String[index_2]!='\0')
	{
		third_String[index_1]=second_String[index_2];
		index_2++;
		index_1++;
	}
	third_String[index_1]='\0';
	return third_String;
}
int getLength(char* string)  // this function take the length of the string.
{
	int counter=0;
	while(string[counter]!='\0')
	{
		counter++;
	}
	return counter;
}			

