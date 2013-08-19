#ifndef __mel_wcd_fxcpu_fx3u_h__
#define __mel_wcd_fxcpu_fx3u_h__

#include "mel_common.h"
#include "mel_wpg_device.h"

#define MEL_WCD_SUBHEADER_SIZE 10
#define MEL_WCD_CODE_SIZE 10

typedef struct tagWCD_FX3U_DevRange{

	PLCRegDevice	item;
	int				count;

}WCD_FX3U_DevRange;

typedef struct tagWCD_FX3U_DevName{

#define MEL_WCD_DEVICENAME_MAX_BYTES (40)
	unsigned char raw[MEL_WCD_DEVICENAME_MAX_BYTES+1];

}WCD_FX3U_DevName;

//////////////////////////////////////////////////////////////////////////
//EXPORT DATA STRUCTURE
typedef struct tagWCD_FXCPU_FX3U{

	int n_codes;
	int n_names;

	WCD_FX3U_DevRange*	codes;
	WCD_FX3U_DevName*	names;

}WCD_FXCPU_FX3U;
//////////////////////////////////////////////////////////////////////////

/* returns: done stream size > 0, error =0. */
int mel_wcd_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wcdx);
/* returns: done stream size > 0, error =0. */
int	mel_wcd_FXCPU_FX3_encoder(void* wcdx,unsigned char* stream);
/* destroy wcd instance */
void mel_wcd_FXCPU_FX3U_free(void* wcdx);

#endif /* __mel_wcd_fxcpu_fx3u_h__ */
