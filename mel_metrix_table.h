#ifndef __mel_metrix_table_h__
#define __mel_metrix_table_h__

#include "mel_wpg_tables.h"

typedef struct tagWPGMetrixOperandRecord
{
	char* txt_usage;
	char* txt_operands[2/*MEL_PLC_FXCPU_TYPE_LAST*/];
	//char* explanation;

}WPGMetrixOperandRecord;

typedef struct tagWPGMetrixRecord
{
	WPGMetrixOperandRecord operands[MEL_INSTRUCT_OPERANDS_MAX];

}WPGMetrixRecord;

//Totally: 528 instructions; 209 done (40 %)
static WPGMetrixRecord WPG_METRIX[MEL_WPG_OpID_TOTALLY] =
{
	{ //"LD" 

			"S",	{	"XYMrTCSb",
						"XYMrTCS"},	0},
	{ //"LDI" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"LDP" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"LDF" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"AND" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ANI" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ANDP" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ANDF" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"OR" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ORI" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ORP" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ORF" 

			"S",	{	"XYMrTCSb",
						"XYMrTCSb"},	0},
	{ //"ANB" 
			0	},
	{ //"ORB" 
			0	},
	{ //"MPS" 
			0	},
	{ //"MRD" 
			0	},
	{ //"MPP" 
			0	},
	{ //"INV" 
			0	},
	{ //"MEP" 
			0	},
	{ //"MEF" 
			0	},
	{ //"OUT" 

			"SDP",	{	"YMrSb",
						"YMrS"},	0},
	{ //"OUT" 

			"D",	{	"TC",
						"TC"},	

			"SP",	{	"DRK",
						"DRK"},	0},
	{ //"SET" 

			"SDP",	{	"YMrTCSbxymstcDRUVZKHEAP",
						"YMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"RST" 

			"D",	{	"YMrTCSbtcDRVZ",
						"YMrTCStcDRVZ"},	0},
	{ //"PLS" 
			0	},
	{ //"PLF" 
			0	},
	{ //"MC" 

			"P",	{	"N",
						"N"},	

			"P",	{	"YM",
						"YM"},	0},
	{ //"MCR" 

			"P",	{	"N",
						"N"},	0},
	{ //"NOP" 
			0	},
	{ //"END" 
			0	},
	{ //"CJ" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"CJP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"CALL" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"CALLP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SRET" 
			0	},
	{ //"IRET" 
			0	},
	{ //"EI" 
			0	},
	{ //"DI" 
			0	},
	{ //"FEND" 
			0	},
	{ //"WDT" 
			0	},
	{ //"WDTP" 
			0	},
	{ //"FOR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"NEXT" 
			0	},
	{ //"CMP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"CMPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DCMP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DCMPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ZCP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ZCPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DZCP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DZCPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"MOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"MOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DMOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DMOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SMOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SMOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"CML" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"CMLP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DCML" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DCMLP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"BMOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"BMOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"FMOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"FMOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DFMOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DFMOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"XCH" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"XCHP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DXCH" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DXCHP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"BCD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"BCDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DBCD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DBCDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"BIN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"BINP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DBIN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DBINP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ADD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ADDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DADD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DADDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SUB" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SUBP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DSUB" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DSUBP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"MUL" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"MULP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DMUL" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DMULP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SUB" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SUBP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DSUB" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DSUBP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"INC" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"INCP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DINC" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DINCP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEC" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DECP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DDEC" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DDECP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WAND" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WANDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DAND" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DANDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WOR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WORP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DOR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DORP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WXOR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WXORP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DXOR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DXORP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"NEG" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"NEGP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DNEG" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DNEGP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ROR" 
			0	},
	{ //"RORP" 
			0	},
	{ //"DROR" 
			0	},
	{ //"DRORP" 
			0	},
	{ //"ROL" 
			0	},
	{ //"ROLP" 
			0	},
	{ //"DROL" 

			"D",	{	"rTCymsDRUVZ",
						"rTCymsDRVZ"},	

			"P",	{	"DRKH",
						"DRKH"},	0},
	{ //"DROLP" 
			0	},
	{ //"RCR" 
			0	},
	{ //"RCRP" 
			0	},
	{ //"DRCR" 
			0	},
	{ //"DRCRP" 
			0	},
	{ //"RCL" 
			0	},
	{ //"RCLP" 
			0	},
	{ //"DRCL" 
			0	},
	{ //"DRCLP" 
			0	},
	{ //"SFTR" 
			0	},
	{ //"SFTRP" 
			0	},
	{ //"SFTL" 
			0	},
	{ //"SFTLP" 
			0	},
	{ //"WSFR" 
			0	},
	{ //"WSFRP" 
			0	},
	{ //"WSFL" 
			0	},
	{ //"WSFLP" 
			0	},
	{ //"SFWR" 
			0	},
	{ //"SFWRP" 
			0	},
	{ //"SFRD" 
			0	},
	{ //"SFRDP" 
			0	},
	{ //"ZRST" 

			"D",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"D",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ZRSTP" 

			"D",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"D",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DECO" 
			0	},
	{ //"DECOP" 
			0	},
	{ //"ENCO" 
			0	},
	{ //"ENCOP" 
			0	},
	{ //"SUM" 
			0	},
	{ //"SUMP" 
			0	},
	{ //"DSUM" 
			0	},
	{ //"DSUMP" 
			0	},
	{ //"BON" 
			0	},
	{ //"BONP" 
			0	},
	{ //"DBON" 
			0	},
	{ //"DBONP" 
			0	},
	{ //"MEAN" 
			0	},
	{ //"MEANP" 
			0	},
	{ //"DMEAN" 
			0	},
	{ //"DMEANP" 
			0	},
	{ //"ANS" 
			0	},
	{ //"ANR" 
			0	},
	{ //"ANRP" 
			0	},
	{ //"SQR" 
			0	},
	{ //"SQRP" 
			0	},
	{ //"DSQR" 
			0	},
	{ //"DSQRP" 
			0	},
	{ //"FLT" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"FLTP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DFLT" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DFLTP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"REF" 
			0	},
	{ //"REFP" 
			0	},
	{ //"REFF" 
			0	},
	{ //"REFFP" 
			0	},
	{ //"MTR" 
			0	},
	{ //"DHSCS" 
			0	},
	{ //"DHSCR" 
			0	},
	{ //"DHSZ" 
			0	},
	{ //"SPD" 
			0	},
	{ //"DSPD" 
			0	},
	{ //"PLSY" 
			0	},
	{ //"DPLSY" 
			0	},
	{ //"PWM" 
			0	},
	{ //"PLSR" 
			0	},
	{ //"DPLSR" 
			0	},
	{ //"IST" 
			0	},
	{ //"SER" 
			0	},
	{ //"SERP" 
			0	},
	{ //"DSER" 
			0	},
	{ //"DSERP" 
			0	},
	{ //"ABSD" 
			0	},
	{ //"DABSD" 
			0	},
	{ //"INCD" 
			0	},
	{ //"TTMR" 
			0	},
	{ //"STMR" 
			0	},
	{ //"ALT" 
			0	},
	{ //"ALTP" 
			0	},
	{ //"RAMP" 
			0	},
	{ //"ROTC" 
			0	},
	{ //"SORT" 
			0	},
	{ //"TKY" 
			0	},
	{ //"DTKY" 
			0	},
	{ //"HKY" 
			0	},
	{ //"DHKY" 
			0	},
	{ //"DSW" 
			0	},
	{ //"SEGD" 
			0	},
	{ //"SEGDP" 
			0	},
	{ //"SEGL" 
			0	},
	{ //"ARWS" 
			0	},
	{ //"ASC" 
			0	},
	{ //"PR" 
			0	},
	{ //"FROM" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"FROMP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DFROM" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DFROMP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"TO" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"TOP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DTO" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DTOP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"RS" 
			0	},
	{ //"PRUN" 
			0	},
	{ //"PRUNP" 
			0	},
	{ //"DPRUN" 
			0	},
	{ //"DPRUNP" 
			0	},
	{ //"ASCI" 
			0	},
	{ //"ASCIP" 
			0	},
	{ //"HEX" 
			0	},
	{ //"HEXP" 
			0	},
	{ //"CCD" 
			0	},
	{ //"CCDP" 
			0	},
	{ //"VRRD" 
			0	},
	{ //"VRRDP" 
			0	},
	{ //"VRSC" 
			0	},
	{ //"VRSCP" 
			0	},
	{ //"RS2" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"PID" 
			0	},
	{ //"ZPUSH" 
			0	},
	{ //"ZPUSHP" 
			0	},
	{ //"ZPOP" 
			0	},
	{ //"ZPOPP" 
			0	},
	{ //"DECMP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DECMPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEZCP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEZCPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEMOV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEMOVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DESTR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DESTRP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEVAL" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEVALP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEBCD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEBCDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEBIN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEBINP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEADD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEADDP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DESUB" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DESUBP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEMUL" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEMULP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEDIV" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEDIVP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEXP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DEXPP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DLOGE" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DLOGEP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DLOG10" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DLOG10P" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DESQR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DESQRP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DENEG" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DENEGP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"INT" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"INTP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DINT" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DINTP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DSIN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DSINP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DCOS" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DCOSP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DTAN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DTANP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DASIN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DASINP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DACOS" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DACOSP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DATAN" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DATANP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DRAD" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DRADP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DDEG" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"DDEGP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"WSUM" 
			0	},
	{ //"WSUMP" 
			0	},
	{ //"DWSUM" 
			0	},
	{ //"DWSUMP" 
			0	},
	{ //"WTOB" 
			0	},
	{ //"WTOBP" 
			0	},
	{ //"BTOW" 
			0	},
	{ //"BTOWP" 
			0	},
	{ //"UNI" 
			0	},
	{ //"UNIP" 
			0	},
	{ //"DIS" 
			0	},
	{ //"DISP" 
			0	},
	{ //"SWAP" 
			0	},
	{ //"SWAPP" 
			0	},
	{ //"DSWAP" 
			0	},
	{ //"DSWAPP" 
			0	},
	{ //"SORT2" 
			0	},
	{ //"DSORT2" 
			0	},
	{ //"DSZR" 
			0	},
	{ //"DVIT" 
			0	},
	{ //"DDVIT" 
			0	},
	{ //"DTBL" 
			0	},
	{ //"DABS" 
			0	},
	{ //"ZRN" 
			0	},
	{ //"DZRN" 
			0	},
	{ //"PLSV" 
			0	},
	{ //"DPLSV" 
			0	},
	{ //"DRVI" 
			0	},
	{ //"DDRVI" 
			0	},
	{ //"DRVA" 
			0	},
	{ //"DDRVA" 
			0	},
	{ //"TCMP" 
			0	},
	{ //"TCMPP" 
			0	},
	{ //"TZCP" 
			0	},
	{ //"TZCPP" 
			0	},
	{ //"TADD" 
			0	},
	{ //"TADDP" 
			0	},
	{ //"TSUB" 
			0	},
	{ //"TSUBP" 
			0	},
	{ //"HTOS" 
			0	},
	{ //"HTOSP" 
			0	},
	{ //"DHTOS" 
			0	},
	{ //"DHTOSP" 
			0	},
	{ //"STOH" 
			0	},
	{ //"STOHP" 
			0	},
	{ //"DSTOH" 
			0	},
	{ //"DSTOHP" 
			0	},
	{ //"TRD" 
			0	},
	{ //"TRDP" 
			0	},
	{ //"TWR" 
			0	},
	{ //"TWRP" 
			0	},
	{ //"HOUR" 
			0	},
	{ //"DHOUR" 
			0	},
	{ //"GRY" 
			0	},
	{ //"GRYP" 
			0	},
	{ //"DGRY" 
			0	},
	{ //"DGRYP" 
			0	},
	{ //"GBIN" 
			0	},
	{ //"GBINP" 
			0	},
	{ //"DGBIN" 
			0	},
	{ //"DGBINP" 
			0	},
	{ //"RD3A" 
			0	},
	{ //"RD3AP" 
			0	},
	{ //"WR3A" 
			0	},
	{ //"WR3AP" 
			0	},
	{ //"EXTR" 
			0	},
	{ //"COMRD" 
			0	},
	{ //"COMRDP" 
			0	},
	{ //"RND" 
			0	},
	{ //"RNDP" 
			0	},
	{ //"DUTY" 
			0	},
	{ //"CRC" 
			0	},
	{ //"CRCP" 
			0	},
	{ //"DHCMOV" 
			0	},
	{ //"BK+" 
			0	},
	{ //"BK+P" 
			0	},
	{ //"DBK+" 
			0	},
	{ //"DBK+P" 
			0	},
	{ //"BK-" 
			0	},
	{ //"BK-P" 
			0	},
	{ //"DBK-" 
			0	},
	{ //"DBK-P" 
			0	},
	{ //"BKCMP=" 
			0	},
	{ //"BKCMP=P" 
			0	},
	{ //"DBKCMP=" 
			0	},
	{ //"DBKCMP=P" 
			0	},
	{ //"BKCMP>" 
			0	},
	{ //"BKCMP>P" 
			0	},
	{ //"DBKCMP>" 
			0	},
	{ //"DBKCMP>P" 
			0	},
	{ //"BKCMP<" 
			0	},
	{ //"BKCMP<P" 
			0	},
	{ //"DBKCMP<" 
			0	},
	{ //"DBKCMP<P" 
			0	},
	{ //"BKCMP<>" 
			0	},
	{ //"BKCMP<>P" 
			0	},
	{ //"DBKCMP<>" 
			0	},
	{ //"DBKCMP<>P" 
			0	},
	{ //"BKCMP<=" 
			0	},
	{ //"BKCMP<=P" 
			0	},
	{ //"DBKCMP<=" 
			0	},
	{ //"DBKCMP<=P" 
			0	},
	{ //"BKCMP>=" 
			0	},
	{ //"BKCMP>=P" 
			0	},
	{ //"DBKCMP>=" 
			0	},
	{ //"DBKCMP>=P" 
			0	},
	{ //"STR" 
			0	},
	{ //"STRP" 
			0	},
	{ //"DSTR" 
			0	},
	{ //"DSTRP" 
			0	},
	{ //"VAL" 
			0	},
	{ //"VALP" 
			0	},
	{ //"DVAL" 
			0	},
	{ //"DVALP" 
			0	},
	{ //"$+" 

			"S",	{	"xymstcDRUA",
						"xymstcDRUA"},	

			"S",	{	"xymstcDRUA",
						"xymstcDRUA"},	

			"D",	{	"ymstcDRU",
						"ymstcDRU"},	0},
	{ //"$+P" 

			"S",	{	"xymstcDRUA",
						"xymstcDRUA"},	

			"S",	{	"xymstcDRUA",
						"xymstcDRUA"},	

			"D",	{	"ymstcDRU",
						"ymstcDRU"},	0},
	{ //"LEN" 
			0	},
	{ //"LENP" 
			0	},
	{ //"RIGHT" 
			0	},
	{ //"RIGHTP" 
			0	},
	{ //"LEFT" 
			0	},
	{ //"LEFTP" 
			0	},
	{ //"MIDR" 
			0	},
	{ //"MIDRP" 
			0	},
	{ //"MIDW" 
			0	},
	{ //"MIDWP" 
			0	},
	{ //"INSTR" 
			0	},
	{ //"INSTRP" 
			0	},
	{ //"$MOV" 
			0	},
	{ //"$MOVP" 
			0	},
	{ //"FDEL" 
			0	},
	{ //"FDELP" 
			0	},
	{ //"FINS" 
			0	},
	{ //"FINSP" 
			0	},
	{ //"POP" 
			0	},
	{ //"POPP" 
			0	},
	{ //"SFR" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SFRP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SFL" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"SFLP" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"LD=" 
			0	},
	{ //"LD<>" 
			0	},
	{ //"LD>" 
			0	},
	{ //"LD>=" 
			0	},
	{ //"LD<" 

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	0},
	{ //"LD<=" 
			0	},
	{ //"LDD=" 
			0	},
	{ //"LDD<>" 

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	0},
	{ //"LDD>" 
			0	},
	{ //"LDD>=" 
			0	},
	{ //"LDD<" 
			0	},
	{ //"LDD<=" 
			0	},
	{ //"AND=" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"AND<>" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"AND>" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"AND>=" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"AND<" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"AND<=" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ANDD=" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ANDD<>" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ANDD>" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ANDD>=" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ANDD<" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"ANDD<=" 

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	

			"SDP",	{	"XYMrTCSbxymstcDRUVZKHEAP",
						"XYMrTCSbxymstcDRUVZKHEAP"},	0},
	{ //"OR=" 
			0	},
	{ //"OR<>" 
			0	},
	{ //"OR>" 
			0	},
	{ //"OR>=" 
			0	},
	{ //"OR<" 
			0	},
	{ //"OR<=" 
			0	},
	{ //"ORD=" 

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	0},
	{ //"ORD<>" 

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	

			"S",	{	"rxymstcDRUVZKH",
						"rxymstcDRVZKH"},	0},
	{ //"ORD>" 
			0	},
	{ //"ORD>=" 
			0	},
	{ //"ORD<" 
			0	},
	{ //"ORD<=" 
			0	},
	{ //"LIMIT" 
			0	},
	{ //"LIMITP" 
			0	},
	{ //"DLIMIT" 
			0	},
	{ //"DLIMITP" 
			0	},
	{ //"BAND" 
			0	},
	{ //"BANDP" 
			0	},
	{ //"DBAND" 
			0	},
	{ //"DBANDP" 
			0	},
	{ //"ZONE" 
			0	},
	{ //"ZONEP" 
			0	},
	{ //"DZONE" 
			0	},
	{ //"DZONEP" 
			0	},
	{ //"SCL" 
			0	},
	{ //"SCLP" 
			0	},
	{ //"DSCL" 
			0	},
	{ //"DSCLP" 
			0	},
	{ //"DABIN" 
			0	},
	{ //"DABINP" 
			0	},
	{ //"DDABIN" 
			0	},
	{ //"DDABINP" 
			0	},
	{ //"BINDA" 
			0	},
	{ //"BINDAP" 
			0	},
	{ //"DBINDA" 
			0	},
	{ //"DBINDAP" 
			0	},
	{ //"SCL2" 
			0	},
	{ //"SCL2P" 
			0	},
	{ //"DSCL2" 
			0	},
	{ //"DSCL2P" 
			0	},
	{ //"IVCK" 
			0	},
	{ //"IVDR" 
			0	},
	{ //"IVRD" 
			0	},
	{ //"IVWR" 
			0	},
	{ //"IVBWR" 
			0	},
	{ //"IVMC" 
			0	},
	{ //"RBFM" 
			0	},
	{ //"WBFM" 
			0	},
	{ //"DHSCT" 
			0	},
	{ //"LOADR" 
			0	},
	{ //"LOADRP" 
			0	},
	{ //"SAVER" 
			0	},
	{ //"INITR" 
			0	},
	{ //"INITRP" 
			0	},
	{ //"LOGR" 
			0	},
	{ //"LOGRP" 
			0	},
	{ //"RWER" 
			0	},
	{ //"RWERP" 
			0	},
	{ //"INITER" 
			0	},
	{ //"INITERP" 
			0	},
	{ //"FLCRT" 
			0	},
	{ //"FLDEL" 
			0	},
	{ //"FLWR" 
			0	},
	{ //"FLRD" 
			0	},
	{ //"FLCMD" 
			0	},
	{ //"FLSTRD" 
			0	}
};

#endif //__mel_metrix_table_h__
