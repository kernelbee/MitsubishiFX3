#include <string.h>//memset
#include <stdio.h>//sprintf

#include "mel_common.h"
#include "mel_wpa_fxcpu_fx3.h"
#include "mel_wcd_fxcpu_fx3.h"
#include "mel_wpg_fxcpu_fx3.h"

#ifdef WIN32
#pragma warning (disable : 4996)
#endif

static const int MultiplyDeBruijnBitPosition2[32] = 
{
	0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
	31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

int	get_log2(unsigned int powerOf2)
{
	//http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
	return(MultiplyDeBruijnBitPosition2[(powerOf2 * 0x077CB531U) >> 27]);
}

int is_within_JIS8(unsigned char uc)
{
	int isin = 0;	

	//JIS8 http://www.ohsumap.ne.jp/lib/code/jis.htm

	if ( (uc >= 0x00 && uc <= 0x7F) || (uc >= 0xA0 && uc <= 0xDF) ) 
	{
		isin = 1;
	}

	return(isin);
}

int	mel_wpg_uint_sizeof(unsigned int number)
{
	int data_size = 1;

	if (number > (256-1))
	{
		if (number > (65536-1) )
		{
			if (number > (16777216-1))
			{
				data_size = 4;
			}
			else
			{
				data_size = 3;
			}
		}
		else{
			data_size = 2;
		}
	}

	return(data_size);
}

int	mel_get_toolbox	(int cpuSeries,int cpuType,MELTools* tools)
{	
	//set all fields to 0
	memset(tools,0x00,sizeof(*tools));

	switch((cpuSeries<<8)|cpuType)
	{
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0_S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0N):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FXU_FX2C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX2N_C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1N_S):
		break;

	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3U_C):
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3G_C):	
		tools->sizeof_wpax	= sizeof(WPA_FXCPU_FX3U);
		tools->wpa_decoder = &mel_wpa_FXCPU_FX3_decoder;
		tools->wpa_encoder = &mel_wpa_FXCPU_FX3_encoder;
		tools->wpa_destroy = &mel_wpa_FXCPU_FX3U_free;
		
		tools->sizeof_wcdx = sizeof(WCD_FXCPU_FX3U);
		tools->wcd_decoder = &mel_wcd_FXCPU_FX3_decoder;
		tools->wcd_encoder = &mel_wcd_FXCPU_FX3_encoder;
		tools->wcd_destroy = &mel_wcd_FXCPU_FX3U_free;
		
		tools->sizeof_wpgx = sizeof(WPG_FXCPU_FX3U);
		tools->wpg_decoder = &mel_wpg_FXCPU_FX3_decoder;
		tools->wpg_encoder = &mel_wpg_FXCPU_FX3_encoder;
		tools->til_decoder = &mel_til_FXCPU_FX3_decoder;
		tools->til_encoder = &mel_til_FXCPU_FX3_encoder;
		tools->wpg_destroy = &mel_wpg_FXCPU_FX3_free;						
		break;	
	}

	tools->cpuSeries	= cpuSeries;
	tools->cpuType		= cpuType;

	return(tools->sizeof_wpax);
}

char* mel_wpg_fxcpu_get_plctypename(int plctype)
{
	char* name = "UNKNOWN";

	switch(plctype)
	{
	case MEL_PLC_FXCPU_TYPE_FX3U_C: name = "FX3U/C"; break;
	case MEL_PLC_FXCPU_TYPE_FX3G_C:	name = "FX3G/C  "; break;
	}

	return(name);
}
