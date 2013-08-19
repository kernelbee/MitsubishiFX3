#ifndef __mel_common_h__
#define __mel_common_h__

//////////////////////////////////////////////////////////////////////////
//PLC SERIES

#define MEL_PLC_SERIES_NULL					0x00

#define MEL_PLC_SERIES_QCPU_Qmode			MEL_PLC_SERIES_NULL
#define MEL_PLC_SERIES_QCPU_QmodeRemoteIO	0x03
#define MEL_PLC_SERIES_QCPU_Amode			0x01
#define MEL_PLC_SERIES_LCPU					MEL_PLC_SERIES_NULL
#define MEL_PLC_SERIES_QSCPU				MEL_PLC_SERIES_NULL
#define MEL_PLC_SERIES_QnACPU				MEL_PLC_SERIES_NULL
#define MEL_PLC_SERIES_ACPU					0x01
#define MEL_PLC_SERIES_MOTION_SCPU			0x06
#define MEL_PLC_SERIES_FXCPU				0x02
#define MEL_PLC_SERIES_CNC					MEL_PLC_SERIES_NULL

//////////////////////////////////////////////////////////////////////////
//PLC TYPE

#define MEL_PLC_TYPE_NULL						0x00

#define MEL_PLC_QCPU_Qmode_TYPE_Q00J			0x30
#define MEL_PLC_QCPU_Qmode_TYPE_Q00				0x31
#define MEL_PLC_QCPU_Qmode_TYPE_Q01				0x32
#define MEL_PLC_QCPU_Qmode_TYPE_Q01U			0x82
#define MEL_PLC_QCPU_Qmode_TYPE_Q00UJ			0x80
#define MEL_PLC_QCPU_Qmode_TYPE_Q02H			0x22
#define MEL_PLC_QCPU_Qmode_TYPE_Q02PH			0x45
#define MEL_PLC_QCPU_Qmode_TYPE_Q02U			0x83
#define MEL_PLC_QCPU_Qmode_TYPE_Q03UD			0x70
#define MEL_PLC_QCPU_Qmode_TYPE_Q03UDE			0x90
#define MEL_PLC_QCPU_Qmode_TYPE_Q04UDH			0x71
#define MEL_PLC_QCPU_Qmode_TYPE_Q04UDEH			0x91
#define MEL_PLC_QCPU_Qmode_TYPE_Q06H			0x23
#define MEL_PLC_QCPU_Qmode_TYPE_Q06PH			0x46
#define MEL_PLC_QCPU_Qmode_TYPE_Q06UDH			0x72
#define MEL_PLC_QCPU_Qmode_TYPE_Q06UDEH			0x92
#define MEL_PLC_QCPU_Qmode_TYPE_Q10UDH			0x75
#define MEL_PLC_QCPU_Qmode_TYPE_Q10UDEH			0x95
#define MEL_PLC_QCPU_Qmode_TYPE_Q12H			0x24
#define MEL_PLC_QCPU_Qmode_TYPE_Q12PH			0x41
#define MEL_PLC_QCPU_Qmode_TYPE_Q12PRH			0x43
#define MEL_PLC_QCPU_Qmode_TYPE_Q13UDH			0x73
#define MEL_PLC_QCPU_Qmode_TYPE_Q13UDEH			0x93
#define MEL_PLC_QCPU_Qmode_TYPE_Q20UDH			0x76
#define MEL_PLC_QCPU_Qmode_TYPE_Q20UDEH			0x96
#define MEL_PLC_QCPU_Qmode_TYPE_Q25H			0x25
#define MEL_PLC_QCPU_Qmode_TYPE_Q25PH			0x42
#define MEL_PLC_QCPU_Qmode_TYPE_Q25PRH			0x44
#define MEL_PLC_QCPU_Qmode_TYPE_Q26UDH			0x74
#define MEL_PLC_QCPU_Qmode_TYPE_Q26UDEH			0x94
#define MEL_PLC_QCPU_Qmode_TYPE_RemoteIO		0x04

#define MEL_PLC_QCPU_Amode_TYPE_Q02H_A			0x41
#define MEL_PLC_QCPU_Amode_TYPE_Q06H_A			0x42

#define MEL_PLC_LCPU_TYPE_L02					0xA1
#define MEL_PLC_LCPU_TYPE_L26BT					0xA2

#define MEL_PLC_QSCPU_TYPE_QS001				0x60

