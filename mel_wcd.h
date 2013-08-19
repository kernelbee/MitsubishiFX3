#ifndef __mel_wcd_h__
#define __mel_wcd_h__

#include "mel_wcd_fxcpu_fx3.h"

/* 
	returns: pointer to allocated (by malloc()) WCD_x structure; in case of failure: =0. 
	Also, it calls internally mel_wpa_get_toolbox() to setup "tools" properly;
*/
void*	mel_wcd_allocate	(int cpuSeries,int cpuType,MELTools* tools);
void	mel_wcd_free		(MELTools* tools,void* wcd);

/* returns: pointer to allocated (by malloc()) WCD_x structure; in case of failure: =0. */
void* mel_wcd_dec	(unsigned char* stream,int enc_size,int* pcpuSeries,int* pcpuType,MELTools* tools);

/* returns: encoded size >0, failure: =0. */
int mel_wcd_enc		(int cpuSeries,int cpuType,void* wcd,unsigned char* stream,MELTools* tools);


#endif /* __mel_wcd_h__ */
