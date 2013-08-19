#include "mel_common.h"
#include "mel_wpg_device.h"
#include "mel_wpg_fxcpu_regs.h"
#include <string.h>//memset
#include <stdio.h>//sprintf

#ifdef WIN32
#pragma warning (disable : 4996)
#endif

char* mel_get_device_name(unsigned int dev_type)
{
	char* name = "Unknown";

	if (dev_type & ( MEL_DEV_TYPE_M
//					|MEL_OTYPE_Msys
					)
		)
	{
		name = "M"; goto __exit_point;
	}

	if (dev_type & ( MEL_DEV_TYPE_Z
					|MEL_DEV_TYPE_ZI
					)
		)
	{
		name = "Z"; goto __exit_point;
	}

	if (dev_type & ( MEL_DEV_TYPE_V
					|MEL_DEV_TYPE_VI
					)
		)
	{
		name = "V"; goto __exit_point;
	}

	if (dev_type & ( MEL_DEV_TYPE_K
					|MEL_DEV_TYPE_K2
					|MEL_DEV_TYPE_Kn
					//|MEL_DEV_TYPE_KnM
					//|MEL_DEV_TYPE_KnS
					//|MEL_DEV_TYPE_KnX
					//|MEL_DEV_TYPE_KnY
					)
		)
	{
		name = "K"; goto __exit_point;
	}

	if (dev_type & ( MEL_DEV_TYPE_H
					|MEL_DEV_TYPE_H2
					)
		)
	{
		name = "H"; goto __exit_point;
	}

	switch(dev_type)
	{
	case MEL_DEV_TYPE_S:	name = "S"; break;
	case MEL_DEV_TYPE_X:	name = "X"; break;
	case MEL_DEV_TYPE_Y:	name = "Y"; break;
	case MEL_DEV_TYPE_T:	name = "T"; break;
	case MEL_DEV_TYPE_C:	name = "C"; break;
	case MEL_DEV_TYPE_D:	name = "D"; break;
	case MEL_DEV_TYPE_R:	name = "R"; break;
	case MEL_DEV_TYPE_SU:	name = "SU"; break;//name = "S"; break;								
	case MEL_DEV_TYPE_bI:	name = "."; break;
	case MEL_DEV_TYPE_E:	name = "E"; break;
	case MEL_DEV_TYPE_P:	name = "P"; break;
	case MEL_DEV_TYPE_I:	name = "I"; break;
	case MEL_DEV_TYPE_N:	name = "N"; break;
	case MEL_DEV_TYPE_STR: name = "";break;
	case MEL_DEV_TYPE_Un: name = "U"; break;
	case MEL_DEV_TYPE_Gn: name = "\\G"; break;
	case MEL_DEV_TYPE_ER:	name = "ER"; break;
	}
__exit_point:
	return(name);
}

unsigned int mel_approx_dtype_by_chars(char first,char second)
{
	unsigned int dtype = MEL_DEV_TYPE_UNKNOWN;

	switch(first)
	{
	case 'M':case 'm': dtype =	 MEL_DEV_TYPE_M
//						|MEL_OTYPE_Msys
						;
		break;

	case 'S':case 's': dtype = MEL_DEV_TYPE_S; break;
	case 'X':case 'x': dtype = MEL_DEV_TYPE_X; break;
	case 'Y':case 'y': dtype = MEL_DEV_TYPE_Y; break;
	case 'T':case 't': dtype = MEL_DEV_TYPE_T; break;
	case 'C':case 'c': dtype = MEL_DEV_TYPE_C; break;
	case 'D':case 'd': dtype = MEL_DEV_TYPE_D; break;
	case 'R':case 'r': dtype = MEL_DEV_TYPE_R; break;
		
	case '.': dtype = MEL_DEV_TYPE_bI; break;
	case '"': dtype = MEL_DEV_TYPE_STR; break;

	case 'V':case 'v': dtype =	 MEL_DEV_TYPE_V
								|MEL_DEV_TYPE_VI
								;
		break;

	case 'Z':case 'z': dtype =	 MEL_DEV_TYPE_Z
								|MEL_DEV_TYPE_ZI
								;
		break;

	case 'K':case 'k': dtype =	 MEL_DEV_TYPE_K
								|MEL_DEV_TYPE_K2
								|MEL_DEV_TYPE_Kn
								;
		break;

	case 'E':case 'e':
		if (second == 'R' || second == 'r')
		{
			dtype =	MEL_DEV_TYPE_ER;
		}
		else
		{
			dtype =	 MEL_DEV_TYPE_E;
		}		
		break;


	case 'H':case 'h': dtype =	 MEL_DEV_TYPE_H
								|MEL_DEV_TYPE_H2
								;
		break;

	case 'P':case 'p': dtype = MEL_DEV_TYPE_P; break;
	case 'I':case 'i': dtype = MEL_DEV_TYPE_I; break;
	case 'N':case 'n': dtype = MEL_DEV_TYPE_N; break;

	case 'U':case 'u': dtype = MEL_DEV_TYPE_Un; break;
	//case '\\':
	case 'G':case 'g': dtype = MEL_DEV_TYPE_Gn; break;
	}

	return(dtype);
}

