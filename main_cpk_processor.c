#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "protocmn.h"

#include "modbus_rtu_crc16.h"
#include "crc.h"
#include "ckp.h"

#include "clickplc_ckp_nick.h"
#include "clickplc_ckp_dview.h"
#include "clickplc_ckp_scr.h"

int main(int argc,char** argv)
{
	char *file_name;
	FILE *fp_i = 0;
	int file_size;
	
	unsigned char *data = 0;
	
	unsigned short unknown_16b_ab;
	unsigned short unknown_16b_ba;
	unsigned int act_crc16;
	unsigned short calc_crc16;
	
	if (argc < 2)
	{
		printf("usage: cpk_processor.exe file_in");
		//				0				 1
		return(1);
	}

	file_name = argv[1];
	
	fp_i = fopen(file_name,"rb");
	fseek(fp_i,0,SEEK_END);
	file_size = ftell(fp_i);
	
	printf("file = %d bytes [0x%x]\n",file_size,file_size);

	data = (unsigned char*)malloc(file_size);

	fseek(fp_i,0,SEEK_SET);
	fread(data,1,file_size,fp_i);
	fclose(fp_i);

	act_crc16 = modbus_rtu_crc16(data+2,file_size-2);
	calc_crc16= crc16poly((char*)data+2,(unsigned short)file_size-2,CUSTOM_CKP_POLY);

	/*
	{
		unsigned short poly;
		unsigned short enc_code_ab;
		unsigned short enc_code_ba;

		enc_code_ab = (((unsigned short)data[0])<<8)|((unsigned short)data[1]);;
		enc_code_ba = (((unsigned short)data[1])<<8)|((unsigned short)data[0]);;
		
		for (poly = 0; poly < 0xffff; poly++)
		{
			calc_crc16= crc16poly((char*)data+2,(unsigned short)file_size-2,poly);

			if (calc_crc16 == enc_code_ab || calc_crc16 == enc_code_ba)
			{
				printf("SUCCESS!!!: %04x, %04x, %04x, %04x\n",calc_crc16,enc_code_ab,enc_code_ba,poly);
			}
		}
	}
	*/
	
	//unknown_16b_0x00 = (((unsigned short)data[0])<<8)|((unsigned short)data[1]);
	unknown_16b_ab = (((unsigned short)data[0])<<8)|((unsigned short)data[1]);
	unknown_16b_ba = (((unsigned short)data[1])<<8)|((unsigned short)data[0]);
	
	/*
	printf("unknown_16b_0x00 = 0x%x [%d d]\n",unknown_16b_0x00,unknown_16b_0x00);
	printf("act_crc16 = 0x%x [%d d]\n",act_crc16,act_crc16);
	printf("calc_crc16 = 0x%x [%d d]\n",calc_crc16,calc_crc16);
	*/

	printf("%04x, %04x, %04x\t",calc_crc16,unknown_16b_ab,unknown_16b_ba);
	if (calc_crc16==unknown_16b_ab)
	{
		printf("AB");
	}else
		if (calc_crc16==unknown_16b_ba)
		{
			printf("BA");
		}
		else
		{
			printf("!=");
		}
	
	if (file_size&0x01)
	{
		printf("=BA");
	}
	else
	{
		printf("=AB");
	}

	printf(" %d %d %d\n",unknown_16b_ab&0x01,unknown_16b_ba&0x01,file_size&0x01);
	
	//return(1);

	{		
		int offset;
		int length;
		int marker;
		int segments[CPK_MARKERS_MAX]={0};

		marker = 0;
		offset = 0;
		length = file_size;

		do 
		{
			int i_marker;
			
			for (i_marker = 0; i_marker < CPK_MARKERS_MAX; i_marker++)
			{
				if (memcmp(data+offset,SCMarkers[i_marker],8)==0)
				{
					printf("%d\t%s\t0x%x [%d]\n",i_marker,SCMarkers[i_marker],offset,offset);
					segments[i_marker] = offset;
				}
			}
			
			offset++;

		} while (offset < length);
	}

	{
		int i;
		CPKTopHeader top_header;
		CPKTopSubHeader *sub_header;
		CPKSCRHeader *scr_header;
		unsigned short tmp16bits;
		
		char fname[64][128] = {0};

		sub_header = (CPKTopSubHeader*)	malloc(CPK_SCR_N_MAX*sizeof(CPKTopSubHeader));
		scr_header = (CPKSCRHeader*)	malloc(CPK_SCR_N_MAX*sizeof(CPKSCRHeader));
		
		fp_i = fopen(file_name,"rb");
				
		fread(&tmp16bits,2,1,fp_i);
		
		top_header.crc16sign = (file_size&0x01)? (tmp16bits) : ( ((tmp16bits&0x00ff)<<8)|( (tmp16bits&0xff00)>>8 ) );

		//printf("top_header.crc16sign = %04x\n",top_header.crc16sign);

		fread(&top_header.seg_0_offset,4,1,fp_i);
		fread(&top_header.seg_0_length,4,1,fp_i);
		
		fread(&top_header.seg_1_offset,4,1,fp_i);
		fread(&top_header.seg_1_length,4,1,fp_i);
		
		fread(&top_header.seg_2_offset,4,1,fp_i);
		fread(&top_header.seg_2_length,4,1,fp_i);
		
		fread(&top_header.seg_3_offset,4,1,fp_i);
		fread(&top_header.seg_3_length,4,1,fp_i);
		
		fread(&top_header.seg_4_offset,4,1,fp_i);
		fread(&top_header.seg_4_length,4,1,fp_i);
		
/*
//SC-PRJ
unsigned int seg_0_offset;
unsigned int seg_0_length;
//SC-INI
unsigned int seg_1_offset;
unsigned int seg_1_length;
//SC-NICK
unsigned int seg_2_offset;
unsigned int seg_2_length;
//SC-DVIEW
unsigned int seg_3_offset;
unsigned int seg_3_length;
//SC-CMORE
unsigned int seg_4_offset;
unsigned int seg_4_length;

*/
		printf("%s: %08x X %d bytes\n",SCMarkers[SCMarkersID_PRJ],top_header.seg_0_offset,top_header.seg_0_length);
		printf("%s: %08x X %d bytes\n",SCMarkers[SCMarkersID_INI],top_header.seg_1_offset,top_header.seg_1_length);
		printf("%s: %08x X %d bytes\n",SCMarkers[SCMarkersID_NICK],top_header.seg_2_offset,top_header.seg_2_length);
		printf("%s: %08x X %d bytes\n",SCMarkers[SCMarkersID_DVIEW],top_header.seg_3_offset,top_header.seg_3_length);
		printf("%s: %08x X %d bytes\n",SCMarkers[SCMarkersID_CMORE],top_header.seg_4_offset,top_header.seg_4_length);

		fread(&top_header.n_scr_files,2,1,fp_i);
												
		for (i = 0; i < top_header.n_scr_files; i++)
		{
			fread(&sub_header[i],sizeof(CPKTopSubHeader),1,fp_i);

			//printf("seg[%d]: 0x%08x\t%d bytes\n",i,sub_header[i].scr_file_offset,sub_header[i].scr_file_length);
			printf("%s%d: %08x X %d bytes\n",SCMarkers[SCMarkersID_SCR],i,sub_header[i].scr_file_offset,sub_header[i].scr_file_length);
		}

		printf("n_scr_files: 0x%04x\t%d\n",top_header.n_scr_files,top_header.n_scr_files);

#if 0
		for (i = 0; i < top_header.n_scr_files; i++)
		{
			int j,k;			
			unsigned int intro_offset;
			
			fseek(fp_i,sub_header[i].scr_file_offset,SEEK_SET);

			fread(scr_header[i].marker,1,CPK_MARKER_SIZE,fp_i);
			fread(scr_header[i].CPKVersionStamp,1,CPK_VERSION_STAMP_SIZE,fp_i);
			fread(scr_header[i].unknown_data,1,CPK_SCR_HEADER_UNKNOWN_DATA_SIZE,fp_i);
			fread(&scr_header[i].scr_id,2,1,fp_i);
			fread(&scr_header[i].scr_fname_length,1,1,fp_i);

			printf("SCR seg[%d]: 0x%08x\t%d bytes\n",i,sub_header[i].scr_file_offset,sub_header[i].scr_file_length);

			printf("CPK ver: [%d.%d]\n",scr_header[i].CPKVersionStamp[3],scr_header[i].CPKVersionStamp[2]);

			printf("Unknown (%d bytes):\t",CPK_SCR_HEADER_UNKNOWN_DATA_SIZE);
			for (j = 0; j < CPK_SCR_HEADER_UNKNOWN_DATA_SIZE/4; j++)
			{
				printf("%02x ",scr_header[i].unknown_data[j]);
			}
			printf("...");
			for (j = CPK_SCR_HEADER_UNKNOWN_DATA_SIZE-CPK_SCR_HEADER_UNKNOWN_DATA_SIZE/4; j < CPK_SCR_HEADER_UNKNOWN_DATA_SIZE; j++)
			{
				printf("%02x ",scr_header[i].unknown_data[j]);
			}
			printf("\n");

			printf("SCR ID: %d\n",scr_header[i].scr_id);


			for (j = 0; j < scr_header[i].scr_fname_length/2; j++)
			{
				fread(scr_header[i].scr_fname + j,2,1,fp_i);

				fname[i][j] = (char)scr_header[i].scr_fname[j];
			}			
						
			printf("SCR Name (%d bytes): \"%s\"\n",scr_header[i].scr_fname_length,fname[i]);

			fread(&scr_header[i].unknown_data2,1,CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE,fp_i);

			printf("Unknown2 (%d bytes):\t",CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE);
			for (j = 0; j < CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE/4; j++)
			{
				printf("%02x ",scr_header[i].unknown_data2[j]);
			}
			printf("...");
			for (j = CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE-CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE/4; j < CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE; j++)
			{
				printf("%02x ",scr_header[i].unknown_data2[j]);
			}
			printf("\n");
			/*
			ptr8 = data 
					+ sub_header[i].scr_file_offset 
					+ CPK_MARKER_SIZE 
					+ CPK_VERSION_STAMP_SIZE 
					+ CPK_SCR_HEADER_UNKNOWN_DATA_SIZE 
					+ 1 
					+ 2 
					+ scr_header[i].scr_fname_length;					

			tmp16 = (unsigned short)ptr8[0]|( (unsigned short)ptr8[1] << 8 );
			while (tmp16 == 0x0041)
			{			
				ptr8+=2;				
				tmp16 = (unsigned short)ptr8[0]|( (unsigned short)ptr8[1] << 8 );
			};
			scr_header[i].ladder_0_offset = ptr8 - data;
			*/

			intro_offset = sub_header[i].scr_file_offset 
							+ CPK_MARKER_SIZE 
							+ CPK_VERSION_STAMP_SIZE 
							+ CPK_SCR_HEADER_UNKNOWN_DATA_SIZE 
							+ 1 
							+ 2 
							+ scr_header[i].scr_fname_length
							+ CPK_SCR_HEADER_UNKNOWN_DATA2_SIZE;

			for (k = 0, j = intro_offset; j < sub_header[i].scr_file_offset + sub_header[i].scr_file_length; k++,j++)
			{								
				
				if (!memcmp(data + j,CPKSCRMagicSign,CPK_MAGICSIGN_SIZE))
				{
					printf("\n===SIGN===\t");
					j+=CPK_MAGICSIGN_SIZE;
					k+=CPK_MAGICSIGN_SIZE;
				}
				else
				{
					if (!memcmp(data + j,CPKSCRMagicSign2,CPK_MAGICSIGN2_SIZE))
					{
						printf("\t===Nullx64===\n");
						j+=CPK_MAGICSIGN2_SIZE;
						k+=CPK_MAGICSIGN2_SIZE;
					}
					else
					
					if (k==0)
					{
						if(data[j]==0x98) printf("\n    ");
						if(data[j]==0xf6) printf("\n    ");
					}
					else
					{
						if(data[j+3]==0x98) printf("\n");
						if(data[j+3]==0xf6) printf("\n");					
					}

					if(isalnum(data[j]))
					{
						printf("%c ",data[j]);
						if(data[j+1]==0)
						{							
							j++;
							k++;
						}
					}
					else
					{
						//if(data[j]) printf("%02x ",data[j]);
						//else printf("%x ",data[j]);

						printf("%02x ",data[j]);
					}

					//if(data[j]==0x20) printf("\n");
				}
			}			
			
			printf("\n\n");
		}
#endif

		printf("##################################	SCR LIST		######################################\n\n");

		for (i = 0; i < top_header.n_scr_files; i++)
		{
			printf("SCR ID: %d\t%04x X %d bytes\t%s\t\n",scr_header[i].scr_id,sub_header[i].scr_file_offset,sub_header[i].scr_file_length,fname[i]);
		}

		fclose(fp_i);
		
		{
#if 0			
			
			CLICKPLC_CKP_NICK sc_nick = {0};
			CLICKPLC_CKP_DVIEW sc_dview = {0};
			//CLICKPLC_CKP_SCR sc_scr = {0};
						
			//////////////////////////////////////////////////////////////////////////
			//SC-PRJ
			//...
			
			//////////////////////////////////////////////////////////////////////////
			//SC-NICK		
			printf_buf_hex_16(data + top_header.seg_2_offset,CLICKPLC_CKP_NICK_HEADER_SIZE+32,"\nSC-NICK partially:\n");
			//printf_buf_hex_16(data + top_header.seg_2_offset+top_header.seg_2_length-CLICKPLC_CKP_NICK_FOOTER_SIZE,
			//					CLICKPLC_CKP_NICK_FOOTER_SIZE,"\nSC-NICK footer:\n");			
			parser_NICK(data + top_header.seg_2_offset,top_header.seg_2_length,&sc_nick);
			destroy_NICK(&sc_nick);			
			
			//////////////////////////////////////////////////////////////////////////
			//SC-DVIEW
			printf_buf_hex_16(data + top_header.seg_3_offset,96,"\nSC-DVIEW: partially\n");
			//printf_buf_hex_16(data + top_header.seg_3_offset+top_header.seg_3_length-CLICKPLC_CKP_NICK_FOOTER_SIZE,
			//					CLICKPLC_CKP_NICK_FOOTER_SIZE,"\nSC-DVIEW footer:\n");
			parser_DVIEW(data + top_header.seg_3_offset,top_header.seg_3_length,&sc_dview);			
			printf("encoded: %d *.cdv files\n",sc_dview.n_cdv_files);
			if(sc_dview.n_cdv_files)
			{
				int i,n;

				n = sc_dview.n_cdv_files;
				for (i = 0; i < n; i++)
				{
					printf("%3d\t%24s;\t%5d bytes\n",i,sc_dview.cdv_files[i].file_name,sc_dview.cdv_files[i].body_size);
				}				
			}
			printf("encoded: %d *.tv files\n",sc_dview.n_tv_files);
			if(sc_dview.n_tv_files)
			{
				int i,n;

				n = sc_dview.n_tv_files;
				for (i = 0; i < n; i++)
				{
					printf("%3d\t%24s;\t%5d bytes\n",i,sc_dview.tv_files[i].file_name,sc_dview.tv_files[i].body_size);
				}				
			}
			destroy_DVIEW(&sc_dview);
			
			//////////////////////////////////////////////////////////////////////////
			//SC-CMORE
			//printf_buf_hex_16(data + top_header.seg_4_offset,top_header.seg_4_length,"SC-CMORE:\n");
#endif			
			//////////////////////////////////////////////////////////////////////////
			//SC-SCR
			
			for (i = 0; i < top_header.n_scr_files; i++)
			{
				CLICKPLC_CKP_SCR sc_scr = {0};
				int done_bytes;
				
				//printf("\nSCR ID: %d\t%04x X %d bytes\t%s\t\n",scr_header[i].scr_id,sub_header[i].scr_file_offset,sub_header[i].scr_file_length,fname[i]);
				//printf_buf_hex_16(data + sub_header[i].scr_file_offset,sub_header[i].scr_file_length,0);				
				
				printf("\n\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
				printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
				printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");

				//done_bytes = parser_SCR(data + sub_header[i].scr_file_offset,sub_header[i].scr_file_length,&sc_scr);
				done_bytes = parser_SCR_v1(data + sub_header[i].scr_file_offset,sub_header[i].scr_file_length,&sc_scr);
								
				//printf("SCR ID [%3d], SCR NAME[%24s]\n",sc_scr.scr_id, sc_scr.name);
				
				/*
				printf_buf_hex_N(sc_scr.header,sizeof(sc_scr.header),"\n\nheader\n",48);
				printf_buf_hex_N(sc_scr.padding_As,sc_scr.n_padding_As,"\n\npadding A's\n",48);
				printf_buf_hex_N(sc_scr.unknown_subhdr,sizeof(sc_scr.unknown_subhdr),"\n\nunknown subhdr\n",48);
				//printf_buf_hex_N(sc_scr.magic_sign,sc_scr.n_magic_sign,"\n\nmagic sign\n",48);
				
				if (sc_scr.n_magic_sign == CPK_MAGICSIGN_SIZE)
				{
					if (memcmp(sc_scr.magic_sign,CPKSCRMagicSign,CPK_MAGICSIGN_SIZE)==0)
					{
						printf("\n############################ sign == MAGIC ##############################\n");
					}
					else
					{
						printf("\n############################ sign != MAGIC ##############################\n");
					}
				}
				
				printf("\nn_padding_0s = %d\n",sc_scr.n_padding_0s);

				if (sc_scr.n_padding_0s > 0)
				{
					//printf_buf_hex_N(sc_scr.padding_0s,sc_scr.n_padding_0s,"\n\npadding 0's\n",32);
					//printf_buf_hex_N(data + sub_header[i].scr_file_offset,1024,"\nGOOD DUMP\n",32);
				}
				else
				{
					printf_buf_hex_N(data + sub_header[i].scr_file_offset,1024,"\nPROBLEM DUMP\n",32);
				}
				
				printf_buf_hex_N(sc_scr.unknown_subhdr2,sizeof(sc_scr.unknown_subhdr2),"\n\nunknown subhdr2\n",48);
				

				printf("\n######################[%s]##########################\n",sc_scr.op_name);
				printf_buf_hex_N(sc_scr.unknown_subhdr3,CKP_SCR_UNKNOWN_SUBHDR_SIZE3,"\nUnknown3:\n",32);
				printf("\n######################[%s]##########################\n",sc_scr.op_text_1);
				
				//printf_buf_hex_N(data + sub_header[i].scr_file_offset + done_bytes,1024,"\nNext 1024:\n",32);
				*/

				destroy_SCR(&sc_scr);
			}			
		}
	}
	
	free(data);

	return(0);
}

