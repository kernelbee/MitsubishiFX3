#include "mel_wpg_item.h"
#include "mel_wpg_instr.h"
#include "mel_wpg_device.h"

#include <stdio.h>//printf,sprintf
#include <stdlib.h>//calloc
#include <string.h>//memset

#ifdef WIN32
#pragma warning (disable : 4996)
#endif

WPG_Item*	mel_wpg_item_decoder	(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,int* psizedone,int* psteps,int* pnopsteps)
{
	WPG_Item* item = 0;
	int size_done = 0;
	int steps = 0;
	int nops = 0;

	if (stream_size > 0)
	{
		int size_brutto;

		size_brutto = stream[0];

		if (size_brutto <= stream_size)
		{
			if (size_brutto == stream[size_brutto-1])
			{
				int size_netto = size_brutto - 2;
				int code;
				
				code = stream[1];
				
				switch(code)
				{
				case WPG_CODE_STATEMENT:
				case WPG_CODE_NOTE:

					item = calloc(1,sizeof(WPG_Item));
					if (item)
					{
						int text_size = size_netto - 2;
						item->itype = (code==WPG_CODE_STATEMENT)? WPG_ITEM_TYPE_STATEMENT
																:WPG_ITEM_TYPE_NOTE;
						item->size = text_size;
						item->body = calloc(1,item->size+1);
						memcpy(item->body,stream+3,item->size);
						*((char*)item->body + item->size) = 0x00;
					}
					size_done = size_brutto;
					break;

				case WPG_CODE_LABEL:					
					/*
					if (size_netto > 1 )
					{
						if (stream[2]==0x02)
						{
							//long: 2 bytes index
						}							
					}
					*/
					size_done += size_brutto;
					size_brutto = stream[size_done + 0];
					if (size_brutto <= stream_size-size_done)
					{
						if (size_brutto == stream[size_done + size_brutto - 1])
						{
							size_netto  = size_brutto - 2;

							item = calloc(1,sizeof(WPG_Item));
							if (item)
							{
								code = stream[size_done + 1];							
								switch(code)
								{
								case MEL_DEV_CODE_P:
									{
										WPG_Label* label;

										item->itype = WPG_ITEM_TYPE_LABEL;
										item->size = sizeof(WPG_Label);
										item->body = calloc(1,item->size);
										if(item->body)
										{
											label = item->body;
											label->is_long = (size_netto==3);
											label->index = (label->is_long)? (int)stream[size_done+2]|((int)stream[size_done+3]<<8)
																			: stream[size_done+2];

											steps = (label->is_long)? 2:1;
										}
									}
									break;
								case MEL_DEV_CODE_I:
									{
										WPG_Interrupt* interr;

										item->itype = WPG_ITEM_TYPE_INTERRUPT;
										item->size = sizeof(WPG_Interrupt);
										item->body = calloc(1,item->size);
										if (item->body)
										{
											interr = item->body;
											interr->index = (size_netto==3)? (int)stream[size_done+2]|((int)stream[size_done+3]<<8)
																			: stream[size_done+2];																						
											steps = 1;											
										}
									}
									break;
								}
							}
						}
					}
					size_done += size_brutto;
					break;

				default:
					{
						int instr_whole_size = size_brutto;

						steps = *psteps;
						//instruction and operands (if any)
						item = mel_wpg_dec_instruction	(cpuSeries,cpuType,stream,stream_size,&instr_whole_size,&steps);
						size_done = instr_whole_size;
						if (item)
						{
							WPG_Instruction* ins = item->body;

							if (ins->opid == MEL_WPG_OpID_NOP)
							{
								nops++;
							}
						}
					}
					break;
				}
			}
		}
	}
	if(item)
	{
		item->steps = steps;
		item->nops	= nops*INSTRUCTIONS_METRIX_FXCPU[MEL_WPG_OpID_NOP].steps;
	}
	*psizedone	= size_done;
	*psteps		= steps;
	*pnopsteps	= nops*INSTRUCTIONS_METRIX_FXCPU[MEL_WPG_OpID_NOP].steps;
	return(item);
}

