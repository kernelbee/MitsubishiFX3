#include "mel_wpg.h"
#include "mel_wpg_item.h"
#include <stdio.h>//printf
#include <stdlib.h>//calloc
#include <string.h>//memset

static unsigned char WPG_HEADER_LADDER[WPA_HEADER_SIZE_BYTES] = 
{
	0x40,0x00,0x22,0x01,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
						/* file comment (32 bytes max) */
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x04,0x02,0x08,0x02, 	0x03,0x03,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,
	/*							  [dtype,series]											*/
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static unsigned char WPG_HEADER_SFC[WPA_HEADER_SIZE_BYTES] = 
{
	0x40,0x00,0x22,0x01,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x04,0x02,0x08,0x02, 	0x03,0x03,0x00,0x04,0x05,0x00,0x00,0xFF,
	/*							  [dtype,series]	          [LADDER or SFC]				*/
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static unsigned char WPG_SUBHEADER_SFC[WPG_SFC_SUBHEADER_SIZE_BYTES] = 
{
	0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x01,0x80,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x04,0x80,0xFF,0xFF,	0xFF,0xFF,0x00,0x00,0x05,0x80,0x00,0x00,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,	0xFF,0xFF,0x06,0x00,0x00,0x80,0x00,0x00
};

static int	mel_enc_wpg_header	(int cpuSeries,int cpuType,unsigned char* stream);

/* returns: pointer to allocated (by malloc()) WPG_x structure; in case of failure: =0. */
void* mel_wpg_dec	(unsigned char* stream,int stream_size,int* pcpuSeries,int* pcpuType,int* pprgMode,MELTools* tools)
{
	void *wpg = 0;
	//set initially as unknown
	*pcpuSeries = MEL_PLC_SERIES_NULL;
	*pcpuType	= MEL_PLC_TYPE_NULL;
	*pprgMode	= WPG_RPOGRAMM_MODE_LADDER;
	//check for too short streams
	if (stream_size > 0x40)
	{
		int hdr_length = (int)stream[0]|((int)stream[1]<<8);

		//0. check header
		if (hdr_length == WPA_HEADER_SIZE_BYTES)
		{			
			int cpuSeries;
			int cpuType;
			int prgMode;

			unsigned char cpuTypeCode;

			//1. decode CPU Series & Type			
			cpuTypeCode	= stream[WPA_CPU_TYPE_OFFSET];
			cpuSeries	= stream[WPA_CPU_SERIES_OFFSET];
			prgMode		= (stream[WPG_PROGRAMM_MODE_OFFSET] != 0x00)? WPG_RPOGRAMM_MODE_LADDER:WPG_RPOGRAMM_MODE_SFC;

			if (cpuSeries == MEL_PLC_SERIES_FXCPU)
			{
				cpuType = FXCPU_TYPES_by_code[cpuTypeCode];
			}
			else
			{
				cpuType = cpuTypeCode;
			}

			*pcpuSeries = cpuSeries;
			*pcpuType	= cpuType;
			*pprgMode	= prgMode;

			if (prgMode == WPG_RPOGRAMM_MODE_LADDER)
			{
				//2. allocate appropriate WPG_x struct
				wpg = mel_wpg_allocate(cpuSeries,cpuType,tools);
				if (wpg)
				{
					int done_size = 0;

					//3. decode all fields of WPG_x
					done_size += tools->wpg_decoder(stream		+hdr_length,
						stream_size	-hdr_length,
						wpg);
					//printf("done: %d\n",done_size);
#if 0				
					if (done_size != stream_size-hdr_length)
					{
						printf("done_size (%d) != stream_size-hdr_length (%d)\n",done_size,stream_size-hdr_length);
						//error has happened during decoding: destroy wpg
						tools->wpg_destroy(wpg);
						wpg = 0;
					}				
					//else{/* successfully done */}
#endif 			
				}
				//else{/* unknown CPU or programm mode/format */}
			}
		}
		//else{/* wrong encoded value of header's size */}
	}
	//else{/* stream is too short */}

	return(wpg);
}

void* mel_til_dec	(unsigned char* stream,int stream_size,int* pcpuSeries,int* pcpuType,MELTools* tools,int maxsteps)
{
	void *wpg = 0;
	//set initially as unknown
	//*pcpuSeries = MEL_PLC_SERIES_NULL;
	//*pcpuType	= MEL_PLC_TYPE_NULL;
	//check for too short streams
	//TODO	
	{	
		int cpuSeries;
		int cpuType;
				
		//1. decode CPU Series & Type			
		//TODO
		
		cpuSeries	= MEL_PLC_SERIES_FXCPU;
		cpuType		= MEL_PLC_FXCPU_TYPE_FX3U_C;

		*pcpuSeries = cpuSeries;
		*pcpuType	= cpuType;

		//2. allocate appropriate WPG_x struct
		wpg = mel_wpg_allocate(cpuSeries,cpuType,tools);
		if (wpg)
		{
			int done_size;
			//3. decode all fields of WPG_x
			done_size = tools->til_decoder(stream,stream_size,wpg,maxsteps);
			//printf("done: %d\n",done_size);
#if 0				
			if (done_size != stream_size)
			{
				printf("done_size (%d) != stream_size(%d)\n",done_size,stream_size);
				//error has happened during decoding: destroy wpg
				tools->wpg_destroy(wpg);
				wpg = 0;
			}				
			//else{/* successfully done */}
#endif 			
		}
		//else{/* unknown CPU */}
	}
	
	return(wpg);
}

/* returns: encoded size >0, failure: =0. */
int mel_wpg_enc		(int cpuSeries,int cpuType,void* wpg,unsigned char* stream,int prgMode,MELTools* tools,int maxsteps)
{
	int enc_size = 0;

	if (prgMode == WPG_RPOGRAMM_MODE_LADDER)
	{
		if (wpg)
		{		
			int hdr_length;
			//encode header
			hdr_length = mel_enc_wpg_header(cpuSeries,cpuType,stream);
			//encode rest of file
			enc_size = tools->wpg_encoder(wpg,stream + hdr_length,maxsteps);
			if (enc_size > 0)
			{
				enc_size += hdr_length;
			}
		}
	}

	return(enc_size);
}
/* returns: encoded size >0, failure: =0. */
int mel_til_enc		(int cpuSeries,int cpuType,void* wpg,unsigned char* stream,MELTools* tools)
{
	int enc_size = 0;

	if (wpg)
	{		
		enc_size = tools->til_encoder(wpg,stream);
	}

	return(enc_size);
}

/* 
	returns: pointer to allocated (by malloc()) WPG_x structure; in case of failure: =0. 
	Also, it calls internally mel_wpa_get_toolbox() to setup "tools" properly;
*/
void*	mel_wpg_allocate	(int cpuSeries,int cpuType,MELTools* tools)
{
	void* wpg = 0;

	if(mel_get_toolbox(cpuSeries,cpuType,tools))
	{
		wpg = calloc(1,tools->sizeof_wpgx);
	}

	return(wpg);
}

void	mel_wpg_free		(MELTools* tools,void* wpg)
{
	if(tools) if(tools->wpg_destroy) if(wpg) tools->wpg_destroy(wpg);
	return;
}

static int	mel_enc_wpg_header	(int cpuSeries,int cpuType,unsigned char* stream)
{
	unsigned char cpuTypeCode;
	//copy all
	memcpy(stream,WPG_HEADER_LADDER,WPA_HEADER_SIZE_BYTES);
	//tune CPU dtype & series
	if (cpuSeries == MEL_PLC_SERIES_FXCPU)
	{
		cpuTypeCode = FXCPU_TYPE_CODES_by_type[cpuType];
	}
	else
	{
		cpuTypeCode = cpuType&0xFF;
	}

	stream[WPA_CPU_TYPE_OFFSET]		= cpuTypeCode;
	stream[WPA_CPU_SERIES_OFFSET]	= cpuSeries	&0xFF;

	return(WPA_HEADER_SIZE_BYTES);
}
