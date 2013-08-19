#include "mel_wpa.h"
#include <string.h>//memset
#include <stdlib.h>//free

#define WPA_CODE_FXCPU_FX3U_FREG_CAPACITY		0x0011
#define WPA_CODE_FXCPU_FX3U_ADDS_CAPACITY		0xE7FF
#define WPA_CODE_FXCPU_FX3U_UNKNOWN1			0xE4FF
#define WPA_CODE_FXCPU_FX3U_PLCSYSTEM1			0xEAFF
#define WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH1		0xEBFF
#define WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH2		0xECFF
#define WPA_CODE_FXCPU_FX3U_UNKNOWN2			0xEDFF
#define WPA_CODE_FXCPU_FX3U_POSITIONING			0xEEFF
#define WPA_CODE_FXCPU_FX3U_IOASSIGMENT			0x0040
#define WPA_CODE_FXCPU_FX3U_BCC					0xEFFF

static int mel_wpa_FXCPU_FX3U_segment_decoder(int segcode,int segsize,unsigned char* segment,WPA_FXCPU_FX3U* wpa);
static int mel_wpa_FXCPU_FX3U_segment_encoder(int* psegcode,WPA_FXCPU_FX3U* wpa,unsigned char* segment);

int mel_wpa_FXCPU_FX3_decoder(unsigned char* stream,int enc_size,void* wpax)
{
	int done_size = 0;
	WPA_FXCPU_FX3U* wpa = (WPA_FXCPU_FX3U*)wpax;
	int goon = 1;

	while(goon && done_size + 2 <= enc_size)
	{
		unsigned char* segment;
		int code;
				
		segment = stream + done_size;
		code = (int)stream[done_size+0]|((int)stream[done_size+1]<<8);
		
		//printf("code: %04X\t",code);
		
		done_size += 2;

		switch (code)
		{
		case WPA_CODE_EOF:
			done_size = enc_size;//successfully done
			goon = 0;//exit from loop
			break;

		default://treat code as a length of segment
			{
				int segsize = code;

				//printf("size: %4d\t",segsize);

				if (segsize >= 4 && done_size - 2 + segsize <= enc_size)
				{	
					int segsize_done;

					code = (int)stream[done_size+1]|((int)stream[done_size+0]<<8);
					
					//printf("\t%04X\n",code);

					segsize_done = mel_wpa_FXCPU_FX3U_segment_decoder(code,segsize,segment,wpa);
					/*
					if (segsize_done != segsize)
					{
						done_size = -segsize + 2;// -segsize + segsize = 0; (error)
						goon = 0;//exit from loop
					}
					*/
#if 0
					if (segsize_done != segsize)
					{
						int i;
						printf("\tunknown segment!\n\t\t\t");

						for (i = 0; i < segsize; i++)
						{
							printf("%02X ",segment[i]);
						}
						printf("\n");
					}
#endif
				}
				//step over done segment
				done_size += segsize - 2;
			}
		}
	};
	
	return(done_size);
}

