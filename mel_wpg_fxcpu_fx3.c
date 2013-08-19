#include "mel_wpg.h"
#include "mel_wpg_fxcpu_fx3.h"
#include "mel_wpg_instr.h"
#include <string.h>//memset
#include <stdlib.h>//free
#include <stdio.h>//printf

#ifdef WIN32
#pragma warning (disable : 4996)
#endif

/* returns: 1-ok,0-ng */
int	mel_wpg_FXCPU_FX3_link_item(void* wpgx,WPG_Item* item)
{
	int ret_code = 0;//ng by default

	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;

	if (wpg && item)
	{
		//back link to last one
		item->prev = wpg->last;

		//link last one to new item
		if (item->prev)
		{
			item->prev->next = item;
		}
		//ending linked list
		item->next = 0;

		if (wpg->first == 0)
		{
			wpg->first = item;
		}
		//set new one as a last one
		wpg->last = item;

		//if(item->nops) printf("NOP added!!![%d]\n",wpg->steps);

		//update program sizes
		wpg->steps		+= item->steps;
		wpg->nop_steps	+= item->nops;
		
		//printf("%d\t%d\n",wpg->steps,wpg->nop_steps);

		ret_code = 1;//ok		
	}

	return(ret_code);
}

/* returns: 1-ok,0-ng */
int	mel_wpg_FXCPU_FX3_unlink_item(void* wpgx,WPG_Item* item)
{
	int ret_code = 0;//ng by default

	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;

	if (wpg && item)
	{		
		int is_exist = 0;
		WPG_Item* item_t;

		//search for item
		item_t = wpg->first;
		while(item_t)
		{
			if (item_t == item)
			{
				is_exist = 1; break;//exit loop
			}
			item_t = item_t->next;
		};

		//delete it from program if found
		if (is_exist)
		{
			//unlink it from previous
			if (item->prev)
			{
				item->prev->next = item->next;				
			}
			else
			{
				//it is first item in list: update pointer to first one.
				wpg->first = item->next;
			}

			//unlink it from next one
			if (item->next)
			{
				item->next->prev = item->prev;
			}
			else
			{
				//it is last item in list: update pointer to last one.
				wpg->last = item->prev;
			}

			//reset self links
			item->prev = 0;
			item->next = 0;

			//update program sizes
			wpg->steps		-= item->steps;
			wpg->nop_steps	-= item->nops;

			ret_code = 1;//ok
		}		
	}

	return(ret_code);
}

