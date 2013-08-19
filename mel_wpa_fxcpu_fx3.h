#ifndef __wpa_fxcpu_fx3u_h__
#define __wpa_fxcpu_fx3u_h__

#include <limits.h>
#include "mel_common.h"
#include "mel_wpg_device.h"

typedef struct tagWPA_FX3U_MemoryCapacity{
	
#define WPA_FX3U_MEMCAPACITY_2000	0x02
#define WPA_FX3U_MEMCAPACITY_4000	0x04
#define WPA_FX3U_MEMCAPACITY_8000	0x08//default
#define WPA_FX3U_MEMCAPACITY_16000	0x10
#define WPA_FX3U_MEMCAPACITY_32000	0x20
#define WPA_FX3U_MEMCAPACITY_64000	0x40	
	int	mem_capacity_in_steps;		
	int cmt_capacity_in_blocks;		//0...15
	int freg_capacity_in_blocks;	//0...14
	int SFBS_in_blocks;				//0 or 8
	int PIS_in_blocks;				//0 or 18
	int BCC_in_blocks;				//0 or 1
	
}WPA_FX3U_MemoryCapacity;

#if 0
int calc_SFM_capacity_in_steps		(WPA_FX3U_MemoryCapacity* mem);
int calc_Program_capacity_in_steps	(WPA_FX3U_MemoryCapacity* mem);
#endif //0

typedef struct tagWPA_FX3U_LatchedDeviceRange{

	int device_code;
	int latched_length;
	int latched_start;
	
}WPA_FX3U_LatchedDeviceRange;

typedef struct tagWPA_FX3U_Devices{

#define WPA_FX3U_MAX_EDITABLE_ITEMS 5
	int 					n_editable_items;
	WPA_FX3U_LatchedDeviceRange	editable_items[WPA_FX3U_MAX_EDITABLE_ITEMS];
	
}WPA_FX3U_Devices;

typedef struct tagWPA_FX3U_PLCName{

#define WPA_FX3U_PLCNAME_MAX_BYTES 32
	unsigned char raw[WPA_FX3U_PLCNAME_MAX_BYTES+2];

}WPA_FX3U_PLCName;

typedef struct tagWPA_FX3U_BCCStation{
#define WPA_FX3U_BuiltInCCLink_LTSetup_STATIONTYPE_None						0x00 //default = disabled
#define WPA_FX3U_BuiltInCCLink_LTSetup_STATIONTYPE_RemoteIOstation_In		0x01
#define WPA_FX3U_BuiltInCCLink_LTSetup_STATIONTYPE_RemoteIOstation_Out		0x02
#define WPA_FX3U_BuiltInCCLink_LTSetup_STATIONTYPE_RemoteIOstation_InOut		0x03
#define WPA_FX3U_BuiltInCCLink_LTSetup_STATIONTYPE_RemoteDeviceStation		0x04
	int type;
	
	int iopoints;		//1...16
	int is_reserve_st;	//no(=0), yes(=1);
	
}WPA_FX3U_BCCStation;

typedef struct tagWPA_FX3U_BuiltInCCLink_LTSetup{

//08 01 {EF FF} (0x108 = 264 bytes totally: 4 + 4 + 256)
#define WPA_FX3U_BuiltInCCLink_LTSetup_BAUDRATE_156kbps		0x00	//default
#define WPA_FX3U_BuiltInCCLink_LTSetup_BAUDRATE_625kbps		0x01
#define WPA_FX3U_BuiltInCCLink_LTSetup_BAUDRATE_2dot5Mbps	0x02
	int baudrate;
#define WPA_FX3U_BuiltInCCLink_LTSetup_POINTMODE_16bit		0x01	//default
#define WPA_FX3U_BuiltInCCLink_LTSetup_POINTMODE_4bit		0x00
	int point_mode;
#define WPA_FX3U_STATIONS_COUNT		64
	WPA_FX3U_BCCStation stations[WPA_FX3U_STATIONS_COUNT];

}WPA_FX3U_BuiltInCCLink_LTSetup;

typedef struct tagWPA_FX3U_IOAssignment{
	
	//max range: [0-255] -> [0-377]octal	
	int input_Start_offset;
	int input_Totally;
	
	//max range: [0-255] -> [0-377]octal
	int output_Start_offset;
	int output_Totally;
	
}WPA_FX3U_IOAssignment;