static int mel_wpa_FXCPU_FX3U_segment_decoder(int segcode,int segsize,unsigned char* segment,WPA_FXCPU_FX3U* wpa)
{
	int encsize = segsize;//Ok by default
	
	switch (segcode)
	{
	case WPA_CODE_MEMORY_CAPACITY:
		//Memory capacity in steps; encoded in thousands
		wpa->page1.mem_capacity_in_steps = ((int)segment[4]|((int)segment[5]<<8))*1000;
		break;

	case WPA_CODE_CMNT_CAPACITY:
		//Comments capacity in blocks (0 to (Mem.capacity/500) - 1); 1 block = 50 points;
		wpa->page1.cmt_capacity_in_blocks = (int)segment[4]|((int)segment[5]<<8);
		break;

	case WPA_CODE_FXCPU_FX3U_FREG_CAPACITY:
		/*
		Mem.capacity	FileRegMaxBlocks
		2000			3
		4000			7
		8000			14
		16000			14
		32000			14
		64000			14
		*/
		wpa->page1.freg_capacity_in_blocks = (int)segment[16]|((int)segment[17]<<8);
		break;

	case WPA_CODE_FXCPU_FX3U_ADDS_CAPACITY:
		{
			int adds_in_blocks = (int)segment[6]|((int)segment[7]<<8);

			wpa->page1.BCC_in_blocks = adds_in_blocks & 1;
			adds_in_blocks -= wpa->page1.BCC_in_blocks;

			if (adds_in_blocks >= 18)
			{
				wpa->page1.PIS_in_blocks	= 18;//PIS in blocks (0=no or 18=yes)
			}
			else
			{
				wpa->page1.PIS_in_blocks	= 0;//PIS in blocks (0=no or 18=yes)
			}
			adds_in_blocks -= wpa->page1.PIS_in_blocks;

			wpa->page1.SFBS_in_blocks = (adds_in_blocks == 8)? 8:0;

		}
		break;

	case WPA_CODE_DEVICES:
		wpa->page2.n_editable_items = (int)segment[4]|((int)segment[5]<<8);
		{
			int i;
			for (i = 0; i < wpa->page2.n_editable_items && i < WPA_FX3U_MAX_EDITABLE_ITEMS; i++)
			{
				wpa->page2.editable_items[i].device_code	= (int)segment[6*i+6+0]|((int)segment[6*i+6+1]<<8);
				wpa->page2.editable_items[i].latched_length = (int)segment[6*i+6+2]|((int)segment[6*i+6+3]<<8);
				wpa->page2.editable_items[i].latched_start	= (int)segment[6*i+6+4]|((int)segment[6*i+6+5]<<8);
			}
		}
		break;

	case WPA_CODE_PLCNAME:
		//ASCII name of PLC (max: 32 bytes); by default: 33 0x20 + ending null 0x00 = 34 bytes totally		
		memcpy(wpa->page3.raw,segment+4,WPA_FX3U_PLCNAME_MAX_BYTES+2);
		break;

	case WPA_CODE_FXCPU_FX3U_IOASSIGMENT:
		wpa->page4.input_Totally		= (int)segment[12]|((int)segment[13]<<8);
		wpa->page4.input_Start_offset	= (int)segment[14]|((int)segment[15]<<8);
		wpa->page4.output_Totally		= (int)segment[34]|((int)segment[35]<<8);
		wpa->page4.input_Start_offset	= (int)segment[36]|((int)segment[37]<<8);
		break;

	case WPA_CODE_FXCPU_FX3U_PLCSYSTEM1:
		wpa->page5.is_battery_less_drive_mode	= segment[4];
		wpa->page5.modem						= segment[5];
		wpa->page5.terminal						= ((int)segment[6]<<8)|(int)segment[7];
		break;

	case WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH1:
		wpa->page6.ch1.protocol = segment[4];
		wpa->page6.ch1.databits = segment[5];
		wpa->page6.ch1.parity	= segment[6];
		wpa->page6.ch1.stopbits = segment[7];
		wpa->page6.ch1.baudrate = segment[8];
		wpa->page6.ch1.is_header_on			= segment[9];
		wpa->page6.ch1.is_terminator_on		= segment[10];
		wpa->page6.ch1.is_control_line_on	= segment[11];
		{
			int hw_type = ((int)segment[12]<<8)|(segment[13]);
			switch(hw_type)
			{
			case 0x0000:
				wpa->page6.ch1.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_RegularRS232C;
				break;
			case 0x0101:
				wpa->page6.ch1.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_RS485;
				break;
			case 0x0100:
				if(wpa->page6.ch1.is_control_line_on==0)
				{
					wpa->page6.ch1.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_InterLinkRS232C;
				}
				else
				{
					wpa->page6.ch1.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_ModemRS232C;
				}
			}
		}		
		wpa->page6.ch1.is_sum_check_on		= segment[14];
		wpa->page6.ch1.transmission_cntrl	= segment[15];
		wpa->page6.ch1.station_number		= segment[16];
		wpa->page6.ch1.timeout_judge_time_x10ms = segment[17];
		wpa->page6.ch1.is_enabled			= (wpa->page6.ch1.baudrate!=WPA_FX3U_PLCSYSTEM2_BAUDRATE_CH_DESABLED)? 1:0;
		break;

	case WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH2:
		//Channel index = 1 {segment[4] = 0x01; segment[5] = 0x00;}
		wpa->page6.ch2.protocol				= segment[6];
		wpa->page6.ch2.databits				= segment[7];
		wpa->page6.ch2.parity				= segment[8];
		wpa->page6.ch2.stopbits				= segment[9];
		wpa->page6.ch2.baudrate				= segment[10];
		wpa->page6.ch2.is_header_on			= segment[11];
		wpa->page6.ch2.is_terminator_on		= segment[12];
		wpa->page6.ch2.is_control_line_on	= segment[13];
		{
			int hw_type = ((int)segment[14]<<8)|(int)segment[15];
			switch(hw_type)
			{
			case 0x0000:
				wpa->page6.ch2.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_RegularRS232C;
				break;
			case 0x0101:
				wpa->page6.ch2.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_RS485;
				break;
			case 0x0100:				
				if(wpa->page6.ch2.is_control_line_on==0)
				{
					wpa->page6.ch2.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_InterLinkRS232C;
				}
				else
				{
					wpa->page6.ch2.hw_type = WPA_FX3U_PLCSYSTEM2_CNTRLINE_ModemRS232C;
				}
			}
		}
		wpa->page6.ch2.is_sum_check_on		= segment[16];
		wpa->page6.ch2.transmission_cntrl	= segment[17];		
		wpa->page6.ch2.station_number		= segment[18];
		wpa->page6.ch2.timeout_judge_time_x10ms = (int)segment[19]|((int)segment[20]<<8);
		//segment[21] = 0x00;
		wpa->page6.ch2.is_enabled			= (wpa->page6.ch2.baudrate!=WPA_FX3U_PLCSYSTEM2_BAUDRATE_CH_DESABLED)? 1:0;
		break;

	case WPA_CODE_FXCPU_FX3U_POSITIONING:
		if (segsize > 4)
		{
			int i;
			int first_tab = 4;

			for (i = 0; i < WPA_FX3U_POS_YTABS_COUNT; i++)
			{
				int j;
				int i_tab = 4 + i*(28+WPA_FX3U_POS_INSTR_COUNT*10);
				int first_instr;

				wpa->page7.indivs.ytabs[i].BiasSpeed_Hz			= (int)segment[i_tab+0]|((int)segment[i_tab+1]<<8);
				wpa->page7.indivs.ytabs[i].MaxSpeed_Hz			= (int)segment[i_tab+2]|((int)segment[i_tab+3]<<8)|((int)segment[i_tab+4]<<16)|((int)segment[i_tab+5]<<24);
				wpa->page7.indivs.ytabs[i].CreepSpeed_Hz		= (int)segment[i_tab+6]|((int)segment[i_tab+7]<<8);
				wpa->page7.indivs.ytabs[i].ZeroReturnSpeed_Hz	= (int)segment[i_tab+8]|((int)segment[i_tab+9]<<8)|((int)segment[i_tab+10]<<16)|((int)segment[i_tab+11]<<24);				
				wpa->page7.indivs.ytabs[i].AccelerationTime_ms	= (int)segment[i_tab+12]|((int)segment[i_tab+13]<<8);
				wpa->page7.indivs.ytabs[i].DecelerationTime_ms	= (int)segment[i_tab+14]|((int)segment[i_tab+15]<<8);
				wpa->page7.indivs.ytabs[i].InterrInputSource	= (int)segment[i_tab+16]|((int)segment[i_tab+17]<<8);				
				wpa->page7.indivs.ytabs[i].dont_init_postable	= (int)segment[i_tab+18]|((int)segment[i_tab+19]<<8);
				wpa->page7.indivs.ytabs[i].head_address.dtype	= (unsigned int)segment[i_tab+20]|((unsigned int)segment[i_tab+21]<<8);
				wpa->page7.indivs.ytabs[i].head_address.index	= (int)segment[i_tab+22]|((int)segment[i_tab+23]<<8);
				wpa->page7.indivs.ytabs[i].rotation_dir_signal.dtype = (unsigned int)segment[i_tab+24]|((unsigned int)segment[i_tab+25]<<8);
				wpa->page7.indivs.ytabs[i].rotation_dir_signal.index= (int)segment[i_tab+26]|((int)segment[i_tab+27]<<8);
				
				first_instr = i_tab + 28;
				
				for (j = 0; j < WPA_FX3U_POS_INSTR_COUNT; j++)
				{
					int j_instr = first_instr + 10*j;

					wpa->page7.indivs.ytabs[i].instructions[j].type		= (int)segment[j_instr + 0]|((int)segment[j_instr + 1]<<8);
					wpa->page7.indivs.ytabs[i].instructions[j].pulse	= (int)segment[j_instr + 2]|((int)segment[j_instr + 3]<<8)|((int)segment[j_instr + 4]<<16)|((int)segment[j_instr + 5]<<24);
					wpa->page7.indivs.ytabs[i].instructions[j].freq_Hz	= (int)segment[j_instr + 6]|((int)segment[j_instr + 7]<<8)|((int)segment[j_instr + 8]<<16)|((int)segment[j_instr + 9]<<24);
				}
			}
		}
		//else{/* empty segment */}
		break;

	case WPA_CODE_FXCPU_FX3U_BCC:
		wpa->page8.baudrate		= segment[4];
		wpa->page8.point_mode	= segment[5];
		/* segment[6] = 0x00; */
		/* segment[7] = 0x01; */
		{
			int i;
			
			for (i = 0; i < WPA_FX3U_STATIONS_COUNT; i++)
			{
				wpa->page8.stations[i].type				= segment[8+i*4+0];
				wpa->page8.stations[i].iopoints			= segment[8+i*4+1];
				wpa->page8.stations[i].is_reserve_st	= segment[8+i*4+2];
				/* segment[8+i*4+3] = 0x01 */
			}
		}
		break;

	case WPA_CODE_FXCPU_FX3U_UNKNOWN1:
		//0C 00 [E4 FF] {20 20 20 20 20 20 20 20}
	case WPA_CODE_FXCPU_FX3U_UNKNOWN2:
		//26 00 [ED FF] [08 00] {04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00}
	default:
		encsize = 0;
	}

	return(encsize);
}