/* returns: done stream size > 0, error =0. */
int mel_wpg_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wpgx)
{
	int done_size = 0;
	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;

	if (enc_size > MEL_WPG_SUBHEADER_SIZE)
	{
		wpg->magic_num0 = (int)stream[0]|((int)stream[1]<<8);
		wpg->magic_num1 = (int)stream[2]|((int)stream[3]<<8);		
		done_size += MEL_WPG_SUBHEADER_SIZE;

		wpg->steps		= 0;
		wpg->nop_steps	= 0;
		wpg->first	= 0;
		wpg->last	= 0;

		while(done_size < enc_size)
		{
			WPG_Item* item;
			int item_enc_size = 0;
			int steps = wpg->steps;
			int nop_steps;

			item = mel_wpg_item_decoder(MEL_PLC_SERIES_FXCPU,MEL_PLC_FXCPU_TYPE_FX3U_C,
										stream+done_size,enc_size-done_size,&item_enc_size,&steps,&nop_steps);
			done_size += item_enc_size;

			if (item && item->body)
			{				
				//add item to linked list:
				if(!mel_wpg_FXCPU_FX3_link_item(wpg,item))
				{
					//error: can't add item to program.
					done_size = 0; break;//exit from loop
				}

				if (item->itype == WPG_ITEM_TYPE_NOTE && item->prev)
				{
					if (item->prev->itype == WPG_ITEM_TYPE_INSTRUCTION)
					{
						//swap: {instruction,note} -> {note,instruction}
						mel_wpg_item_swap_data(item->prev,item);						
					}
				}				
			}
			else
			{
				//EOF or error: exit from loop
				break;
			}		
		};
	}

	return(done_size);
}
/* returns: done stream size > 0, error =0. */
int	mel_wpg_FXCPU_FX3_encoder(void* wpgx,unsigned char* stream,int maxsteps)
{
	int done_size = 0;
	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;
	WPG_Item* item;
	void* last_swaped_note = 0;
	int steps_totally = 0;

	stream[0] = wpg->magic_num0&0xFF;
	stream[1] = (wpg->magic_num0>>8)&0xFF;
	stream[2] = wpg->magic_num1&0xFF;
	stream[3] = (wpg->magic_num1>>8)&0xFF;
	done_size += MEL_WPG_SUBHEADER_SIZE;

	item = wpg->first;

	while(item)
	{
		int item_enc_size = 0;
		int steps;
		int nop_steps;

		if (item->itype == WPG_ITEM_TYPE_NOTE && item->body != last_swaped_note && item->next)
		{
			if (item->next->itype == WPG_ITEM_TYPE_INSTRUCTION)
			{				
				last_swaped_note = item->body;				
			
				//make (temporary) copies and swap them: {note,instruction} - > {instruction,note}
				{
					WPG_Item item_a;
					WPG_Item item_b;

					memcpy(&item_a,item,sizeof(WPG_Item));
					memcpy(&item_b,item->next,sizeof(WPG_Item));
					
					//link to each other					
					item_a.next = &item_b;
					item_b.prev = &item_a;
					mel_wpg_item_swap_data(&item_a,&item_b);
					//switch to the swapped copy
					item = &item_a;
				}				
			}
		}
		
		item_enc_size = mel_wpg_item_encoder(	item,
												MEL_PLC_SERIES_FXCPU,
												MEL_PLC_FXCPU_TYPE_FX3U_C,
												stream+done_size,
												&steps,
												&nop_steps);
		if (item_enc_size <= 0)
		{
			done_size = 0;
			break;//error: can't encode item
		}
		
		done_size += item_enc_size;
		steps_totally	+= steps;
		//wpg->nop_steps	+= nop_steps;
		
		if (steps_totally > maxsteps)
		{
			done_size = 0;
			goto __exit_point;//error: can't encode item
		}

		item = item->next;
	};

	//printf("totally: %d, max: %d\n",steps_totally,maxsteps);
	
#if 1
	//fill the rest by NOPs
	{
		WPG_Item item = {0};
		WPG_Instruction instr = {0};
		int nop_steps = 0;

		//set rest by NOPs (virtually)
		if (wpg->steps < maxsteps)
		{
			wpg->nop_steps	+= maxsteps - wpg->steps;
			wpg->steps		+= maxsteps - wpg->steps;
		}

		instr.opid			= MEL_WPG_OpID_NOP;
		instr.steps			= INSTRUCTIONS_METRIX_FXCPU[instr.opid].steps;
		instr.bitdepth		= INSTRUCTIONS_METRIX_FXCPU[instr.opid].bitdepth;
		instr.exectype		= INSTRUCTIONS_METRIX_FXCPU[instr.opid].exectype;
		instr.n_params		= INSTRUCTIONS_METRIX_FXCPU[instr.opid].n_operands;
		
		item.itype = WPG_ITEM_TYPE_INSTRUCTION;
		item.body = &instr;
		item.size = sizeof(instr);
		item.steps= instr.steps;
		item.nops = 1;

		while(steps_totally < maxsteps)
		{
			int item_enc_size = 0;
			int steps;
						
			item_enc_size = mel_wpg_item_encoder(	&item,
													MEL_PLC_SERIES_FXCPU,
													MEL_PLC_FXCPU_TYPE_FX3U_C,
													stream+done_size,
													&steps,
													&nop_steps);

			if (item_enc_size <= 0)
			{
				done_size = 0;
				goto __exit_point;//error: can't encode item
			}
			
			done_size += item_enc_size;
			steps_totally += steps;
			
			if (steps_totally > maxsteps)
			{
				done_size = 0;break;//error: program is too big
			}
		};
	}
#endif
__exit_point:
	return(done_size);
}

