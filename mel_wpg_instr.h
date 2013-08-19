#ifndef __mel_wpg_opcodes_h__
#define __mel_wpg_opcodes_h__

#include "mel_wpg_item.h"
#include "mel_wpg_device.h"
#include "mel_wpg_tables.h"

typedef struct tagWPG_Operand
{
#define MEL_OTYPE_EXT_KnM	(1<<0)	//->MEL_WPG_OPERAND_ATTRIB_LINKED
#define MEL_OTYPE_EXT_KnS	(1<<1)	//->MEL_WPG_OPERAND_ATTRIB_LINKED
#define MEL_OTYPE_EXT_KnX	(1<<2)	//->MEL_WPG_OPERAND_ATTRIB_LINKED
#define MEL_OTYPE_EXT_KnY	(1<<3)	//->MEL_WPG_OPERAND_ATTRIB_LINKED

	unsigned int	otype;//bit encoded composit operand type (based on included devices);

#define MEL_WPG_MAX_NDEVS_PER_OPERAND 3
	int				devs_count;
	PLCRegDevice	devs[MEL_WPG_MAX_NDEVS_PER_OPERAND];

#define MEL_WPG_OPERAND_ATTRIB_INDEXED		1	//Vn,Zn
#define MEL_WPG_OPERAND_ATTRIB_LINKED		2	//Kn
#define MEL_WPG_OPERAND_ATTRIB_BITACCESS	4	//.b
#define MEL_WPG_OPERAND_ATTRIB_UNITACCESS	8	//Un\Gm
	unsigned int	attribs;//used by instruction operands validation (comparing with WPGOperandRecord.attribs_allowed);
	
}WPG_Operand;

typedef struct tagWPG_Instruction
{
	int opid;
	int steps;
	unsigned int	bitdepth;
	unsigned int	exectype;

	WPG_Operand		params[MEL_INSTRUCT_OPERANDS_MAX];
	int				n_params;
	unsigned int	attribs;//bitwise accumulator of all params[i].attribs; -> used by steps calculation;
		
}WPG_Instruction;

WPG_Item*	mel_wpg_dec_instruction	(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,int* psizedone,int* psteps);
int			mel_wpg_enc_instruction	(WPG_Item* item,int cpuSeries,int cpuType,unsigned char* stream,int* psteps);

#define MEL_WPG_OPCODE_SIZE_NETTO_MIN 0
#define MEL_WPG_OPCODE_SIZE_NETTO_MAX 4

//returns: (!0)-success; 0-failure: unknown code
int mel_wpg_instr_setup_by_opcode(	unsigned char* enc_code,int code_size,WPG_Instruction* instr);
//returns: opcode's length in bytes; >=0 - success; <0 - failure: unknown opid
int mel_wpg_instr_make_opcode(WPG_Instruction* instr,unsigned char* opcode_bytes);

//works with correct instructions only (must be validated before calling!)
int mel_wpg_instr_steps(WPG_Instruction* instr);

enum
{
	INSTR_STATE_OK = 0,
	INSTR_STATE_BAD_TOOSHORT,
	INSTR_STATE_BAD_TOOLONG,				//index of first wrong char -> *pfirstbad
	INSTR_STATE_BAD_NAME,
	INSTR_STATE_BAD_LESSNOPERANDS,			//min value of n_operands -> *pfirstbad
	INSTR_STATE_BAD_MORENOPERANDS,			//index of last operand -> *pfirstbad

	INSTR_STATE_BAD_DEVICE_TYPE,			//index of operand -> *pfirstbad
	INSTR_STATE_BAD_DEVICE_TYPE_EXT,		//index of operand -> *pfirstbad
	INSTR_STATE_BAD_DEVICE_INDEX,			//index of operand -> *pfirstbad

	INSTR_STATE_BAD_DEVICE_INDEXING,		//index of operand -> *pfirstbad
	INSTR_STATE_BAD_DEVICE_LINKING,			//index of operand -> *pfirstbad
	INSTR_STATE_BAD_DEVICE_BITACCESSING,	//index of operand -> *pfirstbad
	INSTR_STATE_BAD_DEVICE_UNITACCESSING,	//index of operand -> *pfirstbad

	INSTR_STATE_BAD_DEVICE_COUNT,			//index of operand -> *pfirstbad

	INSTR_STATE_BAD_NO_STRCLOSING,			//"..." -> "...
	INSTR_STATE_BAD_NO_SLASH,				//U..\G.. -> U..G..
	INSTR_STATE_BAD_NO_G,					//U..\G.. -> U..G..
	INSTR_STATE_BAD_CHAR					//not JIS8
};
unsigned int mel_wpg_text2instr(int cpuSeries,int cpuType,char* text,int textsize,
								int* pfirstbad_operand,int* pfirstbad_device,
								int* psteps,
								WPG_Instruction* welldone);
char* mel_wpg_instr2text(int cpuSeries,int cpuType,WPG_Instruction* instr,int* ptextsize);

void mel_wpg_printf_opcodes_FXCPU(void);
void mel_wpg_generate_metrix_FXCPU(void);
void mel_wpg_printf_metrix_FXCPU(void);

#endif //__mel_wpg_opcodes_h__
