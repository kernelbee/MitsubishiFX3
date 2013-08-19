#ifndef __mel_wpg_opcodes_table_h__
#define __mel_wpg_opcodes_table_h__

#define MEL_INSTRUCT_OPERANDS_MAX 8

typedef struct tagWPGOperandRecord
{
	char* txt_usage;
	char* txt_operands[2/*MEL_PLC_FXCPU_TYPE_LAST*/];
	char* explanation;

#define MEL_DUSAGE_SRC		(1<<0)	//1
#define MEL_DUSAGE_DST		(1<<1)	//2
#define MEL_DUSAGE_BTH		(MEL_DUSAGE_SRC|MEL_DUSAGE_DST)
#define MEL_DUSAGE_PRM		(1<<2)	//4	//parameter (n, m etc.)
	unsigned int	dusage;

/*
#define MEL_DTYPE_ABIT			(1<<0)	//1
#define MEL_DTYPE_16BIT_BIN		(1<<1)	//2
#define MEL_DTYPE_32BIT_BIN		(1<<2)	//4
#define MEL_DTYPE_64BIT_BIN		(1<<3)	//8
#define MEL_DTYPE_16BIT_BCD		(1<<4)	//16
#define MEL_DTYPE_32BIT_BCD		(1<<5)	//32
#define MEL_DTYPE_1632BIT_BCD	(1<<6)	//64
#define MEL_DTYPE_JISASCIITEXT	(1<<7)	//128
#define MEL_DTYPE_ASCII_STRING	(1<<8)	//256
#define MEL_DTYPE_REAL_BIN		(1<<9)	//512
#define MEL_DTYPE_REAL_SCI		(1<<10)	//1024
	unsigned int	data_type;
*/

	// 			1) Bit devices
	// 				•X : Input relay (X)
	// 				•Y : Output relay (Y)
	// 				•M : Auxiliary relay (M)
	// 				•S : State relay (S) etc.
	// 			2) Word devices
	// 				•K : Decimal integer
	// 				•H : Hexadecimal integer
	// 				•KnX: Input relay (X) with digit specification*1
	// 				•KnY: Output relay (Y) with digit specification*1
	// 				•KnM: Auxiliary relay (M) with digit specification*1
	// 				•KnS: State relay (S) with digit specification*1
	// 				•T : Timer (T) current value
	// 				•C : Counter (C) current value
	// 				•D : Data register (file register)
	// 				•V, Z: Index register
	// 				•Modify: Availability of indexing using index register etc.		
	// 			
	// 				*1. Kn without specification indicates K1 to K4 for 16 bits, and K1 to K8 for 32 bits.
	struct operand_attr{
		
		/////////////////////////////at least 1 device operands////////////////////////////////
		//bits of MEL_DEV_TYPE_x:			All unique device types
		unsigned int	applicable_dtypes;
		//bits of MEL_DEV_TYPE_EXT_x:		M/D sys, M/D sys read-only, C 32-bit
		unsigned int	applicable_dtypes_ext;

		/////////////////////////////for 2 or 3 devices operands///////////////////////////////////
		//bits of MEL_OTYPE_EXT_x:			KnM,KnS,KnX,KnY
		unsigned int	applicable_otypes;
		//bits of MEL_WPG_OPERAND_ATTRIB_x:	{indexing|linking|bitaccess|unitaccess}
		unsigned int	allowed_attribs;

	} dbits[2/*MEL_PLC_FXCPU_TYPE_LAST*/];

}WPGOperandRecord;

typedef struct tagWPGOpNameRecord
{
	char*	name;
	int		namelen;

}WPGOpNameRecord;

typedef struct tagWPGInstructionRecord
{
	//unsigned int opcode;
	//char*		name;
	//int		namelen;
		
	int		folder;
	//int		folderlen;

	int		fnc;

	//unsigned int	plc_models;
	//int				plc_min_ver_x100;//>= v2.20: min_ver = 220; 	max_ver = 10000
	//int				plc_max_ver_x100;//<  v2.20: min_ver = 0; 		max_ver = 219
#define MEL_BITDEPTH_INDEP			0
#define MEL_BITDEPTH_1BIT			(1<<1)	//2
#define MEL_BITDEPTH_16BIT			(1<<2)	//4
#define MEL_BITDEPTH_32BIT			(1<<3)	//8
#define MEL_BITDEPTH_1632BIT		(MEL_BITDEPTH_16BIT|MEL_BITDEPTH_32BIT)

	unsigned int	bitdepth;

	int				steps;//according to bit depth, operands, indexing, etc.

	// 	.Execution Time (= A*n + B*m + C, µs) depending on: 
	// 		PLC model
	// 			ON/OFF state
	// 			instruction bit depth	
	// 			operands (n,m)		
	// 		

#define MEL_EXEC_CONT	(1<<1)	//2
#define MEL_EXEC_PULS	(1<<2)	//4

	// 	.Execution dtype:
	unsigned int	exectype;

	// 	.NumberOfOperands: 0,1,2...
	int				n_operands;

	// 	
	// 	.Instruction Description (string in selected language)
	// 		general explanation
	
	// 	.Operands[NumberOfOperands]
	WPGOperandRecord operands[MEL_INSTRUCT_OPERANDS_MAX];

	char*	comment;
	
}WPGInstructionRecord;


typedef enum tagMEL_Operations
{
	MEL_OPER_DUMMY = 0,
	MEL_OPER_CONTACTS,
	MEL_OPER_CONNECTS,
	MEL_OPER_OUTS,
	MEL_OPER_MASTERCNT,
	MEL_OPER_OTHERBASICS,
	MEL_OPER_ENDS,
	MEL_OPER_PRGFLOW,
	MEL_OPER_MOVECMP,
	MEL_OPER_ARITHMLOGIC,
	MEL_OPER_ROTSHIFT,
	MEL_OPER_DATAOPS,	
	MEL_OPER_HANDY,
	MEL_OPER_EXTFXIO,
	MEL_OPER_EXTFXDEVS,
	MEL_OPER_DATATRANS2,
	MEL_OPER_FLOATS,
	MEL_OPER_DATAOPS2,
	MEL_OPER_POSCONTROL,
	MEL_OPER_REALTIME,
	MEL_OPER_EXTDEVS,
	MEL_OPER_INTRODUCTION,
	MEL_OPER_OTHERS,
	MEL_OPER_BLOCKOPS,
	MEL_OPER_CHARSTR,
	MEL_OPER_DATAOPS3,
	MEL_OPER_DATACMP,
	MEL_OPER_DATATABLE,
	MEL_OPER_EXTDEVSCOMM_INVERT,
	MEL_OPER_DATATRANS3,
	MEL_OPER_HIGHSPEEDPROC,
	MEL_OPER_EXTENSFILEREGS,
	MEL_OPER_FX3UCFADP

}MEL_Operations;

static char* WPG_OPERATIONS_FOLDERS[] =
{
	"",
	"Contact instruction",
	"Connection instruction",	
	"Out instruction",
	"Master Control instruction",
	"Other Basic instruction",
	"End instruction",
	"Program Flow",
	"Move and compare",
	"Arithmetic and Logical Operation",
	"Rotation and Shift Operation",
	"Data Operation",	
	"Handy Instruction",
	"External FX I/O Device",
	"External FX Device",
	"Data Transfer 2",
	"Floating Point",
	"Data Operations 2",
	"Positioning Control",
	"Real time Clock Control",
	"External Device",
	"Introduction of Alternate Instruction",// (see "Data Comm Edition Manual")
	"Other",
	"Block Data Operation",
	"Character String Control",
	"Data Operations 3",
	"Data Comparison",
	"Data Table Operation",
	"External Device Communication (Inverter Comm.)",
	"Data Transfer 3",
	"High Speed Processing",
	"Extension File Register Control",
	"FX3U-CF-ADP Applied Instruction"
};

