#include "mel_wpg_fxcpu_regs.h"

#include <limits.h> //INT_MIN,MAX
#include <float.h> //FLT_MIN,MAX

int set_dtype_ext_fxcpu(PLCRegDevice* dev,int cpuType)
{
	int isok = 0;//by default: not applicable device type;

	//Filter applicable device types only
	if (dev->dtype & (
						 MEL_DEV_TYPE_M
						|MEL_DEV_TYPE_D
						|MEL_DEV_TYPE_C						
						)
		)
	{
		int i_ext_bit = 0;

		isok = 1;//maybe
		dev->dtype_ext = 0;

		for(i_ext_bit = 0; i_ext_bit < 5; i_ext_bit++)
		{
			unsigned int dtype_base;
			unsigned int ext_type_curr;// = 1<<i_ext_bit;
			switch(i_ext_bit)
			{
			case 0://MEL_DEV_TYPE_EXT_Msys
			case 2://MEL_DEV_TYPE_EXT_Mronly
				dtype_base		= MEL_DEV_TYPE_M;
				ext_type_curr	= MEL_DEV_TYPE_EXT_Msys;
				break;
			case 1://MEL_DEV_TYPE_EXT_Dsys
			case 3://MEL_DEV_TYPE_EXT_Dronly
				dtype_base		= MEL_DEV_TYPE_D;
				ext_type_curr	= MEL_DEV_TYPE_EXT_Dsys;
				break;
			case 4://MEL_DEV_TYPE_EXT_C32bit
				dtype_base		= MEL_DEV_TYPE_C;
				ext_type_curr	= MEL_DEV_TYPE_EXT_C32bit;
				break;
			default:
				continue;
			}
			if (dev->dtype & dtype_base)
			{
				PLCRegDeviceRange* t_range = 0;
				PLCRegDeviceRange range = {0};
				//get range of extended devices
				t_range = get_device_range_fxcpu(dtype_base,ext_type_curr,cpuType,&range);
				if (t_range)
				{//is it extended or not?
					if (dev->index >= range.lowest && dev->index <= range.highest)
					{
						//set "clean" base type
						dev->dtype = dtype_base;
						//set approved extended type
						dev->dtype_ext |= ext_type_curr;
						//set extended type
						switch(i_ext_bit)
						{
						case 1:
						case 3:
							//test is it read-only or not
							//TODO
							if (0)
							{								
								dev->dtype_ext |= MEL_DEV_TYPE_EXT_Mronly;
								dev->dtype_ext |= MEL_DEV_TYPE_EXT_Dronly;
							}							
							break;
						}
					}
					//else{//not system}			
				}else{isok = 0; break;}//error: wrong dev->dtype!
			}
		}
	}
	
	return (isok);
}

//returns: 1-yes,0-no;
int is_valid_E_device_fxcpu		(PLCRegDevice* dev)
{
	int is_valid = 0;//NG by default;

	//filter NaNs
	if (dev->const_flt == dev->const_flt)
	{
		double number = (double)((dev->const_flt < 0)? -dev->const_flt : dev->const_flt);

		//filter values by range
		if (number >= FLT_MIN && number <= FLT_MAX)
		{
			is_valid = 1;
		}
	}
	//else{//is NaN!}

	return(is_valid);
}

int is_valid_I_device_fxcpu		(PLCRegDevice* dev,int cpuType)
{
	int is_valid = 1;//OK by default;

	switch(dev->index)
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
			is_valid = 0;
		}
		break;

	default:
		//Timer interrupt (interrupt in constant cycle)
		if (	!(dev->index >= 610 && dev->index <= 699)
			&&	!(dev->index >= 710 && dev->index <= 799)
			&&	!(dev->index >= 810 && dev->index <= 899)						
			)
		{
			is_valid = 0;
		}
	}
	
	return(is_valid);
}

