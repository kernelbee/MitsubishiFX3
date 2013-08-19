#include "mel_wpa.h"

#include <stdlib.h>//calloc
#include <string.h>//memset

static unsigned char WPA_HEADER[WPA_HEADER_SIZE_BYTES] = 
{
	0x40,0x00,0x22,0x01,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,

	0x20,0x20,0x20,0x20,0x04,0x02,0x08,0x02, 	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	/*							  [type,series]											*/
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static int		mel_enc_wpa_header	(int cpuSeries,int cpuType,unsigned char* stream);

/* returns: pointer to allocated (by malloc()) WPA_x structure; in case of failure: =0. */
void* mel_wpa_dec(unsigned char* stream,int enc_size,int* pcpuSeries,int* pcpuType,MELTools* tools)
{
	void *wpa = 0;
	//set initially as unknown
	*pcpuSeries = MEL_PLC_SERIES_NULL;
	*pcpuType	= MEL_PLC_TYPE_NULL;
	//check for too short streams
	if (enc_size > 0x40)
	{
		int hdr_length = (int)stream[0]|((int)stream[1]<<8);

		//0. check header
		if (hdr_length == WPA_HEADER_SIZE_BYTES)
		{			
			int cpuSeries;			
			int cpuType;
			unsigned char cpuTypeCode;
			
			//1. decode CPU Series & Type			
			cpuTypeCode	= stream[WPA_CPU_TYPE_OFFSET];
			cpuSeries	= stream[WPA_CPU_SERIES_OFFSET];

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
						
			//2. allocate appropriate WPA_x struct
			wpa = mel_wpa_allocate(cpuSeries,cpuType,tools);
			if (wpa)
			{
				int done_size;
				//3. decode all fields of WPA_x
				done_size = tools->wpa_decoder(	stream	+hdr_length,
												enc_size-hdr_length,
												wpa);
				//printf("done: %d\n",done_size);
				if (done_size != enc_size-hdr_length)
				{
					//error has happened during decoding: destroy wpa
					tools->wpa_destroy(wpa);
					wpa = 0;
				}				
				//else{/* successfully done */}
			}
			//else{/* unknown CPU */}
		}
		//else{/* wrong encoded value of header's size */}
	}
	//else{/* stream is too short */}

	return(wpa);
}
/* returns: encoded size >0, failure: =0. */
int mel_wpa_enc(int cpuSeries,int cpuType,void* wpa,unsigned char* stream,MELTools* tools)
{
	int enc_size = 0;

	if (wpa)
	{		
		//if(mel_set_wpa_toolbox(cpuSeries,cpuType,&toolbox))
		{
			int hdr_length;
			//encode header
			hdr_length = mel_enc_wpa_header(cpuSeries,cpuType,stream);
			//encode rest of file
			enc_size = tools->wpa_encoder(wpa,stream + hdr_length);
			if (enc_size > 0)
			{
				enc_size += hdr_length;
			}
		}
	}

	return(enc_size);
}

static void* mel_wpa_allocate(int cpuSeries,int cpuType,MELTools* tools)
{
	void* wpa = 0;
	
	if(mel_get_toolbox(cpuSeries,cpuType,tools))
	{
		wpa = calloc(1,tools->sizeof_wpax);
	}

	return(wpa);
}

void	mel_wpa_free		(MELTools* tools,void* wpa)
{
	if(tools) if(tools->wpa_destroy) if(wpa) tools->wpa_destroy(wpa);
	return;
}

static int	mel_enc_wpa_header	(int cpuSeries,int cpuType,unsigned char* stream)
{
	unsigned char cpuTypeCode;

	//copy all
	memcpy(stream,WPA_HEADER,WPA_HEADER_SIZE_BYTES);
	//tune CPU type & series
	
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