typedef enum tagMEL_WPG_Opcodes
{
	//////////////////////////////////////////////////////////////////////////
	//Basic instructions
	//////////////////////////////////////////////////////////////////////////
	//"Contact instruction",
	MEL_WPG_OpID_LD = 0,
	MEL_WPG_OpID_LDI,
	MEL_WPG_OpID_LDP,
	MEL_WPG_OpID_LDF,	
	MEL_WPG_OpID_AND,
	MEL_WPG_OpID_ANI,
	MEL_WPG_OpID_ANDP,
	MEL_WPG_OpID_ANDF,
	MEL_WPG_OpID_OR,
	MEL_WPG_OpID_ORI,
	MEL_WPG_OpID_ORP,
	MEL_WPG_OpID_ORF,	
	//"Connection instruction",
	MEL_WPG_OpID_ANB,
	MEL_WPG_OpID_ORB,
	MEL_WPG_OpID_MPS,
	MEL_WPG_OpID_MRD,
	MEL_WPG_OpID_MPP,
	MEL_WPG_OpID_INV,
	MEL_WPG_OpID_MEP,
	MEL_WPG_OpID_MEF,
	//"Out instruction",
	MEL_WPG_OpID_OUT,
	MEL_WPG_OpID_OUT_TC,
	MEL_WPG_OpID_SET,
	MEL_WPG_OpID_RST,
	MEL_WPG_OpID_PLS,
	MEL_WPG_OpID_PLF,
	//"Master Control instruction",
	MEL_WPG_OpID_MC,
	MEL_WPG_OpID_MCR,
	//"Other Basic instruction",
	MEL_WPG_OpID_NOP,
	//"End instruction"
	MEL_WPG_OpID_END,
	//////////////////////////////////////////////////////////////////////////
	//"Program Flow"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_CJ,
	MEL_WPG_OpID_CJP,
	MEL_WPG_OpID_CALL,
	MEL_WPG_OpID_CALLP,
	MEL_WPG_OpID_SRET,
	MEL_WPG_OpID_IRET,
	MEL_WPG_OpID_EI,
	MEL_WPG_OpID_DI,
	MEL_WPG_OpID_FEND,
	MEL_WPG_OpID_WDT,
	MEL_WPG_OpID_WDTP,
	MEL_WPG_OpID_FOR,
	MEL_WPG_OpID_NEXT,
	//////////////////////////////////////////////////////////////////////////
	//"Move and compare"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_CMP,
	MEL_WPG_OpID_CMPP,
	MEL_WPG_OpID_DCMP,
	MEL_WPG_OpID_DCMPP,
	MEL_WPG_OpID_ZCP,
	MEL_WPG_OpID_ZCPP,
	MEL_WPG_OpID_DZCP,
	MEL_WPG_OpID_DZCPP,
	MEL_WPG_OpID_MOV,
	MEL_WPG_OpID_MOVP,
	MEL_WPG_OpID_DMOV,
	MEL_WPG_OpID_DMOVP,
	MEL_WPG_OpID_SMOV,
	MEL_WPG_OpID_SMOVP,
	MEL_WPG_OpID_CML,
	MEL_WPG_OpID_CMLP,
	MEL_WPG_OpID_DCML,
	MEL_WPG_OpID_DCMLP,
	MEL_WPG_OpID_BMOV,
	MEL_WPG_OpID_BMOVP,
	MEL_WPG_OpID_FMOV,
	MEL_WPG_OpID_FMOVP,
	MEL_WPG_OpID_DFMOV,
	MEL_WPG_OpID_DFMOVP,
	MEL_WPG_OpID_XCH,
	MEL_WPG_OpID_XCHP,
	MEL_WPG_OpID_DXCH,
	MEL_WPG_OpID_DXCHP,
	MEL_WPG_OpID_BCD,
	MEL_WPG_OpID_BCDP,
	MEL_WPG_OpID_DBCD,
	MEL_WPG_OpID_DBCDP,
	MEL_WPG_OpID_BIN,
	MEL_WPG_OpID_BINP,
	MEL_WPG_OpID_DBIN,
	MEL_WPG_OpID_DBINP,
	//////////////////////////////////////////////////////////////////////////
	//"Arithmetic and Logical Operation",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_ADD,
	MEL_WPG_OpID_ADDP,
	MEL_WPG_OpID_DADD,
	MEL_WPG_OpID_DADDP,
	MEL_WPG_OpID_SUB,
	MEL_WPG_OpID_SUBP,
	MEL_WPG_OpID_DSUB,
	MEL_WPG_OpID_DSUBP,
	MEL_WPG_OpID_MUL,
	MEL_WPG_OpID_MULP,
	MEL_WPG_OpID_DMUL,
	MEL_WPG_OpID_DMULP,
	MEL_WPG_OpID_DIV,
	MEL_WPG_OpID_DIVP,
	MEL_WPG_OpID_DDIV,
	MEL_WPG_OpID_DDIVP,
	MEL_WPG_OpID_INC,
	MEL_WPG_OpID_INCP,
	MEL_WPG_OpID_DINC,
	MEL_WPG_OpID_DINCP,
	MEL_WPG_OpID_DEC,
	MEL_WPG_OpID_DECP,
	MEL_WPG_OpID_DDEC,
	MEL_WPG_OpID_DDECP,
	MEL_WPG_OpID_WAND,
	MEL_WPG_OpID_WANDP,
	MEL_WPG_OpID_DAND,
	MEL_WPG_OpID_DANDP,
	MEL_WPG_OpID_WOR,
	MEL_WPG_OpID_WORP,
	MEL_WPG_OpID_DOR,
	MEL_WPG_OpID_DORP,
	MEL_WPG_OpID_WXOR,
	MEL_WPG_OpID_WXORP,
	MEL_WPG_OpID_DXOR,
	MEL_WPG_OpID_DXORP,
	MEL_WPG_OpID_NEG,
	MEL_WPG_OpID_NEGP,
	MEL_WPG_OpID_DNEG,
	MEL_WPG_OpID_DNEGP,
	//////////////////////////////////////////////////////////////////////////
	//"Rotation and Shift Operation",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_ROR,
	MEL_WPG_OpID_RORP,
	MEL_WPG_OpID_DROR,
	MEL_WPG_OpID_DRORP,
	MEL_WPG_OpID_ROL,
	MEL_WPG_OpID_ROLP,
	MEL_WPG_OpID_DROL,
	MEL_WPG_OpID_DROLP,
	MEL_WPG_OpID_RCR,
	MEL_WPG_OpID_RCRP,
	MEL_WPG_OpID_DRCR,
	MEL_WPG_OpID_DRCRP,
	MEL_WPG_OpID_RCL,
	MEL_WPG_OpID_RCLP,
	MEL_WPG_OpID_DRCL,
	MEL_WPG_OpID_DRCLP,
	MEL_WPG_OpID_SFTR,
	MEL_WPG_OpID_SFTRP,
	MEL_WPG_OpID_SFTL,
	MEL_WPG_OpID_SFTLP,
	MEL_WPG_OpID_WSFR,
	MEL_WPG_OpID_WSFRP,
	MEL_WPG_OpID_WSFL,
	MEL_WPG_OpID_WSFLP,
	MEL_WPG_OpID_SFWR,
	MEL_WPG_OpID_SFWRP,
	MEL_WPG_OpID_SFRD,
	MEL_WPG_OpID_SFRDP,
	//////////////////////////////////////////////////////////////////////////
	//"Data Operation",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_ZRST,
	MEL_WPG_OpID_ZRSTP,
	MEL_WPG_OpID_DECO,
	MEL_WPG_OpID_DECOP,
	MEL_WPG_OpID_ENCO,
	MEL_WPG_OpID_ENCOP,
	MEL_WPG_OpID_SUM,
	MEL_WPG_OpID_SUMP,
	MEL_WPG_OpID_DSUM,
	MEL_WPG_OpID_DSUMP,
	MEL_WPG_OpID_BON,
	MEL_WPG_OpID_BONP,
	MEL_WPG_OpID_DBON,
	MEL_WPG_OpID_DBONP,
	MEL_WPG_OpID_MEAN,
	MEL_WPG_OpID_MEANP,
	MEL_WPG_OpID_DMEAN,
	MEL_WPG_OpID_DMEANP,
	MEL_WPG_OpID_ANS,		
	MEL_WPG_OpID_ANR,
	MEL_WPG_OpID_ANRP,
	MEL_WPG_OpID_SQR,
	MEL_WPG_OpID_SQRP,
	MEL_WPG_OpID_DSQR,
	MEL_WPG_OpID_DSQRP,
	MEL_WPG_OpID_FLT,
	MEL_WPG_OpID_FLTP,
	MEL_WPG_OpID_DFLT,
	MEL_WPG_OpID_DFLTP,
	//////////////////////////////////////////////////////////////////////////
	//"High Speed Processing"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_REF,
	MEL_WPG_OpID_REFP,
	MEL_WPG_OpID_REFF,
	MEL_WPG_OpID_REFFP,
	MEL_WPG_OpID_MTR,
	MEL_WPG_OpID_DHSCS,
	MEL_WPG_OpID_DHSCR,
	MEL_WPG_OpID_DHSZ,
	MEL_WPG_OpID_SPD,
	MEL_WPG_OpID_DSPD,
	MEL_WPG_OpID_PLSY,
	MEL_WPG_OpID_DPLSY,
	MEL_WPG_OpID_PWM,		
	MEL_WPG_OpID_PLSR,
	MEL_WPG_OpID_DPLSR,
	//////////////////////////////////////////////////////////////////////////
	//"Handy Instruction",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_IST,
	MEL_WPG_OpID_SER,
	MEL_WPG_OpID_SERP,
	MEL_WPG_OpID_DSER,
	MEL_WPG_OpID_DSERP,
	MEL_WPG_OpID_ABSD,
	MEL_WPG_OpID_DABSD,
	MEL_WPG_OpID_INCD,		
	MEL_WPG_OpID_TTMR,
	MEL_WPG_OpID_STMR,
	MEL_WPG_OpID_ALT,
	MEL_WPG_OpID_ALTP,
	MEL_WPG_OpID_RAMP,
	MEL_WPG_OpID_ROTC,
	MEL_WPG_OpID_SORT,
	//////////////////////////////////////////////////////////////////////////
	//"External FX I/O Device",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_TKY,
	MEL_WPG_OpID_DTKY,
	MEL_WPG_OpID_HKY,
	MEL_WPG_OpID_DHKY,
	MEL_WPG_OpID_DSW,
	MEL_WPG_OpID_SEGD,
	MEL_WPG_OpID_SEGDP,
	MEL_WPG_OpID_SEGL,
	MEL_WPG_OpID_ARWS,
	MEL_WPG_OpID_ASC,
	MEL_WPG_OpID_PR,
	MEL_WPG_OpID_FROM,
	MEL_WPG_OpID_FROMP,
	MEL_WPG_OpID_DFROM,
	MEL_WPG_OpID_DFROMP,
	MEL_WPG_OpID_TO,
	MEL_WPG_OpID_TOP,
	MEL_WPG_OpID_DTO,
	MEL_WPG_OpID_DTOP,
	//////////////////////////////////////////////////////////////////////////
	//"External FX Device",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_RS,
	MEL_WPG_OpID_PRUN,
	MEL_WPG_OpID_PRUNP,
	MEL_WPG_OpID_DPRUN,
	MEL_WPG_OpID_DPRUNP,
	MEL_WPG_OpID_ASCI,
	MEL_WPG_OpID_ASCIP,
	MEL_WPG_OpID_HEX,
	MEL_WPG_OpID_HEXP,
	MEL_WPG_OpID_CCD,
	MEL_WPG_OpID_CCDP,
	MEL_WPG_OpID_VRRD,
	MEL_WPG_OpID_VRRDP,
	MEL_WPG_OpID_VRSC,
	MEL_WPG_OpID_VRSCP,
	MEL_WPG_OpID_RS2,
	MEL_WPG_OpID_PID,
	//////////////////////////////////////////////////////////////////////////
	//"Data Transfer 2"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_ZPUSH,
	MEL_WPG_OpID_ZPUSHP,
	MEL_WPG_OpID_ZPOP,
	MEL_WPG_OpID_ZPOPP,
	//////////////////////////////////////////////////////////////////////////
	//"Floating Point"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_DECMP,
	MEL_WPG_OpID_DECMPP,
	MEL_WPG_OpID_DEZCP,
	MEL_WPG_OpID_DEZCPP,
	MEL_WPG_OpID_DEMOV,
	MEL_WPG_OpID_DEMOVP,
	MEL_WPG_OpID_DESTR,
	MEL_WPG_OpID_DESTRP,
	MEL_WPG_OpID_DEVAL,
	MEL_WPG_OpID_DEVALP,
	MEL_WPG_OpID_DEBCD,
	MEL_WPG_OpID_DEBCDP,
	MEL_WPG_OpID_DEBIN,
	MEL_WPG_OpID_DEBINP,
	MEL_WPG_OpID_DEADD,
	MEL_WPG_OpID_DEADDP,
	MEL_WPG_OpID_DESUB,
	MEL_WPG_OpID_DESUBP,
	MEL_WPG_OpID_DEMUL,
	MEL_WPG_OpID_DEMULP,
	MEL_WPG_OpID_DEDIV,
	MEL_WPG_OpID_DEDIVP,
	MEL_WPG_OpID_DEXP,
	MEL_WPG_OpID_DEXPP,
	MEL_WPG_OpID_DLOGE,
	MEL_WPG_OpID_DLOGEP,
	MEL_WPG_OpID_DLOG10,
	MEL_WPG_OpID_DLOG10P,
	MEL_WPG_OpID_DESQR,
	MEL_WPG_OpID_DESQRP,
	MEL_WPG_OpID_DENEG,
	MEL_WPG_OpID_DENEGP,
	MEL_WPG_OpID_INT,
	MEL_WPG_OpID_INTP,
	MEL_WPG_OpID_DINT,
	MEL_WPG_OpID_DINTP,
	MEL_WPG_OpID_DSIN,
	MEL_WPG_OpID_DSINP,
	MEL_WPG_OpID_DCOS,
	MEL_WPG_OpID_DCOSP,
	MEL_WPG_OpID_DTAN,
	MEL_WPG_OpID_DTANP,
	MEL_WPG_OpID_DASIN,
	MEL_WPG_OpID_DASINP,
	MEL_WPG_OpID_DACOS,
	MEL_WPG_OpID_DACOSP,
	MEL_WPG_OpID_DATAN,
	MEL_WPG_OpID_DATANP,
	MEL_WPG_OpID_DRAD,
	MEL_WPG_OpID_DRADP,
	MEL_WPG_OpID_DDEG,
	MEL_WPG_OpID_DDEGP,
	//////////////////////////////////////////////////////////////////////////
	//"Data Operations 2"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_WSUM,
	MEL_WPG_OpID_WSUMP,
	MEL_WPG_OpID_DWSUM,
	MEL_WPG_OpID_DWSUMP,
	MEL_WPG_OpID_WTOB,
	MEL_WPG_OpID_WTOBP,
	MEL_WPG_OpID_BTOW,
	MEL_WPG_OpID_BTOWP,
	MEL_WPG_OpID_UNI,
	MEL_WPG_OpID_UNIP,
	MEL_WPG_OpID_DIS,
	MEL_WPG_OpID_DISP,
	MEL_WPG_OpID_SWAP,
	MEL_WPG_OpID_SWAPP,
	MEL_WPG_OpID_DSWAP,
	MEL_WPG_OpID_DSWAPP,
	MEL_WPG_OpID_SORT2,
	MEL_WPG_OpID_DSORT2,
	//////////////////////////////////////////////////////////////////////////
	//"Positioning Control"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_DSZR,
	MEL_WPG_OpID_DVIT,
	MEL_WPG_OpID_DDVIT,
	MEL_WPG_OpID_DTBL,
	MEL_WPG_OpID_DABS,
	MEL_WPG_OpID_ZRN,
	MEL_WPG_OpID_DZRN,
	MEL_WPG_OpID_PLSV,
	MEL_WPG_OpID_DPLSV,
	MEL_WPG_OpID_DRVI,
	MEL_WPG_OpID_DDRVI,
	MEL_WPG_OpID_DRVA,
	MEL_WPG_OpID_DDRVA,
	//////////////////////////////////////////////////////////////////////////
	//"Real time Clock Control"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_TCMP,
	MEL_WPG_OpID_TCMPP,
	MEL_WPG_OpID_TZCP,
	MEL_WPG_OpID_TZCPP,
	MEL_WPG_OpID_TADD,
	MEL_WPG_OpID_TADDP,
	MEL_WPG_OpID_TSUB,
	MEL_WPG_OpID_TSUBP,
	MEL_WPG_OpID_HTOS,
	MEL_WPG_OpID_HTOSP,
	MEL_WPG_OpID_DHTOS,
	MEL_WPG_OpID_DHTOSP,
	MEL_WPG_OpID_STOH,
	MEL_WPG_OpID_STOHP,
	MEL_WPG_OpID_DSTOH,
	MEL_WPG_OpID_DSTOHP,
	MEL_WPG_OpID_TRD,
	MEL_WPG_OpID_TRDP,
	MEL_WPG_OpID_TWR,
	MEL_WPG_OpID_TWRP,
	MEL_WPG_OpID_HOUR,
	MEL_WPG_OpID_DHOUR,
	//////////////////////////////////////////////////////////////////////////
	//"External Device",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_GRY,
	MEL_WPG_OpID_GRYP,
	MEL_WPG_OpID_DGRY,
	MEL_WPG_OpID_DGRYP,
	MEL_WPG_OpID_GBIN,
	MEL_WPG_OpID_GBINP,
	MEL_WPG_OpID_DGBIN,
	MEL_WPG_OpID_DGBINP,
	MEL_WPG_OpID_RD3A,
	MEL_WPG_OpID_RD3AP,
	MEL_WPG_OpID_WR3A,
	MEL_WPG_OpID_WR3AP,
	//////////////////////////////////////////////////////////////////////////
	//"Introduction of Alternate Instruction", (see "Data Comm Edition Manual")
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_EXTR,//FX2N, FX2NC,
	//////////////////////////////////////////////////////////////////////////
	//"Other",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_COMRD,
	MEL_WPG_OpID_COMRDP,
	MEL_WPG_OpID_RND,
	MEL_WPG_OpID_RNDP,
	MEL_WPG_OpID_DUTY,
	MEL_WPG_OpID_CRC,
	MEL_WPG_OpID_CRCP,
	MEL_WPG_OpID_DHCMOV,
	//////////////////////////////////////////////////////////////////////////
	//"Block Data Operation",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_BKplus,//BK+
	MEL_WPG_OpID_BKplusP,		//BK+P
	MEL_WPG_OpID_DBKplus,		//DBK+
	MEL_WPG_OpID_DBKplusP,	//DBK+P
	MEL_WPG_OpID_BKminus,//BK-
	MEL_WPG_OpID_BKminusP,	//BK-P
	MEL_WPG_OpID_DBKminus,	//DBK-
	MEL_WPG_OpID_DBKminusP,	//DBK-P
	MEL_WPG_OpID_BKCMPeq,//BKCMP=
	MEL_WPG_OpID_BKCMPeqP,	//BKCMP=P
	MEL_WPG_OpID_DBKCMPeq,	//DBKCMP=
	MEL_WPG_OpID_DBKCMPeqP,	//DBKCMP=P
	MEL_WPG_OpID_BKCMPbigger,//BKCMP>
	MEL_WPG_OpID_BKCMPbiggerP,	//BKCMP>P
	MEL_WPG_OpID_DBKCMPbigger,	//BKCMP>
	MEL_WPG_OpID_DBKCMPbiggerP,	//DBKCMP>P
	MEL_WPG_OpID_BKCMPless,//BKCMP<
	MEL_WPG_OpID_BKCMPlessP,//BKCMP<P
	MEL_WPG_OpID_DBKCMPless,//DBKCMP<
	MEL_WPG_OpID_DBKCMPlessP,//DBKCMP<P
	MEL_WPG_OpID_BKCMPnoteq,//BKCMP<>
	MEL_WPG_OpID_BKCMPnoteqP,//BKCMP<>P
	MEL_WPG_OpID_DBKCMPnoteq,//DBKCMP<>
	MEL_WPG_OpID_DBKCMPnoteqP,//DBKCMP<>P
	MEL_WPG_OpID_BKCMPlesseq,//BKCMP<=
	MEL_WPG_OpID_BKCMPlesseqP,//BKCMP<=
	MEL_WPG_OpID_DBKCMPlesseq,//DBKCMP<=
	MEL_WPG_OpID_DBKCMPlesseqP,//DBKCMP<=P
	MEL_WPG_OpID_BKCMPbiggereq,//BKCMP>=
	MEL_WPG_OpID_BKCMPbiggereqP,//BKCMP>=
	MEL_WPG_OpID_DBKCMPbiggereq,//DBKCMP>=
	MEL_WPG_OpID_DBKCMPbiggereqP,//DBKCMP>=P
	//////////////////////////////////////////////////////////////////////////
	//"Character String Control"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_STR,
	MEL_WPG_OpID_STRP,
	MEL_WPG_OpID_DSTR,
	MEL_WPG_OpID_DSTRP,
	MEL_WPG_OpID_VAL,
	MEL_WPG_OpID_VALP,
	MEL_WPG_OpID_DVAL,
	MEL_WPG_OpID_DVALP,
	MEL_WPG_OpID_linkstr,//$+ (N bytes string)
	MEL_WPG_OpID_linkstrP,//$+P
	MEL_WPG_OpID_LEN,
	MEL_WPG_OpID_LENP,
	MEL_WPG_OpID_RIGHT,
	MEL_WPG_OpID_RIGHTP,
	MEL_WPG_OpID_LEFT,
	MEL_WPG_OpID_LEFTP,
	MEL_WPG_OpID_MIDR,
	MEL_WPG_OpID_MIDRP,
	MEL_WPG_OpID_MIDW,
	MEL_WPG_OpID_MIDWP,
	MEL_WPG_OpID_INSTR,
	MEL_WPG_OpID_INSTRP,
	MEL_WPG_OpID_strMOV,//$MOV
	MEL_WPG_OpID_strMOVP,//$MOVP
	//////////////////////////////////////////////////////////////////////////
	//"Data Operations 3"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_FDEL,
	MEL_WPG_OpID_FDELP,
	MEL_WPG_OpID_FINS,
	MEL_WPG_OpID_FINSP,
	MEL_WPG_OpID_POP,
	MEL_WPG_OpID_POPP,
	MEL_WPG_OpID_SFR,
	MEL_WPG_OpID_SFRP,
	MEL_WPG_OpID_SFL,
	MEL_WPG_OpID_SFLP,
	//////////////////////////////////////////////////////////////////////////
	//"Data Comparison"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_LDeq,			//LD=
	MEL_WPG_OpID_LDnoteq,		//LD<>
	MEL_WPG_OpID_LDbigger,		//LD>
	MEL_WPG_OpID_LDbiggereq,	//LD>=	
	MEL_WPG_OpID_LDless,		//LD<
	MEL_WPG_OpID_LDlessseq,		//LD<=

	MEL_WPG_OpID_LDDeq,			//LDD=
	MEL_WPG_OpID_LDDnoteq,		//LDD<>
	MEL_WPG_OpID_LDDbigger,		//LDD>
	MEL_WPG_OpID_LDDbiggereq,	//LDD>=	
	MEL_WPG_OpID_LDDless,		//LDD<	
	MEL_WPG_OpID_LDDlessseq,	//LDD<=	

	MEL_WPG_OpID_ANDeq,			//AND=
	MEL_WPG_OpID_ANDnoteq,		//AND<>
	MEL_WPG_OpID_ANDbigger,		//AND>
	MEL_WPG_OpID_ANDbiggereq,	//AND>=
	MEL_WPG_OpID_ANDless,		//AND<
	MEL_WPG_OpID_ANDlessseq,	//AND<=

	MEL_WPG_OpID_ANDDeq,		//ANDD=
	MEL_WPG_OpID_ANDDnoteq,		//ANDD<>
	MEL_WPG_OpID_ANDDbigger,	//ANDD>
	MEL_WPG_OpID_ANDDbiggereq,	//ANDD>=
	MEL_WPG_OpID_ANDDless,		//ANDD<	
	MEL_WPG_OpID_ANDDlessseq,	//ANDD<=	

	MEL_WPG_OpID_OReq,			//OR=
	MEL_WPG_OpID_ORnoteq,		//OR<>
	MEL_WPG_OpID_ORbigger,		//OR>
	MEL_WPG_OpID_ORbiggereq,	//OR>=
	MEL_WPG_OpID_ORless,		//OR<
	MEL_WPG_OpID_ORlessseq,		//OR<=

	MEL_WPG_OpID_ORDeq,			//ORD=
	MEL_WPG_OpID_ORDnoteq,		//ORD<>
	MEL_WPG_OpID_ORDbigger,		//ORD>
	MEL_WPG_OpID_ORDbiggereq,	//ORD>=	
	MEL_WPG_OpID_ORDless,		//ORD<	
	MEL_WPG_OpID_ORDlessseq,	//ORD<=
	
	//////////////////////////////////////////////////////////////////////////
	//"Data Table Operation",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_LIMIT,
	MEL_WPG_OpID_LIMITP,
	MEL_WPG_OpID_DLIMIT,
	MEL_WPG_OpID_DLIMITP,
	MEL_WPG_OpID_BAND,
	MEL_WPG_OpID_BANDP,
	MEL_WPG_OpID_DBAND,
	MEL_WPG_OpID_DBANDP,
	MEL_WPG_OpID_ZONE,
	MEL_WPG_OpID_ZONEP,
	MEL_WPG_OpID_DZONE,
	MEL_WPG_OpID_DZONEP,
	MEL_WPG_OpID_SCL,
	MEL_WPG_OpID_SCLP,
	MEL_WPG_OpID_DSCL,
	MEL_WPG_OpID_DSCLP,
	MEL_WPG_OpID_DABIN,
	MEL_WPG_OpID_DABINP,
	MEL_WPG_OpID_DDABIN,
	MEL_WPG_OpID_DDABINP,
	MEL_WPG_OpID_BINDA,
	MEL_WPG_OpID_BINDAP,
	MEL_WPG_OpID_DBINDA,
	MEL_WPG_OpID_DBINDAP,
	MEL_WPG_OpID_SCL2,
	MEL_WPG_OpID_SCL2P,
	MEL_WPG_OpID_DSCL2,
	MEL_WPG_OpID_DSCL2P,
	//////////////////////////////////////////////////////////////////////////
	//"External Device Communication (Inverter Comm.)"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_IVCK,
	MEL_WPG_OpID_IVDR,
	MEL_WPG_OpID_IVRD,
	MEL_WPG_OpID_IVWR,
	MEL_WPG_OpID_IVBWR,//FX3U/UC only
	MEL_WPG_OpID_IVMC,//FX3U/UC only
	//////////////////////////////////////////////////////////////////////////
	//"Data Transfer 3"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_RBFM,
	MEL_WPG_OpID_WBFM,
	//////////////////////////////////////////////////////////////////////////
	//"High Speed Processing"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_DHSCT,
	//////////////////////////////////////////////////////////////////////////
	//"Extension File Register Control"
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_LOADR,
	MEL_WPG_OpID_LOADRP,
	MEL_WPG_OpID_SAVER,
	MEL_WPG_OpID_INITR,
	MEL_WPG_OpID_INITRP,
	MEL_WPG_OpID_LOGR,
	MEL_WPG_OpID_LOGRP,
	MEL_WPG_OpID_RWER,
	MEL_WPG_OpID_RWERP,
	MEL_WPG_OpID_INITER,
	MEL_WPG_OpID_INITERP,
	//////////////////////////////////////////////////////////////////////////
	//"FX3U-CF-ADP Applied Instruction",
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_FLCRT,
	MEL_WPG_OpID_FLDEL,
	MEL_WPG_OpID_FLWR,
	MEL_WPG_OpID_FLRD,
	MEL_WPG_OpID_FLCMD,
	MEL_WPG_OpID_FLSTRD,
	//////////////////////////////////////////////////////////////////////////
	MEL_WPG_OpID_TOTALLY

}MEL_WPG_Opcodes;