int is_device_in_range_fxcpu(PLCRegDevice* dev,int cpuType)
{
	int isok = 1;
	PLCRegDeviceRange*	t_range;
	PLCRegDeviceRange range={0};

	if (dev->dtype & (	 MEL_DEV_TYPE_E
						|MEL_DEV_TYPE_H2
						|MEL_DEV_TYPE_I
						)
		)
	{
		if (dev->dtype & MEL_DEV_TYPE_I)
		{
			isok = is_valid_I_device_fxcpu(dev,cpuType);
		}
		else
			if (dev->dtype & MEL_DEV_TYPE_E)
			{
				isok = is_valid_E_device_fxcpu(dev);
			}
			/*
			else
				if (dev->dtype & MEL_DEV_TYPE_H2)
				{
					isok = 1;/always OK
				}
				*/
	}
	else
	{
		t_range = get_device_range_fxcpu(	dev->dtype,dev->dtype_ext,cpuType,&range);
		if (t_range)
		{			
			if (dev->dtype & (MEL_DEV_TYPE_K | MEL_DEV_TYPE_H))
			{
				if (dev->dtype & MEL_DEV_TYPE_K)
				{
					int number = dev->const_int;
					if (number < range.lowest || number > range.highest)
					{
//						if (get_device_range_fxcpu(MEL_DEV_TYPE_K2,0,cpuType,&range))
//						{
// 							if (number < range.lowest || number > range.highest)
// 							{
// 								isok = 0;//impossible
// 							}
// 							else
// 							{
// 								//too big for 16-bit K, extend to 32-bit type:
// 								dev->dtype = MEL_DEV_TYPE_K2;
// 							}
//						}
						//too big for 16-bit K, extend to 32-bit type:
						dev->dtype = MEL_DEV_TYPE_K2;
					}
				}
				if (dev->dtype & MEL_DEV_TYPE_H)
				{
					unsigned int number = dev->const_uint;
					if (/*number < (unsigned int)range.lowest ||*/ number > (unsigned int)range.highest)
					{
						//too big for 16-bit H, extend to 32-bit type:
						dev->dtype = MEL_DEV_TYPE_H2;
					}
				}
			}
			else
			{
				if (dev->index < range.lowest || dev->index > range.highest)
				{
					isok = 0;
				}
			}
		}
		else
		{
			isok = 0;//unknown device type!
		}
	}

	return(isok);
}

PLCRegDeviceRange*	get_device_range_fxcpu(unsigned int dtype,unsigned int dtype_ext,int cpuType,PLCRegDeviceRange* range)
{
	//not supported by default
	PLCRegDeviceRange* t_range = 0;
	range->lowest	= 0;
	range->highest	= 0;

	if (//Filter devices with CPU-specific ranges
			!(dtype & (
						 MEL_DEV_TYPE_X
						|MEL_DEV_TYPE_Y
						|MEL_DEV_TYPE_T
						|MEL_DEV_TYPE_R
						|MEL_DEV_TYPE_P
						|MEL_DEV_TYPE_ER
						|MEL_DEV_TYPE_bI
						|MEL_DEV_TYPE_Un|MEL_DEV_TYPE_Gn
						|MEL_DEV_TYPE_STR
						)
			)
		)
	{	//COMMON RANGES FOR ALL CPUs

		if (dtype & MEL_DEV_TYPE_M)
		{
			if (dtype_ext & MEL_DEV_TYPE_EXT_Msys)
			{
				range->lowest	= 8000;
				range->highest	= 8511;
			}
			else
			{
				range->lowest	= 0;
				range->highest	= 7679;
			}
		}

		if (dtype & MEL_DEV_TYPE_D)
		{
			if (dtype_ext & MEL_DEV_TYPE_EXT_Dsys)
			{
				range->lowest	= 8000;
				range->highest	= 8511;
			}
			else
			{
				range->lowest	= 0;
				range->highest	= 7999;
			}
		}

		if (dtype_ext & MEL_DEV_TYPE_C)
		{
			if (dtype_ext & MEL_DEV_TYPE_EXT_C32bit)
			{
				range->lowest	= 200;
				range->highest	= 255;
			}
			else
			{
				range->lowest	= 0;
				range->highest	= 199;
			}
		}

		if (dtype & MEL_DEV_TYPE_S)
		{
			range->lowest	= 0;
			range->highest	= 4095;
		}

		if (dtype & (MEL_DEV_TYPE_N)
			)
		{
			range->lowest	= 0;
			range->highest	= 7;
		}
		
		if (dtype & (	 MEL_DEV_TYPE_V
						|MEL_DEV_TYPE_VI
						|MEL_DEV_TYPE_Z
						|MEL_DEV_TYPE_ZI
						)
			)
		{
			range->lowest	= 0;
			range->highest	= 7;
		}

		if (dtype & MEL_DEV_TYPE_Kn)
		{
			range->lowest	= 1;
			range->highest	= 8;
		}
	
		if (dtype & MEL_DEV_TYPE_K)
		{
			range->lowest	= -32768;
			range->highest	= +32767;
		}
		if (dtype & MEL_DEV_TYPE_K2)
		{
			range->lowest	= INT_MIN;
			range->highest	= INT_MAX;
		}

		if (dtype & MEL_DEV_TYPE_H)
		{
			range->lowest	= 0;
			range->highest	= 0xFFFF;
		}
	}
	else
	{
		switch(cpuType)
		{
		case MEL_PLC_FXCPU_TYPE_FX3U_C:

			if (dtype & MEL_DEV_TYPE_X)
			{
				range->lowest	= 0;
				range->highest	= 247;
			}

			if (dtype & MEL_DEV_TYPE_Y)
			{
				range->lowest	= 0;
				range->highest	= 247;
			}
			
			if (dtype & MEL_DEV_TYPE_T)
			{
				range->lowest	= 0;
				range->highest	= 511;
			}

			if (dtype & (MEL_DEV_TYPE_R|MEL_DEV_TYPE_ER))
			{
				range->lowest	= 0;
				range->highest	= 32767;
			}

			if (dtype & MEL_DEV_TYPE_P)
			{
				range->lowest	= 0;
				range->highest	= 4095;
			}
			
			if (dtype & MEL_DEV_TYPE_bI)
			{
				range->lowest	= 0;
				range->highest	= 0xF;
			}

			if (dtype & (MEL_DEV_TYPE_Un))
			{
				range->lowest	= 0;
				range->highest	= 7;
			}

			if (dtype & MEL_DEV_TYPE_Gn)
			{
				range->lowest	= 0;
				range->highest	= 32766;
			}

			if (dtype & MEL_DEV_TYPE_STR)
			{
				range->lowest	= 0;
				range->highest	= 32;
			}

			break;

		case MEL_PLC_FXCPU_TYPE_FX3G_C:

			if (dtype & MEL_DEV_TYPE_X)
			{
				range->lowest	= 0;
				range->highest	= 127;
			}
			if (dtype & MEL_DEV_TYPE_Y)
			{
				range->lowest	= 0;
				range->highest	= 127;
			}

			if (dtype & MEL_DEV_TYPE_T)
			{
				range->lowest	= 0;
				range->highest	= 319;
			}

			if (dtype & (MEL_DEV_TYPE_R|MEL_DEV_TYPE_ER))
			{
				range->lowest	= 0;
				range->highest	= 23999;
			}

			if (dtype & MEL_DEV_TYPE_P)
			{
				range->lowest	= 0;
				range->highest	= 2047;
			}


			break;
		}
	}
	//return non-zero pointer if range is not empty;
	if (range->lowest + range->highest)
	{
		t_range = range;
	}
	return(t_range);
}

