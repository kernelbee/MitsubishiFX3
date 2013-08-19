#ifndef __mel_wpa_h__
#define __mel_wpa_h__

#include "mel_wpa_fxcpu_fx3.h"

/* 
	returns: pointer to allocated (by malloc()) WPA_x structure; in case of failure: =0. 
	Also, it calls internally mel_wpa_get_toolbox() to setup "tools" properly;
*/
void*	mel_wpa_allocate	(int cpuSeries,int cpuType,MELTools* tools);
void	mel_wpa_free		(MELTools* tools,void* wpa);

/* returns: pointer to allocated (by malloc()) WPA_x structure; in case of failure: =0. */
void* mel_wpa_dec	(unsigned char* stream,int enc_size,int* pcpuSeries,int* pcpuType,MELTools* tools);

/* returns: encoded size >0, failure: =0. */
int mel_wpa_enc		(int cpuSeries,int cpuType,void* wpa,unsigned char* stream,MELTools* tools);

#endif //__mel_wpa_h__