unsigned int mel_get_device_code(unsigned int dev_type)
{
	unsigned int code = 0;

	//filters

	if (dev_type & (
					MEL_DEV_TYPE_M
					//|MEL_OTYPE_Msys
					))
	{
		dev_type = MEL_DEV_TYPE_M;
	}
	if (dev_type & (// MEL_DEV_TYPE_KnX
					//|MEL_DEV_TYPE_KnY
					//|MEL_DEV_TYPE_KnM
					//|MEL_DEV_TYPE_KnS
					/*|*/MEL_DEV_TYPE_Kn
					)
		)
	{
		//printf("COMPLEX Kn: %08x\n",dev_type);
		dev_type = MEL_DEV_TYPE_Kn;
	}
	
	if (!code)
	{
		switch(dev_type)
		{
		case MEL_DEV_TYPE_M:	//Supplement relay
			code = MEL_DEV_CODE_M;
			break;
		case MEL_DEV_TYPE_S:	//State
			code = MEL_DEV_CODE_S;
			break;
		case MEL_DEV_TYPE_X:	//Input
			code = MEL_DEV_CODE_X;
			break;
		case MEL_DEV_TYPE_Y:	//Output
			code = MEL_DEV_CODE_Y;
			break;
		case MEL_DEV_TYPE_T:	//Timer
			code = MEL_DEV_CODE_T;
			break;
		case MEL_DEV_TYPE_C:	//Counter
			code = MEL_DEV_CODE_C;
			break;
		case MEL_DEV_TYPE_D:	//Data registers
			code = MEL_DEV_CODE_D;
			break;
		case MEL_DEV_TYPE_R:	//File registers
			code = MEL_DEV_CODE_R;
			break;
		case MEL_DEV_TYPE_SU://Special Unit
			code = MEL_DEV_CODE_SU;
			break;
			//indexes
		case MEL_DEV_TYPE_ZI:	//Zx
			code = MEL_DEV_CODE_ZI;
			break;
		case MEL_DEV_TYPE_Z://Zx
			code = MEL_DEV_CODE_Z;
			break;
		case MEL_DEV_TYPE_Kn://Kx
			code = MEL_DEV_CODE_Kn;
			break;
		case MEL_DEV_TYPE_bI://D.b[x]
			code = MEL_DEV_CODE_bI;
			break;
		case MEL_DEV_TYPE_VI:	//Vx
			code = MEL_DEV_CODE_VI;
			break;
		case MEL_DEV_TYPE_V:
			code = MEL_DEV_CODE_V;
			break;

		case MEL_DEV_TYPE_Un:
			code = MEL_DEV_CODE_Un;
			break;
		case MEL_DEV_TYPE_Gn:
			code = MEL_DEV_CODE_Gn;
			break;

			//constants
		case MEL_DEV_TYPE_K:	//Kx
			code = MEL_DEV_CODE_K;
			break;
		case MEL_DEV_TYPE_K2://Kx
			code = MEL_DEV_CODE_K2;
			break;
		case MEL_DEV_TYPE_H:	//Hx
			code = MEL_DEV_CODE_H;
			break;
		case MEL_DEV_TYPE_P:	//Px
			code = MEL_DEV_CODE_P;
			break;
		case MEL_DEV_TYPE_I:	//Ix
			code = MEL_DEV_CODE_I;
			break;
		case MEL_DEV_TYPE_N:	//N0-N7 for MC,MCR		
			code = MEL_DEV_CODE_N;
			break;

		case MEL_DEV_TYPE_H2://Hx
			code = MEL_DEV_CODE_H2;
			break;

		case MEL_DEV_TYPE_E:	//Ex
			code = MEL_DEV_CODE_E;
			break;

		case MEL_DEV_TYPE_STR:
			code = MEL_DEV_CODE_STR;
			break;

		case MEL_DEV_TYPE_ER:
			code = MEL_DEV_CODE_ER;
			break;
		}

	}

	return(code);
}