#if 0
//M
General dtype 
	latched (battery backed) dtype
	Fixed latched (battery backed) dtype 
	Special dtype
	Fixed latched (EEPROM keep) dtype
	//S
	Initial state dtype 
	General dtype 
	Latched (battery backed) dtype
	Fixed latched (battery backed) dtype
	Annunciator dtype
	Initial state dtype (EEPROM keep)
	Fixed latched (EEPROM keep) dtype
	Annunciator dtype (EEPROM keep)
	General dtype
	//T
	For 100 ms pulses 0.1 to 3276.7 sec
	For 10 ms pulses 0.01 to 327.67 sec
	Retentive dtype for 1 ms pulses 0.001 to 32.767 sec (battery backed)
	Retentive dtype for 100 ms pulses 0.1 to 3276.7 sec (battery backed)
	For 1 ms pulses 0.001 to 32.767 sec
	Retentive dtype for 1 ms pulses 0.001 to 32.767 sec (EEPROM keep)
	Retentive dtype for 100 ms pulses 0.1 to 3276.7 sec (EEPROM keep)
	Potentiometer dtype 0 to 255 (numeric value)
	//C
	16-bit up counter Counting range: 0 to 32767
	32-bit bi-directional counter Counting range: ?2,147,483,648 to +2,147,483,647
	--
	General dtype
	Latched (battery backed) dtype (protected by battery against power failure)
	Latched (EEPROM keep) dtype
	--
	High-Speed Counters:
1-phase 1-count input
	1-phase 2-count input
	2-phase 2-count input

	//D
	General dtype
	Latched (battery backed) dtype (backed up by battery against power failure)
	Fixed latched (battery backed) dtype (backed up by battery against power failure)
	Special dtype
	File registers (latched (battery backed) dtype)
	Latched dtype (EEPROM keep)
	File registers (latched (EEPROM keep) dtype)

	//R
	Extension register (R) (latched [battery backed] dtype)
	Extension file register (ER) (file dtype)
	Extension register (R) (General dtype)

#endif