#define MEL_PLC_QnACPU_TYPE_Q2A					0x11
#define MEL_PLC_QnACPU_TYPE_Q2AS_H				MEL_PLC_QnACPU_TYPE_Q2A
#define MEL_PLC_QnACPU_TYPE_Q2AS1				0x12
#define MEL_PLC_QnACPU_TYPE_Q2AS_H_S1			MEL_PLC_QnACPU_TYPE_Q2AS1
#define MEL_PLC_QnACPU_TYPE_Q3A					0x13
#define MEL_PLC_QnACPU_TYPE_Q4A					0x14
#define MEL_PLC_QnACPU_TYPE_Q4AR				MEL_PLC_QnACPU_TYPE_Q4A

#define MEL_PLC_CNC_TYPE_Q4A					MEL_PLC_QnACPU_TYPE_Q4A

#define MEL_PLC_ACPU_TYPE_A0JH					0x02
#define MEL_PLC_ACPU_TYPE_A1FX					0x03
#define MEL_PLC_ACPU_TYPE_A1S_S1				0x04
#define MEL_PLC_ACPU_TYPE_A1SJ					MEL_PLC_ACPU_TYPE_A1S_S1
#define MEL_PLC_ACPU_TYPE_A1SH					0x05
#define MEL_PLC_ACPU_TYPE_A1SJH					MEL_PLC_ACPU_TYPE_A1SH
#define MEL_PLC_ACPU_TYPE_A1N					0x06
#define MEL_PLC_ACPU_TYPE_A2C					0x07
#define MEL_PLC_ACPU_TYPE_A2CJ					MEL_PLC_ACPU_TYPE_A2C
#define MEL_PLC_ACPU_TYPE_A2N_S1				0x08
#define MEL_PLC_ACPU_TYPE_A2S_S1				MEL_PLC_ACPU_TYPE_A2N_S1
#define MEL_PLC_ACPU_TYPE_A2SH_S1				0x09
#define MEL_PLC_ACPU_TYPE_A3N					0x0A
#define MEL_PLC_ACPU_TYPE_A2A_S1				0x0C
#define MEL_PLC_ACPU_TYPE_A3A					0x0D
#define MEL_PLC_ACPU_TYPE_A2U_S1				0x0E
#define MEL_PLC_ACPU_TYPE_A2US_S1				MEL_PLC_ACPU_TYPE_A2U_S1
#define MEL_PLC_ACPU_TYPE_A2AS_S1				MEL_PLC_ACPU_TYPE_A2U_S1
#define MEL_PLC_ACPU_TYPE_A2USH_S1				0x0F
#define MEL_PLC_ACPU_TYPE_A2AS_S30				0x10
#define MEL_PLC_ACPU_TYPE_A2AS_S60				MEL_PLC_ACPU_TYPE_A2AS_S30
#define MEL_PLC_ACPU_TYPE_A3U					MEL_PLC_ACPU_TYPE_A2AS_S30
#define MEL_PLC_ACPU_TYPE_A4U					0x11

#define MEL_PLC_MOTION_SCPU_TYPE_A171SH			0x01
#define MEL_PLC_MOTION_SCPU_TYPE_A172SH			0x02
#define MEL_PLC_MOTION_SCPU_TYPE_A273UH_S3		0x03
#define MEL_PLC_MOTION_SCPU_TYPE_A173UH_S1		0x04

typedef enum tagFXCPU_Types
{
MEL_PLC_FXCPU_TYPE_FX3U_C = 0,
MEL_PLC_FXCPU_TYPE_FX3G_C,
MEL_PLC_FXCPU_TYPE_FX1N_S,
MEL_PLC_FXCPU_TYPE_FX1S,
MEL_PLC_FXCPU_TYPE_FX2N_C,
MEL_PLC_FXCPU_TYPE_FXU_FX2C,
MEL_PLC_FXCPU_TYPE_FX1,
MEL_PLC_FXCPU_TYPE_FX0N,
MEL_PLC_FXCPU_TYPE_FX0_S,
MEL_PLC_FXCPU_TYPE_LAST
}FXCPU_Types;

char* mel_wpg_fxcpu_get_plctypename(int plctype);

#define MEL_PLC_FXCPU_TYPE_CODE_FX0_S				0x01
#define MEL_PLC_FXCPU_TYPE_CODE_FX0N				0x02
#define MEL_PLC_FXCPU_TYPE_CODE_FX1					0x03
#define MEL_PLC_FXCPU_TYPE_CODE_FXU_FX2C			0x04
#define MEL_PLC_FXCPU_TYPE_CODE_FX2N_C				0x05
#define MEL_PLC_FXCPU_TYPE_CODE_FX1S				0x06
#define MEL_PLC_FXCPU_TYPE_CODE_FX1N_S				0x07
#define MEL_PLC_FXCPU_TYPE_CODE_FX3U_C				0x08
#define MEL_PLC_FXCPU_TYPE_CODE_FX3G				0x09