unsigned int mel_get_device_type(unsigned int dev_code)
{
	unsigned int dtype = MEL_DEV_TYPE_UNKNOWN;

	switch(dev_code)
	{
	case MEL_DEV_CODE_M:	//Supplement relay
		dtype = MEL_DEV_TYPE_M;
		break;
	case MEL_DEV_CODE_S:	//State
		dtype = MEL_DEV_TYPE_S;
		break;
	case MEL_DEV_CODE_X:	//Input
		dtype = MEL_DEV_TYPE_X;
		break;
	case MEL_DEV_CODE_Y:	//Output
		dtype = MEL_DEV_TYPE_Y;
		break;
	case MEL_DEV_CODE_T:	//Timer
		dtype = MEL_DEV_TYPE_T;
		break;
	case MEL_DEV_CODE_C:	//Counter
		dtype = MEL_DEV_TYPE_C;
		break;
	case MEL_DEV_CODE_D:	//Data registers
		dtype = MEL_DEV_TYPE_D;
		break;
	case MEL_DEV_CODE_ER:	//Extension File registers
		dtype = MEL_DEV_TYPE_ER;
		break;
	case MEL_DEV_CODE_R:	//File registers
		dtype = MEL_DEV_TYPE_R;
		break;
	case MEL_DEV_CODE_SU://Special Unit
		dtype = MEL_DEV_TYPE_SU;
		break;
		//indexes
	case MEL_DEV_CODE_ZI:	//Zx
		dtype = MEL_DEV_TYPE_ZI;
		break;
	case MEL_DEV_CODE_Z://Zx
		dtype = MEL_DEV_TYPE_Z;
		break;
	case MEL_DEV_CODE_Kn://Kx
		dtype = MEL_DEV_TYPE_Kn;
		break;
	case MEL_DEV_CODE_bI://D.b[x]
		dtype = MEL_DEV_TYPE_bI;
		break;
	case MEL_DEV_CODE_VI:	//Vx
		dtype = MEL_DEV_TYPE_VI;
		break;
	case MEL_DEV_CODE_V:
		dtype = MEL_DEV_TYPE_V;
		break;
	case MEL_DEV_CODE_Un:
		dtype = MEL_DEV_TYPE_Un;
		break;
	case MEL_DEV_CODE_Gn:
		dtype = MEL_DEV_TYPE_Gn;
		break;
		//constants
	case MEL_DEV_CODE_K:	//Kx
		dtype = MEL_DEV_TYPE_K;
		break;
	case MEL_DEV_CODE_K2://Kx
		dtype = MEL_DEV_TYPE_K2;
		break;
	case MEL_DEV_CODE_H:	//Hx
		dtype = MEL_DEV_TYPE_H;
		break;
	case MEL_DEV_CODE_P:	//Px
		dtype = MEL_DEV_TYPE_P;
		break;
	case MEL_DEV_CODE_I:	//Ix
		dtype = MEL_DEV_TYPE_I;
		break;
	case MEL_DEV_CODE_N:	//N0-N7 for MC,MCR		
		dtype = MEL_DEV_TYPE_N;
		break;

	case MEL_DEV_CODE_H2://Hx
		dtype = MEL_DEV_TYPE_H2;
		break;

	case MEL_DEV_CODE_E:	//Ex
		dtype = MEL_DEV_TYPE_E;
		break;

	case MEL_DEV_CODE_STR:
		dtype = MEL_DEV_TYPE_STR;
		break;
	}

	return(dtype);
}

