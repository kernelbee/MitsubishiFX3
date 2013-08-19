#ifndef __mel_wpg_fxcpu_fx3u_h__
#define __mel_wpg_fxcpu_fx3u_h__

#include "mel_common.h"
#include "mel_wpg_item.h"

#define MEL_WPG_SUBHEADER_SIZE 4

//////////////////////////////////////////////////////////////////////////
//EXPORT DATA STRUCTURE
typedef struct tagWPG_FXCPU_FX3U
{
	int magic_num0;
	int magic_num1;

	int steps;
	int nop_steps;

	WPG_Item* first;
	WPG_Item* last;
	
}WPG_FXCPU_FX3U;
//////////////////////////////////////////////////////////////////////////

/* returns: done stream size > 0, error =0. */
int mel_wpg_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wpgx);
/* returns: done stream size > 0, error =0. */
int mel_til_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wpgx,int maxsteps);
/* returns: done stream size > 0, error =0. */
int	mel_wpg_FXCPU_FX3_encoder(void* wpgx,unsigned char* stream,int maxsteps);
/* returns: done stream size > 0, error =0. */
int	mel_til_FXCPU_FX3_encoder(void* wpgx,unsigned char* stream);
/* destroy wpg instance */
void mel_wpg_FXCPU_FX3_free(void* wpgx);

/* returns: 1-ok,0-ng */
int	mel_wpg_FXCPU_FX3_link_item(void* wpgx,WPG_Item* item);
/* returns: 1-ok,0-ng */
int	mel_wpg_FXCPU_FX3_unlink_item(void* wpgx,WPG_Item* item);

#endif /* __mel_wpg_fxcpu_fx3u_h__ */