static unsigned char FXCPU_TYPE_CODES_by_type[MEL_PLC_FXCPU_TYPE_LAST] = 
{
	MEL_PLC_FXCPU_TYPE_CODE_FX3U_C,
	MEL_PLC_FXCPU_TYPE_CODE_FX3G,
	MEL_PLC_FXCPU_TYPE_CODE_FX1N_S,
	MEL_PLC_FXCPU_TYPE_CODE_FX1S,
	MEL_PLC_FXCPU_TYPE_CODE_FX2N_C,
	MEL_PLC_FXCPU_TYPE_CODE_FXU_FX2C,
	MEL_PLC_FXCPU_TYPE_CODE_FX1,
	MEL_PLC_FXCPU_TYPE_CODE_FX0N,
	MEL_PLC_FXCPU_TYPE_CODE_FX0_S
};
static unsigned char FXCPU_TYPES_by_code[256] = 
{
	 0,
	 MEL_PLC_FXCPU_TYPE_FX0_S,
	 MEL_PLC_FXCPU_TYPE_FX0N,
	 MEL_PLC_FXCPU_TYPE_FX1,
	 MEL_PLC_FXCPU_TYPE_FXU_FX2C,
	 MEL_PLC_FXCPU_TYPE_FX2N_C,
	 MEL_PLC_FXCPU_TYPE_FX1S,
	 MEL_PLC_FXCPU_TYPE_FX1N_S,
	 MEL_PLC_FXCPU_TYPE_FX3U_C,
	 MEL_PLC_FXCPU_TYPE_FX3G_C,
	 0
};
//////////////////////////////////////////////////////////////////////////

#define WPA_HEADER_SIZE_BYTES		64 //0x40
#define WPG_SFC_SUBHEADER_SIZE_BYTES	48 //0x30

#define WPA_CPU_TYPE_OFFSET			0x26
#define WPA_CPU_SERIES_OFFSET		0x27
#define WPG_PROGRAMM_MODE_OFFSET	0x2A
/*
char*	mnemonic;			//D,M,X,Y,...
int		base;				//number of first device
int		total;//[PLC model]	//amount of all these devices
*/

#define WPA_CODE_MEMORY_CAPACITY	0xE0FF
#define WPA_CODE_CMNT_CAPACITY		0x16FF
#define WPA_CODE_DEVICES			0x0120
#define WPA_CODE_PLCNAME			0x0100
#define WPA_CODE_EOF				0xFFFF

#define WPG_CODE_STATEMENT		0x80
#define WPG_CODE_NOTE			0x82
#define WPG_CODE_LABEL			0x3C

typedef struct tagMELTools
{
	int cpuSeries;
	int cpuType;
	
	int sizeof_wpax;
	/* returns: done stream size > 0, error =0. */
	int		(*wpa_decoder)(unsigned char* stream,int enc_size,void* wpax);
	/* returns: done stream size > 0, error =0. */
	int		(*wpa_encoder)(void* wpax,unsigned char* stream);
	void	(*wpa_destroy)(void* wpax);

	int sizeof_wcdx;
	/* returns: done stream size > 0, error =0. */
	int		(*wcd_decoder)(unsigned char* stream,int enc_size,void* wcdx);
	/* returns: done stream size > 0, error =0. */
	int		(*wcd_encoder)(void* wcdx,unsigned char* stream);
	void	(*wcd_destroy)(void* wcdx);
	
	int sizeof_wpgx;
	/* returns: done stream size > 0, error =0. */
	int		(*wpg_decoder)(unsigned char* stream,int enc_size,void* wpgx);
	int		(*til_decoder)(unsigned char* stream,int enc_size,void* wpgx,int maxsteps);
	/* returns: done stream size > 0, error =0. */
	int		(*wpg_encoder)(void* wpgx,unsigned char* stream,int maxsteps);
	int		(*til_encoder)(void* wpgx,unsigned char* stream);
	void	(*wpg_destroy)(void* wpgx);

}MELTools;

/* 
	Please, use it to setup "tools" properly.
	returns: required size of wpax instance, bytes; 0 - failure;	
*/
int		mel_get_toolbox	(int cpuSeries,int cpuType,MELTools* tools);
/* To check character */
int		is_within_JIS8(unsigned char uc);

int		get_log2(unsigned int powerOf2);

int		mel_wpg_uint_sizeof		(unsigned int number);

#define MEL_WPG_MAX_STATEMENT_TEXT_SIZE	64
#define MEL_WPG_MAX_NOTE_TEXT_SIZE		32

#define MEL_WPG_MIN_TIL_SIZE			5	//END + (CR,LF/LF,CR)
#define MEL_WPG_MIN_INSTR_TEXT_SIZE		2	//*x, EI,DI, etc.
#define MEL_WPG_MAX_INSTR_TEXT_SIZE		256	//???

#endif //__mel_common_h__