char* mel_device2text(PLCRegDevice* dev,int* ptextsize)
{
	char* retptr = 0;

	static char text[MEL_WPG_MAX_INSTR_TEXT_SIZE] = {0};
	int textsize = 0;

	char devnumber[MEL_WPG_MAX_INSTR_TEXT_SIZE] = {0};
	int devnumberlen;

	char* devname;
	int devnamelen;

	unsigned int dtype = dev->dtype;

	text[0] = 0;

	devname = mel_get_device_name(dtype);
	devnamelen = strlen(devname);

	strcat(text,devname);
	textsize += devnamelen;

	if (	!(dtype&(MEL_DEV_TYPE_K
					|MEL_DEV_TYPE_K2
					|MEL_DEV_TYPE_bI
					|MEL_DEV_TYPE_E
					|MEL_DEV_TYPE_H
					|MEL_DEV_TYPE_H2
					|MEL_DEV_TYPE_STR
					)
				)
		)
	{
		if ( !(dtype&(MEL_DEV_TYPE_X|MEL_DEV_TYPE_Y)))
		{
			devnumberlen = sprintf(devnumber,"%d",dev->index);
		}		
		else
			devnumberlen = sprintf(devnumber,"%03o",dev->index);
	}
	else
	{
		if (!(dtype&MEL_DEV_TYPE_E))
		{
			if (	!(dtype&(MEL_DEV_TYPE_H
							|MEL_DEV_TYPE_H2
							|MEL_DEV_TYPE_bI
							|MEL_DEV_TYPE_STR
							)
						)
				)
			{							
				devnumberlen = sprintf(devnumber,"%d",dev->const_int);
			}
			else
			{
				if (dtype&(MEL_DEV_TYPE_H2|MEL_DEV_TYPE_H))
				{								
					devnumberlen = sprintf(devnumber,"%X",dev->const_uint);
				}

				if (dtype&MEL_DEV_TYPE_STR)
				{
					devnumberlen = sprintf(devnumber,"\"%s\"",dev->const_str);
				}

				if (dtype&MEL_DEV_TYPE_bI)
				{
					devnumberlen = sprintf(devnumber,"%X",dev->index);
				}
			}
		}
		else
		{						
			devnumberlen = sprintf(devnumber,"%f",dev->const_flt);
		}
	}

	strcat(text,devnumber);
	textsize += devnumberlen;

	retptr = text;
	*ptextsize = textsize;
	return(retptr);
}