int	mel_wpa_FXCPU_FX3_encoder(void* wpax,unsigned char* stream)
{
	int done_size = 0;
	WPA_FXCPU_FX3U* wpa = (WPA_FXCPU_FX3U*)wpax;
	int segcode;
	int goon = 1;

	/*
	size:    6		E0FF
	size:    6		16FF
	size:   18		0011
	size:    8		E7FF
	size:   36		0120
	size:   38		0100
	size:   12		E4FF
	unknown segment!
	size:    8		EAFF
	size:   18		EBFF
	size:   22		ECFF
	size:   38		EDFF
	unknown segment!
	size: 4116		EEFF
	size:  264		EFFF
	size:   54		0040
	*/

	segcode = WPA_CODE_MEMORY_CAPACITY;

	do 
	{
		unsigned char* segment = stream + done_size;

		switch (segcode)
		{
		case WPA_CODE_EOF:
			//encode ending code
			segment[0] = (WPA_CODE_EOF>>8)&0xFF;
			segment[1] = WPA_CODE_EOF&0xFF;
			done_size += 2;
			//and exit from loop gracefully
			goon = 0;
			break;

		default:
			{
				int segsize;
								
				segsize = mel_wpa_FXCPU_FX3U_segment_encoder(&segcode,wpa,segment);
				done_size += segsize;
			}
			break;
		}

	} while (goon);

	return(done_size);
}