typedef struct tagWPA_FX3U_PLCSystem1{

	int is_battery_less_drive_mode;		//on(0x01)\off(0x00)
	
#define 	WPA_FX3U_PLCSYSTEM1_MODEM_None				0x00
#define 	WPA_FX3U_PLCSYSTEM1_MODEM_UserRegMode		0x01
#define 	WPA_FX3U_PLCSYSTEM1_MODEM_AIWA				0x02
#define 	WPA_FX3U_PLCSYSTEM1_MODEM_OMRON				0x03
#define 	WPA_FX3U_PLCSYSTEM1_MODEM_UserModeCH1		0x04
#define 	WPA_FX3U_PLCSYSTEM1_MODEM_UserModeCH2		0x05	
	int modem;
	
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_None			0x0000
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X000			0x0100
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X001			0x0101
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X002			0x0102
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X003			0x0103
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X004			0x0104
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X005			0x0105
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X006			0x0106
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X007			0x0107
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X010			0x0108
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X011			0x0109
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X012			0x010A
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X013			0x010B
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X014			0x010C
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X015			0x010D
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X016			0x010E
#define 	WPA_FX3U_PLCSYSTEM1_TERMINAL_X017			0x010F
	int terminal;

}WPA_FX3U_PLCSystem1;

typedef struct tagWPA_FX3U_PLCSystem2_CommChannel{

	int is_enabled;//=(.baudrate!=0)
//offset 0xC8
#define 	WPA_FX3U_PLCSYSTEM2_PROTOCOL_NoneProcedural	0x00
#define 	WPA_FX3U_PLCSYSTEM2_PROTOCOL_Dedicated 		0x01
	int protocol;
//offset 0xC9
#define 	WPA_FX3U_PLCSYSTEM2_DATABITS_7bit		0x00
#define 	WPA_FX3U_PLCSYSTEM2_DATABITS_8bit		0x01
	int databits;
//offset 0xCA
#define 	WPA_FX3U_PLCSYSTEM2_PARITY_None			0x00
#define 	WPA_FX3U_PLCSYSTEM2_PARITY_Odd			0x01
#define 	WPA_FX3U_PLCSYSTEM2_PARITY_Even			0x02
	int parity;
//offset 0xCB
#define 	WPA_FX3U_PLCSYSTEM2_STOPBITS_1bit		0x00
#define 	WPA_FX3U_PLCSYSTEM2_STOPBITS_2bit		0x01
	int stopbits;
//offset 0xCC
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_38400		0x0A
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_19200		0x09
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_9600		0x08
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_4800		0x07
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_2400		0x06
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_1200		0x05
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_600			0x04
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_300			0x03
#define 	WPA_FX3U_PLCSYSTEM2_BAUDRATE_CH_DESABLED	0x00
	int baudrate;
//offset 0xCD
	int is_header_on;			//(off/on): 00/01
//offset 0xCE	
	int is_terminator_on;		//(off/on): 00/01
//offset 0xCF
	int is_control_line_on;	//(off/on): 00/01
//offset 0xD0-0xD1
#define 	WPA_FX3U_PLCSYSTEM2_CNTRLINE_RegularRS232C		0x00	//Control line=00 (unlocked)
#define 	WPA_FX3U_PLCSYSTEM2_CNTRLINE_RS485				0x01	//Control line=01 (locked)
#define 	WPA_FX3U_PLCSYSTEM2_CNTRLINE_InterLinkRS232C		0x02	//Control line=00 (unlocked)
#define 	WPA_FX3U_PLCSYSTEM2_CNTRLINE_ModemRS232C			0x03	//Control line=01 (locked)
	int hw_type;
//offset 0xD2
	int is_sum_check_on;		//(off/on): 00/01
//offset 0xD3
#define 	WPA_FX3U_PLCSYSTEM2_TRANSMCNTRL_Form1_noCRLF	0x00
#define 	WPA_FX3U_PLCSYSTEM2_TRANSMCNTRL_Form4_byCRLF	0x01
	int transmission_cntrl;
//offset 0xD4
	int station_number;			//00...FF(ch1) or CCC(ch2)
//offset 0xD5
#define 	WPA_FX3U_PLCSYSTEM2_TIMEOUT_DISABLED			0x0000
#define 	WPA_FX3U_PLCSYSTEM2_TIMEOUT_10ms				0x0001	//min
#define 	WPA_FX3U_PLCSYSTEM2_TIMEOUT_2550ms			0x00FF	//ch1-max
#define 	WPA_FX3U_PLCSYSTEM2_TIMEOUT_32760ms			0x0CCC	//ch2-max
	int timeout_judge_time_x10ms;	

}WPA_FX3U_PLCSystem2_CommChannel;

typedef struct tagWPA_FX3U_PLCSystem2{
	
	WPA_FX3U_PLCSystem2_CommChannel ch1;
	WPA_FX3U_PLCSystem2_CommChannel ch2;

}WPA_FX3U_PLCSystem2;

