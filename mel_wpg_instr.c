#include "mel_common.h"
#include "mel_wpg_instr.h"
#include "mel_wpg_fxcpu_regs.h"

#include <string.h>//memset
#include <stdio.h>
#include <stdlib.h>//strtod,strtol
//#include <errno.h> //errno

#ifdef WIN32
#pragma warning (disable : 4996)
#define STRNCASECMP(X,Y,N)  _strnicmp(X,Y,N)
#else
#define STRNCASECMP(X,Y,N)  strncasecmp(X,Y,N)
#endif

#define PRINTF_FOLDER_AS_DEFS	1
#define PRINTF_BITDEPTH_AS_DEFS 1
#define PRINTF_EXECTYPE_AS_DEFS 1
#define PRINTF_DUSAGE_AS_DEFS	1
#define PRINTF_BITS_AS_DEFS		0

static void			mel_wpg_add_attributes	(unsigned int dev_type,unsigned int* pattribs);
static int			mel_wpg_dec_operand		(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,WPG_Operand* oper);
static int			mel_wpg_enc_operand		(WPG_Operand* oper,int cpuSeries,int cpuType,unsigned char* stream);
static unsigned int mel_wpg_get_operand_from_txt(int cpuSeries,int cpuType,char* txt,unsigned int bitdepth,WPG_Operand* dst,int* pfirstbad_device);
static unsigned int mel_wpg_check_operand_syntax(int cpuSeries,int cpuType,WPG_Operand* operand,unsigned int bitdepth,int* pfirstbad_device);
static unsigned int mel_wpg_check_operand_usability(int cpuSeries,int cpuType,WPG_Operand* operand,unsigned int bitdepth,WPGOperandRecord* tbl_operand,int* pfirstbad_device);
static unsigned int mel_wpg_check_device_types(PLCRegDevice* device,struct operand_attr* rules);
static void			mel_wpg_arrange_devices	(WPG_Operand* operand);
static unsigned int mel_wpg_txt2args_slicer	(char* text,int textsize,char* args,int* pfields);
static unsigned int mel_wpg_txt2ASCII		(int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device);
static unsigned int mel_wpg_txt2UG			(int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device);
static unsigned int mel_wpg_txt2FLOAT		(int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device);
static unsigned int mel_wpg_txt2COMPOSED	(unsigned int dtype,
											 int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device);
static unsigned int mel_wpg_txt2operand		(int cpuSeries,int cpuType,char* text,int textsize,WPG_Operand* operand,
												WPGOperandRecord* tbl_operand,unsigned int bitdepth,int* pfirstbad_device);

static void mel_wpg_debug(WPG_Instruction* instruction,unsigned int stcode,int firstbad_operand,int firstbad_device);

int mel_wpg_instr_make_opcode(WPG_Instruction* instr,unsigned char* opcode_bytes)
{	
	int opcode_size = -1;
	int opid	= instr->opid;
	int steps	= instr->steps;

	switch(opid)
	{
		//0 bytes
	case MEL_WPG_OpID_NOP:	opcode_size = 0; break;

		//1 or 2 bytes
	case MEL_WPG_OpID_LD:
	case MEL_WPG_OpID_LDI:
	case MEL_WPG_OpID_LDP:
	case MEL_WPG_OpID_LDF:
	case MEL_WPG_OpID_AND:
	case MEL_WPG_OpID_ANI:
	case MEL_WPG_OpID_ANDP:
	case MEL_WPG_OpID_ANDF:
	case MEL_WPG_OpID_OR:
	case MEL_WPG_OpID_ORI:
	case MEL_WPG_OpID_ORP:
	case MEL_WPG_OpID_ORF:
	case MEL_WPG_OpID_ANB:
	case MEL_WPG_OpID_ORB:	
	case MEL_WPG_OpID_MPS:
	case MEL_WPG_OpID_MRD:
	case MEL_WPG_OpID_MPP:
	case MEL_WPG_OpID_INV:
	case MEL_WPG_OpID_MEP:
	case MEL_WPG_OpID_MEF:
	case MEL_WPG_OpID_OUT:		
	case MEL_WPG_OpID_OUT_TC:	
	case MEL_WPG_OpID_SET:
	case MEL_WPG_OpID_RST:
	case MEL_WPG_OpID_PLS:
	case MEL_WPG_OpID_PLF:

	case MEL_WPG_OpID_MC:
	case MEL_WPG_OpID_MCR:
	case MEL_WPG_OpID_END:
	case MEL_WPG_OpID_FEND:
	
		opcode_size = (steps > 1)? 2:1;
		break;
		
	default://3 or 4 bytes

		//4 bytes
		if (opid >= MEL_WPG_OpID_LDeq && opid <= MEL_WPG_OpID_ORDlessseq)
		{	
			opcode_size = 4;

			//[2],[3]: minor opcodes
			if (opid >= MEL_WPG_OpID_LDeq && opid <= MEL_WPG_OpID_LDDlessseq)
			{	//LDx
				opcode_bytes[2] = opid - MEL_WPG_OpID_LDeq;
				opcode_bytes[3] = 0x10;
			}
			if (opid >= MEL_WPG_OpID_ANDeq && opid <= MEL_WPG_OpID_ANDDlessseq)
			{	//ANDx
				opcode_bytes[2] = opid - MEL_WPG_OpID_ANDeq;
				opcode_bytes[3] = 0x11;
			}
			if (opid >= MEL_WPG_OpID_OReq && opid <= MEL_WPG_OpID_ORDlessseq)
			{	//ORx
				opcode_bytes[2] = opid - MEL_WPG_OpID_OReq;
				opcode_bytes[3] = 0x12;
			}
		}
		else
		{
			//3 bytes
			opcode_size = 3;

			switch(opid)
			{
				//0x49
			case MEL_WPG_OpID_linkstr:	opcode_bytes[2] = 0x23;	break;
			case MEL_WPG_OpID_ADD:		opcode_bytes[2] = 0x28;	break;
			case MEL_WPG_OpID_SUB:		opcode_bytes[2] = 0x2A;	break;
			case MEL_WPG_OpID_DSUB:		opcode_bytes[2] = 0x2B;	break;
			case MEL_WPG_OpID_MUL:		opcode_bytes[2] = 0x2C;	break;
			case MEL_WPG_OpID_DMUL:		opcode_bytes[2] = 0x2D;	break;
			case MEL_WPG_OpID_DEADD:	opcode_bytes[2] = 0x30;	break;
			case MEL_WPG_OpID_DESUB:	opcode_bytes[2] = 0x31;	break;
			case MEL_WPG_OpID_DEMUL:	opcode_bytes[2] = 0x32;	break;
			case MEL_WPG_OpID_DEDIV:	opcode_bytes[2] = 0x33;	break;
				//0x4A
			case MEL_WPG_OpID_INC:		opcode_bytes[2] = 0x00;	break;
			case MEL_WPG_OpID_DEC:		opcode_bytes[2] = 0x02;	break;
				//0x4B			
			case MEL_WPG_OpID_DNEG:		opcode_bytes[2] = 0x0F;	break;			
			case MEL_WPG_OpID_DINT:		opcode_bytes[2] = 0x05;	break;
				//0x4C
			case MEL_WPG_OpID_MOV:		opcode_bytes[2] = 0x00;	break;	
			case MEL_WPG_OpID_DMOV:		opcode_bytes[2] = 0x01;	break;	
			case MEL_WPG_OpID_BMOV:		opcode_bytes[2] = 0x06;	break;	
			case MEL_WPG_OpID_DEMOV:	opcode_bytes[2] = 0x0F;	break;
				//0x4D
			case MEL_WPG_OpID_CJ:		opcode_bytes[2] = 0x00;	break;
				//0x4F
			case MEL_WPG_OpID_WAND:		opcode_bytes[2] = 0x01;	break;
			case MEL_WPG_OpID_WOR:		opcode_bytes[2] = 0x03;	break;
			case MEL_WPG_OpID_DAND:		opcode_bytes[2] = 0x09;	break;
			case MEL_WPG_OpID_DOR:		opcode_bytes[2] = 0x0B;	break;
				//0x50
			case MEL_WPG_OpID_DROL:		opcode_bytes[2] = 0x06;	break;
				//0x51
			case MEL_WPG_OpID_SFR:		opcode_bytes[2] = 0x00;	break;
			case MEL_WPG_OpID_SFL:		opcode_bytes[2] = 0x01;	break;
				//0x53
			case MEL_WPG_OpID_ZRST:		opcode_bytes[2] = 0x19;	break;
			case MEL_WPG_OpID_FLT:		opcode_bytes[2] = 0x22;	break;
			case MEL_WPG_OpID_DFLT:		opcode_bytes[2] = 0x23;	break;
				//0x54
			case MEL_WPG_OpID_CALL:		opcode_bytes[2] = 0x01;	break;
				//0x56
			case MEL_WPG_OpID_FROM:		opcode_bytes[2] = 0x00;	break;
			case MEL_WPG_OpID_TO:		opcode_bytes[2] = 0x02;	break;
				//0x5F
			case MEL_WPG_OpID_RS2:		opcode_bytes[2] = 0x1D;	break;
				//0x69
			case MEL_WPG_OpID_DI:		opcode_bytes[2] = 0x00;	break;
			case MEL_WPG_OpID_EI:		opcode_bytes[2] = 0x01;	break;
			case MEL_WPG_OpID_IRET:		opcode_bytes[2] = 0x03;	break;
				//0x6A
			case MEL_WPG_OpID_FOR:		opcode_bytes[2] = 0x00;	break;
			case MEL_WPG_OpID_NEXT:		opcode_bytes[2] = 0x01;	break;
			case MEL_WPG_OpID_SRET:		opcode_bytes[2] = 0x07;	break;

			default: opcode_size = -1;//unsupported opid
			}
		}
	}
	//[0]: major opcode
	if (opcode_size > 0)
	{
		opcode_bytes[0] = INSTRUCTIONS_OPCODES_FXCPU[opid];
	}
	//[1]: steps
	if (opcode_size > 1)
	{
		opcode_bytes[1] = steps&0xFF;
	}

	return(opcode_size);
}