int	mel_wpg_item_encoder	(WPG_Item* item,int cpuSeries,int cpuType,unsigned char* stream,int* psteps,int* pnopsteps)
{
	int enc_size = 0;
	int size_netto = 0;
	int steps = 0;
	int nops = 0;

	switch(item->itype)
	{
	case WPG_ITEM_TYPE_INSTRUCTION:
		{
			int instr_whole_size;

			//instruction and operands (if any)
			instr_whole_size = mel_wpg_enc_instruction	(item,cpuSeries,cpuType,stream,&steps);
			enc_size = instr_whole_size;
			{
				WPG_Instruction* ins = item->body;

				if (ins->opid == MEL_WPG_OpID_NOP)
				{
					nops++;
				}
			}
		}
		break;

	case WPG_ITEM_TYPE_STATEMENT:
	case WPG_ITEM_TYPE_NOTE:
		stream[1] = (item->itype==WPG_ITEM_TYPE_NOTE)? WPG_CODE_NOTE:WPG_CODE_STATEMENT;
		stream[2] = 0x00;
		memcpy(stream+3,item->body,item->size);
		size_netto = item->size + 2;
		break;

	case WPG_ITEM_TYPE_LABEL:
		{
			WPG_Label* label = (WPG_Label*)item->body;
			int size_brutto;
						
			stream[1] = WPG_CODE_LABEL;
			if (label->is_long)
			{
				size_netto = 2;
				stream[2] = 0x02;
			}
			else
			{
				size_netto = 1;
			}
			
			size_brutto = size_netto + 2;
			stream[0] = size_brutto;
			stream[size_brutto-1] = stream[0];
			
			enc_size = size_brutto;

			stream[enc_size+1] = MEL_DEV_CODE_P;
			if(label->is_long)
			{
				stream[enc_size+2] = label->index&0xFF;
				stream[enc_size+3] = (label->index>>8)&0xFF;
				size_netto = 3;

				steps = 2;
			}
			else
			{
				stream[enc_size+2] = label->index&0xFF;
				size_netto = 2;

				steps = 1;
			}
			size_brutto = size_netto + 2;
			stream[enc_size+0] = size_brutto;
			stream[enc_size+size_brutto-1] = stream[enc_size+0];

			enc_size += size_brutto;			
		}				
		break;

	case WPG_ITEM_TYPE_INTERRUPT:
		{
			WPG_Interrupt* interr = (WPG_Interrupt*)item->body;
			int size_brutto;

			stream[1] = WPG_CODE_LABEL;
			size_netto = 1;
			size_brutto = size_netto + 2;
			stream[0] = size_brutto;
			stream[size_brutto-1] = stream[0];

			enc_size = size_brutto;
			
			stream[enc_size+1] = MEL_DEV_CODE_I;

			if(interr->index > 255)
			{
				stream[enc_size+2] = interr->index&0xFF;
				stream[enc_size+3] = (interr->index>>8)&0xFF;
				size_netto = 3;
			}
			else
			{
				stream[enc_size+2] = interr->index&0xFF;
				size_netto = 2;
			}

			size_brutto = size_netto + 2;
			stream[enc_size+0] = size_brutto;
			stream[enc_size+size_brutto-1] = stream[enc_size+0];
						
			enc_size += size_brutto;
			steps = 1;
		}
		break;
	}
	if (enc_size==0)
	{
		if (size_netto > 0)
		{
			int size_brutto = size_netto + 2;

			stream[0] = size_brutto;
			stream[size_brutto-1] = stream[0];
			enc_size = size_brutto;
		}
	}
	//printf("enc: itype[%d]\t%d bytes\n",item->itype,enc_size);
	*psteps	= steps;
	*pnopsteps	= nops*INSTRUCTIONS_METRIX_FXCPU[MEL_WPG_OpID_NOP].steps;
	return(enc_size);
}