int mel_wpg_dec_device(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,PLCRegDevice* dev)
{	
	int size_done = 0;

	if (stream_size > 2)
	{
		int size_brutto = stream[0];
		if (size_brutto <= stream_size)
		{
#if 0
			{
				int i_byte;
				printf("\t\t[");
				for (i_byte = 0; i_byte < size_brutto; i_byte++)
				{
					printf("%02X ",stream[i_byte]);
				}
				printf("]");
			}
#endif			
			if (size_brutto == stream[size_brutto - 1])
			{
				int size_netto = size_brutto - 2;
				int data_size = size_netto - 1;
				unsigned int data;
				unsigned int code;

				size_done = size_brutto;
				code = stream[1];
				//fill common fields				
				dev->dtype = mel_get_device_type(code);
				//decode value
				switch(data_size)
				{
				case 1:
					data = stream[2];
					break;
				case 2:
					data = ((int)stream[3]<<8)|(int)stream[2];
					break;
				case 3:
					data = ((int)stream[4]<<16)|((int)stream[3]<<8)|(int)stream[2];
					break;
				case 4:
					data = ((int)stream[5]<<24)|((int)stream[4]<<16)|((int)stream[3]<<8)|(int)stream[2];
					break;
				default:
					if (code == MEL_DEV_CODE_STR)
					{
						if (data_size > 32)
						{
							size_done = 0;//error: too long string
							goto __exit_point;
						}
					}
					else
					{
						size_done = 0;//error: too big encoded device's value
						goto __exit_point;
					}
				}

				//handle properly
				switch(code)
				{	//devices
				case MEL_DEV_CODE_M:	//Supplement relay
				//case MEL_OTYPE_Msys:	//Special Supplement relay
				case MEL_DEV_CODE_S:	//State
				case MEL_DEV_CODE_X:	//Input
				case MEL_DEV_CODE_Y:	//Output
				case MEL_DEV_CODE_T:	//Timer
				case MEL_DEV_CODE_C:	//Counter
				case MEL_DEV_CODE_D:	//Data registers
				case MEL_DEV_CODE_R:	//File registers
				case MEL_DEV_CODE_SU://Special Unit
				case MEL_DEV_CODE_Z:
				case MEL_DEV_CODE_V:
					//indexes
				case MEL_DEV_CODE_ZI://Z				
				case MEL_DEV_CODE_Kn://Kn
				case MEL_DEV_CODE_bI://D.b[x]
				case MEL_DEV_CODE_VI://V
				case MEL_DEV_CODE_Un:
				case MEL_DEV_CODE_Gn:
				case MEL_DEV_CODE_P:	//P
				case MEL_DEV_CODE_I:	//I
				case MEL_DEV_CODE_N:	//N0-N7 for MC,MCR
					dev->index = data;
					break;

					//constants
				case MEL_DEV_CODE_K:	//K
				case MEL_DEV_CODE_K2:	//K				
					dev->const_int = data;
					break;

				case MEL_DEV_CODE_H:	//H
				case MEL_DEV_CODE_H2:	//H
					dev->const_uint = data;
					break;

				case MEL_DEV_CODE_E:	//E
					if(data_size == 4)
					{
						union{
							unsigned int	tint;
							float			tflt;
						}tmp;

						tmp.tint =	data;
						dev->const_flt = tmp.tflt;
					}
					else
					{
						size_done = 0;//error
					}
					break;

				case MEL_DEV_CODE_STR:
					memcpy(dev->const_str,stream+2,data_size);
					dev->const_str[data_size] = 0;//add ending null
					dev->const_str_length = data_size;
					break;

				default: 
					size_done = 0;//error
				}
			}//else{//error: wrong data in stream}
		}//else{//error: stream is too short}
	}//else{//error: stream is too short}
__exit_point:
	return(size_done);
}

int	mel_wpg_enc_device(PLCRegDevice* dev,int cpuSeries,int cpuType,unsigned char* stream)
{
	int size_done = 0;
	int data_size = 0;
	unsigned int data = 0;
	unsigned int code = 0;
	unsigned int devices = MEL_DEV_TYPE_M				//Supplement relay
//							|MEL_OTYPE_Msys		//Special Supplement relay
							|MEL_DEV_TYPE_S				//State
							|MEL_DEV_TYPE_X				//Input
							|MEL_DEV_TYPE_Y				//Output
							|MEL_DEV_TYPE_T				//Timer
							|MEL_DEV_TYPE_C				//Counter
							|MEL_DEV_TYPE_D				//Data registers
							|MEL_DEV_TYPE_R				//File registers
							|MEL_DEV_TYPE_ER			//Extension File registers
							|MEL_DEV_TYPE_SU			//Special Unit
							//indexes
							|MEL_DEV_TYPE_ZI			//Zx
							|MEL_DEV_TYPE_Z				//Zx
							|MEL_DEV_TYPE_Kn
							|MEL_DEV_TYPE_bI			//D.b[x]
							|MEL_DEV_TYPE_VI			//Vx
							|MEL_DEV_TYPE_V
							|MEL_DEV_TYPE_Un
							|MEL_DEV_TYPE_Gn
							|MEL_DEV_TYPE_P				//Px
							|MEL_DEV_TYPE_I				//Ix
							|MEL_DEV_TYPE_N				//N0-N7 for MC,MCR
							;

					//devices
	if ( !(dev->dtype & (~devices)) && (dev->dtype & devices) )
	{
		data = dev->index;
	}
	else				//constants: decimal
		if (dev->dtype & (MEL_DEV_TYPE_K
						|MEL_DEV_TYPE_K2
						)
			)
		{
			data = dev->const_int;
		}
		else			//constants: hexadecimal
			if (dev->dtype & (MEL_DEV_TYPE_H
							|MEL_DEV_TYPE_H2
							)
				)
			{
				data = dev->const_uint;
			}
			else		//constants: floating point numbers
				if (dev->dtype & MEL_DEV_TYPE_E)
				{
					{
						union{
							unsigned int	tint;
							float			tflt;
						}tmp;

						tmp.tflt = dev->const_flt;
						data = tmp.tint;
						data_size = 4;
					}
				}
				else	//constants: strings
					if (dev->dtype & MEL_DEV_TYPE_STR)
					{
						//data_size = strlen(dev->const_str);
						data_size = dev->const_str_length;
						memcpy(stream+2,dev->const_str,data_size);
					}
					else
						goto __exit_point;//error: unknown device dtype

	if (!(dev->dtype&MEL_DEV_TYPE_STR))
	{
		if (data_size == 0)
		{
			data_size = mel_wpg_uint_sizeof(data);
		}

		switch(data_size)
		{
		case 1:
			stream[2] = data;
			break;
		case 2:
			stream[2] = data&0xFF;
			stream[3] = (data>>8)&0xFF;
			break;
		case 3:
			stream[2] = data&0xFF;
			stream[3] = (data>>8)&0xFF;
			stream[4] = (data>>16)&0xFF;
			break;
		case 4:
			stream[2] = data&0xFF;
			stream[3] = (data>>8)&0xFF;
			stream[4] = (data>>16)&0xFF;
			stream[5] = (data>>24)&0xFF;
			break;
		}
	}

	size_done = data_size + 1 + 2;
	stream[0] = size_done;
	stream[size_done - 1] = stream[0];
	code = mel_get_device_code(dev->dtype);
	stream[1] = code;		

__exit_point:
	return(size_done);
}

