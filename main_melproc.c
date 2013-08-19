#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handy_utils.h"
#include "mel_wpa.h"
#include "mel_wcd.h"
#include "mel_wpg.h"
#include "mel_wpg_fxcpu_fx3.h"

#include "mel_wpg_instr.h"

#include "mel_metrix.h"

#pragma warning (disable : 4996)

#define WCD_NAME_SIZEOF 40

int process_wcd(int argc,char** argv);
int process_wpg(int argc,char** argv);
int process_wpa(int argc,char** argv);
int process_til(int argc,char** argv);

int main(int argc,char** argv)
{
	char *ext;
	size_t fname_len;
	
#if 0
	mel_metrix_stats();
	//mel_wpg_printf_opcodes_FXCPU();
	//mel_wpg_generate_metrix_FXCPU();
	mel_wpg_printf_metrix_FXCPU();

	//mel_metrix_generate();	
	//mel_metrix_printf();
	//mel_opnames_printf();
	return(0);
#endif

	if (argc < 2)
	{
		printf("usage: *.exe file_in");
		//				0	 1			 1
		return(1);
	}
	
	fname_len = strlen(argv[1]);
	
	ext = argv[1] + fname_len - 3;

	printf("EXTENSION IS: %s\n\n\n",ext);

	if (!strcmp(ext,"wcd"))
	{
		return process_wcd(argc,argv);
	}

	if (!strcmp(ext,"wpg"))
	{
		return process_wpg(argc,argv);
	}

	if (!strcmp(ext,"til"))
	{
		return process_til(argc,argv);
	}	

	if (!strcmp(ext,"wpa"))
	{
		return process_wpa(argc,argv);
	}

	return 1;
}

int process_wpg(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;

	file_name = argv[1];

	fp_i = fopen(file_name,"rb");
	if (fp_i)
	{
		unsigned char* data = 0;
		int file_size;

		fseek(fp_i,0,SEEK_END);
		file_size = ftell(fp_i);

		fseek(fp_i,0,SEEK_SET);

		data = (unsigned char*)malloc(file_size);
		if (data)
		{
			void* wpg;
			MELTools tools;
			int cpuSeries;
			int cpuType;
			int prgMode;

			fread(data,1,file_size,fp_i);

			wpg = mel_wpg_dec(data,file_size,&cpuSeries,&cpuType,&prgMode,&tools);
			if (wpg)
			{
				unsigned char* buf2;
				int buf2_size;
				int enc_size;
				//int min_size;
				int maxsteps = 16000;//= wpa->page1.mem_capacity_in_steps

				{
					WPG_FXCPU_FX3U* wpg_fx = wpg;

					printf("\tDEC WPG STEPS: %d\tNOPs: %d (%.2f %%)\tUSED: %d steps (%.2f %%)\n",
						wpg_fx->steps,wpg_fx->nop_steps,
						100.0*(double)wpg_fx->nop_steps/(double)wpg_fx->steps,
						wpg_fx->steps - wpg_fx->nop_steps,
						100.0*(double)(wpg_fx->steps-wpg_fx->nop_steps)/(double)wpg_fx->steps
						);
				}

				//////////////////////////////////////////////////////////////////////////
				//CHECK WPG DECODING/ENCODING
				//////////////////////////////////////////////////////////////////////////
				//buf2_size = file_size;
				buf2_size = 64*1024;
				buf2 = malloc(buf2_size);
				memset(buf2,0xDD,buf2_size);

				enc_size = mel_wpg_enc(cpuSeries,cpuType,wpg,buf2,prgMode,&tools,maxsteps);
				{
					FILE* fp_o;

					fp_o = fopen("out.wpg","wb+");
					fwrite(buf2,1,enc_size,fp_o);
					fclose(fp_o);
					fflush(fp_o);
				}
				{
					WPG_FXCPU_FX3U* wpg_fx = wpg;

					printf("\tENC WPG STEPS: %d\tNOPs: %d (%.2f %%)\tUSED: %d steps (%.2f %%)\n",
						wpg_fx->steps,wpg_fx->nop_steps,
						100.0*(double)wpg_fx->nop_steps/(double)wpg_fx->steps,
						wpg_fx->steps - wpg_fx->nop_steps,
						100.0*(double)(wpg_fx->steps-wpg_fx->nop_steps)/(double)wpg_fx->steps
						);
				}

				printf("\n\n\n");

				//min_size = (enc_size < file_size)? enc_size:file_size;
				if (compare_buffers(data,file_size,buf2,enc_size)==0)
				//if (compare_buffers(data,min_size,buf2,min_size)==0)
				{
					printf("WPG -> DAT -> WPG: SUCCESS\n");
				}
				else
				{
					printf("WPG -> DAT -> WPG: DIFFERENT WPGs\n");
				}

				free(buf2);

				//////////////////////////////////////////////////////////////////////////
				//CHECK TIL ENCODING/DECODING
				//////////////////////////////////////////////////////////////////////////
				buf2_size = 640*1024;
				buf2 = malloc(buf2_size);
				memset(buf2,0xDD,buf2_size);

				enc_size = mel_til_enc(cpuSeries,cpuType,wpg,buf2,&tools);
				{
					FILE* fp_o;

					fp_o = fopen("out.til","wb+");
					fwrite(buf2,1,enc_size,fp_o);
					fclose(fp_o);
					fflush(fp_o);
				}								
				{
					void* wpg_til;

					wpg_til = mel_til_dec(buf2,enc_size,&cpuSeries,&cpuType,&tools,maxsteps);
					if (wpg_til)
					{
						unsigned char* buf3;
						int buf3_size;
						int enc_size3;

						buf3_size = 64*1024;
						buf3 = malloc(buf3_size);
						memset(buf3,0xDD,buf3_size);

						enc_size3 = mel_wpg_enc(cpuSeries,cpuType,wpg_til,buf3,prgMode,&tools,maxsteps);
						{
							FILE* fp_o;

							fp_o = fopen("til.wpg","wb+");
							fwrite(buf3,1,enc_size3,fp_o);
							fclose(fp_o);
							fflush(fp_o);
						}
												
						if (compare_buffers(data,file_size,buf3,enc_size3)==0)							
						{
							printf("WPG -> DAT -> TIL -> DAT -> WPG: SUCCESS\n");
						}
						else
						{
							printf("WPG -> DAT -> TIL -> DAT -> WPG: DIFFERENT WPGs\n");
						}

						free(buf3);
						mel_wpg_free(&tools,wpg_til);
					}
					else
					{
						printf("Can't decode TIL completely!\n");
					}
				}
				free(buf2);
				mel_wpg_free(&tools,wpg);
			}
			else
			{
				printf("Can't decode WPG completely! Program Mode: %d\n",prgMode);
			}

			free(data);
		}

		fclose(fp_i);
	}		
	
	return(0);
}

