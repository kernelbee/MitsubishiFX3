#include "mel_wcd.h"

#include <stdlib.h>//calloc
#include <string.h>//memset

static unsigned char WCD_HEADER[WPA_HEADER_SIZE_BYTES] = 
{
	0x40,0x00,0x22,0x01,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,

	0x20,0x20,0x20,0x20,0x04,0x02,0x08,0x02, 	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	/*							  [type,series]											*/
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static int		mel_enc_wcd_header	(int cpuSeries,int cpuType,unsigned char* stream);

/* returns: pointer to allocated (by malloc()) WCD_x structure; in case of failure: =0. */
void* mel_wcd_dec(unsigned char* stream,int enc_size,int* pcpuSeries,int* pcpuType,MELTools* tools)
{
	void *wcd = 0;
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

			//2. allocate appropriate WCD_x struct
			wcd = mel_wcd_allocate(cpuSeries,cpuType,tools);
			if (wcd)
			{
				int done_size;
				//3. decode all fields of WCD_x
				done_size = tools->wcd_decoder(	stream	+hdr_length,
												enc_size-hdr_length,
												wcd);
				//printf("done: %d\n",done_size);
				if (done_size != enc_size-hdr_length)
				{
					//error has happened during decoding: destroy wcd
					tools->wcd_destroy(wcd);
					wcd = 0;
				}				
				//else{/* successfully done */}
			}
			//else{/* unknown CPU */}
		}
		//else{/* wrong encoded value of header's size */}
	}
	//else{/* stream is too short */}

	return(wcd);
}
/* returns: encoded size >0, failure: =0. */
int mel_wcd_enc(int cpuSeries,int cpuType,void* wcd,unsigned char* stream,MELTools* tools)
{
	int enc_size = 0;

	if (wcd)
	{		
		int hdr_length;
		//encode header
		hdr_length = mel_enc_wcd_header(cpuSeries,cpuType,stream);
		//encode rest of file
		enc_size = tools->wcd_encoder(wcd,stream + hdr_length);
		if (enc_size > 0)
		{
			enc_size += hdr_length;
		}
	}

	return(enc_size);
}

void* mel_wcd_allocate(int cpuSeries,int cpuType,MELTools* tools)
{
	void* wcd = 0;

	if(mel_get_toolbox(cpuSeries,cpuType,tools))
	{
		wcd = calloc(1,tools->sizeof_wcdx);
	}

	return(wcd);
}

void	mel_wcd_free		(MELTools* tools,void* wcd)
{
	if(tools) if(tools->wcd_destroy) if(wcd) tools->wcd_destroy(wcd);
	return;
}

static int	mel_enc_wcd_header	(int cpuSeries,int cpuType,unsigned char* stream)
{
	unsigned char cpuTypeCode;

	//copy all
	memcpy(stream,WCD_HEADER,WPA_HEADER_SIZE_BYTES);
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
