//###########################################################################
//
// FILE:    scale.c
//
// TITLE:
//
// NOTES:
//  1)
//###########################################################################
//
//  Ver  | dd mmm yyyy | Who  		| Description of changes
// ======|=============|============|========================================
//  00 	   06 15 2015 	 HPCS
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2008. All rights reserved.
//###########################################################################
#include "system_defines.h"
#include "cyclone_device.h"
#include "variables.h"
#include "pmbus.h"
#include "function_definitions.h"

//--------------------------------------------------------------
//               Uint32 qnote multiply linear 11 fit
//--------------------------------------------------------------
Uint32 qnote_linear11_multiply_fit(struct qnote x, int16 linear11, Uint32 max_value)
{
	struct qnote final,y;
	int32 mantissa;

	if(linear11 & 0x0400)             // if mantissa is negative
		y.mantissa = linear11 | 0xFC00;
	else
		y.mantissa = linear11 & 0x07FF;

	y.exponent = linear11 >> 11;

	final.exponent = x.exponent + y.exponent;
	mantissa = (int32)x.mantissa * (int32)y.mantissa; 

	while(final.exponent > 0)
	{
		mantissa = mantissa << 1;
		final.exponent = final.exponent - 1; 
	}            
	while(final.exponent < 0)
	{
		mantissa = mantissa >> 1;
		final.exponent = final.exponent + 1;
	}
	final.mantissa = mantissa;

	if(mantissa > max_value)
		mantissa = max_value;

	return mantissa;             
}

//--------------------------------------------------------------
//               Uint32 qnote multiply linear 16 fit
//--------------------------------------------------------------
Uint32 qnote_linear16_multiply_fit(struct qnote x, Uint16 linear16_mantissa, int8 vout_mode, Uint32 max_value)
{
	struct qnote final;
	Uint32 mantissa;

	if(vout_mode & 0x0010)             // if exponent is negative
		vout_mode = vout_mode | 0xFFE0;
	else
		vout_mode = vout_mode & 0x001F;

		final.exponent = x.exponent + vout_mode;
	mantissa = (Uint32)x.mantissa * (Uint32)linear16_mantissa; 

	while(final.exponent > 0)
	{
		mantissa = mantissa << 1;
		final.exponent = final.exponent - 1; 
	}            
	while(final.exponent < 0)
	{
		mantissa = mantissa >> 1;
		final.exponent = final.exponent + 1;
	}
	final.mantissa = mantissa;

	if(mantissa > max_value) 
		mantissa = max_value;

	return mantissa;             
}

//--------------------------------------------------------------
//               int32 qnote scale
//--------------------------------------------------------------
struct qnote qnote_scale_int32 (struct qnote x, int32 y)
{
	struct qnote final;
	int64 mantissa;
	final.exponent = x.exponent;

	mantissa = (int64)x.mantissa * (int64)y; 
	while(abs(mantissa) > 0x7FFF)
	{
		mantissa = mantissa >> 1;
		final.exponent = final.exponent + 1;
	}
	final.mantissa = mantissa;

	return final;
}

//--------------------------------------------------------------
//              int16 qnote scale
//--------------------------------------------------------------
struct qnote qnote_scale (struct qnote x, int16 y)
{
	struct qnote final;
	int32 mantissa;
	final.exponent = x.exponent;

	mantissa = (int32)x.mantissa * (int32)y; 
	while(abs(mantissa) > 0x00007FFF)
	{
		mantissa = mantissa >> 1;
		final.exponent = final.exponent + 1;
	}
	final.mantissa = mantissa;

	return final;
}

//--------------------------------------------------------------------
//          Change data from Linear11 Data Format to Qnote
//--------------------------------------------------------------------
struct qnote linear11_to_qnote (int16 linear11)
{
	struct qnote final;

	if(linear11 & 0x0400)             // if mantissa is negative
		final.mantissa = linear11 | 0xFC00;
	else
		final.mantissa = linear11 & 0x07FF;

	final.exponent = linear11 >> 11;

	return final;
}

//----------------------------------------------------------------------
//         Change data from Qnote to Linear11 Data Format
//----------------------------------------------------------------------
int16 qnote_to_linear11 (struct qnote x)
{
	int16 linear11;

	while(abs(x.mantissa) > 0x3FF)
	{
		x.mantissa = x.mantissa >> 1;
		x.exponent = x.exponent + 1;
	}
	linear11 = (x.exponent << 11) + (x.mantissa & 0x07FF);

	return linear11;
}

//----------------------------------------------------------
//                   Qnote add
//----------------------------------------------------------
struct qnote qnote_add(struct qnote x, struct qnote y)
{
	struct qnote final;
	int64 mantissa,mantissa_x,mantissa_y;

	mantissa_x = (int64)x.mantissa;
	mantissa_y = (int64)y.mantissa;

	while(x.exponent > y.exponent)
	{
		mantissa_x  = mantissa_x << 1;
		x.exponent = x.exponent -1;	
	}	
	while(x.exponent < y.exponent)
	{
		mantissa_y  = mantissa_y << 1;
		y.exponent = y.exponent -1;	
	}	
	final.exponent = x.exponent;
	mantissa = mantissa_x + mantissa_y;

	while(abs(mantissa) > 0x00007FFF)
	{
		mantissa = mantissa >> 1;
		final.exponent = final.exponent + 1;
	}
	final.mantissa = mantissa;

	return final;
}

//----------------------------------------------------------
//                  Qnote subtract
//----------------------------------------------------------
struct qnote qnote_subtract(struct qnote x, struct qnote y)
{
	struct qnote final;
	int64 mantissa,mantissa_x,mantissa_y;

	mantissa_x = (int64)x.mantissa;
	mantissa_y = (int64)y.mantissa;

	while(x.exponent > y.exponent)
	{
		mantissa_x  = mantissa_x << 1;
		x.exponent = x.exponent -1;	
	}	
	while(x.exponent < y.exponent)
	{
		mantissa_y  = mantissa_y << 1;
		y.exponent = y.exponent -1;	
	}	
	final.exponent = x.exponent;
	mantissa = mantissa_x - mantissa_y;

	while(abs(mantissa) > 0x00007FFF)
	{
		mantissa = mantissa >> 1;
		final.exponent = final.exponent + 1;
	}
	final.mantissa = mantissa;

	return final;
}


