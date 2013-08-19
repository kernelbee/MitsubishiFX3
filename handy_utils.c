#include "handy_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int compare_buffers(void* buf1,int buf1_size,void* buf2,int buf2_size)
{
	int n_diffs = 1;
	unsigned char* b1 = buf1;
	unsigned char* b2 = buf2;

	if (buf1_size == buf2_size)
	{
		if (memcmp(b1,b2,buf1_size))
		{
#if 1
			int j;

			for (j = 0 ; j < buf1_size; j++)
			{	
				if(!(j%16))
				{
					printf("\n%04X\t",j);
				}

				if (b1[j]!=b2[j])
				{
					n_diffs++;

					printf(" [%02X->%02X] ",b1[j],b2[j]);
				}
				else
				{
					printf("%02X ",b1[j]);
				}				
			}

			printf("\n");
#endif
		}
		else
		{
			//printf("SUCCESS\n");
			n_diffs = 0;
		}
	}
	else
	{
		printf("DIFFERENT ENC/DEC SIZES!!!: %d != %d\n", buf1_size,buf2_size);
	}
	
	return(n_diffs);
}
