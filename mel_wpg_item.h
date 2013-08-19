#ifndef __mel_wpg_item_h__
#define __mel_wpg_item_h__

#include "mel_common.h"

typedef struct tagWPG_Label
{
	int					is_long;
	int					index;

}WPG_Label;

typedef struct tagWPG_Iterrupt
{
	int					index;

}WPG_Interrupt;

typedef struct tagWPG_Item
{
#define WPG_ITEM_TYPE_UNKNOWN		0
#define WPG_ITEM_TYPE_INSTRUCTION	2
#define WPG_ITEM_TYPE_STATEMENT		4
#define WPG_ITEM_TYPE_NOTE			8
#define WPG_ITEM_TYPE_LABEL			16
#define WPG_ITEM_TYPE_INTERRUPT		32

	int		itype;
	void*	body;
	int		size;
	int		steps;
	int		nops;
	/////////////////////////
	struct tagWPG_Item* prev;
	struct tagWPG_Item* next;
	/////////////////////////
}WPG_Item;

WPG_Item*	mel_wpg_item_decoder	(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,int* psizedone,int* psteps,int* pnopsteps);
int			mel_wpg_item_encoder	(WPG_Item* item,int cpuSeries,int cpuType,unsigned char* stream,int* psteps,int* pnopsteps);

#define ITEM_STATE_OK					0
#define ITEM_STATE_BAD_TOOSHORT			(1<<0)
#define ITEM_STATE_BAD_TOOLONG			(1<<1)
#define ITEM_STATE_BAD_INSTRUCTION		(1<<2)
#define ITEM_STATE_BAD_NOTE				(1<<3)
#define ITEM_STATE_BAD_STATEMENT		(1<<4)
#define ITEM_STATE_BAD_INTERRUPT		(1<<5)
#define ITEM_STATE_BAD_LABEL			(1<<6)
#define ITEM_STATE_BAD_UNKNOWN			(1<<7)
#define ITEM_STATE_BAD_NO_MEMORY		(1<<31)

//TXT -> ITEM {INSTRUCTION{OPERAND(S){DEVICE(S)}},STATEMENT,NOTE,LABEL,INTERRUPT}
unsigned int mel_wpg_text2item(int cpuSeries,int cpuType,char* text,int textsize,
								unsigned int* pinstrerrcode,int* pfirstbad_operand,int* pfirstbad_device,
								int* psteps,int* pnopsteps,
								WPG_Item* welldone);
//ITEM {INSTRUCTION{OPERAND(S){DEVICE(S)}},STATEMENT,NOTE,LABEL,INTERRUPT} -> TXT
char* mel_wpg_item2text(int cpuSeries,int cpuType,WPG_Item* item,int* ptextsize);

//return: encoded line size
int mel_til_get_line(unsigned char* stream,int stream_size,char* line,int* pline_dec_size,int max_line_size);

void mel_wpg_item_swap_data(WPG_Item* item_a,WPG_Item* item_b);

#endif /* __mel_wpg_item_h__ */