//TXT -> ITEM {INSTRUCTION{OPERAND(S){DEVICE(S)}},STATEMENT,NOTE,LABEL,INTERRUPT}
unsigned int mel_wpg_text2item(int cpuSeries,int cpuType,char* text,int textsize,
										unsigned int* pinstrerrcode,int* pfirstbad_operand,int* pfirstbad_device,
										int* psteps,int* pnopsteps,
										WPG_Item* welldone)
{
	unsigned int stcode = ITEM_STATE_BAD_TOOSHORT;
	int steps = 0;
	int nops = 0;

	if (textsize >= 1)
	{
		switch(text[0])
		{
		case '*'://STATEMENT or NOTE
			if (textsize >= 2)
			{
				int dataoffset;
				if (text[1]!='<')
				{//STATEMENT
					if (textsize <= MEL_WPG_MAX_STATEMENT_TEXT_SIZE+1)
					{
						dataoffset = 1;
						welldone->itype = WPG_ITEM_TYPE_STATEMENT;
						stcode = ITEM_STATE_OK;
					}
					else
					{
						//stcode = ITEM_STATE_BAD_STATEMENT;
						stcode = ITEM_STATE_BAD_TOOLONG;
					}
				}
				else
				{					
					//NOTE
					if (textsize > 2)
					{
						if (textsize <= MEL_WPG_MAX_NOTE_TEXT_SIZE+2)
						{
							dataoffset = 2;
							welldone->itype = WPG_ITEM_TYPE_NOTE;
							stcode = ITEM_STATE_OK;
						}
						else
						{
							//stcode = ITEM_STATE_BAD_NOTE;
							stcode = ITEM_STATE_BAD_TOOLONG;
						}
					}
				}
				if (stcode == ITEM_STATE_OK)
				{
					welldone->size = textsize - dataoffset;
					welldone->body = calloc(1,welldone->size+1);
					if (welldone->body)
					{
						memcpy(welldone->body,text+dataoffset,welldone->size);
						*((char*)welldone->body + welldone->size) = 0x00;
					}
					else
					{
						stcode = ITEM_STATE_BAD_NO_MEMORY;
					}
				}
			}
			break;

		case 'I'://INTERRUPT or INSTRUCTION
		case 'i':
		case 'P'://LABEL or INSTRUCTION
		case 'p':
			if (textsize > 1)
			{				
				stcode = ITEM_STATE_BAD_UNKNOWN;
				//try to separate labels/interrupts and 'P/p'-instructions
				//in case of labels/interrupts: all characters, starting from second one, must be decimal digits
				if (text[1] >= '0' && text[1] <= '9')
				{
					int is_wrong_char = 0;
					int i;
					//...all characters, starting from second one, must be decimal digits
					i = 2;					
					while(i < textsize)
					{
						if (!(text[i] >= '0' && text[i] <= '9'))
						{
							is_wrong_char = 1;
							break;
						}
						i++;
					};

					if (!is_wrong_char)
					{
						long int val;
						char* endptr = 0;

						//in case of labels/interrupts: second character is digit [0-9]
						errno = 0;
						val = strtol(text+1,&endptr,10);
						if (errno == 0)
						{
							if (endptr!=text)
							{
								if (*endptr == '\0')//"one parameter" argument
								{
									if (	text[0] == 'I'
										||	text[0] == 'i')
									{	//INTERRUPT
										if (val >= 0)
										{
											stcode = ITEM_STATE_OK;
											welldone->itype = WPG_ITEM_TYPE_INTERRUPT;
											welldone->size = sizeof(WPG_Interrupt);
											welldone->body = calloc(1,welldone->size);
											if (welldone->body)
											{
												WPG_Interrupt* interr;
												interr = welldone->body;
												interr->index = val;
												steps = 1;
											}
											else
											{
												stcode = ITEM_STATE_BAD_NO_MEMORY;
											}											
										}
										else
										{

											stcode = ITEM_STATE_BAD_INTERRUPT;
										}	
									}
									else
									{	//LABEL
										if (val >= 0)
										{
											stcode = ITEM_STATE_OK;
											welldone->itype = WPG_ITEM_TYPE_LABEL;
											welldone->size = sizeof(WPG_Label);
											welldone->body = calloc(1,welldone->size);
											if (welldone->body)
											{
												WPG_Label* label;
												label = welldone->body;
												label->index = val;
												label->is_long = (val > 255)? 1:0;
												steps = (label->is_long)? 2:1;
											}
											else
											{
												stcode = ITEM_STATE_BAD_NO_MEMORY;
											}
										}
										else
										{

											stcode = ITEM_STATE_BAD_LABEL;
										}
									}
								}
							}
						}
					}
					else
					{
						if (	text[0] == 'I'
							||	text[0] == 'i')
						{
							stcode = ITEM_STATE_BAD_INTERRUPT;
						}
						else
						{
							stcode = ITEM_STATE_BAD_LABEL;
						}

					}
				}
				else
				{
					goto __try_instruction;
				}				
			}
			break;

__try_instruction:
		default://maybe INSTRUCTION
			{
				unsigned int stcodeINSTR;				
				WPG_Instruction doneInstr = {0};
				
				stcodeINSTR = mel_wpg_text2instr(cpuSeries,cpuType,text,textsize,
												pfirstbad_operand,pfirstbad_device,&steps,&doneInstr);
				if (stcodeINSTR == INSTR_STATE_OK)
				{						
						welldone->itype = WPG_ITEM_TYPE_INSTRUCTION;
						welldone->size = sizeof(WPG_Instruction);
						welldone->body = calloc(1,welldone->size);
						if (welldone->body)
						{
							memcpy(welldone->body,&doneInstr,welldone->size);
							stcode = ITEM_STATE_OK;
							if (doneInstr.opid == MEL_WPG_OpID_NOP)
							{								
								nops++;
							}

							//DEBUG
							{
								unsigned char buf[1024] = {0};
								int encsize;
								int tsteps;							

								encsize = mel_wpg_enc_instruction(welldone,cpuSeries,cpuType,buf,&tsteps);
								if (encsize > 0)
								{
									/*
									int i;
									printf("%s (%d steps)\t",text,tsteps);
									for (i = 0; i < encsize; i++)
									{
										printf("%02X ",buf[i]);
									}
									printf("\n");									
									*/
								}
								else
								{
									printf("ERROR OF INSTR ENCODING! {%s}\n",text);
									exit(2);
								}								
							}
						}
						else
						{
							stcode = ITEM_STATE_BAD_NO_MEMORY;
						}
				}
				else	stcode = ITEM_STATE_BAD_INSTRUCTION;
				*pinstrerrcode = stcodeINSTR;
			}
		}
	}
	if (stcode == ITEM_STATE_OK)
	{
		welldone->steps = steps;
		welldone->nops	= nops;
	}
	if (nops)
	{
		printf("NOP added!!![%d]\n",*psteps);
	}
	*psteps = steps;
	*pnopsteps = nops;
	return(stcode);
}