#if 0
int is_reg_in_range(PLCRegDevice* reg,int cpuSeries,int cpuType)
{
	int isok = 1;
	int i_dtype = get_log2(reg->dtype);
	int lowest	= REGS_METRIX_FXCPU[i_dtype][cpuType].lowest;
	int highest = REGS_METRIX_FXCPU[i_dtype][cpuType].highest;

	if (reg->dtype &( MEL_DEV_TYPE_M	//Supplement relay
//					|MEL_OTYPE_Msys	//Special Supplement relay
					|MEL_DEV_TYPE_S	//State
					|MEL_DEV_TYPE_X	//Input
					|MEL_DEV_TYPE_Y	//Output
					|MEL_DEV_TYPE_T	//Timer
					|MEL_DEV_TYPE_C	//Counter
					|MEL_DEV_TYPE_D	//Data registers
					|MEL_DEV_TYPE_R	//File registers
					|MEL_DEV_TYPE_ER	//Extension File registers
					|MEL_DEV_TYPE_SU//Special Unit
					|MEL_DEV_TYPE_ZI	//Zx
					|MEL_DEV_TYPE_Z//Zx
					|MEL_DEV_TYPE_Kn
					|MEL_DEV_TYPE_bI//D.b[x]
					|MEL_DEV_TYPE_VI	//Vx
					|MEL_DEV_TYPE_V
					|MEL_DEV_TYPE_Un
					|MEL_DEV_TYPE_Gn
					|MEL_DEV_TYPE_P	//Px
					|MEL_DEV_TYPE_I	//Ix
					|MEL_DEV_TYPE_N	//N0-N7 for MC,MCR			
					)
		)
	{
		if ( lowest != highest)
		{
			if (reg->index < lowest || reg->index > highest)
			{			
				isok = 0;
			}
		}
		else
		{
			switch(cpuType)
			{
			case MEL_PLC_FXCPU_TYPE_FX3U_C:
			case MEL_PLC_FXCPU_TYPE_FX3G_C:
				if (reg->dtype & MEL_DEV_TYPE_I)
				{
					switch(reg->index)
					{
						//Input interrupt (interrupt triggered by external signal)
					case 0:	//X000 I001 I000
					case 1:
					case 100://X001 I101 I100
					case 101:
					case 200://X002 I201 I200
					case 201:
					case 300://X003 I301 I300
					case 301:
					case 400://X004 I401 I400
					case 401:
					case 500://X005 I501 I500
						break;
						//Counter Interrupt - Interrupt Triggered by Counting Up of High-Speed Counter
					case 10:
					case 20:
					case 30:
					case 40:
					case 50:
					case 60:
						if(cpuType == MEL_PLC_FXCPU_TYPE_FX3G_C)
						{
							isok = 0;
						}
						break;

					default:
						//Timer interrupt (interrupt in constant cycle)
						if (	!(reg->index >= 610 && reg->index <= 699)
							&&	!(reg->index >= 710 && reg->index <= 799)
							&&	!(reg->index >= 810 && reg->index <= 899)						
							)
						{
							isok = 0;
						}
					}
				}
				break;
			}
		}
	}
	return(isok);
}
#endif 