int process_til(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;

	file_name = argv[1];

	fp_i = fopen(file_name,"rb");
	if (fp_i)
	{
		unsigned char* data = 0;
		int file_size;

		fseek(fp_i,0,SEEK_END);
		file_size = ftell(fp_i);

		fseek(fp_i,0,SEEK_SET);

		data = (unsigned char*)malloc(file_size);
		if (data)
		{
			int cpuSeries	= MEL_PLC_SERIES_FXCPU;
			int cpuType		= MEL_PLC_FXCPU_TYPE_FX3U_C;

			void* wpg;
			MELTools tools;

			int maxsteps = 16000;//= wpa->page1.mem_capacity_in_steps

			fread(data,1,file_size,fp_i);

			wpg = mel_til_dec(data,file_size,&cpuSeries,&cpuType,&tools,maxsteps);
			if (wpg)
			{
				unsigned char* buf2;
				int enc_size;
				
				//buf2 = malloc(file_size);
				buf2 = malloc(64*1024);
				memset(buf2,0xDD,file_size);

				{
					WPG_FXCPU_FX3U* wpg_fx = wpg;

					printf("\tDEC TIL STEPS: %d\tNOPs: %d (%.2f %%)\tUSED: %d steps (%.2f %%)\n",
						wpg_fx->steps,wpg_fx->nop_steps,
						100.0*(double)wpg_fx->nop_steps/(double)wpg_fx->steps,
						wpg_fx->steps - wpg_fx->nop_steps,
						100.0*(double)(wpg_fx->steps-wpg_fx->nop_steps)/(double)wpg_fx->steps
						);
				}
				
				enc_size = mel_wpg_enc(cpuSeries,cpuType,wpg,buf2,WPG_RPOGRAMM_MODE_LADDER,&tools,maxsteps);

				{
					FILE* fp_o;

					fp_o = fopen("til.wpg","wb+");
					fwrite(buf2,1,enc_size,fp_o);
					fclose(fp_o);
					fflush(fp_o);
				}

				{
					WPG_FXCPU_FX3U* wpg_fx = wpg;

					printf("\tENC WPG STEPS: %d\tNOPs: %d (%.2f %%)\tUSED: %d steps (%.2f %%)\n",
						wpg_fx->steps,wpg_fx->nop_steps,
						100.0*(double)wpg_fx->nop_steps/(double)wpg_fx->steps,
						wpg_fx->steps - wpg_fx->nop_steps,
						100.0*(double)(wpg_fx->steps-wpg_fx->nop_steps)/(double)wpg_fx->steps
						);
				}
				
				free(buf2);
				mel_wpg_free(&tools,wpg);
			}
			else
			{
				printf("Can't decode TIL completely!\n");
			}


			free(data);
		}

		fclose(fp_i);
	}
	return(0);
}

