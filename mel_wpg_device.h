#ifndef __mel_wpg_device_h__
#define __mel_wpg_device_h__

#define MEL_DEV_CODE_M	0x90	//or supplement relay
#define MEL_DEV_CODE_S	0x98	//State Relay
#define MEL_DEV_CODE_X	0x9C	//Input
#define MEL_DEV_CODE_Y	0x9D	//Output
#define MEL_DEV_CODE_T	0xC2	//Timer
#define MEL_DEV_CODE_C	0xC5	//Counter
#define MEL_DEV_CODE_D	0xA8	//Data registers
#define MEL_DEV_CODE_Gn	0xAB	//Gx (as an [index]: MOV U3\G15)
#define MEL_DEV_CODE_R	0xAF	//File registers
#define MEL_DEV_CODE_ER	0xAE	//Extension File registers

#define MEL_DEV_CODE_ZI	0xF0	//Zx (as an [index]: OUT Y000Z0)
#define MEL_DEV_CODE_Kn	0xF1	//Kx (as an bits counter: OUT K8Y000)
#define MEL_DEV_CODE_bI	0xF2	//D.b[x]
#define MEL_DEV_CODE_VI	0xF4	//Vx (as an [index]: OUT Y000V0)
#define MEL_DEV_CODE_Un	0xF8	//Ux (as an [index]: MOV U3\G15)
#define MEL_DEV_CODE_Z	0xCC	//Zx (as a variable: MOV K0 Z0)
#define MEL_DEV_CODE_V	0xCD	//Vx (as a variable: MOV K0 V0)

#define MEL_DEV_CODE_SU	0xD8	//Special Unit

#define MEL_DEV_CODE_K	0xE8	//Kx (16-bit signed integer)
#define MEL_DEV_CODE_K2	0xE9	//Kx (32-bit signed integer)
#define MEL_DEV_CODE_H	0xEA	//Hx (16-bit unsigned integer in hex)
#define MEL_DEV_CODE_H2	0xEB	//Hx (32-bit unsigned integer in hex)
#define MEL_DEV_CODE_E	0xEC	//Ex (floating point number)
#define MEL_DEV_CODE_STR 0xEE	//"..."
#define MEL_DEV_CODE_P	0xD0	//Px (label or subroutine)
#define MEL_DEV_CODE_I	0xD1	//Ix (interrupt code)
#define MEL_DEV_CODE_N	0xD2	//N0-N7 for MC,MCR


typedef struct tagPLCRegDevice
{
#define MEL_DEV_TYPE_UNKNOWN (0)

#define MEL_DEV_TYPE_M		(1<<0)	//1			//or supplement relay
#define MEL_DEV_TYPE_S		(1<<1)	//2			//State Relay
#define MEL_DEV_TYPE_X		(1<<2)	//4			//Input
#define MEL_DEV_TYPE_Y		(1<<3)	//8			//Output
#define MEL_DEV_TYPE_T		(1<<4)	//16		//Timer
#define MEL_DEV_TYPE_C		(1<<5)	//32		//Counter
#define MEL_DEV_TYPE_D		(1<<6)	//64		//Data registers
#define MEL_DEV_TYPE_R		(1<<7)	//128		//File registers
#define MEL_DEV_TYPE_ER		(1<<8)	//256		//Extension File Registers 

#define MEL_DEV_TYPE_ZI		(1<<9)	//512		//Zx (as an [index]: OUT Y000Z0)
#define MEL_DEV_TYPE_Kn		(1<<10)	//1024		//Kx (as an bits counter: OUT K8Y000)
#define MEL_DEV_TYPE_bI		(1<<11)	//2048		//D.b[x]
#define MEL_DEV_TYPE_VI		(1<<12)	//4096		//Vx (as an [index]: OUT Y000V0)
#define MEL_DEV_TYPE_Z		(1<<13)	//8192		//Zx (as a variable: MOV K0 Z0)
#define MEL_DEV_TYPE_V		(1<<14)	//16384		//Vx (as a variable: MOV K0 V0)

#define MEL_DEV_TYPE_K		(1<<15)	//32768		//Kx (16-bit signed integer)
#define MEL_DEV_TYPE_K2		(1<<16)	//65536		//Kx (32-bit signed integer)
#define MEL_DEV_TYPE_H		(1<<17)	//131072	//Hx (16-bit unsigned integer in hex)
#define MEL_DEV_TYPE_H2		(1<<18)	//262144	//Hx (32-bit unsigned integer in hex)
#define MEL_DEV_TYPE_E		(1<<19)	//524288	//Ex (floating point number)
#define MEL_DEV_TYPE_P		(1<<20)	//1048576	//Px (label or subroutine)
#define MEL_DEV_TYPE_I		(1<<21)	//2097152	//Ix (interrupt code)
#define MEL_DEV_TYPE_N		(1<<22)	//4194304	//N0-N7 for MC,MCR

#define MEL_DEV_TYPE_Un		(1<<23) // U0
#define MEL_DEV_TYPE_Gn		(1<<24) // /G0
#define MEL_DEV_TYPE_STR	(1<<25) //"..."

#define MEL_DEV_TYPE_SU		(1<<26)	//Special Unit !!!UNUSED!!!

	unsigned int dtype;

#define MEL_DEV_TYPE_EXT_Msys	(1<<0)
#define MEL_DEV_TYPE_EXT_Dsys	(1<<1)
#define MEL_DEV_TYPE_EXT_Mronly	(1<<2)//"Read only" [M] system regs
#define MEL_DEV_TYPE_EXT_Dronly	(1<<3)//"Read only" [D] system regs
#define MEL_DEV_TYPE_EXT_C32bit	(1<<4)//32-bit counters (>= C200)

	unsigned int dtype_ext;

	int index;

	int				const_int;	//K,K2
	unsigned int	const_uint;	//H,H2
	float			const_flt;	//E
#define MEL_STRING_MAX_LENGTH 32
	char			const_str[MEL_STRING_MAX_LENGTH+1];//"..."
	int				const_str_length;

}PLCRegDevice;

char*			mel_get_device_name(unsigned int dev_type);
unsigned int	mel_get_device_code(unsigned int dev_type);
unsigned int	mel_get_device_type(unsigned int dev_code);
unsigned int	mel_approx_dtype_by_chars(char first,char second);

char*			mel_device2text(PLCRegDevice* dev,int* ptextsize);

typedef struct tagPLCRegDeviceRange
{
	int lowest;
	int highest;
	
}PLCRegDeviceRange;

int	mel_wpg_dec_device		(int cpuSeries,int cpuType,unsigned char* stream,int stream_size,PLCRegDevice* dev);
int	mel_wpg_enc_device		(PLCRegDevice* dev,int cpuSeries,int cpuType,unsigned char* stream);

//[..] + V/Zn
int is_indexing_allowed(PLCRegDevice* reg,int cpuSeries,int cpuType);
//Kn + [..]
int is_linking_allowed(PLCRegDevice* reg,int cpuSeries,int cpuType);
//[..] + .n
int is_bitaccess_allowed(PLCRegDevice* reg,int cpuSeries,int cpuType);

int is_it_VorZ(unsigned int dtype);

#endif //__mel_wpg_device_h__