/* returns: done stream size > 0, error =0. */
int mel_til_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wpgx,int maxsteps)
{
	int done_size = 0;
	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;

	if (enc_size > MEL_WPG_MIN_TIL_SIZE)
	{
		//int is_END_reached = 0;
				
		wpg->magic_num0 = 0x0004;
		wpg->magic_num1 = 0xFFFF;		

		wpg->steps		= 0;
		wpg->nop_steps	= 0;
		wpg->first	= 0;
		wpg->last	= 0;

		while(done_size < enc_size)
		{
			WPG_Item welldone = {0};			
			int item_enc_size = 0;
			int steps = wpg->steps;
			int nop_steps;
			
			unsigned int stcode;
			unsigned int stcodeInstr;
			int firstbad_operand;
			int firstbad_device;
			char line[MEL_WPG_MAX_INSTR_TEXT_SIZE+1] = {0};
			int line_dec_size;

			//get one line from ASCII stream			
			item_enc_size = mel_til_get_line(	stream + done_size,enc_size - done_size,
												line,&line_dec_size,MEL_WPG_MAX_INSTR_TEXT_SIZE);
			done_size += item_enc_size;
						
			//try to construct item from line
			stcode = mel_wpg_text2item(MEL_PLC_SERIES_FXCPU,
												MEL_PLC_FXCPU_TYPE_FX3U_C,
												line,line_dec_size,
												&stcodeInstr,
												&firstbad_operand,
												&firstbad_device,
												&steps,
												&nop_steps,
												&welldone
												);
			if (stcode == ITEM_STATE_OK)
			{
				WPG_Item* item;
				//create new item
				item = calloc(1,sizeof(WPG_Item));
				if (item)
				{
					//setup new item
					memcpy(item,&welldone,sizeof(WPG_Item));

					//add item to linked list:
					if(!mel_wpg_FXCPU_FX3_link_item(wpg,item))
					{
						//error: can't add item to program.
						done_size = 0; break;//exit from loop
					}
					
					/*
					if(		item->itype == WPG_ITEM_TYPE_INSTRUCTION
						&&	((WPG_Instruction*)item->body)->opid== MEL_WPG_OpID_END				
						)
					{
						is_END_reached = 1;
					}
					*/

					if (wpg->steps > maxsteps)
					{
						done_size = 0; break;//exit from loop
					}
				}
				else
				{
					//ITEM_STATE_BAD_NO_MEMORY
					done_size = 0; break;//exit from loop
				}
			}
			else
			{
				//stcode,stcodeInstr
				done_size = 0; break;//exit from loop
			}
		};

		//set rest by NOPs (virtually)
		if (done_size > 0)
		{			
			wpg->nop_steps	+= maxsteps - wpg->steps;
			wpg->steps		+= maxsteps - wpg->steps;
		}
	}

	return(done_size);
}

int	mel_til_FXCPU_FX3_encoder(void* wpgx,unsigned char* stream)
{
	int done_size = 0;
	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;
	WPG_Item* item;	

	item = wpg->first;
	while(item)
	{
		int item_enc_size = 0;
		char* til_line;

		til_line = mel_wpg_item2text(MEL_PLC_SERIES_FXCPU,MEL_PLC_FXCPU_TYPE_FX3U_C,item,&item_enc_size);
		if (item_enc_size <= 0)
		{
			done_size = 0; break;//error: can't encode item
		}
		memcpy(stream + done_size,til_line,item_enc_size);
		stream[done_size + item_enc_size + 0] = 0x0D;
		stream[done_size + item_enc_size + 1] = 0x0A;
		done_size += item_enc_size + 2;

		if(		item->itype == WPG_ITEM_TYPE_INSTRUCTION
			&&	((WPG_Instruction*)item->body)->opid== MEL_WPG_OpID_END				
			)
		{
			break;//exit from loop
		}

		item = item->next;
	};
	return(done_size);
}

/* destroy wpg instance */
void mel_wpg_FXCPU_FX3_free(void* wpgx)
{
	WPG_FXCPU_FX3U* wpg = (WPG_FXCPU_FX3U*)wpgx;

	if (wpg)
	{
		WPG_Item* item;
		
		item = wpg->first;

		while(item)
		{
			WPG_Item* tobedead = item;
			
			if (item->body)
			{
				free(item->body);
			}
			
			item = item->next;
			
			free(tobedead);
		};

		free(wpg);
	}

	return;
}