static int mel_wpa_FXCPU_FX3U_segment_encoder(int* psegcode,WPA_FXCPU_FX3U* wpa,unsigned char* segment)
{
	int segsize = 0;
	int segcode = *psegcode;
	int nextcode = WPA_CODE_EOF;
		
	switch(segcode)
	{
	case WPA_CODE_MEMORY_CAPACITY:
		segsize = 6;
		{
			int temp = wpa->page1.mem_capacity_in_steps/1000;

			//Memory capacity in steps; encoded as 16-bit = in thousands
			segment[4] = temp&0xFF;
			segment[5] = (temp>>8)&0xFF;
		}
		nextcode = WPA_CODE_CMNT_CAPACITY;
		break;

	case WPA_CODE_CMNT_CAPACITY:
		segsize = 6;
		{
			int temp = wpa->page1.cmt_capacity_in_blocks;
			//Comments capacity in blocks (0 to (Mem.capacity/500) - 1); 1 block = 50 points;
			segment[4] = temp&0xFF;
			segment[5] = (temp>>8)&0xFF;
		}
		nextcode = WPA_CODE_FXCPU_FX3U_FREG_CAPACITY;
		break;

	case WPA_CODE_FXCPU_FX3U_FREG_CAPACITY:
		segsize = 18;
		{
			int temp = wpa->page1.freg_capacity_in_blocks;

			memset(segment+4,0x00,segsize-4-2);
			segment[16] = temp&0xFF;
			segment[17] = (temp>>8)&0xFF;
		}
		nextcode = WPA_CODE_FXCPU_FX3U_ADDS_CAPACITY;
		break;

	case WPA_CODE_FXCPU_FX3U_ADDS_CAPACITY:
		segsize = 8;
		{
			int temp;

			temp =  wpa->page1.BCC_in_blocks 
					+ wpa->page1.PIS_in_blocks 
					+ wpa->page1.SFBS_in_blocks;

			segment[4] = 0x00;
			segment[5] = 0x00;
			segment[6] = temp&0xFF;
			segment[7] = (temp>>8)&0xFF;
		}
		nextcode = WPA_CODE_DEVICES;
		break;

	case WPA_CODE_DEVICES:
		segsize = 36;
		{
			int temp = wpa->page2.n_editable_items;
			int i;

			segment[4] = temp&0xFF;
			segment[5] = (temp>>8)&0xFF;

			for (i = 0; i < wpa->page2.n_editable_items && i < WPA_FX3U_MAX_EDITABLE_ITEMS; i++)
			{
				int temp1;
				int temp2;
				int temp3;

				temp1 = wpa->page2.editable_items[i].device_code;
				segment[6*i+6+0] = temp1&0xFF;
				segment[6*i+6+1] = (temp1>>8)&0xFF;

				temp2 = wpa->page2.editable_items[i].latched_length;
				segment[6*i+6+2] = temp2&0xFF;
				segment[6*i+6+3] = (temp2>>8)&0xFF;

				temp3 = wpa->page2.editable_items[i].latched_start;
				segment[6*i+6+4] = temp3&0xFF;
				segment[6*i+6+5] = (temp3>>8)&0xFF;;
			}
		}
		nextcode = WPA_CODE_PLCNAME;
		break;

	case WPA_CODE_PLCNAME:
		segsize = 38;		
		//ASCII name of PLC (max: 32 bytes); by default: 33 0x20 + ending null 0x00 = 34 bytes totally		
		memcpy(segment+4,wpa->page3.raw,WPA_FX3U_PLCNAME_MAX_BYTES+2);
		nextcode = WPA_CODE_FXCPU_FX3U_UNKNOWN1;
		break;

	case WPA_CODE_FXCPU_FX3U_UNKNOWN1:
		segsize = 12;
		//0C 00 [E4 FF] {20 20 20 20 20 20 20 20}
		memset(segment+4,0x20,segsize-4);
		nextcode = WPA_CODE_FXCPU_FX3U_PLCSYSTEM1;
		break;

	case WPA_CODE_FXCPU_FX3U_PLCSYSTEM1:
		segsize = 8;
		segment[4] = wpa->page5.is_battery_less_drive_mode;
		segment[5] = wpa->page5.modem;
		{
			int temp = wpa->page5.terminal;
			segment[6] = temp&0xFF;
			segment[7] = (temp>>8)&0xFF;
		}
		nextcode = WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH1;
		break;

	case WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH1:
		segsize = 18;

		if (wpa->page6.ch1.is_enabled)
		{
			segment[4] = wpa->page6.ch1.protocol;
			segment[5] = wpa->page6.ch1.databits;
			segment[6] = wpa->page6.ch1.parity;
			segment[7] = wpa->page6.ch1.stopbits;
			segment[8] = wpa->page6.ch1.baudrate;
			segment[9] = wpa->page6.ch1.is_header_on;
			segment[10] = wpa->page6.ch1.is_terminator_on;
			segment[11] = wpa->page6.ch1.is_control_line_on;
			{
				int temp = 0;
				switch(wpa->page6.ch1.hw_type)
				{
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_RegularRS232C:
					//0x0000
					break;
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_RS485:
					temp = 0x0101;
					break;
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_InterLinkRS232C:
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_ModemRS232C:
					temp = 0x0100;
					break;
				}
				segment[12] = (temp>>8)&0xFF;
				segment[13] = temp&0xFF;
			}
			segment[14] = wpa->page6.ch1.is_sum_check_on;
			segment[15] = wpa->page6.ch1.transmission_cntrl;
			segment[16] = wpa->page6.ch1.station_number;
			segment[17] = wpa->page6.ch1.timeout_judge_time_x10ms;
		}
		else
		{
			memset(segment+4,0x00,segsize-4);
		}
				
		nextcode = WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH2;
		break;

	case WPA_CODE_FXCPU_FX3U_PLCSYSTEM2_CH2:
		segsize = 22;
		
		//Channel index = 1
		segment[4] = 0x01; segment[5] = 0x00;

		if(wpa->page6.ch2.is_enabled)
		{
			segment[6] = wpa->page6.ch2.protocol;
			segment[7] = wpa->page6.ch2.databits;
			segment[8] = wpa->page6.ch2.parity;
			segment[9] = wpa->page6.ch2.stopbits;
			segment[10] = wpa->page6.ch2.baudrate;
			segment[11] = wpa->page6.ch2.is_header_on;
			segment[12] = wpa->page6.ch2.is_terminator_on;
			segment[13] = wpa->page6.ch2.is_control_line_on;
			{
				int temp = 0;
				switch(wpa->page6.ch2.hw_type)
				{
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_RegularRS232C:
					//0x0000
					break;
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_RS485:
					temp = 0x0101;
					break;
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_InterLinkRS232C:
				case WPA_FX3U_PLCSYSTEM2_CNTRLINE_ModemRS232C:
					temp = 0x0100;
					break;
				}
				segment[14] = (temp>>8)&0xFF;
				segment[15] = temp&0xFF;
			}
			segment[16] = wpa->page6.ch2.is_sum_check_on;
			segment[17] = wpa->page6.ch2.transmission_cntrl;
			segment[18] = wpa->page6.ch2.station_number;
			{
				int temp  = wpa->page6.ch2.timeout_judge_time_x10ms;
				segment[19] = temp&0xFF;
				segment[20] = (temp>>8)&0xFF;
			}
			segment[21] = 0x00;
		}
		else
		{			
			memset(segment+6,0x00,segsize-6);
		}
		nextcode = WPA_CODE_FXCPU_FX3U_UNKNOWN2;
		break;

	case WPA_CODE_FXCPU_FX3U_UNKNOWN2:		
		//26 00 [ED FF] [08 00] {04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00, 04 00 00 00}
		{
			int temp = 8;
			int i;
			segment[4] = temp&0xFF;
			segment[5] = (temp>>8)&0xFF;
			memset(segment+6,0x00,4*temp);
			for (i = 0; i < temp; i++)
			{				
				segment[6+4*i+0] = 0x04;
			}
			segsize  = 6 + 4*temp;
		}
		nextcode = WPA_CODE_FXCPU_FX3U_POSITIONING;
		break;

	case WPA_CODE_FXCPU_FX3U_POSITIONING:
		if (wpa->page1.PIS_in_blocks > 0)
		{			
			int i;
			int first_tab = 4;

			segsize = 4116;

			for (i = 0; i < WPA_FX3U_POS_YTABS_COUNT; i++)
			{
				int j;
				int i_tab = 4 + i*(28+WPA_FX3U_POS_INSTR_COUNT*10);
				int first_instr;

				int temp1;
				int temp2;
				int temp3;
				int temp4;
				unsigned int utemp;

				temp1 = wpa->page7.indivs.ytabs[i].BiasSpeed_Hz;				
				segment[i_tab+0] = temp1&0xFF;
				segment[i_tab+1] = (temp1>>8)&0xFF;

				temp2 = wpa->page7.indivs.ytabs[i].MaxSpeed_Hz;
				segment[i_tab+2] = temp2&0xFF;
				segment[i_tab+3] = (temp2>>8)&0xFF;
				segment[i_tab+4] = (temp2>>16)&0xFF;
				segment[i_tab+5] = (temp2>>24)&0xFF;
								
				temp3 = wpa->page7.indivs.ytabs[i].CreepSpeed_Hz;
				segment[i_tab+6] = temp3&0xFF;
				segment[i_tab+7] = (temp3>>8)&0xFF;

				temp4 = wpa->page7.indivs.ytabs[i].ZeroReturnSpeed_Hz;
				segment[i_tab+8] = temp4&0xFF;
				segment[i_tab+9] = (temp4>>8)&0xFF;
				segment[i_tab+10] = (temp4>>16)&0xFF;
				segment[i_tab+11] = (temp4>>24)&0xFF;

				temp1 = wpa->page7.indivs.ytabs[i].AccelerationTime_ms;
				segment[i_tab+12] = temp1&0xFF;
				segment[i_tab+13] = (temp1>>8)&0xFF;
				
				temp2 = wpa->page7.indivs.ytabs[i].DecelerationTime_ms;
				segment[i_tab+14] = temp2&0xFF;
				segment[i_tab+15] = (temp2>>8)&0xFF;
				
				temp3 = wpa->page7.indivs.ytabs[i].InterrInputSource;
				segment[i_tab+16] = temp3&0xFF;
				segment[i_tab+17] = (temp3>>8)&0xFF;
				
				temp4 = wpa->page7.indivs.ytabs[i].dont_init_postable;
				segment[i_tab+18] = temp4&0xFF;
				segment[i_tab+19] = (temp4>>8)&0xFF;
				
				utemp = wpa->page7.indivs.ytabs[i].head_address.dtype;
				segment[i_tab+20] = utemp&0xFF;
				segment[i_tab+21] = (utemp>>8)&0xFF;
				
				temp2 = wpa->page7.indivs.ytabs[i].head_address.index;
				segment[i_tab+22] = temp2&0xFF;
				segment[i_tab+23] = (temp2>>8)&0xFF;
								
				utemp = wpa->page7.indivs.ytabs[i].rotation_dir_signal.dtype;
				segment[i_tab+24] = utemp&0xFF;
				segment[i_tab+25] = (utemp>>8)&0xFF;

				temp4 = wpa->page7.indivs.ytabs[i].rotation_dir_signal.index;
				segment[i_tab+26] = temp4&0xFF;
				segment[i_tab+27] = (temp4>>8)&0xFF;

				first_instr = i_tab + 28;

				for (j = 0; j < WPA_FX3U_POS_INSTR_COUNT; j++)
				{
					int j_instr = first_instr + 10*j;

					temp1 = wpa->page7.indivs.ytabs[i].instructions[j].type;
					segment[j_instr + 0] =  temp1&0xFF;
					segment[j_instr + 1] = (temp1>>8)&0xFF;

					temp2 = wpa->page7.indivs.ytabs[i].instructions[j].pulse;
					segment[j_instr + 2] =  temp2&0xFF;
					segment[j_instr + 3] = (temp2>>8)&0xFF;
					segment[j_instr + 4] = (temp2>>16)&0xFF;
					segment[j_instr + 5] = (temp2>>24)&0xFF;
					
					temp3 = wpa->page7.indivs.ytabs[i].instructions[j].freq_Hz;					
					segment[j_instr + 6] =  temp3&0xFF;
					segment[j_instr + 7] = (temp3>>8)&0xFF;
					segment[j_instr + 8] = (temp3>>16)&0xFF;
					segment[j_instr + 9] = (temp3>>24)&0xFF;
				}
			}
		}
		else
		{
			segsize = 4;
		}

		//else{/* empty segment */}
		if (wpa->page1.BCC_in_blocks > 0)
		{
			nextcode = WPA_CODE_FXCPU_FX3U_BCC;
		}else
		{
			nextcode = WPA_CODE_FXCPU_FX3U_IOASSIGMENT;
		}
		break;

	case WPA_CODE_FXCPU_FX3U_BCC:
		segsize = 264;

		segment[4] = wpa->page8.baudrate;
		segment[5] = wpa->page8.point_mode;
		segment[6] = 0x00;
		segment[7] = 0x01;
		{
			int i;
			
			for (i = 0; i < WPA_FX3U_STATIONS_COUNT; i++)
			{
				segment[8+i*4+0] = wpa->page8.stations[i].type;
				segment[8+i*4+1] = wpa->page8.stations[i].iopoints;
				segment[8+i*4+2] = wpa->page8.stations[i].is_reserve_st;
				segment[8+i*4+3] = 0x01;
			}
		}
		
		nextcode = WPA_CODE_FXCPU_FX3U_IOASSIGMENT;
		break;

	case WPA_CODE_FXCPU_FX3U_IOASSIGMENT:
		segsize = 54;
		segment[4] = 0x02;
		segment[5] = 0x00;
		segment[6] = 0x00;
		segment[7] = 0x00;
		segment[8] = 0x00;
		segment[9] = 0x00;
		segment[10] = 0x00;
		segment[11] = 0x02;
		{
			int temp = wpa->page4.input_Totally;
			segment[12] = temp&0xFF;
			segment[13] = (temp>>8)&0xFF;
		}		
		{
			int temp = wpa->page4.input_Start_offset;
			segment[14] = temp&0xFF;
			segment[15] = (temp>>8)&0xFF;
		}
		memset(segment+16,0x20,16);		
		segment[32] = 0x01;
		segment[33] = 0x03;
		{
			int temp = wpa->page4.output_Totally;
			segment[34] = temp&0xFF;
			segment[35] = (temp>>8)&0xFF;
		}
		{
			int temp = wpa->page4.input_Start_offset;
			segment[36] = temp&0xFF;
			segment[37] = (temp>>8)&0xFF;
		}
		memset(segment+38,0x20,16);
		nextcode = WPA_CODE_EOF;
		break;

	default:
		break;
	}

	if (segsize>0)
	{
		//encode segment's header
		segment[0] = segsize&0xFF;
		segment[1] = (segsize>>8)&0xFF;
		segment[2] = (segcode>>8)&0xFF;
		segment[3] = segcode&0xFF;
	}

	*psegcode = nextcode;
	return segsize;
}

/* destroy wpa instance */
void mel_wpa_FXCPU_FX3U_free(void* wpax)
{
	WPA_FXCPU_FX3U* wpa = (WPA_FXCPU_FX3U*)wpax;

	if (wpa)
	{

		free(wpa);
	}

	return;

}