int process_wpa(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;
	
	int file_size;
		
//	unsigned char header_0[64] = {0};
//	int header_0_size = sizeof(header_0);
	MELTools tools;
	unsigned char *buff = 0;
	void* wpa;
	int cpuSeries;
	int cpuType;
	
	file_name = argv[1];
	
	fp_i = fopen(file_name,"rb");
	fseek(fp_i,0,SEEK_END);
	file_size = ftell(fp_i);
	
	fseek(fp_i,0,SEEK_SET);
	
	buff = malloc(file_size);
	fread(buff,1,file_size,fp_i);
	if (fp_i) fclose(fp_i);

	wpa = mel_wpa_dec(buff,file_size,&cpuSeries,&cpuType,&tools);	
	if(wpa)
	{
		WPA_FXCPU_FX3U* options = (WPA_FXCPU_FX3U*)wpa;		
		unsigned char* buf2;
		int enc_size;
		
		buf2 = malloc(file_size);
		memset(buf2,0xDD,file_size);

		enc_size = mel_wpa_enc(cpuSeries,cpuType,wpa,buf2,&tools);

		if (compare_buffers(buff,file_size,buf2,enc_size)==0)
		{
			printf("SUCCESS\n");
		}
		else
		{
			printf("WPA: ENC != DEC\n");
		}

		free(buf2);
		
#if 0
		int i;

		printf(
			"\n\n\n"
			"%d\toptions->page1.mem_capacity_in_steps\n"
			"%d\toptions->page1.cmt_capacity_in_blocks\n"
			"%d\toptions->page1.freg_capacity_in_blocks\n"
			"%d\toptions->page1.SFBS_in_blocks\n"
			"%d\toptions->page1.PIS_in_blocks\n"
			"%d\toptions->page1.BCC_in_blocks\n"
			,
			options->page1.mem_capacity_in_steps,
			options->page1.cmt_capacity_in_blocks,
			options->page1.freg_capacity_in_blocks,
			options->page1.SFBS_in_blocks,
			options->page1.PIS_in_blocks,
			options->page1.BCC_in_blocks
			);
		
		printf("%d\toptions->page2.n_editable_items\n",options->page2.n_editable_items);

		for (i = 0; i < options->page2.n_editable_items; i++)
		{
			printf("options->page2.editable_items[%d]:\n",i);
			printf(	"\t%04X\t.device_code\n"					
					"\t%4d\t.latched_start\n"
					"\t%4d\t.latched_length\n",
				options->page2.editable_items[i].device_code,				
				options->page2.editable_items[i].latched_start,
				options->page2.editable_items[i].latched_length
				);
		}

		printf("%d\toptions->page4.input_Totally\n"
				"%d\toptions->page4.input_Start_offset\n"
				"%d\toptions->page4.output_Totally\n"
				"%d\toptions->page4.output_Start_offset\n",
			options->page4.input_Totally,
			options->page4.input_Start_offset,
			options->page4.output_Totally,
			options->page4.output_Start_offset
			);

		printf("\n"
			"%d\toptions->page5.is_battery_less_drive_mode\n"
			"%02X\toptions->page5.modem\n"
			"%04X\toptions->page5.terminal\n",
			options->page5.is_battery_less_drive_mode,
			options->page5.modem,
			options->page5.terminal
			);
		
		//options->page6.ch1.is_enabled,
		printf
			("\n"
			"\tCH1 enabled: %d\n"
			"%d\t.protocol\n"
			"%d\t.databits\n"
			"%d\t.parity\n"
			"%d\t.stopbits\n"
			"%d\t.baudrate\n"
			"%d\t.is_header_on\n"
			"%d\t.is_terminator_on\n"
			"%d\t.is_control_line_on\n"
			"%d\t.hw_type\n"
			"%d\t.is_sum_check_on\n"
			"%d\t.transmission_cntrl\n"
			"%d\t.station_number\n"
			"%d\t.timeout_judge_time_x10ms\n\n",
			options->page6.ch1.is_enabled,
			options->page6.ch1.protocol,
			options->page6.ch1.databits,
			options->page6.ch1.parity,
			options->page6.ch1.stopbits,
			options->page6.ch1.baudrate,
			options->page6.ch1.is_header_on,
			options->page6.ch1.is_terminator_on,
			options->page6.ch1.is_control_line_on,
			options->page6.ch1.hw_type,
			options->page6.ch1.is_sum_check_on,
			options->page6.ch1.transmission_cntrl,
			options->page6.ch1.station_number,
			options->page6.ch1.timeout_judge_time_x10ms
			);

		printf
			("\n"
			"\tCH2 enabled: %d\n"
			"%d\t.protocol\n"
			"%d\t.databits\n"
			"%d\t.parity\n"
			"%d\t.stopbits\n"
			"%d\t.baudrate\n"
			"%d\t.is_header_on\n"
			"%d\t.is_terminator_on\n"
			"%d\t.is_control_line_on\n"
			"%d\t.hw_type\n"
			"%d\t.is_sum_check_on\n"
			"%d\t.station_number\n"
			"%d\t.timeout_judge_time_x10ms\n\n",
			options->page6.ch2.is_enabled,
			options->page6.ch2.protocol,
			options->page6.ch2.databits,
			options->page6.ch2.parity,
			options->page6.ch2.stopbits,
			options->page6.ch2.baudrate,
			options->page6.ch2.is_header_on,
			options->page6.ch2.is_terminator_on,
			options->page6.ch2.is_control_line_on,
			options->page6.ch2.hw_type,
			options->page6.ch2.is_sum_check_on,
			options->page6.ch2.transmission_cntrl,
			options->page6.ch2.station_number,			
			options->page6.ch2.timeout_judge_time_x10ms
			);

		if(options->page1.PIS_in_blocks)
		{
			int i,j;

			for (i = 0; i < WPA_FX3U_POS_YTABS_COUNT; i++)
			{
				printf
					("\nY[%d]\n"				
					"%d\t.BiasSpeed_Hz\n"
					"%d\t.MaxSpeed_Hz\n"
					"%d\t.CreepSpeed_Hz\n"
					"%d\t.ZeroReturnSpeed_Hz\n"
					"%d\t.AccelerationTime_ms\n"
					"%d\t.DecelerationTime_ms\n"
					"%d\t.InterrInputSource\n"
					"%d\t.dont_init_postable\n"
					"%02X\t.head_address.type\n"
					"%d\t.head_address.index\n"
					"%02X\t.rotation_dir_signal.type\n"
					"%d\t.rotation_dir_signal.index\n",
					i,
					options->page7.indivs.ytabs[i].BiasSpeed_Hz,
					options->page7.indivs.ytabs[i].MaxSpeed_Hz,
					options->page7.indivs.ytabs[i].CreepSpeed_Hz,
					options->page7.indivs.ytabs[i].ZeroReturnSpeed_Hz,
					options->page7.indivs.ytabs[i].AccelerationTime_ms,
					options->page7.indivs.ytabs[i].DecelerationTime_ms,
					options->page7.indivs.ytabs[i].InterrInputSource,
					options->page7.indivs.ytabs[i].dont_init_postable,
					options->page7.indivs.ytabs[i].head_address.type,
					options->page7.indivs.ytabs[i].head_address.index,
					options->page7.indivs.ytabs[i].rotation_dir_signal.type,
					options->page7.indivs.ytabs[i].rotation_dir_signal.index
					);				

				for (j = 0; j < WPA_FX3U_POS_INSTR_COUNT; j++)
				{
					if (options->page7.indivs.ytabs[i].instructions[j].type != WPA_FX3U_POS_INSTR_TYPE_NONE)
					{
						printf(	"\n\tINSTR[%d]:\n"
								"\t%d\ttype;\n"
								"\t%d\tpulse;\n"
								"\t%d\tfreq;\n",
								j,
								options->page7.indivs.ytabs[i].instructions[j].type,
								options->page7.indivs.ytabs[i].instructions[j].pulse,
								options->page7.indivs.ytabs[i].instructions[j].freq_Hz
							);
					}
				}
			}
		}

		if(options->page1.BCC_in_blocks)
		{
			int i;

			printf(
				"\nBCC:\n"
				"%d\t.baudrate\n"
				"%d\t.point_mode\n",
				options->page8.baudrate,
				options->page8.point_mode
				);

			for (i = 0; i < WPA_FX3U_STATIONS_COUNT; i++)
			{
				if(options->page8.stations[i].type != WPA_FX3U_BuiltInCCLink_LTSetup_STATIONTYPE_None)
				{
					printf(
						"[%d]"
						"\t%d\t.type\n"
						"\t%d\t.iopoints\n"
						"\t%d\t.reserved or not\n",
						i,
						options->page8.stations[i].type,
						options->page8.stations[i].iopoints,
						options->page8.stations[i].is_reserve_st
						);
				}
			}
		}

#endif
		mel_wpa_free(&tools,wpa);
	}
	else
	{
		printf("can't decode MEL wpa-file: unknown PLC\n");
	}
	
	printf(
		"\n"
		"CPU Series:\t0x%02X\n"
		"CPU Type:\t0x%02X\n",			
		cpuSeries,cpuType
		);


	if(buff) free(buff);

	return(0);
}