int mel_wpg_instr_setup_by_opcode(	unsigned char* enc_code,int code_size,WPG_Instruction* instr)
{
	int opid = -1;//unknown (by default)
	int steps = 0;
	
	if (code_size > 1)	steps = enc_code[1];

	switch(code_size)
	{
	case 0://////////////////////////////////////////////////////////////////////////
		//NOP
		opid = MEL_WPG_OpID_NOP;	steps = 1;	break;
	case 1://////////////////////////////////////////////////////////////////////////
	case 2://////////////////////////////////////////////////////////////////////////
		switch(enc_code[0])
		{
		case 0x00:opid = MEL_WPG_OpID_LD;		break;
		case 0x01:opid = MEL_WPG_OpID_LDI;		break;
		case 0x02:opid = MEL_WPG_OpID_LDP;		break;
		case 0x03:opid = MEL_WPG_OpID_LDF;		break;

		case 0x06:opid = MEL_WPG_OpID_OR;		break;

		case 0x09:opid = MEL_WPG_OpID_ORF;		break;

		case 0x0C:opid = MEL_WPG_OpID_AND;		break;
		case 0x0D:opid = MEL_WPG_OpID_ANI;		break;
		case 0x0E:opid = MEL_WPG_OpID_ANDP;		break;

		case 0x18:opid = MEL_WPG_OpID_ORB;		break;
		case 0x19:opid = MEL_WPG_OpID_ANB;		break;
		case 0x1A:opid = MEL_WPG_OpID_MPS;		break;
		case 0x1B:opid = MEL_WPG_OpID_MRD;		break;
		case 0x1C:opid = MEL_WPG_OpID_MPP;		break;
			
		case 0x20:opid = MEL_WPG_OpID_OUT;		break;
		case 0x21:opid = MEL_WPG_OpID_OUT_TC;	break;

		case 0x23:opid = MEL_WPG_OpID_SET;		break;
		case 0x24:opid = MEL_WPG_OpID_RST;		break;

		case 0x2C:opid = MEL_WPG_OpID_MC;		break;

		case 0x30:opid = MEL_WPG_OpID_MCR;		break;

		case 0x33:opid = MEL_WPG_OpID_FEND;		break;
		case 0x34:opid = MEL_WPG_OpID_END;		break;
		}
		break;

	case 3://////////////////////////////////////////////////////////////////////////
		switch(enc_code[0])
		{
		case 0x49:
			switch(enc_code[2])
			{
			case 0x23://			0xFF490723,	//MEL_WPG_OpID_linkstr		(!)RANGED: [0xFF49[07]23...0xFF49[25]23]
				opid = MEL_WPG_OpID_linkstr;	break;
			case 0x28:// 			0xFF490728,	//MEL_WPG_OpID_ADD
				opid = MEL_WPG_OpID_ADD;		break;
			case 0x2A:// 			0xFF49072A,	//MEL_WPG_OpID_SUB
				opid = MEL_WPG_OpID_SUB;		break;
			case 0x2B:// 			0xFF490D2B,	//MEL_WPG_OpID_DSUB
				opid = MEL_WPG_OpID_DSUB;		break;
			case 0x2C:// 			0xFF49072C,	//MEL_WPG_OpID_MUL
				opid = MEL_WPG_OpID_MUL;		break;
			case 0x2D:// 			0xFF490D2D,	//MEL_WPG_OpID_DMUL
				opid = MEL_WPG_OpID_DMUL;		break;
			case 0x30:// 			0xFF490D30,	//MEL_WPG_OpID_DEADD
				opid = MEL_WPG_OpID_DEADD;		break;
			case 0x31:// 			0xFF490D31,	//MEL_WPG_OpID_DESUB
				opid = MEL_WPG_OpID_DESUB;		break;
			case 0x32:// 			0xFF490D32,	//MEL_WPG_OpID_DEMUL
				opid = MEL_WPG_OpID_DEMUL;		break;
			case 0x33:// 			0xFF490D33,	//MEL_WPG_OpID_DEDIV
				opid = MEL_WPG_OpID_DEDIV;		break;
			}
			break;
		case 0x4A:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF4A0300,	//MEL_WPG_OpID_INC
				opid = MEL_WPG_OpID_INC; break;
			case 0x02:// 			0xFF4A0302,	//MEL_WPG_OpID_DEC
				opid = MEL_WPG_OpID_DEC; break;
			}
			break;
		case 0x4B:
			switch(enc_code[2])
			{
			case 0x05:// 			0xFF4B0905,	//MEL_WPG_OpID_DINT
				opid = MEL_WPG_OpID_DINT; break;
			case 0x0F:// 			0xFF4B050F,	//MEL_WPG_OpID_DNEG
				opid = MEL_WPG_OpID_DNEG; break;
			}
			break;
		case 0x4C:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF4C0500,	//MEL_WPG_OpID_MOV
				opid = MEL_WPG_OpID_MOV; break;
			case 0x01:// 			0xFF4C0901,	//MEL_WPG_OpID_DMOV
				opid = MEL_WPG_OpID_DMOV; break;
			case 0x06:// 			0xFF4C0706,	//MEL_WPG_OpID_BMOV
				opid = MEL_WPG_OpID_BMOV; break;
			case 0x0F://			0xFF4C090F,	//MEL_WPG_OpID_DEMOV
				opid = MEL_WPG_OpID_DEMOV; break;
			}
			break;
		case 0x4D:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF4D0300,	//MEL_WPG_OpID_CJ
				opid = MEL_WPG_OpID_CJ; break;
			}
			// 			0x00000000,	//MEL_WPG_OpID_CJP
			break;

		case 0x4F:
			switch(enc_code[2])
			{
			case 0x01:// 			0xFF4F0701,	//MEL_WPG_OpID_WAND
				opid = MEL_WPG_OpID_WAND; break;
			case 0x03:// 			0xFF4F0703,	//MEL_WPG_OpID_WOR
				opid = MEL_WPG_OpID_WOR; break;
			case 0x09:// 			0xFF4F0D09,	//MEL_WPG_OpID_DAND
				opid = MEL_WPG_OpID_DAND; break;
			case 0x0B:// 			0xFF4F0D0B,	//MEL_WPG_OpID_DOR
				opid = MEL_WPG_OpID_DOR; break;
			}
			break;

		case 0x50:
			switch(enc_code[2])
			{
			case 0x06:// 			0xFF500906,	//MEL_WPG_OpID_DROL
				opid = MEL_WPG_OpID_DROL; break;
			}
			break;

		case 0x51:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF510500,	//MEL_WPG_OpID_SFR
				opid = MEL_WPG_OpID_SFR; break;
			case 0x01:// 			0xFF510501,	//MEL_WPG_OpID_SFL
				opid = MEL_WPG_OpID_SFL; break;
			}
			break;

		case 0x53:
			switch(enc_code[2])
			{
			case 0x19:// 			0xFF530519,	//MEL_WPG_OpID_ZRST
				opid = MEL_WPG_OpID_ZRST; break;
			case 0x22:// 			0xFF530522,	//MEL_WPG_OpID_FLT
				opid = MEL_WPG_OpID_FLT; break;
			case 0x23:// 			0xFF530523,	//MEL_WPG_OpID_DFLT
				opid = MEL_WPG_OpID_DFLT; break;
			}
			break;
		case 0x54:opid = MEL_WPG_OpID_CALL;
			switch(enc_code[2])
			{
			case 0x01:// 			0xFF540301,	//MEL_WPG_OpID_CALL
				opid = MEL_WPG_OpID_CALL; break;
			}
			// 			0x00000000,	//MEL_WPG_OpID_CALLP
			break;

		case 0x56:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF560900,	//MEL_WPG_OpID_FROM
				opid = MEL_WPG_OpID_FROM; break;
			case 0x02:// 			0xFF560902,	//MEL_WPG_OpID_TO
				opid = MEL_WPG_OpID_TO; break;
			}
			break;

		case 0x5F:
			switch(enc_code[2])
			{
			case 0x1D://0xFF5F0B1D,	//MEL_WPG_OpID_RS2
				opid = MEL_WPG_OpID_RS2; break;				
			}
			break;

		case 0x69:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF690100,	//MEL_WPG_OpID_DI
				opid = MEL_WPG_OpID_DI; break;
			case 0x01:// 			0xFF690101,	//MEL_WPG_OpID_EI
				opid = MEL_WPG_OpID_EI; break;
			case 0x03:// 			0xFF690103,	//MEL_WPG_OpID_IRET
				opid = MEL_WPG_OpID_IRET; break;				
			}
			break;
		case 0x6A:
			switch(enc_code[2])
			{
			case 0x00:// 			0xFF6A0300,	//MEL_WPG_OpID_FOR
				opid = MEL_WPG_OpID_FOR; break;
			case 0x01:// 			0xFF6A0101,	//MEL_WPG_OpID_NEXT
				opid = MEL_WPG_OpID_NEXT; break;
			case 0x07:// 			0xFF6A0107,	//MEL_WPG_OpID_SRET
				opid = MEL_WPG_OpID_SRET; break;
			}
			break;
		}
		break;

	case 4://////////////////////////////////////////////////////////////////////////
		switch(enc_code[0])
		{
		case 0x40:
			switch(enc_code[3])
			{
			case 0x10:	opid = MEL_WPG_OpID_LDeq;	break;
			case 0x11:	opid = MEL_WPG_OpID_ANDeq;	break;
			case 0x12:	opid = MEL_WPG_OpID_OReq;	break;
			}
			if (opid != -1)
			{
				if (enc_code[2] >= 0x00 && enc_code[2]<= 0x0B)
				{
					int tvalue = enc_code[2];
					if (tvalue >= 0x06)
					{
						opid  += 6;//LDx -> LDDx, ANDx -> ANDDx, ORx -> ORDx
						tvalue-= 6;
					}
					opid += tvalue;//==,<>,>,>=,<,<=
				}
				else
					opid = -1;//unknown!
			}
			break;
		}
		break;
	}
		
	if (opid != -1)
	{
		if (instr)
		{
			if (INSTRUCTIONS_METRIX_FXCPU[opid].folder)
			{
				instr->opid			= opid;
								
				if(!steps && INSTRUCTIONS_METRIX_FXCPU[opid].steps)
					steps = INSTRUCTIONS_METRIX_FXCPU[opid].steps;				
				
				//instr->steps		= steps;
				
				instr->bitdepth		= INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth;
				instr->exectype		= INSTRUCTIONS_METRIX_FXCPU[opid].exectype;
				instr->n_params = INSTRUCTIONS_METRIX_FXCPU[opid].n_operands;				
			}
			else{opid = -1;}
		}
	}
	instr->steps = steps;//set steps anyway (in both cases: good and not)
	return(opid != -1);
}
//works with correct instructions only (must be validated before calling!)
int mel_wpg_instr_steps(WPG_Instruction* instr)
{
	int steps = 0;
		
	if (INSTRUCTIONS_METRIX_FXCPU[instr->opid].steps > 0)	
	{
		steps = INSTRUCTIONS_METRIX_FXCPU[instr->opid].steps;
	}
	else
	{	
		steps = 1;//default

		switch(instr->opid)
		{
		case MEL_WPG_OpID_LD:
		case MEL_WPG_OpID_LDI:
		case MEL_WPG_OpID_AND:
		case MEL_WPG_OpID_ANI:
		case MEL_WPG_OpID_OR:
		case MEL_WPG_OpID_ORI:			
			//if (instr->is_indexed)
			if (instr->attribs &	(
									 MEL_WPG_OPERAND_ATTRIB_INDEXED
									|MEL_WPG_OPERAND_ATTRIB_BITACCESS
									)
				)
			{
				steps = 3;
			}
			else
			{
				switch(instr->params[0].devs[0].dtype)
				{
				case MEL_DEV_TYPE_M:
				//case MEL_OTYPE_Msys:
				//case MEL_DEV_TYPE_M|MEL_OTYPE_Msys:

					if (instr->params[0].devs[0].index>= 1536 && instr->params[0].devs[0].index <= 3583)
					{
						steps = 2;
					}
					else
						if (instr->params[0].devs[0].index>= 3584 && instr->params[0].devs[0].index <= 7679)
						{
							steps = 3;
						}
						else
							if (instr->params[0].devs[0].index >= 8256 && instr->params[0].devs[0].index <= 8511)
							{
								steps = 2;
							}
					break;

				case MEL_DEV_TYPE_S:
					if (instr->params[0].devs[0].index > 1023)
					{
						steps = 2;
					}
					break;
				}
			}
			break;

		case MEL_WPG_OpID_LDP:
		case MEL_WPG_OpID_LDF:
		case MEL_WPG_OpID_ANDP:
		case MEL_WPG_OpID_ANDF:
		case MEL_WPG_OpID_ORP:
		case MEL_WPG_OpID_ORF:				
			steps = 2;//default
			//if (instr->is_indexed)
			if (instr->attribs & (	 MEL_WPG_OPERAND_ATTRIB_BITACCESS
									)
				)
			{
				steps = 3;
			}
			else
				if (	instr->params[0].devs[0].dtype == MEL_DEV_TYPE_M
					//||	instr->params[0].devs[0].dtype == MEL_OTYPE_Msys
					//||	instr->params[0].devs[0].dtype == (MEL_OTYPE_Msys|MEL_DEV_TYPE_M)
					)
				{
					if (instr->params[0].devs[0].index >= 3584 && instr->params[0].devs[0].index <= 7679)
					{
						steps = 3;
					}
				}
			break;

		case MEL_WPG_OpID_OUT:
		case MEL_WPG_OpID_OUT_TC:
			//if(instr->is_indexed)
			if (instr->attribs & (	 MEL_WPG_OPERAND_ATTRIB_INDEXED
									|MEL_WPG_OPERAND_ATTRIB_BITACCESS
									)
				)
			{
				steps = 3;
				switch (instr->params[0].devs[0].dtype)
				{
				case MEL_DEV_TYPE_T:
					if (instr->params[0].devs[0].index <= 511)//?
					{
						steps = 4;
					}
					break;
				case MEL_DEV_TYPE_C:
					if (instr->params[0].devs[0].index <= 199)//?
					{
						steps = 4;
					}
					break;
				}
			}
			else
			{
				switch (instr->params[0].devs[0].dtype)
				{
				case MEL_DEV_TYPE_M:
				//case MEL_OTYPE_Msys:
				//case MEL_DEV_TYPE_M|MEL_OTYPE_Msys:

					if (	(instr->params[0].devs[0].index >= 1536 && instr->params[0].devs[0].index <= 3583) 
						||	(instr->params[0].devs[0].index >= 8000 && instr->params[0].devs[0].index <= 8511)//?
						)
					{
						steps = 2;
					}
					else
						if (instr->params[0].devs[0].index >= 3584 && instr->params[0].devs[0].index <= 7679)
						{
							steps = 3;
						}
					break;
				case MEL_DEV_TYPE_S:
					steps = 2;
					break;
				case MEL_DEV_TYPE_T:
					if (instr->params[0].devs[0].index <= 511)//?
					{
						steps = 3;
					}
					break;
				case MEL_DEV_TYPE_C:
					steps = 3;
					if (instr->params[0].devs[0].index >= 200 && instr->params[0].devs[0].index <= 255)//?
					{
						steps = 5;
					}
					break;
				}
			}
			break;

		case MEL_WPG_OpID_SET:
			//if(instr->is_indexed)
			if (instr->attribs & (	 MEL_WPG_OPERAND_ATTRIB_INDEXED
									|MEL_WPG_OPERAND_ATTRIB_BITACCESS
									)
				)
			{
				steps = 3;
			}
			else
			{
				switch (instr->params[0].devs[0].dtype)
				{
				case MEL_DEV_TYPE_M:
				//case MEL_OTYPE_Msys:
				//case MEL_DEV_TYPE_M|MEL_OTYPE_Msys:

					if (	(instr->params[0].devs[0].index >= 1536 && instr->params[0].devs[0].index <= 3583) 
						||	(instr->params[0].devs[0].index >= 8000 && instr->params[0].devs[0].index <= 8511) //?
						)
					{
						steps = 2;
					}
					else
						if (instr->params[0].devs[0].index >= 3584 && instr->params[0].devs[0].index <= 7679)
						{
							steps = 3;
						}

					break;

				case MEL_DEV_TYPE_S:
					if (instr->params[0].devs[0].index <= 4095)//?
					{
						steps = 2;
					}
					break;
				}
			}
			break;

		case MEL_WPG_OpID_RST:
			//if(instr->is_indexed)
			if (instr->attribs & (	 MEL_WPG_OPERAND_ATTRIB_INDEXED
									|MEL_WPG_OPERAND_ATTRIB_BITACCESS
									)
				)
			{
				steps = 3;
			}
			else
			{
				switch (instr->params[0].devs[0].dtype)
				{
				case MEL_DEV_TYPE_M:
				//case MEL_OTYPE_Msys:
				//case MEL_DEV_TYPE_M|MEL_OTYPE_Msys:
					if (	(instr->params[0].devs[0].index >= 1536 && instr->params[0].devs[0].index <= 3583) 
						||	(instr->params[0].devs[0].index >= 8000 && instr->params[0].devs[0].index <= 8511) //?
						)
					{
						steps = 2;
					}
					else
						if (instr->params[0].devs[0].index >= 3584 && instr->params[0].devs[0].index <= 7679)
						{
							steps = 3;
						}
					break;

				case MEL_DEV_TYPE_S:
					if (instr->params[0].devs[0].index <= 4095)//?
					{
						steps = 2;
					}
					break;
				case MEL_DEV_TYPE_T:
					if (instr->params[0].devs[0].index <= 511)//?
					{
						steps = 2;
					}
					break;
				case MEL_DEV_TYPE_C:
					if (instr->params[0].devs[0].index <= 255)//?
					{
						steps = 2;
					}					
					break;
				}
			}			
			break;

		case MEL_WPG_OpID_PLS:
		case MEL_WPG_OpID_PLF:
			steps = 2;//default
			//if (instr->is_indexed)
			if (instr->attribs & (	 MEL_WPG_OPERAND_ATTRIB_INDEXED									
									)
				)
			{
				steps = 3;
			}
			else
			{
				switch(instr->params[0].devs[0].dtype)
				{
				case MEL_DEV_TYPE_M:
				//case MEL_OTYPE_Msys:
				//case MEL_DEV_TYPE_M|MEL_OTYPE_Msys:

					if (instr->params[0].devs[0].index >= 3584 && instr->params[0].devs[0].index <= 7679)
					{
						steps = 3;
					}
					break;
				}
			}
			break;

		case MEL_WPG_OpID_MC:
			//if (instr->n_params > 1)
			//{
			steps = 3;//default;
			switch(instr->params[1].devs[0].dtype)
			{
			case MEL_DEV_TYPE_M:
			//case MEL_OTYPE_Msys:
			//case MEL_DEV_TYPE_M|MEL_OTYPE_Msys:

				if (instr->params[1].devs[0].index >= 3584 && instr->params[1].devs[0].index <= 7679)
				{
					steps = 4;
				}
				break;
			}
			//}
			break;

		case MEL_WPG_OpID_linkstr:
			{
				//int strsize = strlen(instr->params[1].devs[0].const_str);
				int strsize = instr->params[1].devs[0].const_str_length;
				steps = (strsize >= 2)? (7 + (strsize - 2) ):(7);
			}			
		}
	}
	return(steps);
}

