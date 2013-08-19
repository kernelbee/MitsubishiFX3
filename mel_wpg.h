#ifndef __mel_wpg_h__
#define __mel_wpg_h__

#include "mel_wpg_fxcpu_fx3.h"

//////////////////////////////////////////////////////////////////////////
/* 
	returns: pointer to allocated (by malloc()) WPG_x structure; in case of failure: =0. 
	Also, it calls internally mel_wpa_get_toolbox() to setup "tools" properly;
*/
void*	mel_wpg_allocate	(int cpuSeries,int cpuType,MELTools* tools);
void	mel_wpg_free		(MELTools* tools,void* wpg);

#define WPG_RPOGRAMM_MODE_LADDER	0
#define WPG_RPOGRAMM_MODE_SFC		1

/* returns: pointer to allocated (by malloc()) WPG_x structure; in case of failure: =0. */
void* mel_wpg_dec	(unsigned char* stream,int stream_size,int* pcpuSeries,int* pcpuType,int* pprgMode,MELTools* tools);
void* mel_til_dec	(unsigned char* stream,int stream_size,int* pcpuSeries,int* pcpuType,MELTools* tools,int maxsteps);

/* returns: encoded size >0, failure: =0. */
int mel_wpg_enc		(int cpuSeries,int cpuType,void* wpg,unsigned char* stream,int prgMode,MELTools* tools,int maxsteps);
int mel_til_enc		(int cpuSeries,int cpuType,void* wpg,unsigned char* stream,MELTools* tools);

#endif /* __mel_wpg_h__ */