int is_indexing_allowed(PLCRegDevice* reg,int cpuSeries,int cpuType)
{
	int isok = 0;//not allowed (by default);

	switch((cpuSeries<<8)|cpuType)
	{
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0_S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0N):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FXU_FX2C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX2N_C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1N_S):
		break;

	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3U_C):
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3G_C):
		if (//not allowed
			!(reg->dtype & 
				(
				 MEL_DEV_TYPE_Un
				|MEL_DEV_TYPE_V
				|MEL_DEV_TYPE_Z
				|MEL_DEV_TYPE_bI
				|MEL_DEV_TYPE_E
				)
			)
			&&
			//not allowed
			!(reg->dtype_ext & 
				MEL_DEV_TYPE_EXT_Msys
			)
			&&
			//allowed
			(reg->dtype & 
				(//bit devices
				 MEL_DEV_TYPE_X
				|MEL_DEV_TYPE_Y
				|MEL_DEV_TYPE_M
				|MEL_DEV_TYPE_S
				|MEL_DEV_TYPE_T
				|MEL_DEV_TYPE_C//(16-bit only)
				//word devices
				|MEL_DEV_TYPE_D
				|MEL_DEV_TYPE_R
				|MEL_DEV_TYPE_ER
				|MEL_DEV_TYPE_P//!!! -> function pointers !!!
				//word/dword constants
				|MEL_DEV_TYPE_K|MEL_DEV_TYPE_K2
				|MEL_DEV_TYPE_H|MEL_DEV_TYPE_H2
				//BMF
				|MEL_DEV_TYPE_Gn
				)
			)
		)
		{
			//except 32-bit counters
			if (reg->dtype & MEL_DEV_TYPE_C && reg->index >= 200)
			{
				goto __exit_point;
			}
			//allowed!
			isok = 1;
		}
		break;
	}

__exit_point:
	return(isok);
}

int is_linking_allowed(PLCRegDevice* reg,int cpuSeries,int cpuType)
{
	int isok = 0;//not allowed (by default);

	switch((cpuSeries<<8)|cpuType)
	{
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0_S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0N):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FXU_FX2C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX2N_C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1N_S):
		break;

	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3U_C):
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3G_C):

		if (//allowed
			(reg->dtype & 
			(//bit devices
			 MEL_DEV_TYPE_X
			|MEL_DEV_TYPE_Y
			|MEL_DEV_TYPE_M//|MEL_OTYPE_Msys
			|MEL_DEV_TYPE_S
			)
			)
			)
		{
			isok = 1;//allowed!
		}
		break;
	}
	return(isok);
}

int is_bitaccess_allowed(PLCRegDevice* reg,int cpuSeries,int cpuType)
{
	int isok = 0;//not allowed (by default);

	switch((cpuSeries<<8)|cpuType)
	{
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0_S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX0N):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FXU_FX2C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX2N_C):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1S):
		break;
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX1N_S):
		break;

	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3U_C):
	case ((MEL_PLC_SERIES_FXCPU<<8)|MEL_PLC_FXCPU_TYPE_FX3G_C):

		if (//not allowed
			!(reg->dtype &
			(//all other except D
			~MEL_DEV_TYPE_D
			)
			)
			&&
			//allowed
			(reg->dtype & 
			(//word devices
			 MEL_DEV_TYPE_D
			)
			)
			)
		{			
			isok = 1;//allowed!
		}
		break;
	}
	return(isok);
}

int is_it_VorZ(unsigned int dtype)
{
	int yes_or_no = 0;

	if (dtype & (MEL_DEV_TYPE_V
		|MEL_DEV_TYPE_VI
		|MEL_DEV_TYPE_Z
		|MEL_DEV_TYPE_ZI
		)
		)
	{
		//V or Z without number -> V0 or Z0;
		yes_or_no = 1;
	}

	return(yes_or_no);
}