int process_wcd(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;
	int file_size;
	
	MELTools tools;
	unsigned char *buff = 0;
	void* wcd;
	int cpuSeries;
	int cpuType;	

	file_name = argv[1];
	
	fp_i = fopen(file_name,"rb");
	fseek(fp_i,0,SEEK_END);
	file_size = ftell(fp_i);

	fseek(fp_i,0,SEEK_SET);

	buff = malloc(file_size);
	fread(buff,1,file_size,fp_i);
	if (fp_i) fclose(fp_i);

	wcd = mel_wcd_dec(buff,file_size,&cpuSeries,&cpuType,&tools);	
	if(wcd)
	{
		WCD_FXCPU_FX3U* comments = (WCD_FXCPU_FX3U*)wcd;
		unsigned char* buf2;
		int enc_size;
//		int i;

		buf2 = malloc(file_size);
		memset(buf2,0xDD,file_size);

		enc_size = mel_wcd_enc(cpuSeries,cpuType,wcd,buf2,&tools);
		
		{
			FILE* fp_o;

			fp_o = fopen("out.wcd","wb+");
			fwrite(buf2,1,enc_size,fp_o);
			fclose(fp_o);
			fflush(fp_o);
		}
		
		if (compare_buffers(buff,file_size,buf2,enc_size)==0)
		{
			printf("SUCCESS\n");
		}
		else
		{
			printf("WCD: ENC != DEC\n");
		}

		printf(
			"%d\tcomments->n_codes\n"
			"%d\tcomments->n_names\n",
			comments->n_codes,
			comments->n_names
			);
#if 0
		for (i = 0; i < comments->n_codes; i++)
		{
			printf(
				"%02X\tcomments->codes[i].item.type\n"
				"%d\tcomments->codes[i].item.index\n"
				"%d\tcomments->codes[i].count\n",
				comments->codes[i].item.type,
				comments->codes[i].item.index,
				comments->codes[i].count
				);
		}

		for (i = 0; i < comments->n_names; i++)
		{
			printf("[%48s]\tname[%d]\n",
				comments->names[i].raw,i
				);
		}
#endif


		free(buf2);

		mel_wcd_free(&tools,wcd);
	}
	else
	{
		printf("can't decode MEL wcd-file: unknown PLC\n");
	}

	printf(
		"\n"
		"CPU Series:\t0x%02X\n"
		"CPU Type:\t0x%02X\n",			
		cpuSeries,cpuType
		);
	
	if(buff) free(buff);
	
	if (fp_i)
	{
		fclose(fp_i);
	}

	return(0);
}