static unsigned char INSTRUCTIONS_OPCODES_FXCPU[MEL_WPG_OpID_TOTALLY] = 
{
	0x00,	//MEL_WPG_OpID_LD
	0x01,	//MEL_WPG_OpID_LDI
	0x02,	//MEL_WPG_OpID_LDP
	0x03,	//MEL_WPG_OpID_LDF
	0x0C,	//MEL_WPG_OpID_AND
	0x0D,	//MEL_WPG_OpID_ANI
	0x0E,	//MEL_WPG_OpID_ANDP
	    0xFF,	//MEL_WPG_OpID_ANDF
	0x06,	//MEL_WPG_OpID_OR
	    0xFF,	//MEL_WPG_OpID_ORI
	    0xFF,	//MEL_WPG_OpID_ORP
	0x09,	//MEL_WPG_OpID_ORF
	0x19,	//MEL_WPG_OpID_ANB
	0x18,	//MEL_WPG_OpID_ORB
	0x1A,	//MEL_WPG_OpID_MPS
	0x1B,	//MEL_WPG_OpID_MRD
	0x1C,	//MEL_WPG_OpID_MPP
	    0xFF,	//MEL_WPG_OpID_INV
	    0xFF,	//MEL_WPG_OpID_MEP
	    0xFF,	//MEL_WPG_OpID_MEF
	0x20,	//MEL_WPG_OpID_OUT
	0x21,	//MEL_WPG_OpID_OUT_TC
	0x23,	//MEL_WPG_OpID_SET
	0x24,	//MEL_WPG_OpID_RST
	    0xFF,	//MEL_WPG_OpID_PLS
	    0xFF,	//MEL_WPG_OpID_PLF
	0x2C,	//MEL_WPG_OpID_MC
	0x30,	//MEL_WPG_OpID_MCR
	0xFF,	//MEL_WPG_OpID_NOP
	0x34,	//MEL_WPG_OpID_END
	0x4D,	//MEL_WPG_OpID_CJ
	    0xFF,	//MEL_WPG_OpID_CJP
	0x54,	//MEL_WPG_OpID_CALL
	    0xFF,	//MEL_WPG_OpID_CALLP
	0x6A,	//MEL_WPG_OpID_SRET
	0x69,	//MEL_WPG_OpID_IRET
	0x69,	//MEL_WPG_OpID_EI
	0x69,	//MEL_WPG_OpID_DI
	0x33,	//MEL_WPG_OpID_FEND
	    0xFF,	//MEL_WPG_OpID_WDT
	    0xFF,	//MEL_WPG_OpID_WDTP
	0x6A,	//MEL_WPG_OpID_FOR
	0x6A,	//MEL_WPG_OpID_NEXT
	    0xFF,	//MEL_WPG_OpID_CMP
	    0xFF,	//MEL_WPG_OpID_CMPP
	    0xFF,	//MEL_WPG_OpID_DCMP
	    0xFF,	//MEL_WPG_OpID_DCMPP
	    0xFF,	//MEL_WPG_OpID_ZCP
	    0xFF,	//MEL_WPG_OpID_ZCPP
	    0xFF,	//MEL_WPG_OpID_DZCP
	    0xFF,	//MEL_WPG_OpID_DZCPP
	0x4C,	//MEL_WPG_OpID_MOV
	    0xFF,	//MEL_WPG_OpID_MOVP
	0x4C,	//MEL_WPG_OpID_DMOV
	    0xFF,	//MEL_WPG_OpID_DMOVP
	    0xFF,	//MEL_WPG_OpID_SMOV
	    0xFF,	//MEL_WPG_OpID_SMOVP
	    0xFF,	//MEL_WPG_OpID_CML
	    0xFF,	//MEL_WPG_OpID_CMLP
	    0xFF,	//MEL_WPG_OpID_DCML
	    0xFF,	//MEL_WPG_OpID_DCMLP
	0x4C,	//MEL_WPG_OpID_BMOV
	    0xFF,	//MEL_WPG_OpID_BMOVP
	    0xFF,	//MEL_WPG_OpID_FMOV
	    0xFF,	//MEL_WPG_OpID_FMOVP
	    0xFF,	//MEL_WPG_OpID_DFMOV
	    0xFF,	//MEL_WPG_OpID_DFMOVP
	    0xFF,	//MEL_WPG_OpID_XCH
	    0xFF,	//MEL_WPG_OpID_XCHP
	    0xFF,	//MEL_WPG_OpID_DXCH
	    0xFF,	//MEL_WPG_OpID_DXCHP
	    0xFF,	//MEL_WPG_OpID_BCD
	    0xFF,	//MEL_WPG_OpID_BCDP
	    0xFF,	//MEL_WPG_OpID_DBCD
	    0xFF,	//MEL_WPG_OpID_DBCDP
	    0xFF,	//MEL_WPG_OpID_BIN
	    0xFF,	//MEL_WPG_OpID_BINP
	    0xFF,	//MEL_WPG_OpID_DBIN
	    0xFF,	//MEL_WPG_OpID_DBINP
	0x49,	//MEL_WPG_OpID_ADD
	    0xFF,	//MEL_WPG_OpID_ADDP
	    0xFF,	//MEL_WPG_OpID_DADD
	    0xFF,	//MEL_WPG_OpID_DADDP
	0x49,	//MEL_WPG_OpID_SUB
	    0xFF,	//MEL_WPG_OpID_SUBP
	0x49,	//MEL_WPG_OpID_DSUB
	    0xFF,	//MEL_WPG_OpID_DSUBP
	0x49,	//MEL_WPG_OpID_MUL
	    0xFF,	//MEL_WPG_OpID_MULP
	0x49,	//MEL_WPG_OpID_DMUL
	    0xFF,	//MEL_WPG_OpID_DMULP
	    0xFF,	//MEL_WPG_OpID_SUB
	    0xFF,	//MEL_WPG_OpID_SUBP
	    0xFF,	//MEL_WPG_OpID_DSUB
	    0xFF,	//MEL_WPG_OpID_DSUBP
	0x4A,	//MEL_WPG_OpID_INC
	    0xFF,	//MEL_WPG_OpID_INCP
	    0xFF,	//MEL_WPG_OpID_DINC
	    0xFF,	//MEL_WPG_OpID_DINCP
	0x4A,	//MEL_WPG_OpID_DEC
	    0xFF,	//MEL_WPG_OpID_DECP
	    0xFF,	//MEL_WPG_OpID_DDEC
	    0xFF,	//MEL_WPG_OpID_DDECP
	0x4F,	//MEL_WPG_OpID_WAND
	    0xFF,	//MEL_WPG_OpID_WANDP
	0x4F,	//MEL_WPG_OpID_DAND
	    0xFF,	//MEL_WPG_OpID_DANDP
	0x4F,	//MEL_WPG_OpID_WOR
	    0xFF,	//MEL_WPG_OpID_WORP
	0x4F,	//MEL_WPG_OpID_DOR
	    0xFF,	//MEL_WPG_OpID_DORP
	    0xFF,	//MEL_WPG_OpID_WXOR
	    0xFF,	//MEL_WPG_OpID_WXORP
	    0xFF,	//MEL_WPG_OpID_DXOR
	    0xFF,	//MEL_WPG_OpID_DXORP
	    0xFF,	//MEL_WPG_OpID_NEG
	    0xFF,	//MEL_WPG_OpID_NEGP
	0x4B,	//MEL_WPG_OpID_DNEG
	    0xFF,	//MEL_WPG_OpID_DNEGP
	    0xFF,	//MEL_WPG_OpID_ROR
	    0xFF,	//MEL_WPG_OpID_RORP
	    0xFF,	//MEL_WPG_OpID_DROR
	    0xFF,	//MEL_WPG_OpID_DRORP
	    0xFF,	//MEL_WPG_OpID_ROL
	    0xFF,	//MEL_WPG_OpID_ROLP
	0x50,	//MEL_WPG_OpID_DROL
	    0xFF,	//MEL_WPG_OpID_DROLP
	    0xFF,	//MEL_WPG_OpID_RCR
	    0xFF,	//MEL_WPG_OpID_RCRP
	    0xFF,	//MEL_WPG_OpID_DRCR
	    0xFF,	//MEL_WPG_OpID_DRCRP
	    0xFF,	//MEL_WPG_OpID_RCL
	    0xFF,	//MEL_WPG_OpID_RCLP
	    0xFF,	//MEL_WPG_OpID_DRCL
	    0xFF,	//MEL_WPG_OpID_DRCLP
	    0xFF,	//MEL_WPG_OpID_SFTR
	    0xFF,	//MEL_WPG_OpID_SFTRP
	    0xFF,	//MEL_WPG_OpID_SFTL
	    0xFF,	//MEL_WPG_OpID_SFTLP
	    0xFF,	//MEL_WPG_OpID_WSFR
	    0xFF,	//MEL_WPG_OpID_WSFRP
	    0xFF,	//MEL_WPG_OpID_WSFL
	    0xFF,	//MEL_WPG_OpID_WSFLP
	    0xFF,	//MEL_WPG_OpID_SFWR
	    0xFF,	//MEL_WPG_OpID_SFWRP
	    0xFF,	//MEL_WPG_OpID_SFRD
	    0xFF,	//MEL_WPG_OpID_SFRDP
	0x53,	//MEL_WPG_OpID_ZRST
	    0xFF,	//MEL_WPG_OpID_ZRSTP
	    0xFF,	//MEL_WPG_OpID_DECO
	    0xFF,	//MEL_WPG_OpID_DECOP
	    0xFF,	//MEL_WPG_OpID_ENCO
	    0xFF,	//MEL_WPG_OpID_ENCOP
	    0xFF,	//MEL_WPG_OpID_SUM
	    0xFF,	//MEL_WPG_OpID_SUMP
	    0xFF,	//MEL_WPG_OpID_DSUM
	    0xFF,	//MEL_WPG_OpID_DSUMP
	    0xFF,	//MEL_WPG_OpID_BON
	    0xFF,	//MEL_WPG_OpID_BONP
	    0xFF,	//MEL_WPG_OpID_DBON
	    0xFF,	//MEL_WPG_OpID_DBONP
	    0xFF,	//MEL_WPG_OpID_MEAN
	    0xFF,	//MEL_WPG_OpID_MEANP
	    0xFF,	//MEL_WPG_OpID_DMEAN
	    0xFF,	//MEL_WPG_OpID_DMEANP
	    0xFF,	//MEL_WPG_OpID_ANS
	    0xFF,	//MEL_WPG_OpID_ANR
	    0xFF,	//MEL_WPG_OpID_ANRP
	    0xFF,	//MEL_WPG_OpID_SQR
	    0xFF,	//MEL_WPG_OpID_SQRP
	    0xFF,	//MEL_WPG_OpID_DSQR
	    0xFF,	//MEL_WPG_OpID_DSQRP
	 0x53,	//MEL_WPG_OpID_FLT
	    0xFF,	//MEL_WPG_OpID_FLTP
	 0x53,	//MEL_WPG_OpID_DFLT
	    0xFF,	//MEL_WPG_OpID_DFLTP
	    0xFF,	//MEL_WPG_OpID_REF
	    0xFF,	//MEL_WPG_OpID_REFP
	    0xFF,	//MEL_WPG_OpID_REFF
	    0xFF,	//MEL_WPG_OpID_REFFP
	    0xFF,	//MEL_WPG_OpID_MTR
	    0xFF,	//MEL_WPG_OpID_DHSCS
	    0xFF,	//MEL_WPG_OpID_DHSCR
	    0xFF,	//MEL_WPG_OpID_DHSZ
	    0xFF,	//MEL_WPG_OpID_SPD
	    0xFF,	//MEL_WPG_OpID_DSPD
	    0xFF,	//MEL_WPG_OpID_PLSY
	    0xFF,	//MEL_WPG_OpID_DPLSY
	    0xFF,	//MEL_WPG_OpID_PWM
	    0xFF,	//MEL_WPG_OpID_PLSR
	    0xFF,	//MEL_WPG_OpID_DPLSR
	    0xFF,	//MEL_WPG_OpID_IST
	    0xFF,	//MEL_WPG_OpID_SER
	    0xFF,	//MEL_WPG_OpID_SERP
	    0xFF,	//MEL_WPG_OpID_DSER
	    0xFF,	//MEL_WPG_OpID_DSERP
	    0xFF,	//MEL_WPG_OpID_ABSD
	    0xFF,	//MEL_WPG_OpID_DABSD
	    0xFF,	//MEL_WPG_OpID_INCD
	    0xFF,	//MEL_WPG_OpID_TTMR
	    0xFF,	//MEL_WPG_OpID_STMR
	    0xFF,	//MEL_WPG_OpID_ALT
	    0xFF,	//MEL_WPG_OpID_ALTP
	    0xFF,	//MEL_WPG_OpID_RAMP
	    0xFF,	//MEL_WPG_OpID_ROTC
	    0xFF,	//MEL_WPG_OpID_SORT
	    0xFF,	//MEL_WPG_OpID_TKY
	    0xFF,	//MEL_WPG_OpID_DTKY
	    0xFF,	//MEL_WPG_OpID_HKY
	    0xFF,	//MEL_WPG_OpID_DHKY
	    0xFF,	//MEL_WPG_OpID_DSW
	    0xFF,	//MEL_WPG_OpID_SEGD
	    0xFF,	//MEL_WPG_OpID_SEGDP
	    0xFF,	//MEL_WPG_OpID_SEGL
	    0xFF,	//MEL_WPG_OpID_ARWS
	    0xFF,	//MEL_WPG_OpID_ASC
	    0xFF,	//MEL_WPG_OpID_PR
	0x56,	//MEL_WPG_OpID_FROM
	    0xFF,	//MEL_WPG_OpID_FROMP
	    0xFF,	//MEL_WPG_OpID_DFROM
	    0xFF,	//MEL_WPG_OpID_DFROMP
	0x56,	//MEL_WPG_OpID_TO
	    0xFF,	//MEL_WPG_OpID_TOP
	    0xFF,	//MEL_WPG_OpID_DTO
	    0xFF,	//MEL_WPG_OpID_DTOP
	    0xFF,	//MEL_WPG_OpID_RS
	    0xFF,	//MEL_WPG_OpID_PRUN
	    0xFF,	//MEL_WPG_OpID_PRUNP
	    0xFF,	//MEL_WPG_OpID_DPRUN
	    0xFF,	//MEL_WPG_OpID_DPRUNP
	    0xFF,	//MEL_WPG_OpID_ASCI
	    0xFF,	//MEL_WPG_OpID_ASCIP
	    0xFF,	//MEL_WPG_OpID_HEX
	    0xFF,	//MEL_WPG_OpID_HEXP
	    0xFF,	//MEL_WPG_OpID_CCD
	    0xFF,	//MEL_WPG_OpID_CCDP
	    0xFF,	//MEL_WPG_OpID_VRRD
	    0xFF,	//MEL_WPG_OpID_VRRDP
	    0xFF,	//MEL_WPG_OpID_VRSC
	    0xFF,	//MEL_WPG_OpID_VRSCP
	0x5F,	//MEL_WPG_OpID_RS2
	    0xFF,	//MEL_WPG_OpID_PID
	    0xFF,	//MEL_WPG_OpID_ZPUSH
	    0xFF,	//MEL_WPG_OpID_ZPUSHP
	    0xFF,	//MEL_WPG_OpID_ZPOP
	    0xFF,	//MEL_WPG_OpID_ZPOPP
	    0xFF,	//MEL_WPG_OpID_DECMP
	    0xFF,	//MEL_WPG_OpID_DECMPP
	    0xFF,	//MEL_WPG_OpID_DEZCP
	    0xFF,	//MEL_WPG_OpID_DEZCPP
	0x4C,	//MEL_WPG_OpID_DEMOV
	    0xFF,	//MEL_WPG_OpID_DEMOVP
	    0xFF,	//MEL_WPG_OpID_DESTR
	    0xFF,	//MEL_WPG_OpID_DESTRP
	    0xFF,	//MEL_WPG_OpID_DEVAL
	    0xFF,	//MEL_WPG_OpID_DEVALP
	    0xFF,	//MEL_WPG_OpID_DEBCD
	    0xFF,	//MEL_WPG_OpID_DEBCDP
	    0xFF,	//MEL_WPG_OpID_DEBIN
	    0xFF,	//MEL_WPG_OpID_DEBINP
	0x49,	//MEL_WPG_OpID_DEADD
	    0xFF,	//MEL_WPG_OpID_DEADDP
	0x49,	//MEL_WPG_OpID_DESUB
	    0xFF,	//MEL_WPG_OpID_DESUBP
	0x49,	//MEL_WPG_OpID_DEMUL
	    0xFF,	//MEL_WPG_OpID_DEMULP
	0x49,	//MEL_WPG_OpID_DEDIV
	    0xFF,	//MEL_WPG_OpID_DEDIVP
	    0xFF,	//MEL_WPG_OpID_DEXP
	    0xFF,	//MEL_WPG_OpID_DEXPP
	    0xFF,	//MEL_WPG_OpID_DLOGE
	    0xFF,	//MEL_WPG_OpID_DLOGEP
	    0xFF,	//MEL_WPG_OpID_DLOG10
	    0xFF,	//MEL_WPG_OpID_DLOG10P
	    0xFF,	//MEL_WPG_OpID_DESQR
	    0xFF,	//MEL_WPG_OpID_DESQRP
	    0xFF,	//MEL_WPG_OpID_DENEG
	    0xFF,	//MEL_WPG_OpID_DENEGP
	    0xFF,	//MEL_WPG_OpID_INT
	    0xFF,	//MEL_WPG_OpID_INTP
	0x4B,	//MEL_WPG_OpID_DINT
	    0xFF,	//MEL_WPG_OpID_DINTP
	    0xFF,	//MEL_WPG_OpID_DSIN
	    0xFF,	//MEL_WPG_OpID_DSINP
	    0xFF,	//MEL_WPG_OpID_DCOS
	    0xFF,	//MEL_WPG_OpID_DCOSP
	    0xFF,	//MEL_WPG_OpID_DTAN
	    0xFF,	//MEL_WPG_OpID_DTANP
	    0xFF,	//MEL_WPG_OpID_DASIN
	    0xFF,	//MEL_WPG_OpID_DASINP
	    0xFF,	//MEL_WPG_OpID_DACOS
	    0xFF,	//MEL_WPG_OpID_DACOSP
	    0xFF,	//MEL_WPG_OpID_DATAN
	    0xFF,	//MEL_WPG_OpID_DATANP
	    0xFF,	//MEL_WPG_OpID_DRAD
	    0xFF,	//MEL_WPG_OpID_DRADP
	    0xFF,	//MEL_WPG_OpID_DDEG
	    0xFF,	//MEL_WPG_OpID_DDEGP
	    0xFF,	//MEL_WPG_OpID_WSUM
	    0xFF,	//MEL_WPG_OpID_WSUMP
	    0xFF,	//MEL_WPG_OpID_DWSUM
	    0xFF,	//MEL_WPG_OpID_DWSUMP
	    0xFF,	//MEL_WPG_OpID_WTOB
	    0xFF,	//MEL_WPG_OpID_WTOBP
	    0xFF,	//MEL_WPG_OpID_BTOW
	    0xFF,	//MEL_WPG_OpID_BTOWP
	    0xFF,	//MEL_WPG_OpID_UNI
	    0xFF,	//MEL_WPG_OpID_UNIP
	    0xFF,	//MEL_WPG_OpID_DIS
	    0xFF,	//MEL_WPG_OpID_DISP
	    0xFF,	//MEL_WPG_OpID_SWAP
	    0xFF,	//MEL_WPG_OpID_SWAPP
	    0xFF,	//MEL_WPG_OpID_DSWAP
	    0xFF,	//MEL_WPG_OpID_DSWAPP
	    0xFF,	//MEL_WPG_OpID_SORT2
	    0xFF,	//MEL_WPG_OpID_DSORT2
	    0xFF,	//MEL_WPG_OpID_DSZR
	    0xFF,	//MEL_WPG_OpID_DVIT
	    0xFF,	//MEL_WPG_OpID_DDVIT
	    0xFF,	//MEL_WPG_OpID_DTBL
	    0xFF,	//MEL_WPG_OpID_DABS
	    0xFF,	//MEL_WPG_OpID_ZRN
	    0xFF,	//MEL_WPG_OpID_DZRN
	    0xFF,	//MEL_WPG_OpID_PLSV
	    0xFF,	//MEL_WPG_OpID_DPLSV
	    0xFF,	//MEL_WPG_OpID_DRVI
	    0xFF,	//MEL_WPG_OpID_DDRVI
	    0xFF,	//MEL_WPG_OpID_DRVA
	    0xFF,	//MEL_WPG_OpID_DDRVA
	    0xFF,	//MEL_WPG_OpID_TCMP
	    0xFF,	//MEL_WPG_OpID_TCMPP
	    0xFF,	//MEL_WPG_OpID_TZCP
	    0xFF,	//MEL_WPG_OpID_TZCPP
	    0xFF,	//MEL_WPG_OpID_TADD
	    0xFF,	//MEL_WPG_OpID_TADDP
	    0xFF,	//MEL_WPG_OpID_TSUB
	    0xFF,	//MEL_WPG_OpID_TSUBP
	    0xFF,	//MEL_WPG_OpID_HTOS
	    0xFF,	//MEL_WPG_OpID_HTOSP
	    0xFF,	//MEL_WPG_OpID_DHTOS
	    0xFF,	//MEL_WPG_OpID_DHTOSP
	    0xFF,	//MEL_WPG_OpID_STOH
	    0xFF,	//MEL_WPG_OpID_STOHP
	    0xFF,	//MEL_WPG_OpID_DSTOH
	    0xFF,	//MEL_WPG_OpID_DSTOHP
	    0xFF,	//MEL_WPG_OpID_TRD
	    0xFF,	//MEL_WPG_OpID_TRDP
	    0xFF,	//MEL_WPG_OpID_TWR
	    0xFF,	//MEL_WPG_OpID_TWRP
	    0xFF,	//MEL_WPG_OpID_HOUR
	    0xFF,	//MEL_WPG_OpID_DHOUR
	    0xFF,	//MEL_WPG_OpID_GRY
	    0xFF,	//MEL_WPG_OpID_GRYP
	    0xFF,	//MEL_WPG_OpID_DGRY
	    0xFF,	//MEL_WPG_OpID_DGRYP
	    0xFF,	//MEL_WPG_OpID_GBIN
	    0xFF,	//MEL_WPG_OpID_GBINP
	    0xFF,	//MEL_WPG_OpID_DGBIN
	    0xFF,	//MEL_WPG_OpID_DGBINP
	    0xFF,	//MEL_WPG_OpID_RD3A
	    0xFF,	//MEL_WPG_OpID_RD3AP
	    0xFF,	//MEL_WPG_OpID_WR3A
	    0xFF,	//MEL_WPG_OpID_WR3AP
	    0xFF,	//MEL_WPG_OpID_EXTR
	    0xFF,	//MEL_WPG_OpID_COMRD
	    0xFF,	//MEL_WPG_OpID_COMRDP
	    0xFF,	//MEL_WPG_OpID_RND
	    0xFF,	//MEL_WPG_OpID_RNDP
	    0xFF,	//MEL_WPG_OpID_DUTY
	    0xFF,	//MEL_WPG_OpID_CRC
	    0xFF,	//MEL_WPG_OpID_CRCP
	    0xFF,	//MEL_WPG_OpID_DHCMOV
	    0xFF,	//MEL_WPG_OpID_BK+
	    0xFF,	//MEL_WPG_OpID_BK+P
	    0xFF,	//MEL_WPG_OpID_DBK+
	    0xFF,	//MEL_WPG_OpID_DBK+P
	    0xFF,	//MEL_WPG_OpID_BK-
	    0xFF,	//MEL_WPG_OpID_BK-P
	    0xFF,	//MEL_WPG_OpID_DBK-
	    0xFF,	//MEL_WPG_OpID_DBK-P
	    0xFF,	//MEL_WPG_OpID_BKCMP=
	    0xFF,	//MEL_WPG_OpID_BKCMP=P
	    0xFF,	//MEL_WPG_OpID_DBKCMP=
	    0xFF,	//MEL_WPG_OpID_DBKCMP=P
	    0xFF,	//MEL_WPG_OpID_BKCMP>
	    0xFF,	//MEL_WPG_OpID_BKCMP>P
	    0xFF,	//MEL_WPG_OpID_DBKCMP>
	    0xFF,	//MEL_WPG_OpID_DBKCMP>P
	    0xFF,	//MEL_WPG_OpID_BKCMP<
	    0xFF,	//MEL_WPG_OpID_BKCMP<P
	    0xFF,	//MEL_WPG_OpID_DBKCMP<
	    0xFF,	//MEL_WPG_OpID_DBKCMP<P
	    0xFF,	//MEL_WPG_OpID_BKCMP<>
	    0xFF,	//MEL_WPG_OpID_BKCMP<>P
	    0xFF,	//MEL_WPG_OpID_DBKCMP<>
	    0xFF,	//MEL_WPG_OpID_DBKCMP<>P
	    0xFF,	//MEL_WPG_OpID_BKCMP<=
	    0xFF,	//MEL_WPG_OpID_BKCMP<=P
	    0xFF,	//MEL_WPG_OpID_DBKCMP<=
	    0xFF,	//MEL_WPG_OpID_DBKCMP<=P
	    0xFF,	//MEL_WPG_OpID_BKCMP>=
	    0xFF,	//MEL_WPG_OpID_BKCMP>=P
	    0xFF,	//MEL_WPG_OpID_DBKCMP>=
	    0xFF,	//MEL_WPG_OpID_DBKCMP>=P
	    0xFF,	//MEL_WPG_OpID_STR
	    0xFF,	//MEL_WPG_OpID_STRP
	    0xFF,	//MEL_WPG_OpID_DSTR
	    0xFF,	//MEL_WPG_OpID_DSTRP
	    0xFF,	//MEL_WPG_OpID_VAL
	    0xFF,	//MEL_WPG_OpID_VALP
	    0xFF,	//MEL_WPG_OpID_DVAL
	    0xFF,	//MEL_WPG_OpID_DVALP
	0x49,	//MEL_WPG_OpID_linkstr		(!)RANGED: [    0xFF49[07]23...    0xFF49[25]23]
	    0xFF,	//MEL_WPG_OpID_$+P_
	    0xFF,	//MEL_WPG_OpID_LEN
	    0xFF,	//MEL_WPG_OpID_LENP
	    0xFF,	//MEL_WPG_OpID_RIGHT
	    0xFF,	//MEL_WPG_OpID_RIGHTP
	    0xFF,	//MEL_WPG_OpID_LEFT
	    0xFF,	//MEL_WPG_OpID_LEFTP
	    0xFF,	//MEL_WPG_OpID_MIDR
	    0xFF,	//MEL_WPG_OpID_MIDRP
	    0xFF,	//MEL_WPG_OpID_MIDW
	    0xFF,	//MEL_WPG_OpID_MIDWP
	    0xFF,	//MEL_WPG_OpID_INSTR
	    0xFF,	//MEL_WPG_OpID_INSTRP
	    0xFF,	//MEL_WPG_OpID_$MOV
	    0xFF,	//MEL_WPG_OpID_$MOVP
	    0xFF,	//MEL_WPG_OpID_FDEL
	    0xFF,	//MEL_WPG_OpID_FDELP
	    0xFF,	//MEL_WPG_OpID_FINS
	    0xFF,	//MEL_WPG_OpID_FINSP
	    0xFF,	//MEL_WPG_OpID_POP
	    0xFF,	//MEL_WPG_OpID_POPP
	0x51,	//MEL_WPG_OpID_SFR
	    0xFF,	//MEL_WPG_OpID_SFRP
	0x51,	//MEL_WPG_OpID_SFL
	    0xFF,	//MEL_WPG_OpID_SFLP

	0x40,	//MEL_WPG_OpID_LD=
	0x40,	//MEL_WPG_OpID_LD<>
	0x40,	//MEL_WPG_OpID_LD>
	0x40,	//MEL_WPG_OpID_LD>=
	0x40,	//MEL_WPG_OpID_LD<
	0x40,	//MEL_WPG_OpID_LD<=

	0x40,	//MEL_WPG_OpID_LDD=	
	0x40,	//MEL_WPG_OpID_LDD<>
	0x40,	//MEL_WPG_OpID_LDD>
	0x40,	//MEL_WPG_OpID_LDD>=
	0x40,	//MEL_WPG_OpID_LDD<	
	0x40,	//MEL_WPG_OpID_LDD<=	
	
	0x40,	//MEL_WPG_OpID_AND=
	0x40,	//MEL_WPG_OpID_AND<>
	0x40,	//MEL_WPG_OpID_AND>
	0x40,	//MEL_WPG_OpID_AND>=
	0x40,	//MEL_WPG_OpID_AND<
	0x40,	//MEL_WPG_OpID_AND<=

	0x40,	//MEL_WPG_OpID_ANDD=
	0x40,	//MEL_WPG_OpID_ANDD<>		
	0x40,	//MEL_WPG_OpID_ANDD>
	0x40,	//MEL_WPG_OpID_ANDD>=
	0x40,	//MEL_WPG_OpID_ANDD<	
	0x40,	//MEL_WPG_OpID_ANDD<=	
	
	0x40,	//MEL_WPG_OpID_OR=
	0x40,	//MEL_WPG_OpID_OR<>	
	0x40,	//MEL_WPG_OpID_OR>
	0x40,	//MEL_WPG_OpID_OR>=
	0x40,	//MEL_WPG_OpID_OR<	
	0x40,	//MEL_WPG_OpID_OR<=

	0x40,	//MEL_WPG_OpID_ORD=
	0x40,	//MEL_WPG_OpID_ORD<>	
	0x40,	//MEL_WPG_OpID_ORD>
	0x40,	//MEL_WPG_OpID_ORD>=
	0x40,	//MEL_WPG_OpID_ORD<
	0x40,	//MEL_WPG_OpID_ORD<=
	
	
	    0xFF,	//MEL_WPG_OpID_LIMIT
	    0xFF,	//MEL_WPG_OpID_LIMITP
	    0xFF,	//MEL_WPG_OpID_DLIMIT
	    0xFF,	//MEL_WPG_OpID_DLIMITP
	    0xFF,	//MEL_WPG_OpID_BAND
	    0xFF,	//MEL_WPG_OpID_BANDP
	    0xFF,	//MEL_WPG_OpID_DBAND
	    0xFF,	//MEL_WPG_OpID_DBANDP
	    0xFF,	//MEL_WPG_OpID_ZONE
	    0xFF,	//MEL_WPG_OpID_ZONEP
	    0xFF,	//MEL_WPG_OpID_DZONE
	    0xFF,	//MEL_WPG_OpID_DZONEP
	    0xFF,	//MEL_WPG_OpID_SCL
	    0xFF,	//MEL_WPG_OpID_SCLP
	    0xFF,	//MEL_WPG_OpID_DSCL
	    0xFF,	//MEL_WPG_OpID_DSCLP
	    0xFF,	//MEL_WPG_OpID_DABIN
	    0xFF,	//MEL_WPG_OpID_DABINP
	    0xFF,	//MEL_WPG_OpID_DDABIN
	    0xFF,	//MEL_WPG_OpID_DDABINP
	    0xFF,	//MEL_WPG_OpID_BINDA
	    0xFF,	//MEL_WPG_OpID_BINDAP
	    0xFF,	//MEL_WPG_OpID_DBINDA
	    0xFF,	//MEL_WPG_OpID_DBINDAP
	    0xFF,	//MEL_WPG_OpID_SCL2
	    0xFF,	//MEL_WPG_OpID_SCL2P
	    0xFF,	//MEL_WPG_OpID_DSCL2
	    0xFF,	//MEL_WPG_OpID_DSCL2P
	    0xFF,	//MEL_WPG_OpID_IVCK
	    0xFF,	//MEL_WPG_OpID_IVDR
	    0xFF,	//MEL_WPG_OpID_IVRD
	    0xFF,	//MEL_WPG_OpID_IVWR
	    0xFF,	//MEL_WPG_OpID_IVBWR
	    0xFF,	//MEL_WPG_OpID_IVMC
	    0xFF,	//MEL_WPG_OpID_RBFM
	    0xFF,	//MEL_WPG_OpID_WBFM
	    0xFF,	//MEL_WPG_OpID_DHSCT
	    0xFF,	//MEL_WPG_OpID_LOADR
	    0xFF,	//MEL_WPG_OpID_LOADRP
	    0xFF,	//MEL_WPG_OpID_SAVER
	    0xFF,	//MEL_WPG_OpID_INITR
	    0xFF,	//MEL_WPG_OpID_INITRP
	    0xFF,	//MEL_WPG_OpID_LOGR
	    0xFF,	//MEL_WPG_OpID_LOGRP
	    0xFF,	//MEL_WPG_OpID_RWER
	    0xFF,	//MEL_WPG_OpID_RWERP
	    0xFF,	//MEL_WPG_OpID_INITER
	    0xFF,	//MEL_WPG_OpID_INITERP
	    0xFF,	//MEL_WPG_OpID_FLCRT
	    0xFF,	//MEL_WPG_OpID_FLDEL
	    0xFF,	//MEL_WPG_OpID_FLWR
	    0xFF,	//MEL_WPG_OpID_FLRD
	    0xFF,	//MEL_WPG_OpID_FLCMD
	    0xFF	//MEL_WPG_OpID_FLSTRD
};