static int	mel_wpg_dec_operand	(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,WPG_Operand* oper)
{	
	int size_done = 0;
	int goon;

	//clean up first
	memset(oper,0x00,sizeof(*oper));//oper->devs_count = 0;
	goon = 1;
	do 
	{
		if (oper->devs_count >= MEL_WPG_MAX_NDEVS_PER_OPERAND)
		{
			size_done = 0;//error
			goon = 0;//prevent overrun in case of corrupted stream
		}
		else
		{
			int dev_size;

			dev_size = mel_wpg_dec_device(	cpuSeries,cpuType,
											stream + size_done,
											stream_size - size_done,
											&oper->devs[oper->devs_count]);
			if (dev_size > 0)
			{
				unsigned int dtype = oper->devs[oper->devs_count].dtype;

				size_done += dev_size;

				/*
				KnXm,...	wpg-encoding: Kn,Xm
				KnXmVk,...	wpg-encoding: Kn,Vk,Xm

				XmVk,...	wpg-encoding: Vk,Xm

				Dn.b,...	wpg-encoding: .b,Dn

				Un\Gm,...	wpg-encoding: Un,Gm
				Un\GmVk,...	wpg-encoding: Un,Vk,Gm
				*/
				//separate two-devices operands from single ones
				if (	dtype & (MEL_DEV_TYPE_VI
								|MEL_DEV_TYPE_ZI
								|MEL_DEV_TYPE_Kn
								|MEL_DEV_TYPE_bI
								|MEL_DEV_TYPE_Un
								)
					)
				{
					//there is second device (indexed by first one)
					mel_wpg_add_attributes(dtype,&oper->attribs);
					//just keep decoding till second device;
				}
				else
				{
					goon = 0;//finish
				}

				oper->devs_count++;
			}
			else
			{
				goon = 0;//error
			}
		}
	} while (goon);
	//printf("oper->devs_count: %d\t",oper->devs_count);
	return(size_done);
}

//using mel_wpg_enc_device()

static int	mel_wpg_enc_operand(WPG_Operand* oper,int cpuSeries,int cpuType,unsigned char* stream)
{
	int size_done = 0;
	int i;

	for (i = 0; i < oper->devs_count; i++)
	{
		int dev_size;

		dev_size = mel_wpg_enc_device(&oper->devs[i],cpuSeries,cpuType,stream + size_done);

		if (dev_size > 0)
		{
			size_done += dev_size;
		}
		else
		{
			size_done = 0;//error: can't encode device
			break;
		}
	}
	return(size_done);
}

WPG_Item*	mel_wpg_dec_instruction	(	int cpuSeries,int cpuType,
										unsigned char* stream,int stream_size,
										int* psizedone,int* psteps)
{
	WPG_Item* item = 0;
	int size_done = 0;
	int size_brutto = *psizedone;
	int size_netto = size_brutto - 2;	
	WPG_Instruction instr = {0};//used temporary only for opcode decoding
	int steps = 0;
	int is_recognized;

#if 1
	//DEBUG
	static int is_after_END = 0;	
	if(is_after_END && !(*psteps))
	{
		is_after_END = 0;
	}
#endif

	if(		size_netto < MEL_WPG_OPCODE_SIZE_NETTO_MIN 
		||	size_netto > MEL_WPG_OPCODE_SIZE_NETTO_MAX )
		goto __exit_point;
	
	size_done += size_brutto;
	is_recognized = mel_wpg_instr_setup_by_opcode(stream+1,size_netto,&instr);

#if 1
	if (!is_after_END)
	{
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		if(size_netto > 0)
		{
			int i_byte;
			printf("%d\t{",is_recognized);
			for (i_byte = 0; i_byte < size_brutto; i_byte++)
			{
				printf("%02X ",stream[i_byte]);
			}
			printf("}\n");
		}
	}
#endif

	if (is_recognized)
	{
		item = calloc(1,sizeof(WPG_Item));
		if (item)
		{
			item->itype = WPG_ITEM_TYPE_INSTRUCTION;
			item->size = sizeof(WPG_Instruction); 
			item->body = calloc(1,item->size);
			if (item->body)
			{
				WPG_Instruction* ins = item->body;

				memcpy(ins,&instr,sizeof(WPG_Instruction));

				if (ins->n_params > 0)
				{
					int i;
					//decode operands (if any)
					for (i = 0; i < ins->n_params; i++)
					{
						int oper_size;

						memset(&ins->params[i],0x00,sizeof(WPG_Operand));

						oper_size = mel_wpg_dec_operand(cpuSeries,cpuType,
														stream		+size_done,
														stream_size	-size_done,
														&ins->params[i]);
						size_done += oper_size;
						
						if (oper_size == 0)
						{
							//error (UNKNOWN OPERAND): quit from loop
							printf("UNKNOWN OPERAND!\n");
							free(item->body);
							free(item); item = 0;
							goto __exit_point;

						}
						
						//accumulate attributes
						ins->attribs |= ins->params[i].attribs;

#if 1						
						//DEBUG
						if (!is_after_END)
						{
							int i_byte;
							printf("\t\t\t(");
							for (i_byte = size_done - oper_size; i_byte < size_done; i_byte++)
							{
								printf("%02X ",stream[i_byte]);
							}
							printf(");\n");
							//printf("\t");
						}	
#endif
					}
				}

				if (!ins->steps)
				{	//get instruction cost in steps (according to operands)
					ins->steps = mel_wpg_instr_steps(ins);
				}				
				//instruction cost in steps
				steps = ins->steps;

#if 1
				//DEBUG				
				{
					int instrtextsize;
					char* instrtext;

					instrtext  = mel_wpg_instr2text(cpuSeries,cpuType,ins,&instrtextsize);

					if (instrtext)
					{
						unsigned int stcode;
						int firstbad_operand = 0;
						int firstbad_device = 0;
						WPG_Instruction welldone = {0};
						int steps;

#if 1										

						stcode = mel_wpg_text2instr(cpuSeries,cpuType,instrtext,instrtextsize,
													&firstbad_operand,&firstbad_device,
													&steps,&welldone);
						if (!is_after_END)
						{
							printf("\"%s\"\n",instrtext);
							printf("%0X\t[%d:%d]\t\t\t%5d\t%s\n\n",stcode,firstbad_operand,firstbad_device,*psteps,instrtext);
						}
						
						//DEBUG
						if(stcode!=INSTR_STATE_OK) exit(1);

						if (stcode==INSTR_STATE_OK)
						{
							int instrtextsize2;
							char* instrtext2;
							char first[MEL_WPG_MAX_INSTR_TEXT_SIZE] = {0};

							strcpy(first,instrtext);

							instrtext2  = mel_wpg_instr2text(cpuSeries,cpuType,&welldone,&instrtextsize2);
							if (instrtext2)
							{
								if (strcmp(first,instrtext2))
								{
									printf("DIFFERENT!!!:\n[%s]!=[%s]\n",first,instrtext2);
								}
							}
							//printf("%s\t\t\t%s\n",first,instrtext2);
						}
						//printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif						
					}
				}
#endif
				if (!is_after_END && ins->opid == MEL_WPG_OpID_END)
				{
					is_after_END = 1;
				}				
			}
			else
			{//error(NO MEMORY)
				free(item); item = 0;
				goto __exit_point;
			}
		}
		//else{//error(NO MEMORY)}		
	}
	else
	{
		printf("UNKNOWN INSTR CODE: %02X\t{",stream[1]);
		{
			int i_byte;
			for (i_byte = 0; i_byte < size_brutto; i_byte++)
			{
				printf("%02X ",stream[i_byte]);
			}
		}
		printf("}\n");
	}

__exit_point:
		
	*psizedone	= size_done;
	*psteps		= steps;
	return(item);
}

int	mel_wpg_enc_instruction	(WPG_Item* item,int cpuSeries,int cpuType,unsigned char* stream,int* psteps)
{
	int size_done = 0;
	int size_netto;	
	int steps = 0;
	WPG_Instruction* ins = item->body;
	
	size_netto = mel_wpg_instr_make_opcode(ins,stream+1);

	if (	size_netto >= MEL_WPG_OPCODE_SIZE_NETTO_MIN
		&&	size_netto <= MEL_WPG_OPCODE_SIZE_NETTO_MAX)
	{
		int size_brutto;

		size_brutto = size_netto + 2;
		stream[0] = size_brutto;
		stream[size_brutto-1] = stream[0];

		size_done = size_brutto;

		//encode operands (if any)
		if (ins->n_params > 0)
		{
			int i;		
			for (i = 0; i < ins->n_params; i++)
			{
				int oper_size;						
				oper_size = mel_wpg_enc_operand(&ins->params[i],cpuSeries,cpuType,stream+size_done);
				size_done += oper_size;
				if (oper_size == 0)
				{
					size_done = 0;//error
					break;
				}
			}
		}
		//get instruction cost in steps
		steps = ins->steps;
	}//else{//error: wrong ins->opid}
	*psteps	= steps;
	return(size_done);
}