#if 0
int process_wcd(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;
	int file_size;
	int offset;

	unsigned char header_0[64] = {0};
	int header_0_size = sizeof(header_0);

	int header_1_size = 10;

	unsigned int unknown_32b_0x42;

	unsigned short unknown_16b_0x46;
	unsigned short unknown_16b_0x48;

	unsigned short body_sizeof;
	unsigned short codes_sizeof;
	unsigned short names_sizeof;

	unsigned char *codes = 0;
	unsigned char *names = 0;

	int one_name_sizeof = WCD_NAME_SIZEOF;
	int n_names;

	file_name = argv[1];

	fp_i = fopen(file_name,"rb");
	fseek(fp_i,0,SEEK_END);
	file_size = ftell(fp_i);

	fseek(fp_i,0,SEEK_SET);

	fread(header_0,1,header_0_size,fp_i);
	fread(&body_sizeof,2,1,fp_i);

	fread(&unknown_32b_0x42,4,1,fp_i);
	fread(&unknown_16b_0x46,2,1,fp_i);
	fread(&unknown_16b_0x48,2,1,fp_i);

	codes_sizeof = body_sizeof - header_1_size;

	printf("file = %d bytes\n",file_size);

	printf("codes_sizeof = %d bytes\n",codes_sizeof);

	printf("unknown_16b_0x46 = %d\n",unknown_16b_0x46);
	printf("unknown_16b_0x48 = %d\n",unknown_16b_0x48);

	if (codes_sizeof > 0)
	{
		codes = (unsigned char*)malloc(codes_sizeof);

		offset = ftell(fp_i);
		printf("codes offset: %d [0x%04x]\n",offset,offset);

		fread(codes,1,codes_sizeof,fp_i);

		{
			int i;
			int n_tmp = 0;

#define CODES_STEP 10
			for (i = 0; i < codes_sizeof; i += CODES_STEP)
			{
				int j;

				unsigned short code_id;
				unsigned int code_first;
				unsigned int code_count;

				memcpy(&code_id,	&codes[i+0],2);
				memcpy(&code_first,	&codes[i+2],4);
				memcpy(&code_count,	&codes[i+6],4);

				printf("%d\t",i/CODES_STEP);
				for (j = 0; j < CODES_STEP; j++)
				{
					printf("%02x ",codes[i + j]);
				}
				printf("\t%04x\t%d\t%d",code_id,code_first,code_count);

				printf("\n");

				n_tmp += code_count;
			}
			//printf("total N = %d\n",n_tmp);
		}

		names_sizeof = file_size - (header_0_size + header_1_size + codes_sizeof);
		names = (unsigned char*)malloc(names_sizeof);

		printf("names_sizeof = %d bytes\n",names_sizeof);

		printf("names / unknown_16b_0x46 = %d\n",names_sizeof/unknown_16b_0x46);
		printf("names / unknown_16b_0x48 = %d\n",names_sizeof/unknown_16b_0x48);

		printf("names / one_name_sizeof = %d\n",names_sizeof/one_name_sizeof);

		offset = ftell(fp_i);

		printf("names offset: %d [0x%04x]\n",offset,offset);
		fread(names,1,names_sizeof,fp_i);

		n_names = names_sizeof/one_name_sizeof;

		//one_name_sizeof
		{
			int i;

			for (i = 0; i < n_names; i++)
			{
				char one_name[WCD_NAME_SIZEOF+1];

				memcpy(one_name,names+i*WCD_NAME_SIZEOF,WCD_NAME_SIZEOF);
				one_name[WCD_NAME_SIZEOF] = 0;

				printf("%d\t[%s]\n",i,one_name);
			}
		}
	}

	if (fp_i)
	{
		fclose(fp_i);
	}

	if(codes) free(codes);
	if(names) free(names);

	return(0);
}

