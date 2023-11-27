/********************************************************************
 ************ COPYRIGHT (c) 2023 by ke0ff, Taylor, TX   *************
 *
 *  File name: segments.h
 *
 *  Module:    Control
 *
 *  Summary:   This is the header file for segments.c.
 *
 *******************************************************************/


/********************************************************************
 *  File scope declarations revision history:
 *    11-24-23 jmh:  creation date
 *
 *******************************************************************/

//------------------------------------------------------------------------------
// extern defines
//------------------------------------------------------------------------------

#ifndef	SEGINCL
extern	U8 digit0[38][3];
extern	U8 digit1[38][3];
extern	U8 digit2[38][3];
extern	U8 digit3[38][3];
extern	U8 digit4[38][3];
extern	U8 digit5[38][3];
extern	U8 digit6[38][3];
extern	U8 digit7[38][3];
extern	U8 digit8[38][3];
extern	U8 digit9[38][3];
extern	U8 digitdp[3];
#endif

#define	DIGITDP_Y	3			// Y offset for dp (skips un-needed space in the digit area)
#define	DIGITMS_Y	18			// Y offset for small "S" (skips un-needed space in the digit area)
#define	DIGITSS_Y	(25-12)		// Y offset for small "S" (skips un-needed space in the digit area)
