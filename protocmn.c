#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "protocmn.h"

int find_code_offset(unsigned char* stream,int stream_length,unsigned char code)
{
	int is_exist = CODE_NOT_EXIST;//by default
	int i;		
	for (i = 0; i < stream_length; i++)
	{
		if (stream[i]==code)
		{
			is_exist = i;
			break;
		}
	}	
	return(is_exist);
}

void printf_buf_hex(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{
		printf("%02x ",buf[i]);
	}
	//printf("\n");
	//printf("\n\t\t\t\t\t\t");

	return;
}

void printf_buf_dec(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{
		printf("%03d ",buf[i]);
	}
	printf("\n");

	return;
}

void printf_buf_hex_16(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{		
		if(!(i%16)) printf("\n");

		printf("%02x ",buf[i]);		
	}
	printf("\n");

	return;
}

void printf_buf_hex_16c(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s = \n{",header);
	for (i = 0; i<size; i++)
	{		
		if(!(i%16)) printf("\n");

		printf("0x%02x, ",buf[i]);
	}
	printf("}\n");

	return;
}


void printf_buf_ascii_16(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{		
		if(!(i%16)) printf("\n");

		if(isgraph(buf[i])) printf("%2c ",buf[i]);
		else				printf("%02x ",buf[i]);
	}
	printf("\n");

	return;
}

void printf_buf_ascii_16fx(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{		
		if(!(i%16)) printf("\n");

		if(isgraph(buf[i])) printf("%c",buf[i]);
		else				printf(".");
	}
	printf("\n");

	return;
}

void printf_buf_ascii_32fx(unsigned char* buf,int size,char* header)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{		
		if(!(i%32)) printf("\n");

		if(isgraph(buf[i])) printf("%c",buf[i]);
		else				printf("[%02X]",buf[i]);
	}
	printf("\n");

	return;
}

int is_pascal_string(unsigned char* stream,char* dstbuf,int* plength)
{
	int retval = 0;

	//1.check strlen
	if (stream[0] > 0)
	{
		int i;
		int strlength = stream[0];

		if ((strlength > 2) && (strlength < 256) )
		{
			retval = 1;

			//1.check ending null
			if (stream[strlength]==0)
			{
				//2.check every even byte
				for (i = 0; i < strlength-1; i+=2)
				{
					char ascii_ch;
					char ascii_ch2;

					ascii_ch = stream[i+1];
					ascii_ch2 = stream[i+1+1];

					if ( (ascii_ch<0x20) || (ascii_ch>0x7e) || (ascii_ch2!=0) )
					//if ( (ascii_ch<9) || (ascii_ch>0x7e) || (ascii_ch2!=0) )
					{
						retval = 0;
						//printf("\t\t%02x, %02x, %d, %02x: range!\n",stream[0],stream[strlength],strlength,ascii_ch);
						break;
					}

					//special case: " "
					if (strlength == 3)
					{
						if (ascii_ch == 0x20)
						{
							retval = 0;							
							break;							
						}
					}

					dstbuf[i/2] = stream[i+1];
				}		
				if (retval)
				{
					dstbuf[strlength/2] = 0;
					*plength = strlength+1;
				}
			}
			else
			{
				retval = 0;
				//printf("\t\t%02x, %02x, %d	ending null!\n",stream[0],stream[strlength],strlength);
			}
		}
	}

	return(retval);
}

int enc_pascal_string(char* text,int textsize,unsigned char* stream)
{
	int encsize;
	int i;

	encsize = 2*textsize+1;

	stream[0] = encsize;
	for (i = 0; i < textsize; i++)
	{
		stream[1+2*i+0] = text[i];
		stream[1+2*i+1] = 0x00;
	}
	stream[encsize++] = 0x00;

	return(encsize);
}

void printf_buf_hex_N(unsigned char* buf,int size,char* header,int N)
{
	int i;

	if(header) printf("%s",header);
	for (i = 0; i<size; i++)
	{		
		if(!(i%N)) printf("\n");
		
		if(!(i%8)) printf("|");
		printf("%02x ",buf[i]);		
	}
	printf("\n");

	return;
}