void mel_wpg_printf_metrix_FXCPU(void)
{
	int opid;
	int prev_folder = -1;

	printf("static WPGInstructionRecord INSTRUCTIONS_METRIX_FXCPU[MEL_WPG_OpID_TOTALLY] = \n{\n");

	for (opid = MEL_WPG_OpID_LD; opid < MEL_WPG_OpID_TOTALLY; opid++)
	{
		//size_t name_strlen;
		char lasting_text[256] = {0};

		printf("\t");
		
		if (prev_folder != INSTRUCTIONS_METRIX_FXCPU[opid].folder)
		{
			printf("// \"%s\"\n",WPG_OPERATIONS_FOLDERS[INSTRUCTIONS_METRIX_FXCPU[opid].folder]);
			printf("\t");
		}
		
		//printf("{0x%08X, ",INSTRUCTIONS_METRIX_FXCPU[opid].opcode);
		//printf("\"%s\",",INSTRUCTIONS_METRIX_FXCPU[opid].name,"");
		/*
		printf("{\"%s\",",INSTRUCTIONS_METRIX_FXCPU[opid].name);
		name_strlen = strlen(INSTRUCTIONS_METRIX_FXCPU[opid].name);
		{
			int diff = 10-name_strlen;

			while (diff-- > 0)
			{
				printf(" ");
			}
		}
		printf("%d",name_strlen);
		printf(",\t");
		*/
		//printf("{//\"%s\"\n\t\t",INSTRUCTIONS_METRIX_FXCPU[opid].name);
		printf("{//\"%s\"\n\t\t",WPG_OPNAMES[opid].name);

		if (INSTRUCTIONS_METRIX_FXCPU[opid].folder)
		{
#if PRINTF_FOLDER_AS_DEFS
			char* folder_name;
			char bitdepth_name[256]={0};

			switch(INSTRUCTIONS_METRIX_FXCPU[opid].folder)
			{
				case MEL_OPER_CONTACTS:		folder_name = "CONTACTS"; break;
				case MEL_OPER_CONNECTS:		folder_name = "CONNECTS"; break;
				case MEL_OPER_OUTS:			folder_name = "OUTS"; break;
				case MEL_OPER_MASTERCNT:	folder_name = "MASTERCNT"; break;
				case MEL_OPER_OTHERBASICS:	folder_name = "OTHERBASICS"; break;
				case MEL_OPER_ENDS:			folder_name = "ENDS"; break;
				case MEL_OPER_PRGFLOW:		folder_name = "PRGFLOW"; break;
				case MEL_OPER_MOVECMP:		folder_name = "MOVECMP"; break;
				case MEL_OPER_ARITHMLOGIC:	folder_name = "ARITHMLOGIC"; break;
				case MEL_OPER_ROTSHIFT:		folder_name = "ROTSHIFT"; break;
				case MEL_OPER_DATAOPS:		folder_name = "DATAOPS"; break;	
				case MEL_OPER_HANDY:		folder_name = "HANDY"; break;
				case MEL_OPER_EXTFXIO:		folder_name = "EXTFXIO"; break;
				case MEL_OPER_EXTFXDEVS:	folder_name = "EXTFXDEVS"; break;
				case MEL_OPER_DATATRANS2:	folder_name = "DATATRANS2"; break;
				case MEL_OPER_FLOATS:		folder_name = "FLOATS"; break;
				case MEL_OPER_DATAOPS2:		folder_name = "DATAOPS2"; break;
				case MEL_OPER_POSCONTROL:	folder_name = "POSCONTROL"; break;
				case MEL_OPER_REALTIME:		folder_name = "REALTIME"; break;
				case MEL_OPER_EXTDEVS:		folder_name = "EXTDEVS"; break;
				case MEL_OPER_INTRODUCTION: folder_name = "INTRODUCTION"; break;
				case MEL_OPER_OTHERS:		folder_name = "OTHERS"; break;
				case MEL_OPER_BLOCKOPS:		folder_name = "BLOCKOPS"; break;
				case MEL_OPER_CHARSTR:		folder_name = "CHARSTR"; break;
				case MEL_OPER_DATAOPS3:		folder_name = "DATAOPS3"; break;
				case MEL_OPER_DATACMP:		folder_name = "DATACMP"; break;
				case MEL_OPER_DATATABLE:	folder_name = "DATATABLE"; break;
				case MEL_OPER_EXTDEVSCOMM_INVERT:	folder_name = "EXTDEVSCOMM_INVERT"; break;
				case MEL_OPER_DATATRANS3:			folder_name = "DATATRANS3"; break;
				case MEL_OPER_HIGHSPEEDPROC:		folder_name = "HIGHSPEEDPROC"; break;
				case MEL_OPER_EXTENSFILEREGS:		folder_name = "EXTENSFILEREGS"; break;
				case MEL_OPER_FX3UCFADP:			folder_name = "FX3UCFADP"; break;
				default:							folder_name = "FOLDER UNKNOWN"; break;
			}

			printf("MEL_OPER_%-18s",folder_name);
			printf(",\t");
/*
			name_strlen = strlen(folder_name);
			printf("%2d",name_strlen);
			printf(",\t");
*/
#else
			printf("%d",INSTRUCTIONS_METRIX_FXCPU[opid].folder);
			printf(",\t");
#endif			
			
			printf("%-3d",INSTRUCTIONS_METRIX_FXCPU[opid].fnc);
			printf(",\t");

#if PRINTF_BITDEPTH_AS_DEFS
			if (INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth == MEL_BITDEPTH_INDEP)
			{
				sprintf(bitdepth_name,"%s","MEL_BITDEPTH_INDEP");
			}
			else
			{
				if (INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth & MEL_BITDEPTH_1BIT)
				{
					sprintf(bitdepth_name,"%s","MEL_BITDEPTH_1BIT");
				}

				if (INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth & MEL_BITDEPTH_16BIT)
				{
					if (bitdepth_name[0]==0)
					{
						sprintf(bitdepth_name,"%s","MEL_BITDEPTH_16BIT");
					}
					else
						strcat(bitdepth_name,"\n\t\t\t\t\t\t\t\t\t|MEL_BITDEPTH_16BIT");
				}

				if (INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth & MEL_BITDEPTH_32BIT)
				{
					if (bitdepth_name[0]==0)
					{
						sprintf(bitdepth_name,"%s","MEL_BITDEPTH_32BIT");
					}
					else
						strcat(bitdepth_name,"\n\t\t\t\t\t\t\t\t\t|MEL_BITDEPTH_32BIT");
				}
			}
			printf("%s",bitdepth_name);
#else
			printf("0x%08X",INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth);
#endif			
			printf(",\t");

			printf("%2d",INSTRUCTIONS_METRIX_FXCPU[opid].steps);
			printf(",\t");

#if PRINTF_EXECTYPE_AS_DEFS
			if (INSTRUCTIONS_METRIX_FXCPU[opid].exectype & MEL_EXEC_CONT)
			{
				printf("MEL_EXEC_CONT");
			}
			if (INSTRUCTIONS_METRIX_FXCPU[opid].exectype & MEL_EXEC_PULS)
			{
				printf("MEL_EXEC_PULS");
			}
#else
			printf("0x%08X",INSTRUCTIONS_METRIX_FXCPU[opid].exectype);
#endif
			printf(",\t");
			printf("%2d",INSTRUCTIONS_METRIX_FXCPU[opid].n_operands);
		
			printf(",\t");
			
			if (INSTRUCTIONS_METRIX_FXCPU[opid].n_operands > 0)
			{
				int i;
				
				printf("\n\t\t{");
				
				for (i = 0; i<INSTRUCTIONS_METRIX_FXCPU[opid].n_operands; i++)
				{
					if(i) printf("\n\t\t");
					
					/*
					if (0==strcmp(INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_usage,"SDBP"))
					{
						INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_usage = "SDP";
					}
					*/
					printf("\"%s\",{",INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_usage);

					{
						int cpu;
						
						for(cpu = MEL_PLC_FXCPU_TYPE_FX3U_C; cpu < 2/*MEL_PLC_FXCPU_TYPE_LAST*/; cpu++)
						{
							printf("\n\t\t\t\t/*%s devs&indx*/\t",mel_wpg_fxcpu_get_plctypename(cpu));
							printf("\"");
							printf("%s",INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu]);
							/*
							{
								char* text = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu];
								while (*text && *text!= ' ')
								{
									printf("%c",*text++);
								}
							}
							*/
							printf("\"");
							if (cpu!=2/*MEL_PLC_FXCPU_TYPE_LAST*/-1)
							{
								printf(",");
							}							
						}
						printf("},\t");
					}

					if(INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].explanation)
						printf("\"%s\",",INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].explanation);
					else
						printf("\"\",");

					//unsigned int dusage:
					{
						char dusage[1024] = {0};
						char tabs[1024] = {0};
						char tabsonly[1024] = {0};

						unsigned int bits = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dusage;

#define PRINTF_DUSAGE_TABS "\t\t\t"
						strcat(tabs,"\n");
						strcat(tabs,PRINTF_DUSAGE_TABS);
						strcpy(tabsonly,tabs);
						strcat(tabs,"|");

						strcat(dusage,tabsonly);

						if (bits != 0)
						{
#if PRINTF_DUSAGE_AS_DEFS
							if (bits & MEL_DUSAGE_SRC)
							{								
								strcat(dusage,"MEL_DUSAGE_SRC");
							}
							if (bits & MEL_DUSAGE_DST)
							{
								if(bits&(MEL_DUSAGE_DST-1)) strcat(dusage,tabs);
								strcat(dusage,"MEL_DUSAGE_DST");
							}
							if (bits & MEL_DUSAGE_PRM)
							{
								if(bits&(MEL_DUSAGE_PRM-1)) strcat(dusage,tabs);
								strcat(dusage,"MEL_DUSAGE_PRM");
							}							
#else							
							{
								char num[256] = {0};
								sprintf(num,"0x%08X",INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dusage);
								strcat(dusage,num);
							}
#endif						
							printf("%s",dusage);
						}
						else
						{
							printf("0");
						}
						printf(",\t");
					}
					
					//unsigned int	otypes_applicable	[MEL_PLC_FXCPU_TYPE_LAST]:					
					{
						int cpu;						
						printf("{");
						for(cpu = MEL_PLC_FXCPU_TYPE_FX3U_C; cpu < 2/*MEL_PLC_FXCPU_TYPE_LAST*/; cpu++)
						{
							char otypes_applicable[1024] = {0};
							char dindexing[1024] = {0};

#define PRINTF_DAPPLIC_TABS "\t\t\t\t\t\t\t"

							char tabs[1024] = {0};
							char tabsonly[1024] = {0};

							unsigned int bits = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].applicable_otypes;
							int isnotfirst = 0;

							strcat(tabs,"\n");
							strcat(tabs,PRINTF_DAPPLIC_TABS);
							strcpy(tabsonly,tabs);							
							strcat(tabs," |");

							strcat(otypes_applicable,tabsonly);
							strcat(otypes_applicable,"/*");
							strcat(otypes_applicable,mel_wpg_fxcpu_get_plctypename(cpu));
							strcat(otypes_applicable," devs*/ {");

							if (bits != 0)
							{
#if PRINTF_BITS_AS_DEFS			
								if (bits & MEL_DEV_TYPE_M)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}
									strcat(otypes_applicable,"MEL_DEV_TYPE_M");
								}
								if (bits & MEL_DEV_TYPE_S)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}
									strcat(otypes_applicable,"MEL_DEV_TYPE_S");
								}
								if (bits & MEL_DEV_TYPE_X)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}
									strcat(otypes_applicable,"MEL_DEV_TYPE_X");
								}
								if (bits & MEL_DEV_TYPE_Y)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_Y");
								}
								if (bits & MEL_DEV_TYPE_T)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_T");
								}
								if (bits & MEL_DEV_TYPE_C)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_C");
								}
								if (bits & MEL_DEV_TYPE_D)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_D");
								}
								if (bits & MEL_DEV_TYPE_R)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_R");
								}
								if (bits & MEL_DEV_TYPE_bI)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_bI");
								}
								if (bits & MEL_DEV_TYPE_Z)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_Z");
								}
								if (bits & MEL_DEV_TYPE_V)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_V");
								}
								if (bits & MEL_DEV_TYPE_K)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_K");
								}
								if (bits & MEL_DEV_TYPE_H)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_H");
								}
								if (bits & MEL_DEV_TYPE_E)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_E");
								}
								if (bits & MEL_DEV_TYPE_P)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_P");
								}
								if (bits & MEL_DEV_TYPE_KnX)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_KnX");
								}
								if (bits & MEL_DEV_TYPE_KnY)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_KnY");
								}
								if (bits & MEL_DEV_TYPE_KnM)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_KnM");
								}
								if (bits & MEL_DEV_TYPE_KnS)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_KnS");
								}
								if (bits & MEL_DEV_TYPE_Un)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_Un");
								}
								if (bits & MEL_DEV_TYPE_Gn)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_Gn");
								}
								if (bits & MEL_DEV_TYPE_STR)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_STR");
								}
								if (bits & MEL_OTYPE_Msys)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_OTYPE_Msys");
								}								
								if (bits & MEL_DEV_TYPE_N)
								{
									if(isnotfirst) strcat(otypes_applicable,tabs);
									else			{strcat(otypes_applicable," ");isnotfirst = 1;}

									strcat(otypes_applicable,"MEL_DEV_TYPE_N");
								}								
								
#else
								{
									char num[256] = {0};

									sprintf(num,"0x%08X",INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].applicable_otypes);
									strcat(otypes_applicable,num);
								}