int process_wpg(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;
	
	int file_size;
	int offset;
	
	unsigned char header_0[64] = {0};
	int header_0_size = sizeof(header_0);
	
	unsigned short subcode_0;
	unsigned short subcode_1;
	
	unsigned char* data = 0;
	int data_size;
	
	int wpg_netto = 0;
	int wpg_commnt = 0;
	int wpg_brutto = 0;

	file_name = argv[1];

	fp_i = fopen(file_name,"rb");
	fseek(fp_i,0,SEEK_END);
	file_size = ftell(fp_i);

	fseek(fp_i,0,SEEK_SET);

	fread(header_0,1,header_0_size,fp_i);

	fread(&subcode_0,1,sizeof(unsigned short),fp_i);
	fread(&subcode_1,1,sizeof(unsigned short),fp_i);

	printf("two sub codes: %04x, %04x\n",subcode_0,subcode_1);

	data_size = file_size - (header_0_size + 2*sizeof(unsigned short));
	data = (unsigned char*)malloc(data_size);

	fread(data,1,data_size,fp_i);

	//offset = ftell(fp_i);

	offset = 0;
	while(offset < data_size)
	{		
		//unsigned short marker;
		unsigned char line_size;
		unsigned char line[256];
		
		line_size = data[offset + 0];
		
		if (line_size > 2)
		{
			switch(data[offset + 1])
			{
			case 0x80://Statement
			case 0x82://Note
				
				wpg_commnt += line_size - 2;

				memcpy(line,data + offset + 3,line_size - 4);
				line[line_size - 4] = 0;
				
				if (data[offset + 1] == 0x80)
				{
					printf("\n\n");
				}
				if (data[offset + 1] == 0x82)
				{
					printf("\n\n\t\t\t");
				}
				printf("< %s >\n",line);
				break;
				
			default://Instruction
				
				wpg_netto += line_size - 2;

				memcpy(line,data + offset + 1,line_size - 2);
				
				{
					int i,n;					
					n = line_size - 2;
					
					for (i = 0; i < n; i++)
					{
						printf("%02x ",line[i]);
					}

					printf("\t\t\t");
				}

				{
					int enc_size;
					unsigned char* codes = line;
					unsigned int	code32bit;
					unsigned short	code16bit;

					enc_size = line_size - 2;

					switch(enc_size)
					{
					case 1:

						switch(codes[0])
						{
						case 0x00://LD NO
							printf("LD");
							break;

						case 0x06://OR
							printf("OR");
							break;

						case 0x0c://AND
							printf("AND");
							break;

						case 0x0d://ANI
							printf("ANI");
							break;

						case 0x18://ORB
							printf("ORB");
							break;

						case 0x19://ANB
							printf("ANB");
							break;

						case 0x1A:
							printf("MPS");
							break;
						case 0x1B:
							printf("MRD");
							break;
						case 0x1C:
							printf("MPP");
							break;
							
						case 0x20://OUT
							printf("OUT");
							break;

						case 0x23://SET
							printf("SET");
							break;

						case 0x24://RST
							printf("RST");
							break;

						case 0x33://FEND
							printf("FEND");
							break;

						case 0x34://END
							printf("END");
							break;

						case 0x3c://3c		LABLE SHORT: Pxx
							printf("LABLE (short)");
							break;
						}
						
						break;

					case 2:

						code16bit = (unsigned int)codes[0]<<8;
						code16bit|= (unsigned int)codes[1];

						switch(code16bit)
						{
						case 0x0C02://NO
							printf("NO (long)");
							break;

						case 0x0D02://NC
							printf("NC (long)");
							break;

						case 0x0302://DOWN
							printf("DOWN");
							break;

						case 0x2003://OUT
							printf("OUT (with indexing, FX)");
							break;

						case 0x2103://OUT
							printf("OUT (timer, FX)");
							break;

						case 0x2104://OUT Timer
							printf("OUT (timer, Q02)");
							break;

						case 0x2302://SET long
							printf("SET (long)");
							break;

						case 0x2402://RST Timer
							printf("RST (long)");
							break;

						case 0x3c02://LABLE long
							printf("LABLE (long)");
							break;

						case 0x2C03:
							printf("MC");
							break;
						case 0x3002:
							printf("MCR");
							break;

						default:

							switch(codes[0])
							{
							case MEL_DEV_TYPE_X:
								printf("X%d",codes[1]);
								break;

							case MEL_DEV_TYPE_Y:
								printf("Y%d",codes[1]);
								break;

							case MEL_DEV_TYPE_M:
								printf("M%d",codes[1]);
								break;

							case MEL_DEV_TYPE_D:
								printf("D%d",codes[1]);
								break;

							case MEL_DEV_TYPE_R:
								printf("R%d",codes[1]);
								break;

							case MEL_DEV_TYPE_T:
								printf("T%d",codes[1]);
								break;

							case MEL_DEV_TYPE_ZI:
								printf("Z%d",codes[1]);
								break;
							case MEL_DEV_TYPE_VI:
								printf("V%d",codes[1]);
								break;
							case MEL_DEV_TYPE_bI:
								printf("bit[%d]",codes[1]);
								break;
							
							case MEL_DEV_TYPE_K:
								printf("#K=%d",codes[1]);
								break;

							case MEL_DEV_TYPE_N:
								printf("#N%d",codes[1]);
								break;
							}
						}
						break;

					case 3:
						
						code32bit = (unsigned int)codes[0]<<16;
						code32bit|= (unsigned int)codes[1]<<8;
						code32bit|= (unsigned int)codes[2];

						switch(code32bit)
						{
						case 0x530519://53 05 19	ZRST
							printf("ZRST");
							break;

						case 0x4a0300://4a 03 00	INC
							printf("INC");
							break;

						case 0x4a0302://4a 03 02		DEC
							printf("DEC");
							break;

						case 0x4b050f://4b 05 0f		DNEG
							printf("DNEG");
							break;

						case 0x4c0500://4c 05 00	MOV
							printf("MOV");
							break;

						case 0x4c0706://4c 07 06	BMOV
							printf("BMOV");
							break;
							
						case 0x4c0901://4c 09 01	DMOV
							printf("DMOV");
							break;

						case 0x4c090f://4c 09 0f	DEMOV
							printf("DEMOV");
							break;

						case 0x4d0300://4d 03 00	CJ
							printf("CJ");
							break;
														
						case 0x510500://51 05 00	SFR
							printf("SFR");
							break;

						case 0x510501://51 05 01    SFL
							printf("SFL");
							break;

						case 0x540301://54 03 01 	CALL
							printf("CALL");
							break;
						
						case 0x490728://49 07 28	ADD
							printf("ADD");
							break;

						case 0x49072c://49 07 2c		MUL
							printf("MUL");
							break;

						case 0x490d2b://49 0d 2b		DSUB
							printf("DSUB");
							break;

						case 0x490d2d://49 0d 2d		DMUL
							printf("DMUL");
							break;

						case 0x690101://EI
							printf("EI");
							break;

						case 0x690103://IRET
							printf("IRET");
							break;

						case 0x6a0101://6a 01 01		NEXT
							printf("NEXT");
							break;

						case 0x6a0107://6a 01 07		SRET
							printf("SRET\n---------------------------------------\n");
							break;

						case 0x6a0300://6a 03 00		FOR
							printf("FOR");
							break;

						case 0x560900://56 09 00	FROM
							printf("FROM");
							break;

						case 0x560902://56 09 02	TO
							printf("TO");
							break;

						case 0x5f0b1d://5f 0b 1d	RS2
							printf("RS2");
							break;

						case 0x4f0701://4f 07 01		WAND
							printf("WAND");
							break;

						case 0x4f0703://4f 07 03		WOR
							printf("WOR");
							break;

						case 0x4f0d09://4f 0d 09	DAND
							printf("DAND");
							break;

						case 0x4f0d0b://4f 0d 0b		DOR
							printf("DOR");
							break;
						}

						break;


					case 4:
						
						code32bit = (unsigned int)codes[0]<<24;
						code32bit|= (unsigned int)codes[1]<<16;
						code32bit|= (unsigned int)codes[2]<<8;
						code32bit|= (unsigned int)codes[3];

						switch(code32bit)
						{
						case 0x40050111://40 05 01 11	<>
							printf("<>");
							break;

						case 0x40050011://40 05 00 11	==
							printf("==");
							break;

						case 0x40090611://40 09 06 11	D=
							printf("D=");
							break;

						case 0x40090811://40 09 08 11	D>
							printf("D>");
							break;
														
						case 0x40090a11://40 09 0a 11	D<
							printf("D<");
							break;							
						}

						break;
					case 5:
						if (codes[0]==MEL_DEV_TYPE_E)
						{
							union{
								unsigned int	tint;
								float			tflt;
							}tmp;

							tmp.tint =		((unsigned int)codes[4]<<24)
										|	((unsigned int)codes[3]<<16)
										|	((unsigned int)codes[2]<<8)
										|	((unsigned int)codes[1]);

							printf("#E=%f",tmp.tflt);
						}
						break;
					case 6:
						break;
					case 7:
						break;
					}
				}

				printf("\n");
			}
		}
		
		offset += line_size;
		
/*
		marker = data[offset + 0];
		marker|= (unsigned short)data[offset + 1]<<8;
		
		switch(marker)
		{
		case 0x0080://Statement
			
			comm_size = data[offset - 1];
			memcpy(comm,data + offset + 2,comm_size - 4);
			comm[comm_size - 4] = 0;

			printf("\n\n80 comm: %s\n",comm);			
			offset += comm_size - 1;			
			break;

		case 0x0082://Note

			comm_size = data[offset - 1];
			memcpy(comm,data + offset + 2,comm_size - 4);
			comm[comm_size - 4] = 0;

			printf("\n\n82 comm: %s\n",comm);
			offset += comm_size - 1;			

			break;

		default:
			
			printf("%02x ",data[offset + 0]);
			
			offset++;
		}
*/
	};
	
	if(data) free(data);
	
	if (fp_i)
	{
		fclose(fp_i);
	}
	
	wpg_brutto = file_size;

	printf("wpg netto: %d bytes (%x)\n",	wpg_netto,wpg_netto);
	printf("wpg cmnts: %d bytes (%x)\n",	wpg_commnt,wpg_commnt);
	printf("wpg brutto: %d bytes (%x)\n",	wpg_brutto,wpg_brutto);

	return(0);
}


#endif 
