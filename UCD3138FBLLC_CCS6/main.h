//###########################################################################
//
// FILE:    main.h
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

#ifndef main_h					// Header file guard
#define main_h


// The EXTERN macro is used to force all global variables to get a single definition.
// If it is main.c, define the variable and if desired initialize it's value.
// If it is not main.c, then just declare it as extern.
#ifdef MAIN_C 					// If it is the main routine
	#define EXTERN	 			// 	it isn't extern, so define it as nothing
#else 							// If it isn't the main routine
	#define EXTERN extern 		// 	it is extern
#endif

// Uncomment one of these two lines:
 	#define	RELEASE_CANDIDATE	(0)	// Intended for TI development.  Diagnostics enabled.
//	#define	RELEASE_CANDIDATE	(1)	// This is a candidate for official release.

//==========================================================================================
// Build options used to select the supported functions.
//==========================================================================================
#if RELEASE_CANDIDATE
	// If release candidate, only support real hardware, not simulator.
	#define	CCS_SIMULATOR	(0)	// Real hardware, not simulator
#else
	// If not release candidate, programmer can choose whether to use Code Composer 
	// simulator or not by uncommenting one of these two lines.
	#define	CCS_SIMULATOR	(0)	// Real hardware, not simulator
//	#define	CCS_SIMULATOR	(1)	// Code Composer Simulator. Will not work on real hardware.
#endif

//==========================================================================================
// #includes
//==========================================================================================
//#include "cyclone_device.h"			// Register and bit definitions for the 9240.

//==========================================================================================
// Macros
//==========================================================================================
#define min(a,b)					(((a)>(b)) ? (b):(a))
#define max(a,b)					(((a)>(b)) ? (a):(b))
#define saturate(x,LoLim,HiLim) 	((x) > (HiLim) ? (HiLim) : \
									((x) < (LoLim) ? (LoLim) : (x)))

#define set_bits(var, mask) 		((var) |= (mask))
#define clear_bits(var, mask) 		((var) &= (~(mask)))

#define set_flag(var, bit)			((var) = (var) | (1<<(bit)))
#define clear_flag(var, bit)		((var) = (var) & (~(1<<(bit))))
#define test_flag(var, bit)			(((var)>>(bit)) & 1)	// Returns 1 if bit set, else 0

#define	is_odd(x)					((x) & 1)				// Returns 1 if odd, else 0
#define	is_even(x)					( (~(x)) & 1 )			// Returns 1 if even, else 0

#define nop    						asm("    nop");

//==========================================================================================
// Hardware defined constants.
//==========================================================================================
// Error ADC Parameters
#define	EADC_BITS		6
#define MAX_EADC_VOLTS	(0.256)	// max range is +/-0.256 volts / gain setting of the error amp.

 // ADC12 Parameters
#define	ADC_BITS		12
#define	MAX_ADC_VOLTS	2.5		// 2.5V full scale

// DAC Parameters
#define MAX_DAC_VOLTS	1.6		// *****EGO Joe questioned this 1.6 or 2.0 ???
#define DAC_BITS		10

#pragma SWI_ALIAS (set_supervisor_mode, 10)
void set_supervisor_mode(void);

#pragma SWI_ALIAS (set_user_mode, 11)
void set_user_mode(void);

#pragma SWI_ALIAS (enable_fast_interrupt, 4)
void enable_fast_interrupt(void);

#pragma SWI_ALIAS (enable_interrupt, 6)
void enable_interrupt(void);


void errlog_reset(void);
void errlog_clear(void);
void end_simulation(void);
void errlog_set(Uint8 error_index);
void main_isr(void);


#endif // End of header guard: #ifndef main_h


