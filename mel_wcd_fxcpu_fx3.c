#include "mel_wcd.h"
#include <string.h>//memset
#include <stdlib.h>//free

/* returns: done stream size > 0, error =0. */
int mel_wcd_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wcdx)
{
	int done_size = 0;
	WCD_FXCPU_FX3U* wcd = (WCD_FXCPU_FX3U*)wcdx;

	if (enc_size > MEL_WCD_SUBHEADER_SIZE)
	{
		int body_size;

		body_size = (int)stream[0]|((int)stream[1]<<8);
		//[2-5]: 00 00 00 00
		//[6,7]: 20 08
		if (body_size > 0)
		{
			int i;
			unsigned char* ptr_codes = stream + MEL_WCD_SUBHEADER_SIZE;
			unsigned char* ptr_names;

			wcd->n_codes = (int)stream[8]|((int)stream[9]<<8);
			if (wcd->n_codes > 0)
			{
				wcd->codes = calloc(wcd->n_codes,sizeof(WCD_FX3U_DevRange));
				if(wcd->codes == 0) goto __exit_point;
			}			

			wcd->n_names = (enc_size - MEL_WCD_SUBHEADER_SIZE - wcd->n_codes*MEL_WCD_CODE_SIZE)/MEL_WCD_DEVICENAME_MAX_BYTES;
			if (wcd->n_names > 0)
			{
				wcd->names = calloc(wcd->n_names,sizeof(WCD_FX3U_DevName));
				if(wcd->names == 0) goto __exit_point;
			}

			for (i = 0; i < wcd->n_codes; i++)
			{
				wcd->codes[i].item.dtype	 = (unsigned int)ptr_codes[0]|((unsigned int)ptr_codes[1]<<8);

				wcd->codes[i].item.index =    (int)ptr_codes[2]
											|((int)ptr_codes[3]<<8)
											|((int)ptr_codes[4]<<16)
											|((int)ptr_codes[5]<<24);

				wcd->codes[i].count		 =    (int)ptr_codes[6]
											|((int)ptr_codes[7]<<8)
											|((int)ptr_codes[8]<<16)
											|((int)ptr_codes[9]<<24);

				ptr_codes += MEL_WCD_CODE_SIZE;
			}
			ptr_names = ptr_codes;
			for (i = 0; i < wcd->n_names; i++)
			{
				memcpy(wcd->names[i].raw,ptr_names,MEL_WCD_DEVICENAME_MAX_BYTES);
				wcd->names[i].raw[MEL_WCD_DEVICENAME_MAX_BYTES] = 0x00;
				ptr_names += MEL_WCD_DEVICENAME_MAX_BYTES;
			}

			done_size = enc_size;
		}
	}
__exit_point:
	return(done_size);
}
/* returns: done stream size > 0, error =0. */
int	mel_wcd_FXCPU_FX3_encoder(void* wcdx,unsigned char* stream)
{
	int done_size = 0;
	WCD_FXCPU_FX3U* wcd = (WCD_FXCPU_FX3U*)wcdx;

	if (wcd)
	{
		int body_size;
		int temp;

		body_size = wcd->n_codes*MEL_WCD_CODE_SIZE + MEL_WCD_SUBHEADER_SIZE;

		temp = body_size;
		stream[0] = temp&0xFF;
		stream[1] = (temp>>8)&0xFF;

		//[2-5]: 00 00 00 00
		//[6,7]: 20 08
		stream[2] = 0x00;
		stream[3] = 0x00;
		stream[4] = 0x00;
		stream[5] = 0x00;

		stream[6] = 0x20;
		stream[7] = 0x08;
		
		temp = wcd->n_codes;
		stream[8] = temp&0xFF;
		stream[9] = (temp>>8)&0xFF;

		if (wcd->n_codes > 0)
		{
			int i;
			unsigned char* ptr_codes = stream + MEL_WCD_SUBHEADER_SIZE;
			unsigned char* ptr_names;

			for (i = 0; i < wcd->n_codes; i++)
			{
				unsigned int temp1;
				int temp2;
				int temp3;

				temp1 = wcd->codes[i].item.dtype;
				ptr_codes[0] = temp1&0xFF;
				ptr_codes[1] = (temp1>>8)&0xFF;
								
				temp2 = wcd->codes[i].item.index;
				ptr_codes[2] = temp2&0xFF;
				ptr_codes[3] = (temp2>>8)&0xFF;
				ptr_codes[4] = (temp2>>16)&0xFF;
				ptr_codes[5] = (temp2>>24)&0xFF;

				temp3 = wcd->codes[i].count;
				ptr_codes[6] = temp3&0xFF;
				ptr_codes[7] = (temp3>>8)&0xFF;
				ptr_codes[8] = (temp3>>16)&0xFF;
				ptr_codes[9] = (temp3>>24)&0xFF;

				ptr_codes += MEL_WCD_CODE_SIZE;
			}
			ptr_names = ptr_codes;
			for (i = 0; i < wcd->n_names; i++)
			{
				memcpy(ptr_names,wcd->names[i].raw,MEL_WCD_DEVICENAME_MAX_BYTES);
				ptr_names += MEL_WCD_DEVICENAME_MAX_BYTES;
			}

			done_size = MEL_WCD_SUBHEADER_SIZE 
						+ wcd->n_codes*MEL_WCD_CODE_SIZE 
						+ wcd->n_names*MEL_WCD_DEVICENAME_MAX_BYTES;
		}
	}
	
	return(done_size);
}

/* destroy wcd instance */
void mel_wcd_FXCPU_FX3U_free(void* wcdx)
{
	WCD_FXCPU_FX3U* wcd = (WCD_FXCPU_FX3U*)wcdx;

	if (wcd)
	{
		if (wcd->codes)
		{
			free(wcd->codes);
		}

		if (wcd->names)
		{
			free(wcd->names);
		}

		free(wcd);
	}

	return;
}