#endif
							}
							else
							{								
								strcat(otypes_applicable,"0");
							}
							printf("%s",otypes_applicable);
							printf(",");
							
							strcpy(tabs,"\n");
							strcat(tabs,PRINTF_DAPPLIC_TABS);
							strcat(tabs,"\t");
							strcpy(tabsonly,tabs);
							strcat(tabs,"|");

							strcat(dindexing,tabsonly);

							strcat(dindexing,"/*");
							strcat(dindexing,mel_wpg_fxcpu_get_plctypename(cpu));
							strcat(dindexing," indx*/");

							isnotfirst = 0;
							//bits = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].dindexing;
							bits = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].allowed_attribs;
							if (bits != 0)
							{
								char num[256] = {0};

								//sprintf(num,"0x%08X",INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].dindexing);
								sprintf(num,"0x%08X",bits);
								strcat(dindexing,num);
							}
							else
							{
								strcat(dindexing," ");
								strcat(dindexing," 0");
							}
							printf("%s",dindexing);
							printf("}");

							if (cpu!=2/*MEL_PLC_FXCPU_TYPE_LAST*/-1)
							{
								printf(",");
							}
						}
						printf("},\t");
					}
				}				
				printf("0}");

			}else
			{
				printf("{{0}}");
			}

		}
		else
		{
			printf("0");
		}
		
		//sprintf(lasting_text,"//MEL_WPG_OpID_%s",INSTRUCTIONS_METRIX_FXCPU[opid].name);
		sprintf(lasting_text,"//MEL_WPG_OpID_%s",WPG_OPNAMES[opid].name);
		if(INSTRUCTIONS_METRIX_FXCPU[opid].comment)
		{
			strcat(lasting_text,"_");
			strcat(lasting_text,INSTRUCTIONS_METRIX_FXCPU[opid].comment);

			printf(",\t\"%s\"",INSTRUCTIONS_METRIX_FXCPU[opid].comment);
		}
		else
		{
			printf(",\t0",INSTRUCTIONS_METRIX_FXCPU[opid].comment);
		}
		
		if(opid!=MEL_WPG_OpID_TOTALLY-1) printf("},\t%s",lasting_text);
		else
			printf("}\t%s",lasting_text);
		
		printf("\n\n");

		prev_folder = INSTRUCTIONS_METRIX_FXCPU[opid].folder;
	}

	printf("};\n");

	return;

}

void mel_wpg_generate_metrix_FXCPU(void)
{
	int opid;
	
	for (opid = MEL_WPG_OpID_LD; opid < MEL_WPG_OpID_TOTALLY; opid++)
	{
		if (INSTRUCTIONS_METRIX_FXCPU[opid].folder)
		{
			if (INSTRUCTIONS_METRIX_FXCPU[opid].n_operands > 0)
			{
				int i;
												
				for (i = 0; i<INSTRUCTIONS_METRIX_FXCPU[opid].n_operands; i++)
				{
					//unsigned int dusage:
					{
						int j,length;
						unsigned int dusage = 0;

#if 0					
						INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_usage = "SDP";
#endif				
						length = strlen(INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_usage);
						
						for (j = 0; j < length; j++)
						{						

							switch(INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_usage[j])
							{//SDBP
							case 'S':dusage |= MEL_DUSAGE_SRC; break;
							case 'D':dusage |= MEL_DUSAGE_DST; break;								
							case 'B':dusage |= MEL_DUSAGE_BTH; break;								
							case 'P':dusage |= MEL_DUSAGE_PRM; break;								
							}						
						}

						INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dusage = dusage;
					}					
					//unsigned int otypes_applicable[]:
					//unsigned int dindexing[]:
					{
						int cpu;

						for(cpu = MEL_PLC_FXCPU_TYPE_FX3U_C; cpu < 2/*MEL_PLC_FXCPU_TYPE_LAST*/; cpu++)
						{
							int j,length;
							char* text;							
							int k = 0;

							unsigned int data = 0;
#if 0
							INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu] = "XYMrTCSbxymstcDRUVZKHEAP";
#endif
	
#if 0
							if (strcmp(INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu],"")==0)
							{
								INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu] = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[0];
							}
#endif							
							text = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu];
							length = strlen(text);

							for (j = 0; j < length; j++)
							{																
								switch(text[j])
								{//XYMTCSbxymstcDRUVZKHEAP
								case 'X':data |= MEL_DEV_TYPE_X; break;
								case 'Y':data |= MEL_DEV_TYPE_Y; break;
								case 'M':data |= MEL_DEV_TYPE_M; break;
								//case 'r':data |= MEL_OTYPE_Msys; break;
								case 'T':data |= MEL_DEV_TYPE_T; break;
								case 'C':data |= MEL_DEV_TYPE_C; break;
								case 'S':data |= MEL_DEV_TYPE_S; break;
								case 'b':data |= MEL_DEV_TYPE_bI; break;
								//case 'x':data |= MEL_DEV_TYPE_KnX; break;
								//case 'y':data |= MEL_DEV_TYPE_KnY; break;
								//case 'm':data |= MEL_DEV_TYPE_KnM; break;
								//case 's':data |= MEL_DEV_TYPE_KnS; break;
								//case 't':data |= MEL_DEV_TYPE_Tw; break;
								//case 'c':data |= MEL_DEV_TYPE_Cw; break;
								case 'D':data |= MEL_DEV_TYPE_D; break;
								case 'R':data |= MEL_DEV_TYPE_R|MEL_DEV_TYPE_ER; break;
								case 'U':data |= MEL_DEV_TYPE_Un|MEL_DEV_TYPE_Gn; break;
								case 'V':data |= MEL_DEV_TYPE_V; break;
								case 'Z':data |= MEL_DEV_TYPE_Z; break;

								case 'K':data |= MEL_DEV_TYPE_K; 
									if(INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth & MEL_BITDEPTH_32BIT) 
										data |= MEL_DEV_TYPE_K2; 
									break;

								case 'H':data |= MEL_DEV_TYPE_H;
									if(INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth & MEL_BITDEPTH_32BIT) 
										data |= MEL_DEV_TYPE_H2;
									break;

								case 'E':data |= MEL_DEV_TYPE_E; break;
								case 'A':data |= MEL_DEV_TYPE_STR; break;
								case 'P':data |= MEL_DEV_TYPE_P; break;
								case 'N':data |= MEL_DEV_TYPE_N; break;
								/*
								case ' '://switch from otypes_applicable to dindexing
									
									INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].otypes_applicable = data;
									
									data = 0;									
									text = INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].txt_operands[cpu]+j+1;
									length = strlen(text);
									if (length>0)
									{
										j = -1;
									}									
								*/
								}
							}

							INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].applicable_otypes = data;

							//INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].dindexing = data;
							//INSTRUCTIONS_METRIX_FXCPU[opid].operands[i].dbits[cpu].attribs_allowed = 
							//	(data)? MEL_WPG_OPERAND_ATTRIB_INDEXED: 0;							
						}
					}
				}
			}else
			{
				memset(&INSTRUCTIONS_METRIX_FXCPU[opid].operands,0x00,sizeof(INSTRUCTIONS_METRIX_FXCPU[opid].operands));
			}
		}
	}

	return;
}

char* mel_wpg_instr2text(int cpuSeries,int cpuType,WPG_Instruction* instr,int* ptextsize)
{
	char* retptr = 0;
	static char text[MEL_WPG_MAX_INSTR_TEXT_SIZE] = {0};
	int textsize = 0;

	text[0] = 0;

	if (instr->opid!=MEL_WPG_OpID_NOP)
	{
		int i;
		int donelen;

		//donelen = sprintf(text,"%s",INSTRUCTIONS_METRIX_FXCPU[instr->opid].name);
		donelen = sprintf(text,"%s",WPG_OPNAMES[instr->opid].name);
		if (donelen > 0)
		{
			textsize += donelen;
			//textsize += INSTRUCTIONS_METRIX_FXCPU[instr->opid].namelen;
		}

		if (instr->n_params > 0)
		{
			for (i = 0; i < instr->n_params; i++)
			{
				char *operand;
				int operandlen;

				strcat(text," ");
				textsize++;

				switch(instr->params[i].devs_count)
				{
				case 1:

					operand = mel_device2text(&instr->params[i].devs[0],&operandlen);
					strcat(text,operand);
					textsize += operandlen;
					break;

				case 2:
				case 3:
				default:
					{
						int i_dev;
						for (i_dev = 0; i_dev < instr->params[i].devs_count; i_dev++)
						{

							if (i_dev + 1 < instr->params[i].devs_count
								&&
								(instr->params[i].devs[i_dev].dtype &(	MEL_DEV_TYPE_VI
																		|MEL_DEV_TYPE_ZI
																		|MEL_DEV_TYPE_bI)
								)
								)
							{	//swap [curr,next] -> [next,curr]
								operand = mel_device2text(&instr->params[i].devs[i_dev + 1],&operandlen);
								strcat(text,operand);
								textsize += operandlen;
								operand = mel_device2text(&instr->params[i].devs[i_dev + 0],&operandlen);
								strcat(text,operand);
								textsize += operandlen;
								//step over next one (since it is already done)
								i_dev++;
							}
							else
							{	//keep it as it is: [curr] -> [curr]
								operand = mel_device2text(&instr->params[i].devs[i_dev + 0],&operandlen);
								strcat(text,operand);
								textsize += operandlen;
							}
						}
					}
					break;
				}
			}
		}
		retptr = text;
	}
	else
	{
		//strcat(text,INSTRUCTIONS_METRIX_FXCPU[instr->opid].name);
		//textsize += INSTRUCTIONS_METRIX_FXCPU[instr->opid].namelen;
		strcat(text,WPG_OPNAMES[instr->opid].name);
		textsize += WPG_OPNAMES[instr->opid].namelen;
		retptr = text;
	}
	
	*ptextsize = textsize;
	return(retptr);
}