/*
typedef struct tagWPA_FX3U_PosRecord{

#define WPA_FX3U_POSRECORD_INDEX_Y0 0
#define WPA_FX3U_POSRECORD_INDEX_Y1 1
#define WPA_FX3U_POSRECORD_INDEX_Y2 2
#define WPA_FX3U_POSRECORD_INDEX_Y3 3
	int y[4];
	
}WPA_FX3U_PosRecord;
*/
typedef struct tagWPA_FX3U_PosRecordRange{

	int min;
	int max;

}WPA_FX3U_PosRecordRange;

//WPA_FX3U_Positioning Limits
static WPA_FX3U_PosRecordRange RangOf_BiasSpeed_Hz 		= {0,INT_MAX};//1/10 or less of max speed
static WPA_FX3U_PosRecordRange RangOf_MaxSpeed_Hz  		= {10,200000};
static WPA_FX3U_PosRecordRange RangOf_CreepSpeed_Hz		= {10,32767};
static WPA_FX3U_PosRecordRange RangOf_ZeroReturnSpeed_Hz = {10,200000};
static WPA_FX3U_PosRecordRange RangOf_AccelerationTime_ms= {50,5000};
static WPA_FX3U_PosRecordRange RangOf_DecelerationTime_ms= {50,5000};

typedef struct tagWPA_FX3U_PosInstruction{
	
#define WPA_FX3U_POS_INSTR_TYPE_NONE 	0	//default
#define WPA_FX3U_POS_INSTR_TYPE_DDVIT	1
#define WPA_FX3U_POS_INSTR_TYPE_DPLSV	2
#define WPA_FX3U_POS_INSTR_TYPE_DDRVI	3
#define WPA_FX3U_POS_INSTR_TYPE_DDRVA	4
	int type;
	int pulse;		//+/-(1...999999)	default: ?
	int freq_Hz;	//10...200000		default: 10
	
}WPA_FX3U_PosInstruction;

typedef struct tagWPA_FX3U_PosOneYInstructions{
	
	int	BiasSpeed_Hz;
	int	MaxSpeed_Hz;
	int CreepSpeed_Hz;
	int	ZeroReturnSpeed_Hz;
	int	AccelerationTime_ms;
	int	DecelerationTime_ms;

#define WPA_FX3U_POSRECORD_INT_X0 0	//default[y0]
#define WPA_FX3U_POSRECORD_INT_X1 1	//default[y1]
#define WPA_FX3U_POSRECORD_INT_X2 2	//default[y2]
#define WPA_FX3U_POSRECORD_INT_X3 3	//default[y3]
#define WPA_FX3U_POSRECORD_INT_X4 4
#define WPA_FX3U_POSRECORD_INT_X5 5
#define WPA_FX3U_POSRECORD_INT_X6 6
#define WPA_FX3U_POSRECORD_INT_X7 7
#define WPA_FX3U_POSRECORD_INT_M864n 8
	int	InterrInputSource;

	PLCRegDevice rotation_dir_signal;
	PLCRegDevice head_address;	
#define WPA_FX3U_POS_INSTR_COUNT 100
	WPA_FX3U_PosInstruction instructions[WPA_FX3U_POS_INSTR_COUNT];		
	int dont_init_postable;//init: 0(default)/ don't init: 1.
	
}WPA_FX3U_PosOneYInstructions;

typedef struct tagWPA_FX3U_PosInstructionsIndividualSetup{

#define WPA_FX3U_POS_YTABS_COUNT 4
	WPA_FX3U_PosOneYInstructions ytabs[WPA_FX3U_POS_YTABS_COUNT];

}WPA_FX3U_PosInstructionsIndividualSetup;

typedef struct tagWPA_FX3U_Positioning{	

	WPA_FX3U_PosInstructionsIndividualSetup indivs;

}WPA_FX3U_Positioning;

//////////////////////////////////////////////////////////////////////////
//EXPORT DATA STRUCTURE
typedef struct tagWPA_FXCPU_FX3U{

	WPA_FX3U_MemoryCapacity page1;
	WPA_FX3U_Devices 		page2;
	WPA_FX3U_PLCName		page3;
	WPA_FX3U_IOAssignment	page4;
	WPA_FX3U_PLCSystem1		page5;
	WPA_FX3U_PLCSystem2		page6;
	WPA_FX3U_Positioning	page7;

	WPA_FX3U_BuiltInCCLink_LTSetup			page8;//if page1.BCC_in_blocks != 0
	WPA_FX3U_PosInstructionsIndividualSetup	page9;//if page1.PIS_in_blocks != 0

}WPA_FXCPU_FX3U;
//////////////////////////////////////////////////////////////////////////

/* returns: done stream size > 0, error =0. */
int mel_wpa_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wpax);
/* returns: done stream size > 0, error =0. */
int	mel_wpa_FXCPU_FX3_encoder(void* wpax,unsigned char* stream);
/* destroy wpa instance */
void mel_wpa_FXCPU_FX3U_free(void* wpax);

#endif //__wpa_fxcpu_fx3u_h__