//ITEM {INSTRUCTION{OPERAND(S){DEVICE(S)}},STATEMENT,NOTE,LABEL,INTERRUPT} -> TXT
char* mel_wpg_item2text(int cpuSeries,int cpuType,WPG_Item* item,int* ptextsize)
{
	char* retptr = 0;
	static char text[MEL_WPG_MAX_INSTR_TEXT_SIZE] = {0};
	int textsize = 0;

	text[0] = 0;

	switch(item->itype)
	{
	case WPG_ITEM_TYPE_INSTRUCTION:
		retptr = mel_wpg_instr2text(cpuSeries,cpuType,(WPG_Instruction*)item->body,ptextsize);
		break;
	case WPG_ITEM_TYPE_STATEMENT:
		*ptextsize = sprintf(text,"*%s",(char*)item->body);
		retptr = text;
		break;
	case WPG_ITEM_TYPE_NOTE:
		*ptextsize = sprintf(text,"*<%s",(char*)item->body);
		retptr = text;
		break;
	case WPG_ITEM_TYPE_LABEL:
		//*ptextsize = sprintf(text,"P%-5d",((WPG_Label*)item->body)->index);
		*ptextsize = sprintf(text,"P%d",((WPG_Label*)item->body)->index);
		retptr = text;
		break;
	case WPG_ITEM_TYPE_INTERRUPT:
		//*ptextsize = sprintf(text,"I%-5d",((WPG_Interrupt*)item->body)->index);
		*ptextsize = sprintf(text,"I%d",((WPG_Interrupt*)item->body)->index);
		retptr = text;
		break;
	}

	return(retptr);
}

int mel_til_get_line(unsigned char* stream,int stream_size,char* line,int* pline_dec_size,int max_line_size)
{
	int line_enc_size = 0;//with LF,CR (0x0A,0x0D)
	int line_dec_size = 0;//without LF,CR and ending null

	while (		line_enc_size < stream_size
			&&	line_dec_size <= max_line_size
			)
	{
		unsigned char curr_byte;

		curr_byte = stream[line_enc_size++];
		if (curr_byte != 0x0A && curr_byte!= 0x0D)
		{
			line[line_dec_size++] = curr_byte;
		}
		else
		{
			line[line_dec_size] = 0x00;//C-string ending null
		}
		if (curr_byte == 0x0A)
		{
			break;//this is the end of line, so exit from loop
		}
	};
	if (line[line_dec_size] != 0x00)
	{
		line[line_dec_size] = 0x00;//C-string ending null
	}
	*pline_dec_size = line_dec_size;
	return(line_enc_size);
}

void mel_wpg_item_swap_data(WPG_Item* item_a,WPG_Item* item_b)
{
	WPG_Item item_t;	
	//save a
	item_t.itype = item_a->itype;
	item_t.body = item_a->body;
	item_t.size = item_a->size;
	item_t.steps= item_a->steps;
	item_t.nops	= item_a->nops;
	//a <- b
	item_a->itype = item_b->itype;
	item_a->body = item_b->body;
	item_a->size = item_b->size;
	item_a->steps= item_b->steps;
	item_a->nops = item_b->nops;
	//b <- saved a
	item_b->itype = item_t.itype;
	item_b->body = item_t.body;
	item_b->size = item_t.size;		
	item_b->steps= item_t.steps;
	item_b->nops = item_t.nops;
	return;
}