static unsigned int mel_wpg_txt2ASCII		(int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_OK;
	int firstbad_device = 0;

	if (txtlen > 1 && txt[0]==txt[txtlen-1])
	{
		dst->devs_count = 1;
		
		if (txtlen > 2)
		{
			memcpy(dst->devs[0].const_str,txt+1,txtlen-2);
		}
		//add ending null
		dst->devs[0].const_str[txtlen-2] = 0;
		dst->devs[0].const_str_length = txtlen-2;
		dst->devs[0].dtype = MEL_DEV_TYPE_STR;
	}
	else
	{
		firstbad_device = txtlen-1;
		stcode = INSTR_STATE_BAD_NO_STRCLOSING;
	}

	//printf("[%s]\n",txt);
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

static unsigned int mel_wpg_txt2UG		(int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_BAD_NO_SLASH;
	int firstbad_device = 0;
	int is_slash = 0;
	int i;

	//U+m1\G+m2[Vi,Zi]
	for(i = 1; i < txtlen; i++)
	{
		if (txt[i]=='\\')
		{
			is_slash = 1;
			break;
		}
	}

	if (is_slash)
	{
		if (txtlen > i + 2)
		{
			if (txt[i+1] == 'G' || txt[i+1] == 'g')
			{	//check m1,m2 indexes U[m1],G[m2]
				/////////////////////////////////FIRST DEVICE/////////////////////////////////////////
				unsigned long val;
				char* endptr = 0;
				
				stcode = INSTR_STATE_BAD_DEVICE_INDEX;

				//must be DECIMAL number -> U[n]
				errno = 0;
				val = strtoul(txt+1,&endptr,10);
				if (errno == 0)
				{
					dst->devs[0].dtype	= MEL_DEV_TYPE_Un;
					dst->devs[0].index	= val;

					if (*endptr == txt[i])//*endptr == '\\'
					{
						/////////////////////////////////SECOND DEVICE/////////////////////////////////////////
						unsigned long val2;
						char* endptr2 = 0;

						firstbad_device++;
						stcode = INSTR_STATE_BAD_DEVICE_INDEX;

						//must be DECIMAL number - >G[n]
						errno = 0;
						val2 = strtoul(endptr+2,&endptr2,10);
						if (errno == 0)
						{	
							dst->devs[1].dtype	= MEL_DEV_TYPE_Gn;
							dst->devs[1].index	= val2;

							if (*endptr2 == '\0')
							{
								stcode = INSTR_STATE_OK;
								dst->devs_count = 2;
							}else
							{//probably "three parameters" argument
								/////////////////////////////////THIRD DEVICE/////////////////////////////////////////
								unsigned int dtype3;
								
								firstbad_device++;
								stcode = INSTR_STATE_BAD_DEVICE_TYPE;

								dtype3 = mel_approx_dtype_by_chars(*endptr2,0);
								if (is_it_VorZ(dtype3))//indexing of G-constants
								{
									long int val3;
									char* endptr3 = 0;
									char* num_ptr;

									num_ptr = endptr2 + 1;

									stcode = INSTR_STATE_BAD_DEVICE_INDEX;

									//must be DECIMAL number - >V[i] or Z[i]
									errno = 0;
									val3 = strtol(num_ptr,&endptr3,10);
									if (errno == 0)
									{
										if (endptr3 == num_ptr)
										{
											val3 = 0;
										}//V or Z without Number: it's normal, treat it as V0 or Z0.

										dst->devs[2].dtype	= (dtype3&MEL_DEV_TYPE_VI)? MEL_DEV_TYPE_VI:MEL_DEV_TYPE_ZI;//Vi or Zi
										dst->devs[2].index	= val3;

										if (*endptr3 == '\0')
										{
											stcode = INSTR_STATE_OK;
											dst->devs_count = 3;											
										}
										else{stcode = INSTR_STATE_BAD_TOOLONG;}

									}//else{firstbad_device = 2;}//stcode = INSTR_STATE_BAD_DEVICE_INDEX;
								}//else{firstbad_device = 2;}//stcode = INSTR_STATE_BAD_DEVICE_TYPE;
							}
						}//else{firstbad_device = 1;}//stcode = INSTR_STATE_BAD_DEVICE_INDEX;
					}//else{//firstbad_device = 0; stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow}
				}//else{//firstbad_device = 0; stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow}
			}
			else
			{
				firstbad_device = i+1;
				stcode = INSTR_STATE_BAD_NO_G;
			}
		}
		else
		{		
			//firstbad_device = 0;
			stcode = INSTR_STATE_BAD_TOOSHORT;
		}
	}//else{//firstbad_device = 0; stcode = INSTR_STATE_BAD_NO_SLASH;}
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

static unsigned int mel_wpg_txt2FLOAT		(int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument
	int firstbad_device = 0;
	double val;
	char* endptr = 0;
	/////////////////////////////////FIRST DEVICE/////////////////////////////////////////
	//must be only one FLOATing point number: E:FLOAT
	errno = 0;
	val = strtod(txt+1,&endptr);
	if (errno == 0)
	{
		if (endptr!=txt && *endptr == '\0')
		{
			dst->devs[0].const_flt = (float)val;
			dst->devs[0].dtype = MEL_DEV_TYPE_E;

			stcode = INSTR_STATE_OK;
			dst->devs_count = 1;
		}
		else{stcode = INSTR_STATE_BAD_TOOLONG;}//error: trash in the end
	}
	//else{stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument}
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

static unsigned int mel_wpg_txt2COMPOSED(unsigned int dtype,
										 int cpuSeries,int cpuType,char* txt,int txtlen,WPG_Operand* dst,int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument
	int firstbad_device = 0;
	//TYPE:NUMBER
	//TYPE:NUMBER+TYPE:NUMBER
	long int val;
	unsigned long uval;
	char* endptr = 0;
	char* num_ptr = 0;

	///////////////////////////////////FIRST DEVICE///////////////////////////////////////
	if (dtype & MEL_DEV_TYPE_ER)
		num_ptr = txt + 2;
	else
		num_ptr = txt + 1;
	//next must be DECIMAL, HEX or OCTAL (X,Y only) integer
	errno = 0;
	if(dtype &(MEL_DEV_TYPE_X|MEL_DEV_TYPE_Y))
	{//OCTAL		
		val = strtol(num_ptr,&endptr,8);		
	}
	else
	{//DECIMAL or HEX
		if (dtype & MEL_DEV_TYPE_H)
		{//HEX
			uval = strtoul(num_ptr,&endptr,16);
		}
		else
		{//DECIMAL
			val = strtol(num_ptr,&endptr,10);
		}
	}
	if (errno == 0)
	{
		if (endptr == num_ptr)
		{
			if (is_it_VorZ(dtype)){val	= 0;}
			else{goto __exit_point;}//stcode = INSTR_STATE_BAD_DEVICE_INDEX;
		}//V or Z without Number: it's normal, treat it as V0 or Z0.
		
		if (  dtype & MEL_DEV_TYPE_K)
		{
			dst->devs[0].const_int  = val;
			dst->devs[0].index		= val;
		}
		else
		{
			if (dtype & MEL_DEV_TYPE_H)
			{
				dst->devs[0].const_uint = uval;
			}
			else
			{
				dst->devs[0].index  = val;
			}				
		}

		if (*endptr == '\0')//"one parameter" argument
		{
			//clear off complex types
			dtype &= ~(	 MEL_DEV_TYPE_Kn
						|MEL_DEV_TYPE_VI
						|MEL_DEV_TYPE_ZI
						);

			dst->devs[0].dtype	= dtype;

			stcode = INSTR_STATE_OK;
			dst->devs_count = 1;
		}
		else
		{//probably "two parameters" argument
			///////////////////////////////////SECOND DEVICE///////////////////////////////////////
			unsigned int dtype2;

			//save first device type
			dst->devs[0].dtype	= dtype;

			firstbad_device++;
			stcode = INSTR_STATE_BAD_DEVICE_TYPE;

			//there are more chars: take it as SECOND parameter
			dtype2 = mel_approx_dtype_by_chars(*endptr,0);
			if (dtype2 != MEL_DEV_TYPE_UNKNOWN)
			{
				long int val2;
				char* endptr2 = 0;

				stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument
								
				if (dtype2 & MEL_DEV_TYPE_ER)
					num_ptr = endptr + 2;
				else
					num_ptr = endptr + 1;

				//next may be DECIMAL, OCTAL or HEX integer
				errno = 0;
				if(dtype2 & (MEL_DEV_TYPE_X|MEL_DEV_TYPE_Y))
				{//OCTAL
					val2 = strtol(num_ptr,&endptr2,8);
				}
				else
				{//DECIMAL or HEX
					if (dtype2 & MEL_DEV_TYPE_bI)
					{
						val2 = strtol(num_ptr,&endptr2,16);
					}
					else
					{
						val2 = strtol(num_ptr,&endptr2,10);
					}
				}

				if (errno == 0)
				{
					if (endptr2 == num_ptr)
					{
						if (is_it_VorZ(dtype2)){val2	= 0;}
						else{goto __exit_point;}//stcode = INSTR_STATE_BAD_DEVICE_INDEX;
					}//V or Z without Number: it's normal, treat it as V0 or Z0.

					dst->devs[1].dtype		= dtype2;
					dst->devs[1].index		= val2;

					if (*endptr2 == '\0')//ending argument
					{
						stcode = INSTR_STATE_OK;
						dst->devs_count = 2;
					}
					else 
					{//probably "three parameters" argument
						///////////////////////////////////THIRD DEVICE///////////////////////////////////////
						unsigned int dtype3;

						firstbad_device++;
						stcode = INSTR_STATE_BAD_DEVICE_TYPE;

						//there are more chars: take it as THIRD parameter
						dtype3 = mel_approx_dtype_by_chars(*endptr2,0);
						if (dtype3 != MEL_DEV_TYPE_UNKNOWN)
						{
							long int val3;
							char* endptr3 = 0;

							stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument

							if (dtype3 & MEL_DEV_TYPE_ER)
								num_ptr = endptr2 + 2;
							else
								num_ptr = endptr2 + 1;
							
							//next: only DECIMAL number (Vi or Zi)
							errno = 0;
							val3 = strtol(num_ptr,&endptr3,10);
							if (errno == 0)
							{
								if (endptr3 == num_ptr)
								{
									if (is_it_VorZ(dtype3)){val3	= 0;}
									else{goto __exit_point;}//stcode = INSTR_STATE_BAD_DEVICE_INDEX;
								}//V or Z without Number: it's normal, treat it as V0 or Z0.

								dst->devs[2].dtype		= dtype3;
								dst->devs[2].index		= val3;

								if (*endptr3 == '\0')//ending argument
								{
									stcode = INSTR_STATE_OK;
									dst->devs_count = 3;
								}
								else{stcode = INSTR_STATE_BAD_TOOLONG;}//error: trash in the end

							}//else{stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument}
						}//else{stcode = INSTR_STATE_BAD_DEVICE_TYPE;}
					}//else{stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument}							
				}//else{stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument}
			}//else{stcode = INSTR_STATE_BAD_DEVICE_TYPE;}
		}//else{stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument}
	}//else{stcode = INSTR_STATE_BAD_DEVICE_INDEX;//overflow or bad argument}
__exit_point:
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

static unsigned int mel_wpg_check_operand_syntax(int cpuSeries,int cpuType,WPG_Operand* operand,unsigned int bitdepth,int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_OK;
	int firstbad_device = 0;

	if (operand->devs_count > 0)
	{		
		int n_devices = operand->devs_count;
		int i_dev;

		for (i_dev = 0 ; i_dev < n_devices; i_dev++)
		{				
			unsigned int dtype = operand->devs[i_dev].dtype;
			//unsigned int dtype_ext = 0;

			firstbad_device = i_dev;			

			/*
			printf("syntax check: %d [0x%08X](%s) [%d]\n",
					i_dev,operand->devs[i_dev].dtype,
					mel_get_device_name(operand->devs[i_dev].dtype),
					operand->devs[i_dev].index);
			*/

			/////////////////////////////////SET DEVICE'S EXT TYPE///////////////////////////////////////
			//try to define extended type (if any).
			//Do it before checking INDEXING: it is not allowed for system M (which is one of extended types).
			if (dtype & (	 MEL_DEV_TYPE_M
							|MEL_DEV_TYPE_D
							|MEL_DEV_TYPE_C
							)
				)
			{
				if(set_dtype_ext_fxcpu(&operand->devs[i_dev],cpuType))
				{	//update local values
					dtype		= operand->devs[i_dev].dtype;
					//dtype_ext	= operand->devs[i_dev].dtype_ext;
				}
			}

			/////////////////////////////////CHECK GRAMMAR RULES/////////////////////////////////////////
			//LINKING
			if (dtype & MEL_DEV_TYPE_Kn)
			{
				if (i_dev)
				{
					dtype &= ~MEL_DEV_TYPE_Kn;//not first -> not KnX,Y,M,S
				}
				else
				{//i_dev == 0
					if (n_devices > 1)
					{
						if (!is_linking_allowed(&operand->devs[i_dev+1],cpuSeries,cpuType))
						{
							dtype &= ~MEL_DEV_TYPE_Kn;//linking is not allowed -> not KnX,Y,M,S
							//is it indexed constant K?
							if (0 == (operand->devs[i_dev+1].dtype & (MEL_DEV_TYPE_VI|MEL_DEV_TYPE_ZI)))
							{
								//error: wrong combination {K + something} (also, it is not linking or indexing).
								stcode = INSTR_STATE_BAD_DEVICE_LINKING;
								break;
							}
							//else{//it is indexed constant K}
						}
						else
						{
							dtype &= ~(MEL_DEV_TYPE_K|MEL_DEV_TYPE_K2);//-> linking, not single constant K
						}
					}
					else
					{
						dtype &= ~MEL_DEV_TYPE_Kn;//just single constant K
					}
				}
			}
			//update [probably] corrected device dtype
			if(operand->devs[i_dev].dtype != dtype) operand->devs[i_dev].dtype = dtype;

			//INDEXING
			if (dtype & (MEL_DEV_TYPE_VI|MEL_DEV_TYPE_ZI))
			{
				if (i_dev + 1 == operand->devs_count)
				{//at last place: seems right place
					if (n_devices > 1)
					{
						dtype &= ~(MEL_DEV_TYPE_V|MEL_DEV_TYPE_Z);//indexing,so it is not single V or Z

						if (!is_indexing_allowed(&operand->devs[i_dev-1],cpuSeries,cpuType))
						{
							//error: indexing is not allowed!
							stcode = INSTR_STATE_BAD_DEVICE_INDEXING;
							break;
						}
						//else{//correct indexing}
					}
					else
					{
						dtype &= ~(MEL_DEV_TYPE_VI|MEL_DEV_TYPE_ZI);//just single V or Z -> not indexing						
					}
					
					//check special error case: (prohibited) usage of V in 32-bit operations
					if (bitdepth & MEL_BITDEPTH_32BIT)
					{
						if (dtype & (MEL_DEV_TYPE_V|MEL_DEV_TYPE_VI))
						{	//error: 32-bit operands: Zn only (Vn is not allowed);
							//	Indexes V,Z: may be used as 32-bit values: {Vi:Zi}, Zi - low 16 bits,
							//	V - high 16 bits of 32-bit index value;
							stcode = INSTR_STATE_BAD_DEVICE_TYPE;
							break;
						}
					}
				}
				else
				{
					//error: must be at last place anyway! (in case of single V or Z it is "last one" too)
					stcode = INSTR_STATE_BAD_DEVICE_INDEXING;
					break;
				}
			}
			//update [probably] corrected device dtype
			if(operand->devs[i_dev].dtype != dtype) operand->devs[i_dev].dtype = dtype;

			//BIT-ACCESSING
			if (dtype & MEL_DEV_TYPE_bI)
			{
				if (i_dev==1)
				{
					if (!is_bitaccess_allowed(&operand->devs[i_dev-1],cpuSeries,cpuType))
					{
						//error: bit-accessing is not allowed!
						stcode = INSTR_STATE_BAD_DEVICE_BITACCESSING;
						break;
					}//else{//correct bit-accessing}
				}
				else
				{
					//error: must be at second place right after Dn!
					stcode = INSTR_STATE_BAD_DEVICE_BITACCESSING;
					break;
				}
			}
			//UNIT-ACCESSING
			if (dtype & MEL_DEV_TYPE_Un)
			{
				if (i_dev)
				{
					//error: must be placed at the beginning!
					stcode = INSTR_STATE_BAD_DEVICE_UNITACCESSING;
					break;
				}//else{//correct unit-accessing}
			}
			if (dtype & MEL_DEV_TYPE_Gn)
			{
				if (i_dev!=1)
				{
					//error: must be second!
					stcode = INSTR_STATE_BAD_DEVICE_UNITACCESSING;
					break;
				}//else{//correct unit-accessing}
			}			
			/////////////////////////////////CHECK DEVICE'S INDEX/NUMBER///////////////////////////////////////
			//check range of device's index and clarify device type (if possible)
			{
				int is_in_range = is_device_in_range_fxcpu(&operand->devs[i_dev],cpuType);				
				if (!is_in_range)
				{
					stcode = INSTR_STATE_BAD_DEVICE_INDEX;
					break;
				}
				else
				{
					//update [probably] corrected device type
					if(operand->devs[i_dev].dtype != dtype) operand->devs[i_dev].dtype = dtype;
				}
			}

			//process complex device types (homonyms, etc.)
			if ( (dtype & (dtype - 1))!=0 )//it is not power of 2 -> composed dtype (not simple one)
			{
				//V,VI, Z,ZI - 100% done before during INDEXING checking;
				//K,K2, H,H2 - partially done before during RANGE checking;
				if (	dtype & (MEL_DEV_TYPE_K
								|MEL_DEV_TYPE_K2
								|MEL_DEV_TYPE_H
								|MEL_DEV_TYPE_H2)
					)
				{
					if (bitdepth & MEL_BITDEPTH_32BIT)
					{
						//simplify composed dtype
						if(dtype & MEL_DEV_TYPE_K) dtype = MEL_DEV_TYPE_K2;
						if(dtype & MEL_DEV_TYPE_H) dtype = MEL_DEV_TYPE_H2;
					}
					else
					{
						//simplify composed dtype
						dtype &= ~(MEL_DEV_TYPE_K2|MEL_DEV_TYPE_H2);
					}
				}
				else
				{
					printf("unknown composed dtype (not simple one)!!! [0x%08X] : %s\n",dtype,mel_get_device_name(dtype));
				}
				
				//update [probably] corrected device type
				if(operand->devs[i_dev].dtype != dtype) operand->devs[i_dev].dtype = dtype;
			}
			/////////////////////////////////SET OPERAND'S ATTRIBUTES///////////////////////////////////////

			//update attributes
			if (	dtype & (MEL_DEV_TYPE_VI
							|MEL_DEV_TYPE_ZI
							|MEL_DEV_TYPE_Kn
							|MEL_DEV_TYPE_bI
							|MEL_DEV_TYPE_Un
							)
				)
			{
				mel_wpg_add_attributes(dtype,&operand->attribs);
			}
		}

		/////////////////////////////////CHECK COMPLEX OPERANDS///////////////////////////////////////
		if (stcode == INSTR_STATE_OK && n_devices > 1)
		{
			switch(n_devices)
			{
			case 2:
				//LINKING
				//INDEXING
				//BIT-ACCESS
				//UNIT-ACCESS
				if (0 == (operand->attribs & (	 MEL_WPG_OPERAND_ATTRIB_INDEXED
												|MEL_WPG_OPERAND_ATTRIB_LINKED
												|MEL_WPG_OPERAND_ATTRIB_BITACCESS
												|MEL_WPG_OPERAND_ATTRIB_UNITACCESS))
					)
				{
					//error: too many devices without any reason
					firstbad_device = 0;
					stcode = INSTR_STATE_BAD_DEVICE_COUNT;
				}
				break;

			case 3:
				//LINKING + INDEXING
				//UNIT-ACCESS + INDEXING
				if (!(	0 == (operand->attribs & MEL_WPG_OPERAND_ATTRIB_LINKED) 
						||
						0 == (operand->attribs & MEL_WPG_OPERAND_ATTRIB_INDEXED) 
						)
					&&
					!(	0 == (operand->attribs & MEL_WPG_OPERAND_ATTRIB_UNITACCESS) 
						||
						0 == (operand->attribs & MEL_WPG_OPERAND_ATTRIB_INDEXED) 
						)
					)
				{
					//error: too many devices without any allowed reason
					firstbad_device = 0;
					stcode = INSTR_STATE_BAD_DEVICE_COUNT;
				}
				break;

			default:
				//error: too many devices
				firstbad_device = 0;
				stcode = INSTR_STATE_BAD_DEVICE_COUNT;
			}
		}
	}
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

static unsigned int mel_wpg_get_operand_from_txt(int cpuSeries,int cpuType,char* txt,unsigned int bitdepth,WPG_Operand* dst,int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_BAD_TOOSHORT;//default
	int firstbad_device = 0;
	int txtlen = strlen(txt);

	//Parse operand's string and get corresponding device(s)
	if (txtlen >= 1)
	{
		unsigned int dtype;

		if (txtlen == 1)//V or Z only
		{
			//check first char: must be known dtype
			dtype = mel_approx_dtype_by_chars(txt[0],0);
			if (! (dtype &(MEL_DEV_TYPE_V|MEL_DEV_TYPE_Z)) )
			{
				goto __exit_point;//stcode = INSTR_STATE_BAD_TOOSHORT;
			}//else{it's OK for V,Z (only).}
		}
		else
		{
			//check first char: must be known dtype
			dtype = mel_approx_dtype_by_chars(txt[0],txt[1]);
		}

		if (dtype != MEL_DEV_TYPE_UNKNOWN)
		{
			if (!(dtype & MEL_DEV_TYPE_STR))
			{
				if (!(dtype & (MEL_DEV_TYPE_Un|MEL_DEV_TYPE_Gn)) )
				{
					if (!(dtype & MEL_DEV_TYPE_bI))
					{
						if (!(dtype & MEL_DEV_TYPE_E))
						{
							stcode = mel_wpg_txt2COMPOSED(dtype,cpuSeries,cpuType,txt,txtlen,dst,&firstbad_device);
						}
						else
						{
							stcode = mel_wpg_txt2FLOAT(cpuSeries,cpuType,txt,txtlen,dst,&firstbad_device);
						}
					}
					else
					{	//error: unexpected '.'
						//printf("error: unexpected [.]\n");
						stcode = INSTR_STATE_BAD_DEVICE_TYPE;
					}
				}
				else
				{
					stcode = mel_wpg_txt2UG(cpuSeries,cpuType,txt,txtlen,dst,&firstbad_device);
				}
			}
			else
			{
				stcode = mel_wpg_txt2ASCII(cpuSeries,cpuType,txt,txtlen,dst,&firstbad_device);
			}
		}
		else
		{
			stcode = INSTR_STATE_BAD_DEVICE_TYPE;
		}
	}
__exit_point:	
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

static void mel_wpg_add_attributes(unsigned int dev_type,unsigned int* pattribs)
{
	if (dev_type &(MEL_DEV_TYPE_VI|MEL_DEV_TYPE_ZI))
	{						
		*pattribs |= MEL_WPG_OPERAND_ATTRIB_INDEXED;
	}

	if (dev_type &MEL_DEV_TYPE_Kn)
	{
		*pattribs |= MEL_WPG_OPERAND_ATTRIB_LINKED;
	}

	if (dev_type &MEL_DEV_TYPE_bI)
	{
		*pattribs |= MEL_WPG_OPERAND_ATTRIB_BITACCESS;
	}

	if (dev_type &MEL_DEV_TYPE_Un)
	{
		*pattribs |= MEL_WPG_OPERAND_ATTRIB_UNITACCESS;
	}

	return;
}

static unsigned int mel_wpg_txt2args_slicer(char* text,int textsize,char* args,int* pfields)
{
	unsigned int stcode = INSTR_STATE_BAD_TOOSHORT;
	const int n = MEL_INSTRUCT_OPERANDS_MAX;
	const int m = MEL_STRING_MAX_LENGTH+2+1;
	
	int arg_sizes[MEL_INSTRUCT_OPERANDS_MAX];

	int fields = 0;

	int is_there_ASCII_start;
	char prev;
	int i,j;

/*
	for (i = 0; i < n; i++)
	{
		memset(args[i],0x00,m*sizeof(char));
	}
*/	
	is_there_ASCII_start = 0;
	arg_sizes[0] = 0; j = 0;
	prev = 0x00;
	for (i = 0; i < textsize + 1; i++)
	{
		char curr;
		int dontadd2arg = 0;
		int loopbreak = 0;

		curr = text[i];

		if (is_within_JIS8((unsigned char)curr))
		{
			switch(curr)
			{
			case '\0'://end of string
				
				if(arg_sizes[fields] > 0)
				{
					args[fields*m+j] = 0x00;					
					fields++;
				}
				if (fields > 0) stcode = INSTR_STATE_OK;
				loopbreak = 1;
				break;

			case '"'://ASCII
				is_there_ASCII_start ^= 1;
				break;

			default: //anything else
				if (!is_there_ASCII_start)
				{
					if (curr == ' ')//space
					{
						if (prev != curr)
						{
							//finish with current field
							args[fields*m + j] = 0x00;							
							//and switch to next one
							fields++; j = 0;
							arg_sizes[fields] = 0;
						}
						//skip spaces
						dontadd2arg = 1;
					}
					else
					{

					}
				}//else{//ASCII(JIS8) string: just add it to the argument...}
				break;
			}
			
			if (loopbreak) break;

			if (!dontadd2arg)
			{
				if (j + 1 < m)
				{
					//add character to current argument
					args[fields*m + j] = curr;
					arg_sizes[fields] = ++j;
				}
				else
				{					
					stcode = INSTR_STATE_BAD_TOOLONG;
					break;
				}
			}		

			prev = curr;
		}
		else
		{
			stcode = INSTR_STATE_BAD_CHAR;
			break;
		}
	}

#if 0
	{
		int i;

		for (i = 0; i < fields; i++)
		{
			printf("%d\t{%s}\n",i,args[i*m]);
		}
	}
#endif //0

	*pfields = fields;
	return(stcode);
}

static void			mel_wpg_arrange_devices	(WPG_Operand* operand)
{
	switch(operand->devs_count)
	{
	case 2:
		/*
			KnXm,...	wpg-encoding: Kn,Xm	-	LINKED
			XmVk,...	wpg-encoding: Vk,Xm	+	INDEXING
			Dn.b,...	wpg-encoding: .b,Dn	+	BITACCESS
			Un\Gm,...	wpg-encoding: Un,Gm	-	UNITACCESS
		*/
		if (!(	operand->attribs & ( MEL_WPG_OPERAND_ATTRIB_LINKED
									|MEL_WPG_OPERAND_ATTRIB_UNITACCESS)
				)
			)
		{//swap devices in complex operands (to keep them in same order as in wpg-files)
			PLCRegDevice tmp;												
			memcpy(&tmp,&operand->devs[0],sizeof(tmp));
			memcpy(&operand->devs[0],&operand->devs[1],sizeof(tmp));
			memcpy(&operand->devs[1],&tmp,sizeof(tmp));
		}
		break;

	case 3:
		/*
			KnXmVk,...	wpg-encoding: Kn,Vk,Xm	+ INDEXING | LINKED
			Un\GmVk,...	wpg-encoding: Un,Vk,Gm	+ INDEXING | UNITACCESS
		*/			
		{	//swap devices in complex operands (to keep them in same order as in wpg-files)
			PLCRegDevice tmp;												
			memcpy(&tmp,&operand->devs[1],sizeof(tmp));
			memcpy(&operand->devs[1],&operand->devs[2],sizeof(tmp));
			memcpy(&operand->devs[2],&tmp,sizeof(tmp));
		}
		break;
	}

	return;
}

static unsigned int mel_wpg_check_device_types(PLCRegDevice* device,struct operand_attr* rules)
{
	unsigned int stcode = INSTR_STATE_OK;

	////////////////////////// CHECK DTYPE////////////////////////////////////////////////
	if ( !(rules->applicable_dtypes	& device->dtype) )
	{
		//NG
		//Device Type is NOT SUPPORTED
		//printf("[%s]\n",text);
		//printf("applicable dtype: 0x%08X\t curr_type: 0x%08X\n",
		//		tbl_operand->dbits[cpuType].applicable_dtypes,
		//		operand->devs[0].dtype);
		//stcode = INSTR_STATE_BAD_DEVICE_TYPE;
	}
	else
		////////////////////////// CHECK DTYPE_EXT////////////////////////////////////////////////
		if ( !(rules->applicable_dtypes_ext	& device->dtype_ext) )
		{
			//NG
			//Device Type Ext is NOT SUPPORTED
			//printf("[%s]\n",text);
			//printf("applicable dtype_ext: 0x%08X\t curr_type_ext: 0x%08X\n",
			//		tbl_operand->dbits[cpuType].applicable_dtypes_ext,
			//		operand->devs[0].dtype_ext);
			//stcode = INSTR_STATE_BAD_DEVICE_TYPE_EXT;
		}
		//else{//OK}
	
	return(stcode);
}

static unsigned int mel_wpg_check_operand_usability(int cpuSeries,int cpuType,
													WPG_Operand* operand,
													unsigned int bitdepth,
													WPGOperandRecord* tbl_operand,
													int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_OK;
	int firstbad_device = 0;

	switch(operand->devs_count)
	{
	case 1:
		stcode = mel_wpg_check_device_types(&operand->devs[0],&tbl_operand->dbits[cpuType]);
		break;

	case 2:
	case 3:
		////////////////////////// CHECK ATTRIBUTES ////////////////////////////////////////////////
		//if (operand->attribs)
		//{
			if ( !(operand->attribs & tbl_operand->dbits[cpuType].allowed_attribs) )
			{	//NG
				int i_attribute = 0;
				do 
				{
					unsigned int attribute = 1<<i_attribute;
					if (operand->attribs & attribute)
					{
						if ( 0 == (tbl_operand->dbits[cpuType].allowed_attribs & attribute) )
						{
							//stcode = INSTR_STATE_BAD_DEVICE_INDEXING + i_attribute;
							break;
						}
					}
				} while (++i_attribute < 4);

			}
		//}
		////////////////////////// CHECK OTYPE (if any) ////////////////////////////////////////////////
		if (operand->otype)
		{
			if ( !(operand->otype & tbl_operand->dbits[cpuType].applicable_otypes) )
			{
				//NG
				//stcode = INSTR_STATE_BAD_DEVICE_TYPE;
			}
		}
		////////////////////////// CHECK DTYPE////////////////////////////////////////////////
		////////////////////////// CHECK DTYPE_EXT////////////////////////////////////////////////
		/*
			KnXm,...	wpg-encoding: Kn,Xm	-	LINKED
			XmVk,...	wpg-encoding: Vk,Xm	+	INDEXING
			Dn.b,...	wpg-encoding: .b,Dn	+	BITACCESS
			Un\Gm,...	wpg-encoding: Un,Gm	-	UNITACCESS
		*/
		/*
			KnXmVk,...	wpg-encoding: Kn,Vk,Xm	+ INDEXING | LINKED
			Un\GmVk,...	wpg-encoding: Un,Vk,Gm	+ INDEXING | UNITACCESS
		*/			
		if ( !(operand->attribs & MEL_WPG_OPERAND_ATTRIB_LINKED) )
		{
			stcode = mel_wpg_check_device_types(&operand->devs[0],&tbl_operand->dbits[cpuType]);
			//we don't check device[1] because it is index (Vi,Zi) or BMF (\Gi) and it was done at parsing stage.
			//we don't check device[2] because it is index (Vi,Zi) and it was done at parsing stage too.
		}
		else
		{
			firstbad_device++;
			stcode = mel_wpg_check_device_types(&operand->devs[1],&tbl_operand->dbits[cpuType]);
			//don't check device[2] because it is index (Vi,Zi) and it was done at parsing stage.
		}
		break;

	default:
		stcode = INSTR_STATE_BAD_DEVICE_COUNT;
	}
	return(stcode);
}

static unsigned int mel_wpg_txt2operand(int cpuSeries,int cpuType,
										char* text,int textsize,
										WPG_Operand* operand,
										WPGOperandRecord* tbl_operand,
										unsigned int bitdepth,
										int* pfirstbad_device)
{
	unsigned int stcode = INSTR_STATE_OK;
	int firstbad_device = 0;

	//1.recognize operand
	stcode = mel_wpg_get_operand_from_txt(	cpuSeries,cpuType,
											text,
											bitdepth,
											operand,
											&firstbad_device);
	//2.compare with allowed types and indexing
	if (stcode == INSTR_STATE_OK)
	{	
		//Post-process converted device(s)
		stcode = mel_wpg_check_operand_syntax(	cpuSeries,cpuType,
												operand,
												bitdepth,
												&firstbad_device);
		if (stcode == INSTR_STATE_OK)
		{
			if (operand->devs_count > 0)
			{
				//Check operand according to instruction's requirements
				stcode = mel_wpg_check_operand_usability(	cpuSeries,cpuType,
															operand,
															bitdepth,
															tbl_operand,
															&firstbad_device);
				if (stcode == INSTR_STATE_OK)
				{
					if (operand->devs_count > 1)
					{
						//keep same order of devices as in wpg-files
						mel_wpg_arrange_devices(operand);
					}
				}
			}
			//else{instruction without operands: it's OK.}
		}
	}
	
	*pfirstbad_device = firstbad_device;
	return(stcode);
}

unsigned int mel_wpg_text2instr(	int cpuSeries,int cpuType,char* text,int textsize,
											int* pfirstbad_operand,int* pfirstbad_device,
											int* psteps,WPG_Instruction* welldone)
{
	unsigned int stcode = INSTR_STATE_BAD_TOOSHORT;//default
	int firstbad_operand = 0;
	int firstbad_device = 0;

	if (text && textsize >= MEL_WPG_MIN_INSTR_TEXT_SIZE)
	{		
		char args[MEL_INSTRUCT_OPERANDS_MAX][MEL_STRING_MAX_LENGTH+2+1]={0};
		int fields;
		
		//if(strcmp("NOP",text)) printf("\t\t|%s|",text);

		stcode = mel_wpg_txt2args_slicer(text,textsize,(char*)args,&fields);
		if (stcode == INSTR_STATE_OK)
		{
			if (fields >= 1)
			{					
				int curr_n_operands = fields - 1;				
				int currlen;
				int namelen;
				int ok_opids_start;
				int ok_opids_count = 0;
				int opid;

				stcode = INSTR_STATE_BAD_NAME;//maybe not...

				currlen = strlen(args[0]);
				for (opid = MEL_WPG_OpID_LD; opid < MEL_WPG_OpID_TOTALLY; opid++)
				{
					//namelen = INSTRUCTIONS_METRIX_FXCPU[opid].namelen;
					namelen = WPG_OPNAMES[opid].namelen;
					if (namelen == currlen)
					{
						//check instruction's name
						//if (!STRNCASECMP(args[0],INSTRUCTIONS_METRIX_FXCPU[opid].name,namelen))
						if (!STRNCASECMP(args[0],WPG_OPNAMES[opid].name,namelen))
						{						
							if(!ok_opids_count) ok_opids_start = opid;
							ok_opids_count++;
						}
					}
				}
				//INSTRUCTION name is GOOD!
				if (ok_opids_count)
				{
					int n_operands;
					
					for (opid = ok_opids_start; opid < ok_opids_start + ok_opids_count; opid++)
					{
						stcode = INSTR_STATE_OK;//maybe...
						//clean up
						memset(welldone,0x00,sizeof(WPG_Instruction));
						n_operands = INSTRUCTIONS_METRIX_FXCPU[opid].n_operands;
						//namelen = INSTRUCTIONS_METRIX_FXCPU[opid].namelen;
						namelen = WPG_OPNAMES[opid].namelen;
						//check number of operands
						if (curr_n_operands == n_operands)
						{
							if (n_operands > 0)
							{
								int i_operand;
								WPGOperandRecord *tbl_operands = INSTRUCTIONS_METRIX_FXCPU[opid].operands;
								//check every operand
								for (i_operand = 0; i_operand < n_operands; i_operand++)
								{									
									stcode = mel_wpg_txt2operand(	cpuSeries,cpuType,
																	args[1+i_operand],
																	strlen(args[1+i_operand]),
																	&welldone->params[i_operand],
																	&tbl_operands[i_operand],
																	INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth,
																	&firstbad_device
																);									
									if (stcode != INSTR_STATE_OK)
									{
										firstbad_operand = i_operand;
										break;
									}
									//accumulate attributes
									welldone->attribs |= welldone->params[i_operand].attribs;
								}
							}
							else
								//check length of whole text: must be the same as instruction's name length
								if (textsize != namelen)
								{
									//printf("fields: %d\n",fields);
									printf("textsize(%d), namelen(%d)\t",textsize,namelen);
									{
										int i;
										for (i = 0; i <textsize; i++)
										{
											printf("%02X ",text[i]);
										}
										printf("\n");
									}
									stcode = INSTR_STATE_BAD_TOOLONG;
									firstbad_operand = namelen;
								}
								else
									stcode = INSTR_STATE_OK;

							//check current opid finally
							if (stcode == INSTR_STATE_OK)
							{
								break;//we got a right opid!
							}
						}
						else
						{	//error: wrong number of operands
							if (curr_n_operands > n_operands)
								stcode = INSTR_STATE_BAD_MORENOPERANDS;
							else
								stcode = INSTR_STATE_BAD_LESSNOPERANDS;
						}
						//printf("%3d:%08X\n",opid,stcode);
					}//search DB by instruction's name
										
					if (stcode == INSTR_STATE_OK)
					{
						//save approved
						welldone->opid		= opid;
						welldone->bitdepth	= INSTRUCTIONS_METRIX_FXCPU[opid].bitdepth;
						welldone->exectype	= INSTRUCTIONS_METRIX_FXCPU[opid].exectype;
						welldone->n_params	= n_operands;
						welldone->steps		= mel_wpg_instr_steps(welldone);
						*psteps = welldone->steps;						
					}
				}//else{INSTR_STATE_BAD_NAME}
			}
			//else{INSTR_STATE_BAD_TOOSHORT}
		}
		else
		{
			firstbad_operand = fields;
		}

		//DEBUG
		mel_wpg_debug(welldone,stcode,firstbad_operand,firstbad_device);
	}

	*pfirstbad_operand	= firstbad_operand;
	*pfirstbad_device	= firstbad_device;
	return(stcode);
}

static void mel_wpg_debug(WPG_Instruction* instruction,unsigned int stcode,int firstbad_operand,int firstbad_device)
{
	switch(stcode)
	{
	case INSTR_STATE_OK:
		break;
	case INSTR_STATE_BAD_TOOSHORT:
		printf("INSTR_STATE_BAD_TOOSHORT!\n");
		break;
	case INSTR_STATE_BAD_TOOLONG:
		printf("INSTR_STATE_BAD_TOOLONG!\n");
		break;
	case INSTR_STATE_BAD_NAME:
		printf("INSTR_STATE_BAD_NAME!\n");
		break;
	case INSTR_STATE_BAD_LESSNOPERANDS:
		printf("INSTR_STATE_BAD_LESSNOPERANDS!\n");
		break;
	case INSTR_STATE_BAD_MORENOPERANDS:
		printf("INSTR_STATE_BAD_MORENOPERANDS!\n");
		break;
	case INSTR_STATE_BAD_DEVICE_TYPE:
		printf("INSTR_STATE_BAD_DEVICE_TYPE!\n");
		printf("[0x%08X + 0x%08X](%s) [%d] \n",
			instruction->params[firstbad_operand].devs[firstbad_device].dtype,
			instruction->params[firstbad_operand].devs[firstbad_device].dtype_ext,
			mel_get_device_name(instruction->params[firstbad_operand].devs[firstbad_device].dtype),
			instruction->params[firstbad_operand].devs[firstbad_device].index);
		break;
	case INSTR_STATE_BAD_DEVICE_TYPE_EXT:
		printf("INSTR_STATE_BAD_DEVICE_TYPE_EXT!\n");
		printf("[0x%08X + 0x%08X](%s) [%d] \n",
			instruction->params[firstbad_operand].devs[firstbad_device].dtype,
			instruction->params[firstbad_operand].devs[firstbad_device].dtype_ext,
			mel_get_device_name(instruction->params[firstbad_operand].devs[firstbad_device].dtype),
			instruction->params[firstbad_operand].devs[firstbad_device].index);
		break;
	case INSTR_STATE_BAD_DEVICE_INDEX:
		printf("INSTR_STATE_BAD_DEVICE_INDEX!\n");
		printf("[0x%08X + 0x%08X](%s) [%d] \n",
				instruction->params[firstbad_operand].devs[firstbad_device].dtype,
				instruction->params[firstbad_operand].devs[firstbad_device].dtype_ext,
				mel_get_device_name(instruction->params[firstbad_operand].devs[firstbad_device].dtype),
				instruction->params[firstbad_operand].devs[firstbad_device].index);
		break;
	case INSTR_STATE_BAD_DEVICE_INDEXING:
		printf("INSTR_STATE_BAD_DEVICE_INDEXING!\n");
		break;
	case INSTR_STATE_BAD_DEVICE_COUNT:
		printf("INSTR_STATE_BAD_DEVICE_COUNT!\n");
		break;

	case INSTR_STATE_BAD_NO_STRCLOSING:
		printf("INSTR_STATE_BAD_NO_STRCLOSING!\n");
		break;

	case INSTR_STATE_BAD_NO_SLASH:
		printf("INSTR_STATE_BAD_NO_SLASH!\n");
		break;

	case INSTR_STATE_BAD_NO_G:
		printf("INSTR_STATE_BAD_NO_G!\n");
		break;

	case INSTR_STATE_BAD_CHAR:
		printf("INSTR_STATE_BAD_CHAR!\n");
		break;
	}

	if (stcode != INSTR_STATE_OK)
	{
		printf("bad: operand [%d], device [%d].\n",firstbad_operand,firstbad_device);
		exit(1);
	}

	return;
}