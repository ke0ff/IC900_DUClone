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

extern	U8 digits0[25][2];
extern	U8 digits1[25][2];
extern	U8 digits2[25][2];
extern	U8 digits3[25][2];
extern	U8 digits4[25][2];
extern	U8 digits5[25][2];
extern	U8 digits6[25][2];
extern	U8 digits7[25][2];
extern	U8 digits8[25][2];
extern	U8 digits9[25][2];
extern	U8 digitmS[18][2];
extern	U8 digitsS[12][2];

extern	U8 segm_a[5][3];
extern	U8 segm_b[19];
extern	U8 segm_c[19];
extern	U8 segm_d[5][3];
extern	U8 segm_e[19][2];
extern	U8 segm_f[19][2];
extern	U8 segm_g[5][3];
extern	U8 segm_p[3];

extern	U8 segs_a[3][2];
extern	U8 segs_b[12];
extern	U8 segs_c[12];
extern	U8 segs_d[3][2];
extern	U8 segs_e[12];
extern	U8 segs_f[12];
extern	U8 segs_g[3][2];
extern	U8 segs_p[2];

#endif

#define SEGM_AX 0       // origin, x
#define SEGM_AY 0       // origin, y
#define SEGM_AO 0       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_BX 2       // origin, x
#define SEGM_BY 0       // origin, y
#define SEGM_BO 1       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_CX 2       // origin, x
#define SEGM_CY 19      // origin, y
#define SEGM_CO 1       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_DX 0       // origin, x
#define SEGM_DY 33      // origin, y
#define SEGM_DO 0       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_EX 0       // origin, x
#define SEGM_EY 19      // origin, y
#define SEGM_EO 2       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_FX 0       // origin, x
#define SEGM_FY 0       // origin, y
#define SEGM_FO 2       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_GX 0       // origin, x
#define SEGM_GY 16      // origin, y
#define SEGM_GO 0       // orientation, 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGM_PX 3       // origin, x
#define SEGM_PY 35      // origin, y
#define SEGM_PO 3       // orientation, 3=vert (4x1 array), 2=vert, (19x2 array), 1=vert (19x1 array), 0=horiz (5x3 array)

#define SEGS_AX 0       // origin, x
#define SEGS_AY 0       // origin, y
#define SEGS_AO 0       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_BX 1       // origin, x
#define SEGS_BY 0       // origin, y
#define SEGS_BO 1       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_CX 1       // origin, x
#define SEGS_CY 13      // origin, y
#define SEGS_CO 1       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_DX 0       // origin, x
#define SEGS_DY 22      // origin, y
#define SEGS_DO 0       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_EX 0       // origin, x
#define SEGS_EY 13      // origin, y
#define SEGS_EO 1       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_FX 0       // origin, x
#define SEGS_FY 0       // origin, y
#define SEGS_FO 1       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_GX 0       // origin, x
#define SEGS_GY 11      // origin, y
#define SEGS_GO 0       // orientation, 1=vert (12x1 array), 0=horiz (3x2 array)

#define SEGS_PX 2       // origin, x
#define SEGS_PY 23      // origin, y
#define SEGS_PO 2       // orientation, 2=vert (2x1 array), 1=vert (12x1 array), 0=horiz (3x2 array)

#define	DIGITDP_Y	3			// Y offset for dp (skips un-needed space in the digit area)
#define	DIGITMS_Y	18			// Y offset for small "S" (skips un-needed space in the digit area)
#define	DIGITSS_Y	(25-12)		// Y offset for small "S" (skips un-needed space in the digit area)