static WPGOpNameRecord WPG_OPNAMES[MEL_WPG_OpID_TOTALLY] =
{
	{"LD",         2},
	{"LDI",        3},
	{"LDP",        3},
	{"LDF",        3},
	{"AND",        3},
	{"ANI",        3},
	{"ANDP",       4},
	{"ANDF",       4},
	{"OR",         2},
	{"ORI",        3},
	{"ORP",        3},
	{"ORF",        3},
	{"ANB",        3},
	{"ORB",        3},
	{"MPS",        3},
	{"MRD",        3},
	{"MPP",        3},
	{"INV",        3},
	{"MEP",        3},
	{"MEF",        3},
	{"OUT",        3},
	{"OUT",        3},
	{"SET",        3},
	{"RST",        3},
	{"PLS",        3},
	{"PLF",        3},
	{"MC",         2},
	{"MCR",        3},
	{"NOP",        3},
	{"END",        3},
	{"CJ",         2},
	{"CJP",        3},
	{"CALL",       4},
	{"CALLP",      5},
	{"SRET",       4},
	{"IRET",       4},
	{"EI",         2},
	{"DI",         2},
	{"FEND",       4},
	{"WDT",        3},
	{"WDTP",       4},
	{"FOR",        3},
	{"NEXT",       4},
	{"CMP",        3},
	{"CMPP",       4},
	{"DCMP",       4},
	{"DCMPP",      5},
	{"ZCP",        3},
	{"ZCPP",       4},
	{"DZCP",       4},
	{"DZCPP",      5},
	{"MOV",        3},
	{"MOVP",       4},
	{"DMOV",       4},
	{"DMOVP",      5},
	{"SMOV",       4},
	{"SMOVP",      5},
	{"CML",        3},
	{"CMLP",       4},
	{"DCML",       4},
	{"DCMLP",      5},
	{"BMOV",       4},
	{"BMOVP",      5},
	{"FMOV",       4},
	{"FMOVP",      5},
	{"DFMOV",      5},
	{"DFMOVP",     6},
	{"XCH",        3},
	{"XCHP",       4},
	{"DXCH",       4},
	{"DXCHP",      5},
	{"BCD",        3},
	{"BCDP",       4},
	{"DBCD",       4},
	{"DBCDP",      5},
	{"BIN",        3},
	{"BINP",       4},
	{"DBIN",       4},
	{"DBINP",      5},
	{"ADD",        3},
	{"ADDP",       4},
	{"DADD",       4},
	{"DADDP",      5},
	{"SUB",        3},
	{"SUBP",       4},
	{"DSUB",       4},
	{"DSUBP",      5},
	{"MUL",        3},
	{"MULP",       4},
	{"DMUL",       4},
	{"DMULP",      5},
	{"SUB",        3},
	{"SUBP",       4},
	{"DSUB",       4},
	{"DSUBP",      5},
	{"INC",        3},
	{"INCP",       4},
	{"DINC",       4},
	{"DINCP",      5},
	{"DEC",        3},
	{"DECP",       4},
	{"DDEC",       4},
	{"DDECP",      5},
	{"WAND",       4},
	{"WANDP",      5},
	{"DAND",       4},
	{"DANDP",      5},
	{"WOR",        3},
	{"WORP",       4},
	{"DOR",        3},
	{"DORP",       4},
	{"WXOR",       4},
	{"WXORP",      5},
	{"DXOR",       4},
	{"DXORP",      5},
	{"NEG",        3},
	{"NEGP",       4},
	{"DNEG",       4},
	{"DNEGP",      5},
	{"ROR",        3},
	{"RORP",       4},
	{"DROR",       4},
	{"DRORP",      5},
	{"ROL",        3},
	{"ROLP",       4},
	{"DROL",       4},
	{"DROLP",      5},
	{"RCR",        3},
	{"RCRP",       4},
	{"DRCR",       4},
	{"DRCRP",      5},
	{"RCL",        3},
	{"RCLP",       4},
	{"DRCL",       4},
	{"DRCLP",      5},
	{"SFTR",       4},
	{"SFTRP",      5},
	{"SFTL",       4},
	{"SFTLP",      5},
	{"WSFR",       4},
	{"WSFRP",      5},
	{"WSFL",       4},
	{"WSFLP",      5},
	{"SFWR",       4},
	{"SFWRP",      5},
	{"SFRD",       4},
	{"SFRDP",      5},
	{"ZRST",       4},
	{"ZRSTP",      5},
	{"DECO",       4},
	{"DECOP",      5},
	{"ENCO",       4},
	{"ENCOP",      5},
	{"SUM",        3},
	{"SUMP",       4},
	{"DSUM",       4},
	{"DSUMP",      5},
	{"BON",        3},
	{"BONP",       4},
	{"DBON",       4},
	{"DBONP",      5},
	{"MEAN",       4},
	{"MEANP",      5},
	{"DMEAN",      5},
	{"DMEANP",     6},
	{"ANS",        3},
	{"ANR",        3},
	{"ANRP",       4},
	{"SQR",        3},
	{"SQRP",       4},
	{"DSQR",       4},
	{"DSQRP",      5},
	{"FLT",        3},
	{"FLTP",       4},
	{"DFLT",       4},
	{"DFLTP",      5},
	{"REF",        3},
	{"REFP",       4},
	{"REFF",       4},
	{"REFFP",      5},
	{"MTR",        3},
	{"DHSCS",      5},
	{"DHSCR",      5},
	{"DHSZ",       4},
	{"SPD",        3},
	{"DSPD",       4},
	{"PLSY",       4},
	{"DPLSY",      5},
	{"PWM",        3},
	{"PLSR",       4},
	{"DPLSR",      5},
	{"IST",        3},
	{"SER",        3},
	{"SERP",       4},
	{"DSER",       4},
	{"DSERP",      5},
	{"ABSD",       4},
	{"DABSD",      5},
	{"INCD",       4},
	{"TTMR",       4},
	{"STMR",       4},
	{"ALT",        3},
	{"ALTP",       4},
	{"RAMP",       4},
	{"ROTC",       4},
	{"SORT",       4},
	{"TKY",        3},
	{"DTKY",       4},
	{"HKY",        3},
	{"DHKY",       4},
	{"DSW",        3},
	{"SEGD",       4},
	{"SEGDP",      5},
	{"SEGL",       4},
	{"ARWS",       4},
	{"ASC",        3},
	{"PR",         2},
	{"FROM",       4},
	{"FROMP",      5},
	{"DFROM",      5},
	{"DFROMP",     6},
	{"TO",         2},
	{"TOP",        3},
	{"DTO",        3},
	{"DTOP",       4},
	{"RS",         2},
	{"PRUN",       4},
	{"PRUNP",      5},
	{"DPRUN",      5},
	{"DPRUNP",     6},
	{"ASCI",       4},
	{"ASCIP",      5},
	{"HEX",        3},
	{"HEXP",       4},
	{"CCD",        3},
	{"CCDP",       4},
	{"VRRD",       4},
	{"VRRDP",      5},
	{"VRSC",       4},
	{"VRSCP",      5},
	{"RS2",        3},
	{"PID",        3},
	{"ZPUSH",      5},
	{"ZPUSHP",     6},
	{"ZPOP",       4},
	{"ZPOPP",      5},
	{"DECMP",      5},
	{"DECMPP",     6},
	{"DEZCP",      5},
	{"DEZCPP",     6},
	{"DEMOV",      5},
	{"DEMOVP",     6},
	{"DESTR",      5},
	{"DESTRP",     6},
	{"DEVAL",      5},
	{"DEVALP",     6},
	{"DEBCD",      5},
	{"DEBCDP",     6},
	{"DEBIN",      5},
	{"DEBINP",     6},
	{"DEADD",      5},
	{"DEADDP",     6},
	{"DESUB",      5},
	{"DESUBP",     6},
	{"DEMUL",      5},
	{"DEMULP",     6},
	{"DEDIV",      5},
	{"DEDIVP",     6},
	{"DEXP",       4},
	{"DEXPP",      5},
	{"DLOGE",      5},
	{"DLOGEP",     6},
	{"DLOG10",     6},
	{"DLOG10P",    7},
	{"DESQR",      5},
	{"DESQRP",     6},
	{"DENEG",      5},
	{"DENEGP",     6},
	{"INT",        3},
	{"INTP",       4},
	{"DINT",       4},
	{"DINTP",      5},
	{"DSIN",       4},
	{"DSINP",      5},
	{"DCOS",       4},
	{"DCOSP",      5},
	{"DTAN",       4},
	{"DTANP",      5},
	{"DASIN",      5},
	{"DASINP",     6},
	{"DACOS",      5},
	{"DACOSP",     6},
	{"DATAN",      5},
	{"DATANP",     6},
	{"DRAD",       4},
	{"DRADP",      5},
	{"DDEG",       4},
	{"DDEGP",      5},
	{"WSUM",       4},
	{"WSUMP",      5},
	{"DWSUM",      5},
	{"DWSUMP",     6},
	{"WTOB",       4},
	{"WTOBP",      5},
	{"BTOW",       4},
	{"BTOWP",      5},
	{"UNI",        3},
	{"UNIP",       4},
	{"DIS",        3},
	{"DISP",       4},
	{"SWAP",       4},
	{"SWAPP",      5},
	{"DSWAP",      5},
	{"DSWAPP",     6},
	{"SORT2",      5},
	{"DSORT2",     6},
	{"DSZR",       4},
	{"DVIT",       4},
	{"DDVIT",      5},
	{"DTBL",       4},
	{"DABS",       4},
	{"ZRN",        3},
	{"DZRN",       4},
	{"PLSV",       4},
	{"DPLSV",      5},
	{"DRVI",       4},
	{"DDRVI",      5},
	{"DRVA",       4},
	{"DDRVA",      5},
	{"TCMP",       4},
	{"TCMPP",      5},
	{"TZCP",       4},
	{"TZCPP",      5},
	{"TADD",       4},
	{"TADDP",      5},
	{"TSUB",       4},
	{"TSUBP",      5},
	{"HTOS",       4},
	{"HTOSP",      5},
	{"DHTOS",      5},
	{"DHTOSP",     6},
	{"STOH",       4},
	{"STOHP",      5},
	{"DSTOH",      5},
	{"DSTOHP",     6},
	{"TRD",        3},
	{"TRDP",       4},
	{"TWR",        3},
	{"TWRP",       4},
	{"HOUR",       4},
	{"DHOUR",      5},
	{"GRY",        3},
	{"GRYP",       4},
	{"DGRY",       4},
	{"DGRYP",      5},
	{"GBIN",       4},
	{"GBINP",      5},
	{"DGBIN",      5},
	{"DGBINP",     6},
	{"RD3A",       4},
	{"RD3AP",      5},
	{"WR3A",       4},
	{"WR3AP",      5},
	{"EXTR",       4},
	{"COMRD",      5},
	{"COMRDP",     6},
	{"RND",        3},
	{"RNDP",       4},
	{"DUTY",       4},
	{"CRC",        3},
	{"CRCP",       4},
	{"DHCMOV",     6},
	{"BK+",        3},
	{"BK+P",       4},
	{"DBK+",       4},
	{"DBK+P",      5},
	{"BK-",        3},
	{"BK-P",       4},
	{"DBK-",       4},
	{"DBK-P",      5},
	{"BKCMP=",     6},
	{"BKCMP=P",    7},
	{"DBKCMP=",    7},
	{"DBKCMP=P",   8},
	{"BKCMP>",     6},
	{"BKCMP>P",    7},
	{"DBKCMP>",    7},
	{"DBKCMP>P",   8},
	{"BKCMP<",     6},
	{"BKCMP<P",    7},
	{"DBKCMP<",    7},
	{"DBKCMP<P",   8},
	{"BKCMP<>",    7},
	{"BKCMP<>P",   8},
	{"DBKCMP<>",   8},
	{"DBKCMP<>P",  9},
	{"BKCMP<=",    7},
	{"BKCMP<=P",   8},
	{"DBKCMP<=",   8},
	{"DBKCMP<=P",  9},
	{"BKCMP>=",    7},
	{"BKCMP>=P",   8},
	{"DBKCMP>=",   8},
	{"DBKCMP>=P",  9},
	{"STR",        3},
	{"STRP",       4},
	{"DSTR",       4},
	{"DSTRP",      5},
	{"VAL",        3},
	{"VALP",       4},
	{"DVAL",       4},
	{"DVALP",      5},
	{"$+",         2},
	{"$+P",        3},
	{"LEN",        3},
	{"LENP",       4},
	{"RIGHT",      5},
	{"RIGHTP",     6},
	{"LEFT",       4},
	{"LEFTP",      5},
	{"MIDR",       4},
	{"MIDRP",      5},
	{"MIDW",       4},
	{"MIDWP",      5},
	{"INSTR",      5},
	{"INSTRP",     6},
	{"$MOV",       4},
	{"$MOVP",      5},
	{"FDEL",       4},
	{"FDELP",      5},
	{"FINS",       4},
	{"FINSP",      5},
	{"POP",        3},
	{"POPP",       4},
	{"SFR",        3},
	{"SFRP",       4},
	{"SFL",        3},
	{"SFLP",       4},
	{"LD=",        3},
	{"LD<>",       4},
	{"LD>",        3},
	{"LD>=",       4},
	{"LD<",        3},
	{"LD<=",       4},
	{"LDD=",       4},
	{"LDD<>",      5},
	{"LDD>",       4},
	{"LDD>=",      5},
	{"LDD<",       4},
	{"LDD<=",      5},
	{"AND=",       4},
	{"AND<>",      5},
	{"AND>",       4},
	{"AND>=",      5},
	{"AND<",       4},
	{"AND<=",      5},
	{"ANDD=",      5},
	{"ANDD<>",     6},
	{"ANDD>",      5},
	{"ANDD>=",     6},
	{"ANDD<",      5},
	{"ANDD<=",     6},
	{"OR=",        3},
	{"OR<>",       4},
	{"OR>",        3},
	{"OR>=",       4},
	{"OR<",        3},
	{"OR<=",       4},
	{"ORD=",       4},
	{"ORD<>",      5},
	{"ORD>",       4},
	{"ORD>=",      5},
	{"ORD<",       4},
	{"ORD<=",      5},
	{"LIMIT",      5},
	{"LIMITP",     6},
	{"DLIMIT",     6},
	{"DLIMITP",    7},
	{"BAND",       4},
	{"BANDP",      5},
	{"DBAND",      5},
	{"DBANDP",     6},
	{"ZONE",       4},
	{"ZONEP",      5},
	{"DZONE",      5},
	{"DZONEP",     6},
	{"SCL",        3},
	{"SCLP",       4},
	{"DSCL",       4},
	{"DSCLP",      5},
	{"DABIN",      5},
	{"DABINP",     6},
	{"DDABIN",     6},
	{"DDABINP",    7},
	{"BINDA",      5},
	{"BINDAP",     6},
	{"DBINDA",     6},
	{"DBINDAP",    7},
	{"SCL2",       4},
	{"SCL2P",      5},
	{"DSCL2",      5},
	{"DSCL2P",     6},
	{"IVCK",       4},
	{"IVDR",       4},
	{"IVRD",       4},
	{"IVWR",       4},
	{"IVBWR",      5},
	{"IVMC",       4},
	{"RBFM",       4},
	{"WBFM",       4},
	{"DHSCT",      5},
	{"LOADR",      5},
	{"LOADRP",     6},
	{"SAVER",      5},
	{"INITR",      5},
	{"INITRP",     6},
	{"LOGR",       4},
	{"LOGRP",      5},
	{"RWER",       4},
	{"RWERP",      5},
	{"INITER",     6},
	{"INITERP",    7},
	{"FLCRT",      5},
	{"FLDEL",      5},
	{"FLWR",       4},
	{"FLRD",       4},
	{"FLCMD",      5},
	{"FLSTRD",     6}
};


