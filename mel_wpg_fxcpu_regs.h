#ifndef __mel_wpg_fxcpu_regs_h__
#define __mel_wpg_fxcpu_regs_h__

#include "mel_common.h"
#include "mel_wpg_device.h"

//returns: 0 - failure: can't set (not applicable device type), 1 - successfully done;
//useful only for: M,D,C
int set_dtype_ext_fxcpu(PLCRegDevice* dev,	//	IN(dtype,index) / OUT (dtype,dtype_ext);
						int cpuType			//	IN
						);

/*
	returns: 0 - failure: can't get (not applicable device type), !0 - successfully done;

	unsigned int dtype,			//	IN
	unsigned int dtype_ext,		//	IN
	int cpuType,				//	IN
	PLCRegDeviceRange* range	//	OUT

	WARNING:

		do not apply for E,H2,I !

*/
PLCRegDeviceRange*	get_device_range_fxcpu(	unsigned int dtype,			//	IN
											unsigned int dtype_ext,		//	IN
											int cpuType,				//	IN
											PLCRegDeviceRange* range	//	OUT
											);
/*
	returns: 0 - out of range; 1- within range;
	PLCRegDevice* dev,	//IN(dtype,index,const_int,_uint,_flt) / OUT (dtype);
	int cpuType			//IN
*/
int is_device_in_range_fxcpu(	PLCRegDevice* dev,	//IN(dtype,index,const_int,_uint,_flt) / OUT (dtype)
								int cpuType			//IN
								);

//returns: 1-yes,0-no;
int is_valid_E_device_fxcpu		(PLCRegDevice* dev);
int is_valid_I_device_fxcpu		(PLCRegDevice* dev,int cpuType);

#endif //__mel_wpg_fxcpu_regs_h__