static WPGInstructionRecord INSTRUCTIONS_METRIX_FXCPU[MEL_WPG_OpID_TOTALLY] = 
{
	// "Contact instruction"
	{//"LD"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCS"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_LD

	{//"LDI"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_LDI

	{//"LDP"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_LDP

	{//"LDF"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_LDF

	{//"AND"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND

	{//"ANI"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANI

	{//"ANDP"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDP

	{//"ANDF"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDF

	{//"OR"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_OR

	{//"ORI"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ORI

	{//"ORP"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ORP

	{//"ORF"
		MEL_OPER_CONTACTS          ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 1,	
		{"S",{
				/*FX3U/C devs&indx*/	"XYMrTCSb",
				/*FX3G/C   devs&indx*/	"XYMrTCSb"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ORF

	// "Connection instruction"
	{//"ANB"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_ANB

	{//"ORB"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_ORB

	{//"MPS"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_MPS

	{//"MRD"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_MRD

	{//"MPP"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_MPP

	{//"INV"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_INV

	{//"MEP"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_PULS,	 0,	{{0}},	0},	//MEL_WPG_OpID_MEP

	{//"MEF"
		MEL_OPER_CONNECTS          ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_PULS,	 0,	{{0}},	0},	//MEL_WPG_OpID_MEF

	// "Out instruction"
	{//"OUT"
		MEL_OPER_OUTS              ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"YMrSb",
				/*FX3G/C   devs&indx*/	"YMrS"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_OUT

	{//"OUT"
		MEL_OPER_OUTS              ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 2,	
		{"D",{
				/*FX3U/C devs&indx*/	"TC",
				/*FX3G/C   devs&indx*/	"TC"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SP",{
				/*FX3U/C devs&indx*/	"DRK",
				/*FX3G/C   devs&indx*/	"DRK"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	"TC"},	//MEL_WPG_OpID_OUT_TC

	{//"SET"
		MEL_OPER_OUTS              ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"YMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"YMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SET

	{//"RST"
		MEL_OPER_OUTS              ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 1,	
		{"D",{
				/*FX3U/C devs&indx*/	"YMrTCSbtcDRVZ",
				/*FX3G/C   devs&indx*/	"YMrTCStcDRVZ"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_RST

	{//"PLS"
		MEL_OPER_OUTS              ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 0,	{{0}},	0},	//MEL_WPG_OpID_PLS

	{//"PLF"
		MEL_OPER_OUTS              ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_PULS,	 0,	{{0}},	0},	//MEL_WPG_OpID_PLF

	// "Master Control instruction"
	{//"MC"
		MEL_OPER_MASTERCNT         ,	0  ,	MEL_BITDEPTH_1BIT,	 0,	MEL_EXEC_CONT,	 2,	
		{"P",{
				/*FX3U/C devs&indx*/	"N",
				/*FX3G/C   devs&indx*/	"N"},	"",
			MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"P",{
				/*FX3U/C devs&indx*/	"YM",
				/*FX3G/C   devs&indx*/	"YM"},	"",
			MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_MC

	{//"MCR"
		MEL_OPER_MASTERCNT         ,	0  ,	MEL_BITDEPTH_INDEP,	 2,	MEL_EXEC_CONT,	 1,	
		{"P",{
				/*FX3U/C devs&indx*/	"N",
				/*FX3G/C   devs&indx*/	"N"},	"",
			MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_MCR

	// "Other Basic instruction"
	{//"NOP"
		MEL_OPER_OTHERBASICS       ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_NOP

	// "End instruction"
	{//"END"
		MEL_OPER_ENDS              ,	0  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_END

	// "Program Flow"
	{//"CJ"
		MEL_OPER_PRGFLOW           ,	0  ,	MEL_BITDEPTH_INDEP,	 3,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CJ

	{//"CJP"
		MEL_OPER_PRGFLOW           ,	0  ,	MEL_BITDEPTH_INDEP,	 3,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CJP

	{//"CALL"
		MEL_OPER_PRGFLOW           ,	1  ,	MEL_BITDEPTH_INDEP,	 3,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CALL

	{//"CALLP"
		MEL_OPER_PRGFLOW           ,	1  ,	MEL_BITDEPTH_INDEP,	 3,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CALLP

	{//"SRET"
		MEL_OPER_PRGFLOW           ,	2  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_SRET

	{//"IRET"
		MEL_OPER_PRGFLOW           ,	3  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_IRET

	{//"EI"
		MEL_OPER_PRGFLOW           ,	4  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_EI

	{//"DI"
		MEL_OPER_PRGFLOW           ,	5  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_DI

	{//"FEND"
		MEL_OPER_PRGFLOW           ,	6  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_FEND

	{//"WDT"
		MEL_OPER_PRGFLOW           ,	7  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_WDT

	{//"WDTP"
		MEL_OPER_PRGFLOW           ,	7  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_PULS,	 0,	{{0}},	0},	//MEL_WPG_OpID_WDTP

	{//"FOR"
		MEL_OPER_PRGFLOW           ,	8  ,	MEL_BITDEPTH_INDEP,	 3,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FOR

	{//"NEXT"
		MEL_OPER_PRGFLOW           ,	9  ,	MEL_BITDEPTH_INDEP,	 1,	MEL_EXEC_CONT,	 0,	{{0}},	0},	//MEL_WPG_OpID_NEXT

	// "Move and compare"
	{//"CMP"
		MEL_OPER_MOVECMP           ,	10 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CMP

	{//"CMPP"
		MEL_OPER_MOVECMP           ,	10 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CMPP

	{//"DCMP"
		MEL_OPER_MOVECMP           ,	10 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DCMP

	{//"DCMPP"
		MEL_OPER_MOVECMP           ,	10 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DCMPP

	{//"ZCP"
		MEL_OPER_MOVECMP           ,	11 ,	MEL_BITDEPTH_16BIT,	 9,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ZCP

	{//"ZCPP"
		MEL_OPER_MOVECMP           ,	11 ,	MEL_BITDEPTH_16BIT,	 9,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ZCPP

	{//"DZCP"
		MEL_OPER_MOVECMP           ,	11 ,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DZCP

	{//"DZCPP"
		MEL_OPER_MOVECMP           ,	11 ,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DZCPP

	{//"MOV"
		MEL_OPER_MOVECMP           ,	12 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_MOV

	{//"MOVP"
		MEL_OPER_MOVECMP           ,	12 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_MOVP

	{//"DMOV"
		MEL_OPER_MOVECMP           ,	12 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DMOV

	{//"DMOVP"
		MEL_OPER_MOVECMP           ,	12 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DMOVP

	{//"SMOV"
		MEL_OPER_MOVECMP           ,	13 ,	MEL_BITDEPTH_16BIT,	11,	MEL_EXEC_CONT,	 5,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SMOV

	{//"SMOVP"
		MEL_OPER_MOVECMP           ,	13 ,	MEL_BITDEPTH_16BIT,	11,	MEL_EXEC_PULS,	 5,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SMOVP

	{//"CML"
		MEL_OPER_MOVECMP           ,	14 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CML

	{//"CMLP"
		MEL_OPER_MOVECMP           ,	14 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_CMLP

	{//"DCML"
		MEL_OPER_MOVECMP           ,	14 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DCML

	{//"DCMLP"
		MEL_OPER_MOVECMP           ,	14 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DCMLP

	{//"BMOV"
		MEL_OPER_MOVECMP           ,	15 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_BMOV

	{//"BMOVP"
		MEL_OPER_MOVECMP           ,	15 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_BMOVP

	{//"FMOV"
		MEL_OPER_MOVECMP           ,	16 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FMOV

	{//"FMOVP"
		MEL_OPER_MOVECMP           ,	16 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FMOVP

	{//"DFMOV"
		MEL_OPER_MOVECMP           ,	16 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DFMOV

	{//"DFMOVP"
		MEL_OPER_MOVECMP           ,	16 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DFMOVP

	{//"XCH"
		MEL_OPER_MOVECMP           ,	17 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_XCH

	{//"XCHP"
		MEL_OPER_MOVECMP           ,	17 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_XCHP

	{//"DXCH"
		MEL_OPER_MOVECMP           ,	17 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DXCH

	{//"DXCHP"
		MEL_OPER_MOVECMP           ,	17 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DXCHP

	{//"BCD"
		MEL_OPER_MOVECMP           ,	18 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_BCD

	{//"BCDP"
		MEL_OPER_MOVECMP           ,	18 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_BCDP

	{//"DBCD"
		MEL_OPER_MOVECMP           ,	18 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DBCD

	{//"DBCDP"
		MEL_OPER_MOVECMP           ,	18 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DBCDP

	{//"BIN"
		MEL_OPER_MOVECMP           ,	19 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_BIN

	{//"BINP"
		MEL_OPER_MOVECMP           ,	19 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_BINP

	{//"DBIN"
		MEL_OPER_MOVECMP           ,	19 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DBIN

	{//"DBINP"
		MEL_OPER_MOVECMP           ,	19 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DBINP

	// "Arithmetic and Logical Operation"
	{//"ADD"
		MEL_OPER_ARITHMLOGIC       ,	20 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ADD

	{//"ADDP"
		MEL_OPER_ARITHMLOGIC       ,	20 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ADDP

	{//"DADD"
		MEL_OPER_ARITHMLOGIC       ,	20 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DADD

	{//"DADDP"
		MEL_OPER_ARITHMLOGIC       ,	20 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DADDP

	{//"SUB"
		MEL_OPER_ARITHMLOGIC       ,	21 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SUB

	{//"SUBP"
		MEL_OPER_ARITHMLOGIC       ,	21 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SUBP

	{//"DSUB"
		MEL_OPER_ARITHMLOGIC       ,	21 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DSUB

	{//"DSUBP"
		MEL_OPER_ARITHMLOGIC       ,	21 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DSUBP

	{//"MUL"
		MEL_OPER_ARITHMLOGIC       ,	22 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_MUL

	{//"MULP"
		MEL_OPER_ARITHMLOGIC       ,	22 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_MULP

	{//"DMUL"
		MEL_OPER_ARITHMLOGIC       ,	22 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DMUL

	{//"DMULP"
		MEL_OPER_ARITHMLOGIC       ,	22 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DMULP

	{//"SUB"
		MEL_OPER_ARITHMLOGIC       ,	23 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SUB

	{//"SUBP"
		MEL_OPER_ARITHMLOGIC       ,	23 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SUBP

	{//"DSUB"
		MEL_OPER_ARITHMLOGIC       ,	23 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DSUB

	{//"DSUBP"
		MEL_OPER_ARITHMLOGIC       ,	23 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DSUBP

	{//"INC"
		MEL_OPER_ARITHMLOGIC       ,	24 ,	MEL_BITDEPTH_16BIT,	 3,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_INC

	{//"INCP"
		MEL_OPER_ARITHMLOGIC       ,	24 ,	MEL_BITDEPTH_16BIT,	 3,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_INCP

	{//"DINC"
		MEL_OPER_ARITHMLOGIC       ,	24 ,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DINC

	{//"DINCP"
		MEL_OPER_ARITHMLOGIC       ,	24 ,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DINCP

	{//"DEC"
		MEL_OPER_ARITHMLOGIC       ,	25 ,	MEL_BITDEPTH_16BIT,	 3,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEC

	{//"DECP"
		MEL_OPER_ARITHMLOGIC       ,	25 ,	MEL_BITDEPTH_16BIT,	 3,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DECP

	{//"DDEC"
		MEL_OPER_ARITHMLOGIC       ,	25 ,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DDEC

	{//"DDECP"
		MEL_OPER_ARITHMLOGIC       ,	25 ,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DDECP

	{//"WAND"
		MEL_OPER_ARITHMLOGIC       ,	26 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_WAND

	{//"WANDP"
		MEL_OPER_ARITHMLOGIC       ,	26 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_WANDP

	{//"DAND"
		MEL_OPER_ARITHMLOGIC       ,	26 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DAND

	{//"DANDP"
		MEL_OPER_ARITHMLOGIC       ,	26 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DANDP

	{//"WOR"
		MEL_OPER_ARITHMLOGIC       ,	27 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_WOR

	{//"WORP"
		MEL_OPER_ARITHMLOGIC       ,	27 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_WORP

	{//"DOR"
		MEL_OPER_ARITHMLOGIC       ,	27 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DOR

	{//"DORP"
		MEL_OPER_ARITHMLOGIC       ,	27 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DORP

	{//"WXOR"
		MEL_OPER_ARITHMLOGIC       ,	28 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_WXOR

	{//"WXORP"
		MEL_OPER_ARITHMLOGIC       ,	28 ,	MEL_BITDEPTH_16BIT,	 7,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_WXORP

	{//"DXOR"
		MEL_OPER_ARITHMLOGIC       ,	28 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DXOR

	{//"DXORP"
		MEL_OPER_ARITHMLOGIC       ,	28 ,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DXORP

	{//"NEG"
		MEL_OPER_ARITHMLOGIC       ,	29 ,	MEL_BITDEPTH_16BIT,	 3,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_NEG

	{//"NEGP"
		MEL_OPER_ARITHMLOGIC       ,	29 ,	MEL_BITDEPTH_16BIT,	 3,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_NEGP

	{//"DNEG"
		MEL_OPER_ARITHMLOGIC       ,	29 ,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DNEG

	{//"DNEGP"
		MEL_OPER_ARITHMLOGIC       ,	29 ,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DNEGP

	// ""
	{//"ROR"
		0,	0},	//MEL_WPG_OpID_ROR

	{//"RORP"
		0,	0},	//MEL_WPG_OpID_RORP

	{//"DROR"
		0,	0},	//MEL_WPG_OpID_DROR

	{//"DRORP"
		0,	0},	//MEL_WPG_OpID_DRORP

	{//"ROL"
		0,	0},	//MEL_WPG_OpID_ROL

	{//"ROLP"
		0,	0},	//MEL_WPG_OpID_ROLP

	// "Rotation and Shift Operation"
	{//"DROL"
		MEL_OPER_ROTSHIFT          ,	31 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"D",{
				/*FX3U/C devs&indx*/	"rTCymsDRUVZ",
				/*FX3G/C   devs&indx*/	"rTCymsDRVZ"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"P",{
				/*FX3U/C devs&indx*/	"DRKH",
				/*FX3G/C   devs&indx*/	"DRKH"},	"",
			MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DROL

	// ""
	{//"DROLP"
		0,	0},	//MEL_WPG_OpID_DROLP

	{//"RCR"
		0,	0},	//MEL_WPG_OpID_RCR

	{//"RCRP"
		0,	0},	//MEL_WPG_OpID_RCRP

	{//"DRCR"
		0,	0},	//MEL_WPG_OpID_DRCR

	{//"DRCRP"
		0,	0},	//MEL_WPG_OpID_DRCRP

	{//"RCL"
		0,	0},	//MEL_WPG_OpID_RCL

	{//"RCLP"
		0,	0},	//MEL_WPG_OpID_RCLP

	{//"DRCL"
		0,	0},	//MEL_WPG_OpID_DRCL

	{//"DRCLP"
		0,	0},	//MEL_WPG_OpID_DRCLP

	{//"SFTR"
		0,	0},	//MEL_WPG_OpID_SFTR

	{//"SFTRP"
		0,	0},	//MEL_WPG_OpID_SFTRP

	{//"SFTL"
		0,	0},	//MEL_WPG_OpID_SFTL

	{//"SFTLP"
		0,	0},	//MEL_WPG_OpID_SFTLP

	{//"WSFR"
		0,	0},	//MEL_WPG_OpID_WSFR

	{//"WSFRP"
		0,	0},	//MEL_WPG_OpID_WSFRP

	{//"WSFL"
		0,	0},	//MEL_WPG_OpID_WSFL

	{//"WSFLP"
		0,	0},	//MEL_WPG_OpID_WSFLP

	{//"SFWR"
		0,	0},	//MEL_WPG_OpID_SFWR

	{//"SFWRP"
		0,	0},	//MEL_WPG_OpID_SFWRP

	{//"SFRD"
		0,	0},	//MEL_WPG_OpID_SFRD

	{//"SFRDP"
		0,	0},	//MEL_WPG_OpID_SFRDP

	// "Data Operation"
	{//"ZRST"
		MEL_OPER_DATAOPS           ,	40 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"D",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"D",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ZRST

	{//"ZRSTP"
		MEL_OPER_DATAOPS           ,	40 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"D",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"D",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ZRSTP

	// ""
	{//"DECO"
		0,	0},	//MEL_WPG_OpID_DECO

	{//"DECOP"
		0,	0},	//MEL_WPG_OpID_DECOP

	{//"ENCO"
		0,	0},	//MEL_WPG_OpID_ENCO

	{//"ENCOP"
		0,	0},	//MEL_WPG_OpID_ENCOP

	{//"SUM"
		0,	0},	//MEL_WPG_OpID_SUM

	{//"SUMP"
		0,	0},	//MEL_WPG_OpID_SUMP

	{//"DSUM"
		0,	0},	//MEL_WPG_OpID_DSUM

	{//"DSUMP"
		0,	0},	//MEL_WPG_OpID_DSUMP

	{//"BON"
		0,	0},	//MEL_WPG_OpID_BON

	{//"BONP"
		0,	0},	//MEL_WPG_OpID_BONP

	{//"DBON"
		0,	0},	//MEL_WPG_OpID_DBON

	{//"DBONP"
		0,	0},	//MEL_WPG_OpID_DBONP

	{//"MEAN"
		0,	0},	//MEL_WPG_OpID_MEAN

	{//"MEANP"
		0,	0},	//MEL_WPG_OpID_MEANP

	{//"DMEAN"
		0,	0},	//MEL_WPG_OpID_DMEAN

	{//"DMEANP"
		0,	0},	//MEL_WPG_OpID_DMEANP

	{//"ANS"
		0,	0},	//MEL_WPG_OpID_ANS

	{//"ANR"
		0,	0},	//MEL_WPG_OpID_ANR

	{//"ANRP"
		0,	0},	//MEL_WPG_OpID_ANRP

	{//"SQR"
		0,	0},	//MEL_WPG_OpID_SQR

	{//"SQRP"
		0,	0},	//MEL_WPG_OpID_SQRP

	{//"DSQR"
		0,	0},	//MEL_WPG_OpID_DSQR

	{//"DSQRP"
		0,	0},	//MEL_WPG_OpID_DSQRP

	// "Data Operation"
	{//"FLT"
		MEL_OPER_DATAOPS           ,	49 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FLT

	{//"FLTP"
		MEL_OPER_DATAOPS           ,	49 ,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FLTP

	{//"DFLT"
		MEL_OPER_DATAOPS           ,	49 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DFLT

	{//"DFLTP"
		MEL_OPER_DATAOPS           ,	49 ,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DFLTP

	// ""
	{//"REF"
		0,	0},	//MEL_WPG_OpID_REF

	{//"REFP"
		0,	0},	//MEL_WPG_OpID_REFP

	{//"REFF"
		0,	0},	//MEL_WPG_OpID_REFF

	{//"REFFP"
		0,	0},	//MEL_WPG_OpID_REFFP

	{//"MTR"
		0,	0},	//MEL_WPG_OpID_MTR

	{//"DHSCS"
		0,	0},	//MEL_WPG_OpID_DHSCS

	{//"DHSCR"
		0,	0},	//MEL_WPG_OpID_DHSCR

	{//"DHSZ"
		0,	0},	//MEL_WPG_OpID_DHSZ

	{//"SPD"
		0,	0},	//MEL_WPG_OpID_SPD

	{//"DSPD"
		0,	0},	//MEL_WPG_OpID_DSPD

	{//"PLSY"
		0,	0},	//MEL_WPG_OpID_PLSY

	{//"DPLSY"
		0,	0},	//MEL_WPG_OpID_DPLSY

	{//"PWM"
		0,	0},	//MEL_WPG_OpID_PWM

	{//"PLSR"
		0,	0},	//MEL_WPG_OpID_PLSR

	{//"DPLSR"
		0,	0},	//MEL_WPG_OpID_DPLSR

	{//"IST"
		0,	0},	//MEL_WPG_OpID_IST

	{//"SER"
		0,	0},	//MEL_WPG_OpID_SER

	{//"SERP"
		0,	0},	//MEL_WPG_OpID_SERP

	{//"DSER"
		0,	0},	//MEL_WPG_OpID_DSER

	{//"DSERP"
		0,	0},	//MEL_WPG_OpID_DSERP

	{//"ABSD"
		0,	0},	//MEL_WPG_OpID_ABSD

	{//"DABSD"
		0,	0},	//MEL_WPG_OpID_DABSD

	{//"INCD"
		0,	0},	//MEL_WPG_OpID_INCD

	{//"TTMR"
		0,	0},	//MEL_WPG_OpID_TTMR

	{//"STMR"
		0,	0},	//MEL_WPG_OpID_STMR

	{//"ALT"
		0,	0},	//MEL_WPG_OpID_ALT

	{//"ALTP"
		0,	0},	//MEL_WPG_OpID_ALTP

	{//"RAMP"
		0,	0},	//MEL_WPG_OpID_RAMP

	{//"ROTC"
		0,	0},	//MEL_WPG_OpID_ROTC

	{//"SORT"
		0,	0},	//MEL_WPG_OpID_SORT

	{//"TKY"
		0,	0},	//MEL_WPG_OpID_TKY

	{//"DTKY"
		0,	0},	//MEL_WPG_OpID_DTKY

	{//"HKY"
		0,	0},	//MEL_WPG_OpID_HKY

	{//"DHKY"
		0,	0},	//MEL_WPG_OpID_DHKY

	{//"DSW"
		0,	0},	//MEL_WPG_OpID_DSW

	{//"SEGD"
		0,	0},	//MEL_WPG_OpID_SEGD

	{//"SEGDP"
		0,	0},	//MEL_WPG_OpID_SEGDP

	{//"SEGL"
		0,	0},	//MEL_WPG_OpID_SEGL

	{//"ARWS"
		0,	0},	//MEL_WPG_OpID_ARWS

	{//"ASC"
		0,	0},	//MEL_WPG_OpID_ASC

	{//"PR"
		0,	0},	//MEL_WPG_OpID_PR

	// "External FX I/O Device"
	{//"FROM"
		MEL_OPER_EXTFXIO           ,	78 ,	MEL_BITDEPTH_16BIT,	 9,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FROM

	{//"FROMP"
		MEL_OPER_EXTFXIO           ,	78 ,	MEL_BITDEPTH_16BIT,	 9,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_FROMP

	{//"DFROM"
		MEL_OPER_EXTFXIO           ,	78 ,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DFROM

	{//"DFROMP"
		MEL_OPER_EXTFXIO           ,	78 ,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DFROMP

	{//"TO"
		MEL_OPER_EXTFXIO           ,	79 ,	MEL_BITDEPTH_16BIT,	 9,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_TO

	{//"TOP"
		MEL_OPER_EXTFXIO           ,	79 ,	MEL_BITDEPTH_16BIT,	 9,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_TOP

	{//"DTO"
		MEL_OPER_EXTFXIO           ,	79 ,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DTO

	{//"DTOP"
		MEL_OPER_EXTFXIO           ,	79 ,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DTOP

	// ""
	{//"RS"
		0,	0},	//MEL_WPG_OpID_RS

	{//"PRUN"
		0,	0},	//MEL_WPG_OpID_PRUN

	{//"PRUNP"
		0,	0},	//MEL_WPG_OpID_PRUNP

	{//"DPRUN"
		0,	0},	//MEL_WPG_OpID_DPRUN

	{//"DPRUNP"
		0,	0},	//MEL_WPG_OpID_DPRUNP

	{//"ASCI"
		0,	0},	//MEL_WPG_OpID_ASCI

	{//"ASCIP"
		0,	0},	//MEL_WPG_OpID_ASCIP

	{//"HEX"
		0,	0},	//MEL_WPG_OpID_HEX

	{//"HEXP"
		0,	0},	//MEL_WPG_OpID_HEXP

	{//"CCD"
		0,	0},	//MEL_WPG_OpID_CCD

	{//"CCDP"
		0,	0},	//MEL_WPG_OpID_CCDP

	{//"VRRD"
		0,	0},	//MEL_WPG_OpID_VRRD

	{//"VRRDP"
		0,	0},	//MEL_WPG_OpID_VRRDP

	{//"VRSC"
		0,	0},	//MEL_WPG_OpID_VRSC

	{//"VRSCP"
		0,	0},	//MEL_WPG_OpID_VRSCP

	// "External FX Device"
	{//"RS2"
		MEL_OPER_EXTFXDEVS         ,	87 ,	MEL_BITDEPTH_INDEP,	11,	MEL_EXEC_CONT,	 5,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_RS2

	// ""
	{//"PID"
		0,	0},	//MEL_WPG_OpID_PID

	{//"ZPUSH"
		0,	0},	//MEL_WPG_OpID_ZPUSH

	{//"ZPUSHP"
		0,	0},	//MEL_WPG_OpID_ZPUSHP

	{//"ZPOP"
		0,	0},	//MEL_WPG_OpID_ZPOP

	{//"ZPOPP"
		0,	0},	//MEL_WPG_OpID_ZPOPP

	// "Floating Point"
	{//"DECMP"
		MEL_OPER_FLOATS            ,	110,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DECMP

	{//"DECMPP"
		MEL_OPER_FLOATS            ,	110,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DECMPP

	{//"DEZCP"
		MEL_OPER_FLOATS            ,	111,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_CONT,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEZCP

	{//"DEZCPP"
		MEL_OPER_FLOATS            ,	111,	MEL_BITDEPTH_32BIT,	17,	MEL_EXEC_PULS,	 4,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEZCPP

	{//"DEMOV"
		MEL_OPER_FLOATS            ,	112,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEMOV

	{//"DEMOVP"
		MEL_OPER_FLOATS            ,	112,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEMOVP

	{//"DESTR"
		MEL_OPER_FLOATS            ,	116,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DESTR

	{//"DESTRP"
		MEL_OPER_FLOATS            ,	116,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DESTRP

	{//"DEVAL"
		MEL_OPER_FLOATS            ,	117,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEVAL

	{//"DEVALP"
		MEL_OPER_FLOATS            ,	117,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEVALP

	{//"DEBCD"
		MEL_OPER_FLOATS            ,	118,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEBCD

	{//"DEBCDP"
		MEL_OPER_FLOATS            ,	118,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEBCDP

	{//"DEBIN"
		MEL_OPER_FLOATS            ,	119,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEBIN

	{//"DEBINP"
		MEL_OPER_FLOATS            ,	119,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEBINP

	{//"DEADD"
		MEL_OPER_FLOATS            ,	120,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEADD

	{//"DEADDP"
		MEL_OPER_FLOATS            ,	120,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEADDP

	{//"DESUB"
		MEL_OPER_FLOATS            ,	121,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DESUB

	{//"DESUBP"
		MEL_OPER_FLOATS            ,	121,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DESUBP

	{//"DEMUL"
		MEL_OPER_FLOATS            ,	122,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEMUL

	{//"DEMULP"
		MEL_OPER_FLOATS            ,	122,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEMULP

	{//"DEDIV"
		MEL_OPER_FLOATS            ,	123,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_CONT,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEDIV

	{//"DEDIVP"
		MEL_OPER_FLOATS            ,	123,	MEL_BITDEPTH_32BIT,	13,	MEL_EXEC_PULS,	 3,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEDIVP

	{//"DEXP"
		MEL_OPER_FLOATS            ,	124,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEXP

	{//"DEXPP"
		MEL_OPER_FLOATS            ,	124,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DEXPP

	{//"DLOGE"
		MEL_OPER_FLOATS            ,	125,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DLOGE

	{//"DLOGEP"
		MEL_OPER_FLOATS            ,	125,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DLOGEP

	{//"DLOG10"
		MEL_OPER_FLOATS            ,	126,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DLOG10

	{//"DLOG10P"
		MEL_OPER_FLOATS            ,	126,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DLOG10P

	{//"DESQR"
		MEL_OPER_FLOATS            ,	127,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DESQR

	{//"DESQRP"
		MEL_OPER_FLOATS            ,	127,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DESQRP

	{//"DENEG"
		MEL_OPER_FLOATS            ,	128,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_CONT,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DENEG

	{//"DENEGP"
		MEL_OPER_FLOATS            ,	128,	MEL_BITDEPTH_32BIT,	 5,	MEL_EXEC_PULS,	 1,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DENEGP

	{//"INT"
		MEL_OPER_FLOATS            ,	129,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_INT

	{//"INTP"
		MEL_OPER_FLOATS            ,	129,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_INTP

	{//"DINT"
		MEL_OPER_FLOATS            ,	129,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DINT

	{//"DINTP"
		MEL_OPER_FLOATS            ,	129,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DINTP

	{//"DSIN"
		MEL_OPER_FLOATS            ,	130,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DSIN

	{//"DSINP"
		MEL_OPER_FLOATS            ,	130,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DSINP

	{//"DCOS"
		MEL_OPER_FLOATS            ,	131,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DCOS

	{//"DCOSP"
		MEL_OPER_FLOATS            ,	131,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DCOSP

	{//"DTAN"
		MEL_OPER_FLOATS            ,	132,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DTAN

	{//"DTANP"
		MEL_OPER_FLOATS            ,	132,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DTANP

	{//"DASIN"
		MEL_OPER_FLOATS            ,	133,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DASIN

	{//"DASINP"
		MEL_OPER_FLOATS            ,	133,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DASINP

	{//"DACOS"
		MEL_OPER_FLOATS            ,	134,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DACOS

	{//"DACOSP"
		MEL_OPER_FLOATS            ,	134,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DACOSP

	{//"DATAN"
		MEL_OPER_FLOATS            ,	135,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DATAN

	{//"DATANP"
		MEL_OPER_FLOATS            ,	135,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DATANP

	{//"DRAD"
		MEL_OPER_FLOATS            ,	136,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DRAD

	{//"DRADP"
		MEL_OPER_FLOATS            ,	136,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DRADP

	{//"DDEG"
		MEL_OPER_FLOATS            ,	137,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DDEG

	{//"DDEGP"
		MEL_OPER_FLOATS            ,	137,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_DDEGP

	// ""
	{//"WSUM"
		0,	0},	//MEL_WPG_OpID_WSUM

	{//"WSUMP"
		0,	0},	//MEL_WPG_OpID_WSUMP

	{//"DWSUM"
		0,	0},	//MEL_WPG_OpID_DWSUM

	{//"DWSUMP"
		0,	0},	//MEL_WPG_OpID_DWSUMP

	{//"WTOB"
		0,	0},	//MEL_WPG_OpID_WTOB

	{//"WTOBP"
		0,	0},	//MEL_WPG_OpID_WTOBP

	{//"BTOW"
		0,	0},	//MEL_WPG_OpID_BTOW

	{//"BTOWP"
		0,	0},	//MEL_WPG_OpID_BTOWP

	{//"UNI"
		0,	0},	//MEL_WPG_OpID_UNI

	{//"UNIP"
		0,	0},	//MEL_WPG_OpID_UNIP

	{//"DIS"
		0,	0},	//MEL_WPG_OpID_DIS

	{//"DISP"
		0,	0},	//MEL_WPG_OpID_DISP

	{//"SWAP"
		0,	0},	//MEL_WPG_OpID_SWAP

	{//"SWAPP"
		0,	0},	//MEL_WPG_OpID_SWAPP

	{//"DSWAP"
		0,	0},	//MEL_WPG_OpID_DSWAP

	{//"DSWAPP"
		0,	0},	//MEL_WPG_OpID_DSWAPP

	{//"SORT2"
		0,	0},	//MEL_WPG_OpID_SORT2

	{//"DSORT2"
		0,	0},	//MEL_WPG_OpID_DSORT2

	{//"DSZR"
		0,	0},	//MEL_WPG_OpID_DSZR

	{//"DVIT"
		0,	0},	//MEL_WPG_OpID_DVIT

	{//"DDVIT"
		0,	0},	//MEL_WPG_OpID_DDVIT

	{//"DTBL"
		0,	0},	//MEL_WPG_OpID_DTBL

	{//"DABS"
		0,	0},	//MEL_WPG_OpID_DABS

	{//"ZRN"
		0,	0},	//MEL_WPG_OpID_ZRN

	{//"DZRN"
		0,	0},	//MEL_WPG_OpID_DZRN

	{//"PLSV"
		0,	0},	//MEL_WPG_OpID_PLSV

	{//"DPLSV"
		0,	0},	//MEL_WPG_OpID_DPLSV

	{//"DRVI"
		0,	0},	//MEL_WPG_OpID_DRVI

	{//"DDRVI"
		0,	0},	//MEL_WPG_OpID_DDRVI

	{//"DRVA"
		0,	0},	//MEL_WPG_OpID_DRVA

	{//"DDRVA"
		0,	0},	//MEL_WPG_OpID_DDRVA

	{//"TCMP"
		0,	0},	//MEL_WPG_OpID_TCMP

	{//"TCMPP"
		0,	0},	//MEL_WPG_OpID_TCMPP

	{//"TZCP"
		0,	0},	//MEL_WPG_OpID_TZCP

	{//"TZCPP"
		0,	0},	//MEL_WPG_OpID_TZCPP

	{//"TADD"
		0,	0},	//MEL_WPG_OpID_TADD

	{//"TADDP"
		0,	0},	//MEL_WPG_OpID_TADDP

	{//"TSUB"
		0,	0},	//MEL_WPG_OpID_TSUB

	{//"TSUBP"
		0,	0},	//MEL_WPG_OpID_TSUBP

	{//"HTOS"
		0,	0},	//MEL_WPG_OpID_HTOS

	{//"HTOSP"
		0,	0},	//MEL_WPG_OpID_HTOSP

	{//"DHTOS"
		0,	0},	//MEL_WPG_OpID_DHTOS

	{//"DHTOSP"
		0,	0},	//MEL_WPG_OpID_DHTOSP

	{//"STOH"
		0,	0},	//MEL_WPG_OpID_STOH

	{//"STOHP"
		0,	0},	//MEL_WPG_OpID_STOHP

	{//"DSTOH"
		0,	0},	//MEL_WPG_OpID_DSTOH

	{//"DSTOHP"
		0,	0},	//MEL_WPG_OpID_DSTOHP

	{//"TRD"
		0,	0},	//MEL_WPG_OpID_TRD

	{//"TRDP"
		0,	0},	//MEL_WPG_OpID_TRDP

	{//"TWR"
		0,	0},	//MEL_WPG_OpID_TWR

	{//"TWRP"
		0,	0},	//MEL_WPG_OpID_TWRP

	{//"HOUR"
		0,	0},	//MEL_WPG_OpID_HOUR

	{//"DHOUR"
		0,	0},	//MEL_WPG_OpID_DHOUR

	{//"GRY"
		0,	0},	//MEL_WPG_OpID_GRY

	{//"GRYP"
		0,	0},	//MEL_WPG_OpID_GRYP

	{//"DGRY"
		0,	0},	//MEL_WPG_OpID_DGRY

	{//"DGRYP"
		0,	0},	//MEL_WPG_OpID_DGRYP

	{//"GBIN"
		0,	0},	//MEL_WPG_OpID_GBIN

	{//"GBINP"
		0,	0},	//MEL_WPG_OpID_GBINP

	{//"DGBIN"
		0,	0},	//MEL_WPG_OpID_DGBIN

	{//"DGBINP"
		0,	0},	//MEL_WPG_OpID_DGBINP

	{//"RD3A"
		0,	0},	//MEL_WPG_OpID_RD3A

	{//"RD3AP"
		0,	0},	//MEL_WPG_OpID_RD3AP

	{//"WR3A"
		0,	0},	//MEL_WPG_OpID_WR3A

	{//"WR3AP"
		0,	0},	//MEL_WPG_OpID_WR3AP

	{//"EXTR"
		0,	0},	//MEL_WPG_OpID_EXTR

	{//"COMRD"
		0,	0},	//MEL_WPG_OpID_COMRD

	{//"COMRDP"
		0,	0},	//MEL_WPG_OpID_COMRDP

	{//"RND"
		0,	0},	//MEL_WPG_OpID_RND

	{//"RNDP"
		0,	0},	//MEL_WPG_OpID_RNDP

	{//"DUTY"
		0,	0},	//MEL_WPG_OpID_DUTY

	{//"CRC"
		0,	0},	//MEL_WPG_OpID_CRC

	{//"CRCP"
		0,	0},	//MEL_WPG_OpID_CRCP

	{//"DHCMOV"
		0,	0},	//MEL_WPG_OpID_DHCMOV

	{//"BK+"
		0,	0},	//MEL_WPG_OpID_BK+

	{//"BK+P"
		0,	0},	//MEL_WPG_OpID_BK+P

	{//"DBK+"
		0,	0},	//MEL_WPG_OpID_DBK+

	{//"DBK+P"
		0,	0},	//MEL_WPG_OpID_DBK+P

	{//"BK-"
		0,	0},	//MEL_WPG_OpID_BK-

	{//"BK-P"
		0,	0},	//MEL_WPG_OpID_BK-P

	{//"DBK-"
		0,	0},	//MEL_WPG_OpID_DBK-

	{//"DBK-P"
		0,	0},	//MEL_WPG_OpID_DBK-P

	{//"BKCMP="
		0,	0},	//MEL_WPG_OpID_BKCMP=

	{//"BKCMP=P"
		0,	0},	//MEL_WPG_OpID_BKCMP=P

	{//"DBKCMP="
		0,	0},	//MEL_WPG_OpID_DBKCMP=

	{//"DBKCMP=P"
		0,	0},	//MEL_WPG_OpID_DBKCMP=P

	{//"BKCMP>"
		0,	0},	//MEL_WPG_OpID_BKCMP>

	{//"BKCMP>P"
		0,	0},	//MEL_WPG_OpID_BKCMP>P

	{//"DBKCMP>"
		0,	0},	//MEL_WPG_OpID_DBKCMP>

	{//"DBKCMP>P"
		0,	0},	//MEL_WPG_OpID_DBKCMP>P

	{//"BKCMP<"
		0,	0},	//MEL_WPG_OpID_BKCMP<

	{//"BKCMP<P"
		0,	0},	//MEL_WPG_OpID_BKCMP<P

	{//"DBKCMP<"
		0,	0},	//MEL_WPG_OpID_DBKCMP<

	{//"DBKCMP<P"
		0,	0},	//MEL_WPG_OpID_DBKCMP<P

	{//"BKCMP<>"
		0,	0},	//MEL_WPG_OpID_BKCMP<>

	{//"BKCMP<>P"
		0,	0},	//MEL_WPG_OpID_BKCMP<>P

	{//"DBKCMP<>"
		0,	0},	//MEL_WPG_OpID_DBKCMP<>

	{//"DBKCMP<>P"
		0,	0},	//MEL_WPG_OpID_DBKCMP<>P

	{//"BKCMP<="
		0,	0},	//MEL_WPG_OpID_BKCMP<=

	{//"BKCMP<=P"
		0,	0},	//MEL_WPG_OpID_BKCMP<=P

	{//"DBKCMP<="
		0,	0},	//MEL_WPG_OpID_DBKCMP<=

	{//"DBKCMP<=P"
		0,	0},	//MEL_WPG_OpID_DBKCMP<=P

	{//"BKCMP>="
		0,	0},	//MEL_WPG_OpID_BKCMP>=

	{//"BKCMP>=P"
		0,	0},	//MEL_WPG_OpID_BKCMP>=P

	{//"DBKCMP>="
		0,	0},	//MEL_WPG_OpID_DBKCMP>=

	{//"DBKCMP>=P"
		0,	0},	//MEL_WPG_OpID_DBKCMP>=P

	{//"STR"
		0,	0},	//MEL_WPG_OpID_STR

	{//"STRP"
		0,	0},	//MEL_WPG_OpID_STRP

	{//"DSTR"
		0,	0},	//MEL_WPG_OpID_DSTR

	{//"DSTRP"
		0,	0},	//MEL_WPG_OpID_DSTRP

	{//"VAL"
		0,	0},	//MEL_WPG_OpID_VAL

	{//"VALP"
		0,	0},	//MEL_WPG_OpID_VALP

	{//"DVAL"
		0,	0},	//MEL_WPG_OpID_DVAL

	{//"DVALP"
		0,	0},	//MEL_WPG_OpID_DVALP

	// "Character String Control"
	{//"$+"
		MEL_OPER_CHARSTR           ,	202,	MEL_BITDEPTH_16BIT,	 0,	MEL_EXEC_CONT,	 3,	
		{"S",{
				/*FX3U/C devs&indx*/	"xymstcDRUA",
				/*FX3G/C   devs&indx*/	"xymstcDRUA"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"S",{
				/*FX3U/C devs&indx*/	"xymstcDRUA",
				/*FX3G/C   devs&indx*/	"xymstcDRUA"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"D",{
				/*FX3U/C devs&indx*/	"ymstcDRU",
				/*FX3G/C   devs&indx*/	"ymstcDRU"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	""},	//MEL_WPG_OpID_$+_

	{//"$+P"
		MEL_OPER_CHARSTR           ,	202,	MEL_BITDEPTH_16BIT,	 0,	MEL_EXEC_PULS,	 3,	
		{"S",{
				/*FX3U/C devs&indx*/	"xymstcDRUA",
				/*FX3G/C   devs&indx*/	"xymstcDRUA"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"S",{
				/*FX3U/C devs&indx*/	"xymstcDRUA",
				/*FX3G/C   devs&indx*/	"xymstcDRUA"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"D",{
				/*FX3U/C devs&indx*/	"ymstcDRU",
				/*FX3G/C   devs&indx*/	"ymstcDRU"},	"",
			MEL_DUSAGE_DST,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	""},	//MEL_WPG_OpID_$+P_

	// ""
	{//"LEN"
		0,	0},	//MEL_WPG_OpID_LEN

	{//"LENP"
		0,	0},	//MEL_WPG_OpID_LENP

	{//"RIGHT"
		0,	0},	//MEL_WPG_OpID_RIGHT

	{//"RIGHTP"
		0,	0},	//MEL_WPG_OpID_RIGHTP

	{//"LEFT"
		0,	0},	//MEL_WPG_OpID_LEFT

	{//"LEFTP"
		0,	0},	//MEL_WPG_OpID_LEFTP

	{//"MIDR"
		0,	0},	//MEL_WPG_OpID_MIDR

	{//"MIDRP"
		0,	0},	//MEL_WPG_OpID_MIDRP

	{//"MIDW"
		0,	0},	//MEL_WPG_OpID_MIDW

	{//"MIDWP"
		0,	0},	//MEL_WPG_OpID_MIDWP

	{//"INSTR"
		0,	0},	//MEL_WPG_OpID_INSTR

	{//"INSTRP"
		0,	0},	//MEL_WPG_OpID_INSTRP

	{//"$MOV"
		0,	0},	//MEL_WPG_OpID_$MOV

	{//"$MOVP"
		0,	0},	//MEL_WPG_OpID_$MOVP

	{//"FDEL"
		0,	0},	//MEL_WPG_OpID_FDEL

	{//"FDELP"
		0,	0},	//MEL_WPG_OpID_FDELP

	{//"FINS"
		0,	0},	//MEL_WPG_OpID_FINS

	{//"FINSP"
		0,	0},	//MEL_WPG_OpID_FINSP

	{//"POP"
		0,	0},	//MEL_WPG_OpID_POP

	{//"POPP"
		0,	0},	//MEL_WPG_OpID_POPP

	// "Data Operations 3"
	{//"SFR"
		MEL_OPER_DATAOPS3          ,	213,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SFR

	{//"SFRP"
		MEL_OPER_DATAOPS3          ,	213,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SFRP

	{//"SFL"
		MEL_OPER_DATAOPS3          ,	214,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SFL

	{//"SFLP"
		MEL_OPER_DATAOPS3          ,	214,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_PULS,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_SFLP

	// ""
	{//"LD="
		0,	0},	//MEL_WPG_OpID_LD=

	{//"LD<>"
		0,	0},	//MEL_WPG_OpID_LD<>

	{//"LD>"
		0,	0},	//MEL_WPG_OpID_LD>

	{//"LD>="
		0,	0},	//MEL_WPG_OpID_LD>=

	// "Data Comparison"
	{//"LD<"
		MEL_OPER_DATACMP           ,	226,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_LD<

	// ""
	{//"LD<="
		0,	0},	//MEL_WPG_OpID_LD<=

	{//"LDD="
		0,	0},	//MEL_WPG_OpID_LDD=

	// "Data Comparison"
	{//"LDD<>"
		MEL_OPER_DATACMP           ,	228,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_LDD<>

	// ""
	{//"LDD>"
		0,	0},	//MEL_WPG_OpID_LDD>

	{//"LDD>="
		0,	0},	//MEL_WPG_OpID_LDD>=

	{//"LDD<"
		0,	0},	//MEL_WPG_OpID_LDD<

	{//"LDD<="
		0,	0},	//MEL_WPG_OpID_LDD<=

	// "Data Comparison"
	{//"AND="
		MEL_OPER_DATACMP           ,	232,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND=

	{//"AND<>"
		MEL_OPER_DATACMP           ,	236,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND<>

	{//"AND>"
		MEL_OPER_DATACMP           ,	233,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND>

	{//"AND>="
		MEL_OPER_DATACMP           ,	238,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND>=

	{//"AND<"
		MEL_OPER_DATACMP           ,	234,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND<

	{//"AND<="
		MEL_OPER_DATACMP           ,	237,	MEL_BITDEPTH_16BIT,	 5,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_AND<=

	{//"ANDD="
		MEL_OPER_DATACMP           ,	232,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDD=

	{//"ANDD<>"
		MEL_OPER_DATACMP           ,	236,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDD<>

	{//"ANDD>"
		MEL_OPER_DATACMP           ,	233,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDD>

	{//"ANDD>="
		MEL_OPER_DATACMP           ,	238,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDD>=

	{//"ANDD<"
		MEL_OPER_DATACMP           ,	234,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDD<

	{//"ANDD<="
		MEL_OPER_DATACMP           ,	237,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"SDP",{
				/*FX3U/C devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP",
				/*FX3G/C   devs&indx*/	"XYMrTCSbxymstcDRUVZKHEAP"},	"",
			MEL_DUSAGE_SRC
			|MEL_DUSAGE_DST
			|MEL_DUSAGE_PRM,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ANDD<=

	// ""
	{//"OR="
		0,	0},	//MEL_WPG_OpID_OR=

	{//"OR<>"
		0,	0},	//MEL_WPG_OpID_OR<>

	{//"OR>"
		0,	0},	//MEL_WPG_OpID_OR>

	{//"OR>="
		0,	0},	//MEL_WPG_OpID_OR>=

	{//"OR<"
		0,	0},	//MEL_WPG_OpID_OR<

	{//"OR<="
		0,	0},	//MEL_WPG_OpID_OR<=

	// "Data Comparison"
	{//"ORD="
		MEL_OPER_DATACMP           ,	240,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ORD=

	{//"ORD<>"
		MEL_OPER_DATACMP           ,	245,	MEL_BITDEPTH_32BIT,	 9,	MEL_EXEC_CONT,	 2,	
		{"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	
		"S",{
				/*FX3U/C devs&indx*/	"rxymstcDRUVZKH",
				/*FX3G/C   devs&indx*/	"rxymstcDRVZKH"},	"",
			MEL_DUSAGE_SRC,	{
							/*FX3U/C devs*/ {0,
								/*FX3U/C indx*/  0},
							/*FX3G/C   devs*/ {0,
								/*FX3G/C   indx*/  0}},	0},	0},	//MEL_WPG_OpID_ORD<>

	// ""
	{//"ORD>"
		0,	0},	//MEL_WPG_OpID_ORD>

	{//"ORD>="
		0,	0},	//MEL_WPG_OpID_ORD>=

	{//"ORD<"
		0,	0},	//MEL_WPG_OpID_ORD<

	{//"ORD<="
		0,	0},	//MEL_WPG_OpID_ORD<=

	{//"LIMIT"
		0,	0},	//MEL_WPG_OpID_LIMIT

	{//"LIMITP"
		0,	0},	//MEL_WPG_OpID_LIMITP

	{//"DLIMIT"
		0,	0},	//MEL_WPG_OpID_DLIMIT

	{//"DLIMITP"
		0,	0},	//MEL_WPG_OpID_DLIMITP

	{//"BAND"
		0,	0},	//MEL_WPG_OpID_BAND

	{//"BANDP"
		0,	0},	//MEL_WPG_OpID_BANDP

	{//"DBAND"
		0,	0},	//MEL_WPG_OpID_DBAND

	{//"DBANDP"
		0,	0},	//MEL_WPG_OpID_DBANDP

	{//"ZONE"
		0,	0},	//MEL_WPG_OpID_ZONE

	{//"ZONEP"
		0,	0},	//MEL_WPG_OpID_ZONEP

	{//"DZONE"
		0,	0},	//MEL_WPG_OpID_DZONE

	{//"DZONEP"
		0,	0},	//MEL_WPG_OpID_DZONEP

	{//"SCL"
		0,	0},	//MEL_WPG_OpID_SCL

	{//"SCLP"
		0,	0},	//MEL_WPG_OpID_SCLP

	{//"DSCL"
		0,	0},	//MEL_WPG_OpID_DSCL

	{//"DSCLP"
		0,	0},	//MEL_WPG_OpID_DSCLP

	{//"DABIN"
		0,	0},	//MEL_WPG_OpID_DABIN

	{//"DABINP"
		0,	0},	//MEL_WPG_OpID_DABINP

	{//"DDABIN"
		0,	0},	//MEL_WPG_OpID_DDABIN

	{//"DDABINP"
		0,	0},	//MEL_WPG_OpID_DDABINP

	{//"BINDA"
		0,	0},	//MEL_WPG_OpID_BINDA

	{//"BINDAP"
		0,	0},	//MEL_WPG_OpID_BINDAP

	{//"DBINDA"
		0,	0},	//MEL_WPG_OpID_DBINDA

	{//"DBINDAP"
		0,	0},	//MEL_WPG_OpID_DBINDAP

	{//"SCL2"
		0,	0},	//MEL_WPG_OpID_SCL2

	{//"SCL2P"
		0,	0},	//MEL_WPG_OpID_SCL2P

	{//"DSCL2"
		0,	0},	//MEL_WPG_OpID_DSCL2

	{//"DSCL2P"
		0,	0},	//MEL_WPG_OpID_DSCL2P

	{//"IVCK"
		0,	0},	//MEL_WPG_OpID_IVCK

	{//"IVDR"
		0,	0},	//MEL_WPG_OpID_IVDR

	{//"IVRD"
		0,	0},	//MEL_WPG_OpID_IVRD

	{//"IVWR"
		0,	0},	//MEL_WPG_OpID_IVWR

	{//"IVBWR"
		0,	0},	//MEL_WPG_OpID_IVBWR

	{//"IVMC"
		0,	0},	//MEL_WPG_OpID_IVMC

	{//"RBFM"
		0,	0},	//MEL_WPG_OpID_RBFM

	{//"WBFM"
		0,	0},	//MEL_WPG_OpID_WBFM

	{//"DHSCT"
		0,	0},	//MEL_WPG_OpID_DHSCT

	{//"LOADR"
		0,	0},	//MEL_WPG_OpID_LOADR

	{//"LOADRP"
		0,	0},	//MEL_WPG_OpID_LOADRP

	{//"SAVER"
		0,	0},	//MEL_WPG_OpID_SAVER

	{//"INITR"
		0,	0},	//MEL_WPG_OpID_INITR

	{//"INITRP"
		0,	0},	//MEL_WPG_OpID_INITRP

	{//"LOGR"
		0,	0},	//MEL_WPG_OpID_LOGR

	{//"LOGRP"
		0,	0},	//MEL_WPG_OpID_LOGRP

	{//"RWER"
		0,	0},	//MEL_WPG_OpID_RWER

	{//"RWERP"
		0,	0},	//MEL_WPG_OpID_RWERP

	{//"INITER"
		0,	0},	//MEL_WPG_OpID_INITER

	{//"INITERP"
		0,	0},	//MEL_WPG_OpID_INITERP

	{//"FLCRT"
		0,	0},	//MEL_WPG_OpID_FLCRT

	{//"FLDEL"
		0,	0},	//MEL_WPG_OpID_FLDEL

	{//"FLWR"
		0,	0},	//MEL_WPG_OpID_FLWR

	{//"FLRD"
		0,	0},	//MEL_WPG_OpID_FLRD

	{//"FLCMD"
		0,	0},	//MEL_WPG_OpID_FLCMD

	{//"FLSTRD"
		0,	0}	//MEL_WPG_OpID_FLSTRD

};


#endif //__mel_wpg_opcodes_table_h__
