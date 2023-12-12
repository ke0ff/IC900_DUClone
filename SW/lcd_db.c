/********************************************************************
 ************ COPYRIGHT (c) 2023 by ke0ff, Taylor, TX   *************
 *
 *  File name: lcd_db.c
 *
 *  Module:    Control
 *
 *  Summary:   This is the parallel I/O module for the clone LCD
 *
 *******************************************************************/

/********************************************************************
 *  File scope declarations revision history:
 *    11-22-23 jmh:  creation date
 *
 *******************************************************************/

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "typedef.h"
#include "init.h"
#include "stdio.h"
#include "lcd_db.h"
#include "segments.h"
#include "graphics.h"
#include "spi.h"
#define	LCD_DB

//------------------------------------------------------------------------------
// Define Statements
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Local Variable Declarations
//-----------------------------------------------------------------------------

// dot matrix graphics sectors
U8	main7[MAIN7_LEN];		// main 7-seg DU memory
U8	sub7[SUB7_LEN];			// sub 7-seg DU memory
U8	mainsm[MAINSM_LEN];		// main smet row
U8	subsm[SUBSM_LEN];		// sub smet row
U8	optrow[OPTROW_LEN];		// opt row

U8	change_flag;			// bitmap of array changed flags

// uPD7225 registers
U8	CS1_reg;				// CS1 status register
U8	CS2_reg;				// CS2 status register

// uPD7225 memory arrays
// segment change memory array
U8	CS1_trig[LCD_MEMLEN];	// trigger
// segment on/off memory array
U8	CS1_dmem[LCD_MEMLEN];	// display mem
// blink on/off memory array
U8	CS1_bmem[LCD_MEMLEN];	// blink mem

// segment change memory array
U8	CS2_trig[LCD_MEMLEN];	// trigger
// segment on/off memory array
U8	CS2_dmem[LCD_MEMLEN];	// display mem
// blink on/off memory array
U8	CS2_bmem[LCD_MEMLEN];	// blink mem

U8	cs1_idx;
U8	cs2_idx;
//U8	cs1_idx;
//U8	cs2_idx;

// segment function LUTs.  The major index is the segment ram array index, the minor index is the
//		memory bit (0x4, 0x2, or 0x1 masks).  The function parameter is segment "ON" = 1 or "OFF" = 0

void (*cs1_fn[32][3])(U8) = {
		{ sg_op2s , sg_op2 , sg_op2m },							// 0x00
		{ sg_bcd_mem_m7b , sg_bcd_mem_m7c , null_fn },			// 0x01
		{ sg_bcd_mem_m7a , sg_bcd_mem_m7g , sg_bcd_mem_m7d },	// 0x02
		{ sg_bcd_mem_m7f , sg_bcd_mem_m7e , null_fn },			// 0x03
		{ sg_mdup , sg_mmin , sg_mskp },						// 0x04
		{ sg_mtne , sg_mmem , null_fn },						// 0x05
		{ null_fn , null_fn , sg_mm6 },							// 0x06
		{ sg_mm3 , sg_mm4 , sg_mm5 },							// 0x07
		{ sg_mm0 , sg_mm1 , sg_mm2 },							// 0x08
		{ null_fn , sg_m00 , null_fn },							// 0x09
		{ sg_bcd_m0b , sg_bcd_m0c , null_fn },					// 0x0A
		{ sg_bcd_m0a , sg_bcd_m0g , sg_bcd_m0d },				// 0x0B
		{ sg_bcd_m0f , sg_bcd_m0e , null_fn },					// 0x0C
		{ sg_bcd_m1b , sg_bcd_m1c , null_fn },					// 0x0D
		{ sg_bcd_m1a , sg_bcd_m1g , sg_bcd_m1d },				// 0x0E
		{ sg_bcd_m1f , sg_bcd_m1e , null_fn },					// 0x0F
		{ sg_bcd_m2b , sg_bcd_m2c , null_fn },					// 0x10
		{ sg_bcd_m2a , sg_bcd_m2g , sg_bcd_m2d },				// 0x11
		{ sg_bcd_m2f , sg_bcd_m2e , null_fn },					// 0x12
		{ sg_bcd_m3b , sg_bcd_m3c , null_fn },					// 0x13
		{ sg_bcd_m3a , sg_bcd_m3g , sg_bcd_m3d },				// 0x14
		{ sg_bcd_m3f , sg_bcd_m3e , null_fn },					// 0x15
		{ sg_bcd_m4b , sg_bcd_m4c , null_fn },					// 0x16
		{ sg_bcd_m4a , sg_bcd_m4g , sg_bcd_m4d },				// 0x17
		{ sg_bcd_m4f , sg_bcd_m4e , null_fn },					// 0x18
		{ sg_bcd_m5b , sg_bcd_m5c , null_fn },					// 0x19
		{ sg_bcd_m5a , sg_bcd_m5g , sg_bcd_m5d },				// 0x1A
		{ sg_bcd_m5f , sg_bcd_m5e , null_fn },					// 0x1B
		{ sg_bcd_m6bc , sg_mdp2 , sg_mdp },						// 0x1C
		{ sg_ow , sg_low , null_fn },							// 0x1D
		{ sg_tss , sg_tsq , sg_mss },							// 0x1E
		{ sg_rit , sg_vxo , sg_ts }								// 0x1F
};

void (*cs2_fn[32][3])(U8) = {
		{ sg_op1s , sg_op1 , sg_op1m },							// 0x00
		{ sg_dsm , sg_dsq , sg_dss },							// 0x01
		{ sg_bcd_mem_s7b , sg_bcd_mem_s7c , null_fn },			// 0x02
		{ sg_bcd_mem_s7a , sg_bcd_mem_s7g , sg_bcd_mem_s7d },	// 0x03
		{ sg_bcd_mem_s7f , sg_bcd_mem_s7e , null_fn },			// 0x04
		{ sg_sdup , sg_smin , sg_sskp },						// 0x05
		{ sg_stne , sg_smem , null_fn },						// 0x06
		{ null_fn , sg_s00 , null_fn },							// 0x07
		{ sg_bcd_s0b , sg_bcd_s0c , null_fn },					// 0x08
		{ sg_bcd_s0a , sg_bcd_s0g , sg_bcd_s0d },				// 0x09
		{ sg_bcd_s0f , sg_bcd_s0e , null_fn },					// 0x0A
		{ sg_bcd_s1b , sg_bcd_s1c , null_fn },					// 0x0B
		{ sg_bcd_s1a , sg_bcd_s1g , sg_bcd_s1d },				// 0x0C
		{ sg_bcd_s1f , sg_bcd_s1e , null_fn },					// 0x0D
		{ sg_bcd_s2b , sg_bcd_s2c , null_fn },					// 0x0E
		{ sg_bcd_s2a , sg_bcd_s2g , sg_bcd_s2d },				// 0x0F
		{ sg_bcd_s2f , sg_bcd_s2e , null_fn },					// 0x10
		{ sg_bcd_s3b , sg_bcd_s3c , null_fn },					// 0x11
		{ sg_bcd_s3a , sg_bcd_s3g , sg_bcd_s3d },				// 0x12
		{ sg_bcd_s3f , sg_bcd_s3e , null_fn },					// 0x13
		{ sg_bcd_s4b , sg_bcd_s4c , null_fn },					// 0x14
		{ sg_bcd_s4a , sg_bcd_s4g , sg_bcd_s4d },				// 0x15
		{ sg_bcd_s4f , sg_bcd_s4e , null_fn },					// 0x16
		{ sg_bcd_s5b , sg_bcd_s5c , null_fn },					// 0x17
		{ sg_bcd_s5a , sg_bcd_s5g , sg_bcd_s5d },				// 0x18
		{ sg_bcd_s5f , sg_bcd_s5e , null_fn },					// 0x19
		{ sg_bcd_s6bc , sg_sdp2 , sg_sdp },						// 0x1A
		{ null_fn , null_fn , sg_sm6 },							// 0x1B
		{ sg_sm3 , sg_sm4 , sg_sm5 },							// 0x1C
		{ sg_sm0 , sg_sm1 , sg_sm2 },							// 0x1D
		{ sg_sub , sg_lck , null_fn },							// 0x1E
		{ sg_prg , sg_mhz , sg_bnd }							// 0x1F
};

//	(*fun_ptr_arr[ch])(a, b);

U8 err1[7][5] = {
		{ 0x39 , 0xE7 , 0x39 , 0x17 , 0xA0 },
		{ 0x45 , 0x12 , 0x45 , 0x14 , 0x20 },
		{ 0x41 , 0x12 , 0x45 , 0x14 , 0x20 },
		{ 0x39 , 0xE2 , 0x45 , 0x17 , 0x20 },
		{ 0x05 , 0x02 , 0x45 , 0x14 , 0x20 },
		{ 0x45 , 0x02 , 0x44 , 0xA4 , 0x20 },
		{ 0x39 , 0x07 , 0x38 , 0x44 , 0x3C },
};

U8 err2[7][5] = {
		{0x1C , 0xF3 , 0x83 , 0xE7 , 0x00},
		{0x22 , 0x89 , 0x00 , 0x88 , 0x80},
		{0x20 , 0x89 , 0x00 , 0x88 , 0x80},
		{0x1C , 0xF1 , 0x00 , 0x88 , 0x80},
		{0x02 , 0x81 , 0x00 , 0x88 , 0x80},
		{0x22 , 0x81 , 0x00 , 0x88 , 0x80},
		{0x1C , 0x83 , 0x80 , 0x87 , 0x00}
};

U8	seg7[16][3] = {		// 7-seg encoder LUT
		{3, 5, 3},		// 0
		{3, 0, 0},		// 1
		{1, 7, 2},		// 2
		{3, 7, 0},		// 3
		{3, 2, 1},		// 4
		{2, 7, 1},		// 5
		{2, 7, 3},		// 6
		{3, 1, 0},		// 7
		{3, 7, 3},		// 8
		{3, 7, 1},		// 9
		{0, 2, 0},		// a
		{0, 7, 3},		// b
		{0, 5, 3},		// c
		{0, 6, 0},		// d
		{2, 6, 2},		// e
		{0, 0, 0}		// f
};

//-----------------------------------------------------------------------------
// Local Fn Declarations
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// process_LCD() handles lcd updates
//-----------------------------------------------------------------------------
void process_LCD(U8 iplfl){
			U8	i;
			U8	j;
			U8	m;
			U8	k;
	static	U8	blinker;

	if(iplfl){
		// initialize LCD and internals
		CS1_reg = 0;
		CS2_reg = 0;
		lcd_setup();
		clear_all();
		// force update
		change_flag = MAIN7_FL|SUB7_FL|MAINSM_FL|SUBSM_FL|OPTROW_FL;
		blinker = 0;
	}
	if((CS1_reg|CS2_reg) & CS_NEBLINK){					// if blink enabled...
		if(is_blink()){									// and a new blink cycle has triggered
			// process blink
			if(blinker&0x01){							// blink segments = ON
				for(i=0; i<LCD_MEMLEN; i++){
					m = CS1_bmem[i] & CS1_dmem[i];
					if(m){
						for(j=0; j<3; j++){
							k = m & 0x01;
							if(k) (*cs1_fn[i][j])(k);
							m >>= 1;
						}
					}
					m = CS2_bmem[i] & CS2_dmem[i];
					if(m){
						for(j=0; j<3; j++){
							k = m & 0x01;
							if(k) (*cs2_fn[i][j])(k);
							m >>= 1;
						}
					}
				}
			}else{										// blink segments = OFF
				for(i=0; i<LCD_MEMLEN; i++){
					m = CS1_bmem[i];
					if(m){
						for(j=0; j<3; j++){
							k = m & 0x01;
							if(k) (*cs1_fn[i][j])(0);
							m >>= 1;
						}
					}
					m = CS2_bmem[i];
					if(m){
						for(j=0; j<3; j++){
							k = m & 0x01;
							if(k) (*cs2_fn[i][j])(0);
							m >>= 1;
						}
					}
				}
			}
			blinker++;
		}
	}
	// if SPI timer expired and change_flag has set bits, update LCD
	trig_scan1(MODE_OR);
	trig_scan2(MODE_OR);
	if(change_flag && is_ssito()){
		kick_ssito();
		for(i=OPTROW_FL; i; i<<=1){
			switch(i&change_flag){
			case MAIN7_FL:
				wrlcd_str(main7, MAIN7_LEN, MAIN7_OFFS);
				break;

			case SUB7_FL:
				wrlcd_str(sub7, SUB7_LEN, SUB7_OFFS);
				break;

			case MAINSM_FL:
				wrlcd_str(mainsm, MAINSM_LEN, MAINSM_OFFS);
				break;

			case SUBSM_FL:
				wrlcd_str(subsm, SUBSM_LEN, SUBSM_OFFS);
				break;

			case OPTROW_FL:
				wrlcd_str(optrow, OPTROW_LEN, OPTROW_OFFS);
				break;

			default:
				break;
			}
		}
		change_flag = 0;
	}
	return;
}

/*
// CSn_reg defines:
#define	CS_DECODE7	0x01			// 7-seg decoder enabled
#define	CS_FBLINK	0x02			// fast blink
#define	CS_SBLINK	0x04			// slow blink
#define	CS_DISPON	0x08			// disp enable

#define	MODE_SET	0x49			// /3 time-div, 1/3 bias, 2E-8 fdiv
#define	BLINK_SLOW	0x1A			// low-bit is flash-rate
#define	BLINK_FAST	0x1B			//  "   " ...
#define	BLINK_OFF	0x18			// disable blink
#define	DISP_ON		0x11			// enable disp
#define	DISP_OFF	0x10			// blank disp
#define	WITH_DECODE	0x15			// 7-seg decode
#define	WITHOUT_DECODE	0x14		// no decode
#define	LOAD_PTR	0xE0			// OR with 0x1f (masked address)
#define	LOAD_PTR2	0xF0			// switch alternate
#define	AMASK		0x1F			// address mask
#define	DMASK		0x0F			// data mask
#define	WR_DMEM		0xD0			// write with (0x0f masked data)
#define	OR_DMEM		0xB0			// OR with (0x0f masked data)
#define	AND_DMEM	0x90			// AND with (0x0f masked data)
#define	CLR_DMEM	0x20			// clear disp. mem
#define	WR_BMEM		0xC0			// write with (0x0f masked data)
#define	OR_BMEM		0xA0			// OR with (0x0f masked data)
#define	AND_BMEM	0x80			// AND with (0x0f masked data)
#define	CLR_BMEM	0x00			// clear blink. mem*/

//-----------------------------------------------------------------------------
// process_SPI() fetches SPI data and processes cmds/data
//-----------------------------------------------------------------------------

void process_SPI(U8 iplfl){
	U16	ii;
	U8	sdata;
	U8	csf1;
	U8	csf2;
	U8	swdat;
//	U8	ilast = 0;;
	U8	i;
	U8	datcmd;
	U8	skip1;
	U8	skip2;

	if(iplfl){
		// IPL init
	}
	while(got_ssi0()){
		skip1 = 0;
		skip2 = 0;
		ii = get_ssi0();
		sdata = (U8)ii;
		i = (U8)(ii >> 8);
		csf1 = i & CS1;
		csf2 = i & CS2;
		datcmd = i & DATA_CMD;

/*		if(csf1){
			if((csf1 & ilast) == 0){
				cs1_idx = 0;
			}
		}
		if(csf2){
			if((csf2 & ilast) == 0){
				cs2_idx = 0;
			}
		}
		ilast = i;*/
		// if data & decode7, set up for write
		if(datcmd){
			if(csf1){
				if(CS1_reg & CS_DECODE7){	// 7-seg decode
					CS1_trig[cs1_idx] = seg7[sdata & DMASK][0] | DATA_RDY | MODE_WR;
					CS1_trig[cs1_idx+1] = seg7[sdata & DMASK][1] | DATA_RDY | MODE_WR;
					CS1_trig[cs1_idx+2] = seg7[sdata & DMASK][2] | DATA_RDY | MODE_WR;
					cs1_idx += 3;
					if(cs1_idx > 0x1f) cs1_idx = 0;
				}else{						// no decode
					CS1_trig[cs1_idx] = ((sdata & DMASK0) >> DSHFT0) | DATA_RDY | MODE_WR;
					CS1_trig[cs1_idx+1] = ((sdata & DMASK1) >> DSHFT1) | DATA_RDY | MODE_WR;
					CS1_trig[cs1_idx+2] = ((sdata & DMASK2) >> DSHFT2) | DATA_RDY | MODE_WR;
					cs1_idx += 3;
					if(cs1_idx > 0x1f) cs1_idx = 0;
				}
				skip1 = 1;
			}
			if(csf2){
				if(CS2_reg & CS_DECODE7){	// 7-seg decode
					CS2_trig[cs2_idx] = seg7[sdata & DMASK][0] | DATA_RDY | MODE_WR;
					CS2_trig[cs2_idx+1] = seg7[sdata & DMASK][1] | DATA_RDY | MODE_WR;
					CS2_trig[cs2_idx+2] = seg7[sdata & DMASK][2] | DATA_RDY | MODE_WR;
					cs2_idx += 3;
					if(cs2_idx > 0x1f) cs2_idx = 0;
				}else{						// no decode
					CS2_trig[cs2_idx] = ((sdata & DMASK0) >> DSHFT0) | DATA_RDY | MODE_WR;
					CS2_trig[cs2_idx+1] = ((sdata & DMASK1) >> DSHFT1) | DATA_RDY | MODE_WR;
					CS2_trig[cs2_idx+2] = ((sdata & DMASK2) >> DSHFT2) | DATA_RDY | MODE_WR;
					cs2_idx += 3;
					if(cs2_idx > 0x1f) cs2_idx = 0;
				}
				skip2 = 1;
			}
		}
		if(csf1 && !skip1){
			if(sdata & 0xC0){						// is a parametric command?
				swdat = sdata & 0xf0;				// mask off data to get command
				switch(swdat){
				default:
					break;

				case LOAD_PTR:
				case LOAD_PTR2:
					cs1_idx = sdata & AMASK;
					cs1_idx = cs1_idx;
					break;

				case WR_DMEM:
					CS1_trig[cs1_idx] = (sdata & BIT_MASK) | DATA_RDY | MODE_WR;
					if(++cs1_idx > 0x1f) cs1_idx = 0;
					break;

				case OR_DMEM:
					CS1_trig[cs1_idx] = (sdata & BIT_MASK) | DATA_RDY | MODE_OR;
					if(++cs1_idx > 0x1f) cs1_idx = 0;
					break;

				case AND_DMEM:
					CS1_trig[cs1_idx] = (sdata & BIT_MASK) | DATA_RDY | MODE_AND;
					if(++cs1_idx > 0x1f) cs1_idx = 0;
					break;
					////////////

				case WR_BMEM:
					CS1_bmem[cs1_idx] = (sdata & BIT_MASK);
					if(++cs1_idx > 0x1f) cs1_idx = 0;
					break;

				case OR_BMEM:
					CS1_bmem[cs1_idx] |= (sdata & BIT_MASK);
					if(++cs1_idx > 0x1f) cs1_idx = 0;
					break;

				case AND_BMEM:
					CS1_bmem[cs1_idx] &= (sdata & BIT_MASK);
					if(++cs1_idx > 0x1f) cs1_idx = 0;
					break;
				}
			}else{
				switch(sdata){		// not a parametric, use the whole byte to dispatch
				case WITH_DECODE:
					CS1_reg |= CS_DECODE7;
					break;

				case WITHOUT_DECODE:
					CS1_reg &= ~CS_DECODE7;
					break;

				case CLR_DMEM:
					for(i=0; i<0x1f; i++){
						CS1_trig[i] = 0x0f;
					}
					break;

				case CLR_BMEM:
					for(i=0; i<0x1f; i++){
						CS1_trig[i] = 0x0f | BLINKFL;
					}
					break;

				case BLINK_SLOW:
					CS1_reg = (CS1_reg & ~CS_NEBLINK) | CS_SBLINK;
					break;

				case BLINK_FAST:
					CS1_reg = (CS1_reg & ~CS_NEBLINK) | CS_FBLINK;
					break;

				case BLINK_OFF:
					CS1_reg = (CS1_reg & ~CS_NEBLINK);
					for(i=0; i<LCD_MEMLEN; i++){
						CS1_trig[i] = CS1_bmem[i] | MODE_OR;
					}
					break;

				case MODE_SET:
				case DISP_ON:
				case DISP_OFF:
				default:
					break;
				}
			}
		}
		if(csf2 && !skip2){
			if(sdata & 0xC0){						// is a parametric command?
				swdat = sdata & 0xf0;				// mask off data to get command
				switch(swdat){
				default:
					break;

				case LOAD_PTR:
				case LOAD_PTR2:
					cs2_idx = sdata & AMASK;
					cs2_idx = cs2_idx;
					break;

				case WR_DMEM:
					CS2_trig[cs2_idx] = (sdata & BIT_MASK) | DATA_RDY | MODE_WR;
					if(++cs2_idx > 0x1f) cs2_idx = 0;
					break;

				case OR_DMEM:
					CS2_trig[cs2_idx] = (sdata & BIT_MASK) | DATA_RDY | MODE_OR;
					if(++cs2_idx > 0x1f) cs2_idx = 0;
					break;

				case AND_DMEM:
					CS2_trig[cs2_idx] = (sdata & BIT_MASK) | DATA_RDY | MODE_AND;
					if(++cs2_idx > 0x1f) cs2_idx = 0;
					break;
					////////////

				case WR_BMEM:
					CS2_bmem[cs2_idx] = (sdata & BIT_MASK);
					if(++cs2_idx > 0x1f) cs2_idx = 0;
					break;

				case OR_BMEM:
					CS2_bmem[cs2_idx] |= (sdata & BIT_MASK);
					if(++cs2_idx > 0x1f) cs2_idx = 0;
					break;

				case AND_BMEM:
					CS2_bmem[cs2_idx] &= (sdata & BIT_MASK);
					if(++cs2_idx > 0x1f) cs2_idx = 0;
					break;
				}
			}else{
				switch(sdata){		// not a parametric, use the whole byte to dispatch
				case WITH_DECODE:
					CS2_reg |= CS_DECODE7;
					break;

				case WITHOUT_DECODE:
					CS2_reg &= ~CS_DECODE7;
					break;

				case CLR_DMEM:
					for(i=0; i<0x1f; i++){
						CS2_trig[i] = 0x0f;
					}
					break;

				case CLR_BMEM:
					for(i=0; i<0x1f; i++){
						CS2_trig[i] = 0x0f | BLINKFL;
					}
					break;

				case BLINK_SLOW:
					CS2_reg = (CS2_reg & ~CS_NEBLINK) | CS_SBLINK;
					break;

				case BLINK_FAST:
					CS2_reg = (CS2_reg & ~CS_NEBLINK) | CS_FBLINK;
					break;

				case BLINK_OFF:
					CS2_reg = (CS2_reg & ~CS_NEBLINK);
					for(i=0; i<LCD_MEMLEN; i++){
						CS2_trig[i] = CS2_bmem[i] | MODE_OR;
					}
					break;

				case MODE_SET:
				case DISP_ON:
				case DISP_OFF:
				default:
					break;
				}
			}
		}
	}
	return;
}

//-----------------------------------------------------------------------------
// set CSn_reg
//-----------------------------------------------------------------------------
void set_csn(U8 m, U8 n){

	CS1_reg = m;
	CS2_reg = n;
}

//-----------------------------------------------------------------------------
// Clear pixel mem arrays
// clear_main7() - main
// clear_sub7() - sub
// clear_optrow() - sub
// clear_mainsm() - main sm
// clear_subsm() - subsm
// clear_all() - all of the above
//-----------------------------------------------------------------------------
void clear_main7(void){
	U16	ii;

	for(ii=0; ii<MAIN7_LEN; ii++){
		main7[ii] = 0;
	}
	return;
}

void clear_sub7(void){
	U16	ii;

	for(ii=0; ii<SUB7_LEN; ii++){
		sub7[ii] = 0;
	}
	return;
}

void clear_optrow(void){
	U16	ii;

	for(ii=0; ii<OPTROW_LEN; ii++){
		optrow[ii] = 0;
	}
	return;
}

void clear_mainsm(void){
	U16	ii;

	for(ii=0; ii<MAINSM_LEN; ii++){
		mainsm[ii] = 0;
	}
	return;
}

void clear_subsm(void){
	U16	ii;

	for(ii=0; ii<SUBSM_LEN; ii++){
		subsm[ii] = 0;
	}
	return;
}

void clear_all(void){
	U8	i;

	clear_main7();
	clear_sub7();
	clear_optrow();
	clear_mainsm();
	clear_subsm();
	for(i=0; i<LCD_MEMLEN; i++){
		CS1_trig[i] = 0;
		CS1_dmem[i] = 0;
		CS1_bmem[i] = 0;
		CS2_trig[i] = 0;
		CS2_dmem[i] = 0;
		CS2_bmem[i] = 0;
	}
	CS1_reg = 0;
	CS2_reg = 0;
	change_flag = 0;
	return;
}

//-----------------------------------------------------------------------------
// disp_err() displays system errors on bottom edge of LCD
//-----------------------------------------------------------------------------
void disp_err(U8 mnum){
	U8	i;
	U8	j;
	U16 addr;

	if((mnum > 2) || (mnum == 0)) return;
	if(mnum == 1) addr = ERR_OFFS;							// msg1 pixel address
	if(mnum == 2) addr = ERR2_OFFS;							// msg2 pixel address
	wrdb(0x98, LCDCMD, STA01);								// graphics ON, text OFF
	for(j=0; j<7; j++){
		wrdb((uint8_t)(addr&0xff), LCDDATA, STA01);			// pixel address
		wrdb((uint8_t)(addr>>8), LCDDATA, STA01);			// pixel address
		wrdb(0x24, LCDCMD, STA01);							// set address pointer
		wrdb(0xB0, LCDCMD, STA01);							// auto write ON
		for(i=0;i<5;i++){    								// send msg
			if(mnum == 1) wrdb(err1[j][i], LCDDATA, STA23);	// pixel address
			if(mnum == 2) wrdb(err2[j][i], LCDDATA, STA23);	// pixel address
		}
		wrdb(0xB2, LCDCMD, STA01);							// auto write OFF
		addr += DROW;
	}
	return;
}

//-----------------------------------------------------------------------------
// lcdClear() clear lcd display RAM
//-----------------------------------------------------------------------------
void lcdClear(void){
	uint16_t i;

	wrdb(0x9c, LCDCMD, STA01);				//graphics ON, text OFF
	wrdb(0x00, LCDDATA, STA01);
	wrdb(0x40, LCDDATA, STA01);
	wrdb(0x24, LCDCMD, STA01);				//set address pointer = 0x0040 (first pixel)
	wrdb(0xB0, LCDCMD, STA01);				//auto write ON
	for(i=0;i<30720/8;i++)					//fill screen with pixels
	{
		wrdb(0x00, LCDDATA, STA23);
	}
	wrdb(0xB2, LCDCMD, STA01);				//auto write OFF
	return;
}

//-----------------------------------------------------------------------------
// wrlcd_str() Send LCD data
//	string pointer
//	string length
//	LCD addr
//-----------------------------------------------------------------------------
void wrlcd_str(U8 *lcd_string, U16 strlen, U16 lcd_addr){
	uint16_t	i;
	uint8_t*	dp;

	wrdb(0x98, LCDCMD, STA01);				//graphics ON, text ON
	wrdb((U8)(lcd_addr&0xff), LCDDATA, STA01);
	wrdb((U8)(lcd_addr>>8), LCDDATA, STA01);
	wrdb(0x24, LCDCMD, STA01);				//set address pointer (first pixel, upper left corner)
	wrdb(0xB0, LCDCMD, STA01);				//auto write ON

	dp = lcd_string;
	for(i=0;i<strlen;i++)    //fill screen with pixels
	{
		wrdb(*dp++, LCDDATA, STA23);
	}
	wrdb(0xB2, LCDCMD, STA01);				//auto write OFF
	return;
}
/*
// examples:
wrlcd_str(main7, MAIN7_LEN, MAIN7_OFFS);
wrlcd_str(sub7, SUB7_LEN, SUB7_OFFS);
wrlcd_str(mainsm, MAINSM_LEN, MAINSM_OFFS);
wrlcd_str(optrow, OPTROW_LEN, OPTROW_OFFS);
wrlcd_str(subsm, SUBSM_LEN, SUBSM_OFFS);
*/

/****************************************************
* Initialization For controller + Setup, to run once*
*****************************************************/

void lcd_setup(void){
	GPIO_PORTB_DIR_R = 0;				// port = in
	GPIO_PORTE_DATA_R = nTRD|nTWR|nTCS;
	lcd_cntl(LDSC_RESET);
	wait(20);

	wrdb(0x00, LCDDATA, STA01);
	wrdb(0x40, LCDDATA, STA01);
	wrdb(0x42, LCDCMD, STA01);			//graphic home address

	wrdb(0x1E, LCDDATA, STA01);
	wrdb(0x00, LCDDATA, STA01);
	wrdb(0x41, LCDCMD, STA01);			//text area

	wrdb(0x1E, LCDDATA, STA01);
	wrdb(0x00, LCDDATA, STA01);
	wrdb(0x43, LCDCMD, STA01);			//graphic area

	wrdb(0x80, LCDCMD, STA01);			//OR mode

	wait(10);
	lcdClear();
	clear_all();
}

///////////////////////////////////////////////////////////////////////////////
// drivers

#define	TLEN	1

//-----------------------------------------------------------------------------
// wrdb() writes data to parallel  port
//-----------------------------------------------------------------------------
void wrdb(U8 data, U8 addr, U8 mask){
	U8	css = nTRD|nTWR|nTCS; //|addr;
	U8	i;

	do{
		i = rddb(1);
		i &= mask;
	}while(i != mask);
	GPIO_PORTB_DIR_R = 0xff;				// port = out
	GPIO_PORTB_DATA_R = swapeo(data); //data; // store data to port
	if(addr) css |= TADDR;
	GPIO_PORTE_DATA_R = css;				// store addr, bus idle
	GPIO_PORTE_DATA_R = css & ~(nTWR|nTCS);	// set OE & WR
	for(i=0; i<TLEN; i++);
	GPIO_PORTE_DATA_R = css & ~(nTWR);		// store bus idle
	for(i=0; i<TLEN; i++);
	GPIO_PORTE_DATA_R = css;				// store bus idle
	GPIO_PORTB_DIR_R = 0;					// port = in

//	wait(10);

	return;
}

//-----------------------------------------------------------------------------
// rddb() reads data from parallel  port
//-----------------------------------------------------------------------------
U8 rddb(U8 addr){
	U8	css = nTRD|nTWR|nTCS; //|addr;	// init port control to idle
	U8	data;
	U8	i;

	GPIO_PORTB_DIR_R = 0;					// port = in
	if(addr) css |= TADDR;
	GPIO_PORTE_DATA_R = css;				// store addr, bus idle
	GPIO_PORTE_DATA_R = css & ~(nTRD|nTCS);	// set OE & RD
	for(i=0; i<TLEN; i++);
	data = GPIO_PORTB_DATA_R;				// get data from port
	GPIO_PORTE_DATA_R = css;				// store bus idle
	for(i=0; i<TLEN; i++);
	return swapeo(data);
}

//-----------------------------------------------------------------------------
// lcd_cntl() sets control lines for parallel  port.
// if cmd = 0xff, re-init the port lines to idle
//-----------------------------------------------------------------------------

void lcd_cntl(U8 cmd){

	switch(cmd){
	case LDSC_RESET:
		GPIO_PORTE_DATA_R |= TRESET;
		wait(20);
		GPIO_PORTE_DATA_R &= ~TRESET;			// toggle reset
		break;

	default:									// init port
	case LDSC_INIT:
		GPIO_PORTB_DIR_R = PORTB_DIRV;
		GPIO_PORTE_DATA_R = PORTE_INIT;
		wait(20);
		GPIO_PORTE_DATA_R &= ~TRESET;			// toggle reset
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// lcd_stat() reads status condition and traps errors
//-----------------------------------------------------------------------------

U8 lcd_stat(U8 mask){
	U8	i;

	do{
		i = rddb(1);
		if(i&STA6){
			i |= 0x10;
		}
		i &= mask;
	}while(i != mask);
	return i;
}

//-----------------------------------------------------------------------------
// debug_db() writes data directly to control port
//-----------------------------------------------------------------------------
void debug_db(U8 data){

	GPIO_PORTE_DATA_R = data;				// store control data
	return;
}


//-----------------------------------------------------------------------------
// wr_mseg() writes 1-of-7-seg LCD memory (main)
//  seg 0-7 = a-g
//-----------------------------------------------------------------------------
void wr_mseg(U8 seg, U8 blank, U16 daddr){
    U8  mode;   // source array type 0,1,2
    U16 addr = daddr;
    U8* iptr;
    U8  i;
    U8  j;

    switch(seg){
    case CSEGA: // a
        addr += SEGM_AX + (SEGM_AY*DROW);
        mode = SEGM_AO;
        iptr = *segm_a;
        break;

    case CSEGB: // b
        addr += SEGM_BX + (SEGM_BY*DROW);
        mode = SEGM_BO;
        iptr = segm_b;
        break;

    case CSEGC: // c
        addr += SEGM_CX + (SEGM_CY*DROW);
        mode = SEGM_CO;
        iptr = segm_c;
        break;

    case CSEGD: // d
        addr += SEGM_DX + (SEGM_DY*DROW);
        mode = SEGM_DO;
        iptr = *segm_d;
        break;

    case CSEGE: // e
        addr += SEGM_EX + (SEGM_EY*DROW);
        mode = SEGM_EO;
        iptr = *segm_e;
        break;

    case CSEGF: // f
        addr += SEGM_FX + (SEGM_FY*DROW);
        mode = SEGM_FO;
        iptr = *segm_f;
        break;

    case CSEGG: // g
        addr += SEGM_GX + (SEGM_GY*DROW);
        mode = SEGM_GO;
        iptr = *segm_g;
        break;

    case CSEGP: // dp
        addr += SEGM_PX + (SEGM_PY*DROW);
        mode = SEGM_PO;
        iptr = segm_p;
        break;

    default:
        mode = 0xff;
        break;
    }
    switch(mode){
    case 0:
        for(j=0; j<5; j++){
            for(i=0; i<3; i++){
                if(blank){
                    main7[addr+i] &= ~(*iptr++);
                }else{
                    main7[addr+i] |= *iptr++;
                }
            }
            addr += DROW;
        }
        break;

    case 1:
        for(j=0; j<19; j++){
            if(blank){
                main7[addr] &= ~(*iptr++);
            }else{
                main7[addr] |= *iptr++;
            }
            addr += DROW;
        }
        break;

    case 2:
        for(j=0; j<19; j++){
            for(i=0; i<2; i++){
                if(blank){
                    main7[addr+i] &= ~(*iptr++);
                }else{
                    main7[addr+i] |= *iptr++;
                }
            }
            addr += DROW;
        }
        break;

    case 3:
        for(j=0; j<4; j++){
            if(blank){
                main7[addr] &= ~(*iptr++);
            }else{
                main7[addr] |= *iptr++;
            }
            addr += DROW;
        }
        break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------
// wr_sseg() writes 1-of-7-seg LCD memory (sub)
//  seg 0-7 = a-g
//-----------------------------------------------------------------------------
void wr_sseg(U8 seg, U8 blank, U16 daddr, U8* aryptr){
    U8  mode;   // source array type 0,1,2
    U16 addr = daddr;
    U8* iptr;
    U8  i;
    U8  j;

    switch(seg){
    case CSEGA: // a
        addr += SEGS_AX + (SEGM_AY*DROW);
        mode = SEGS_AO;
        iptr = *segs_a;
        break;

    case CSEGB: // b
        addr += SEGS_BX + (SEGM_BY*DROW);
        mode = SEGS_BO;
        iptr = segs_b;
        break;

    case CSEGC: // c
        addr += SEGS_CX + (SEGS_CY*DROW);
        mode = SEGS_CO;
        iptr = segs_c;
        break;

    case CSEGD: // d
        addr += SEGS_DX + (SEGS_DY*DROW);
        mode = SEGS_DO;
        iptr = *segs_d;
        break;

    case CSEGE: // e
        addr += SEGS_EX + (SEGS_EY*DROW);
        mode = SEGS_EO;
        iptr = segs_e;
        break;

    case CSEGF: // f
        addr += SEGS_FX + (SEGS_FY*DROW);
        mode = SEGS_FO;
        iptr = segs_f;
        break;

    case CSEGG: // g
        addr += SEGS_GX + (SEGS_GY*DROW);
        mode = SEGS_GO;
        iptr = *segs_g;
        break;

    case CSEGP: // dp
        addr += SEGS_PX + (SEGS_PY*DROW);
        mode = SEGS_PO;
        iptr = segs_p;
        break;

    default:
        mode = 0xff;
        break;
    }


    // orientation, 2=vert (2x1 array), 1=vert (12x1 array), 0=horiz (3x2 array)

    switch(mode){
    case 0:
        for(j=0; j<3; j++){
            for(i=0; i<2; i++){
                if(blank){
                	aryptr[addr+i] &= ~(*iptr++);
                }else{
                	aryptr[addr+i] |= *iptr++;
                }
            }
            addr += DROW;
        }
        break;

    case 1:
        for(j=0; j<12; j++){
            if(blank){
            	aryptr[addr] &= ~(*iptr++);
            }else{
            	aryptr[addr] |= *iptr++;
            }
            addr += DROW;
        }
        break;

    case 2:
        for(j=0; j<2; j++){
            if(blank){
            	aryptr[addr] &= ~(*iptr++);
            }else{
            	aryptr[addr] |= *iptr++;
            }
            addr += DROW;
        }
        break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------
// wr_mdigit() writes 7-seg digit to LCD memory (main)
//-----------------------------------------------------------------------------
void wr_mdigit(char digit, U8 blank, U16 daddr){
    U8  i = 0;
    U8  j;
    U16 addr = daddr;
    U8* iptr;

    switch(digit){
    case '0':
        iptr = *digit0;
        break;

    case '1':
        iptr = *digit1;
        break;

    case '2':
        iptr = *digit2;
        break;

    case '3':
        iptr = *digit3;
        break;

    case '4':
        iptr = *digit4;
        break;

    case '5':
        iptr = *digit5;
        break;

    case '6':
        iptr = *digit6;
        break;

    case '7':
        iptr = *digit7;
        break;

    case '8':
        iptr = *digit8;
        break;

    case '9':
        iptr = *digit9;
        break;

    case 'S':
        iptr = *digitmS;
        break;

    case '.':
        break;

    default:
    case ' ':
        i = 0xff;
        break;
    }
    if(i && (i !=0xff)){
        // blank digit
       iptr = *digit8;
       for(j=0; j<NUMROWS_M; j++){
            for(i=0; i<MDWIDE; i++){
                main7[addr+i] &= ~(*iptr++);
            }
            addr += DROW;
        }
    }else{
        if(digit == '.'){
            addr += NUMROWS_MDP * DROW;
            for(j=0; j<DIGITDP_Y; j++){
                if(blank) main7[addr] &= ~segm_p[j];
                else main7[addr] |= segm_p[j];
                addr += DROW;
            }
        }else{
            if(digit == 'S'){
                addr += NUMROWS_MS * DROW;
                for(j=0; j<DIGITMS_Y; j++){
                    for(i=0; i<MSDWIDE; i++){
                        if(blank) main7[addr+i] &= ~(*(iptr+i));
                        else main7[addr+i] = *(iptr+i);
                    }
                    iptr = (iptr + MSDWIDE);
                    addr += DROW;
                }
            }else{
                for(j=0; j<NUMROWS_M; j++){
                    for(i=0; i<MDWIDE; i++){
                        if(blank) main7[addr+i] &= ~(*(iptr+i));
                        else main7[addr+i] = *(iptr+i);
                    }
                    iptr = (iptr + MDWIDE);
                    addr += DROW;
                }
            }
        }
    }
    return;
}

//-----------------------------------------------------------------------------
// wr_sdigit() writes 7-seg digit to LCD memory (sub)
//-----------------------------------------------------------------------------
void wr_sdigit(char digit, U8 blank, U16 daddr, U8* aryptr){
    U8  i = 0;
    U8  j;
    U16 addr = daddr;
    U8* iptr;

    switch(digit){
    case '0':
        iptr = *digits0;
        break;

    case '1':
        iptr = *digits1;
        break;

    case '2':
        iptr = *digits2;
        break;

    case '3':
        iptr = *digits3;
        break;

    case '4':
        iptr = *digits4;
        break;

    case '5':
        iptr = *digits5;
        break;

    case '6':
        iptr = *digits6;
        break;

    case '7':
        iptr = *digits7;
        break;

    case '8':
        iptr = *digits8;
        break;

    case '9':
        iptr = *digits9;
        break;

    case 'S':
        iptr = *digitsS;
        break;

    case '.':
        break;

    default:
    case ' ':
        i = 0xff;
        break;
    }
    if(i){
        // blank digit
       iptr = *digits8;
       for(j=0; j<NUMROWS_S; j++){
            for(i=0; i<SDWIDE; i++){
                aryptr[addr+i] &= ~(*iptr++);
            }
            addr += DROW;
        }
    }else{
        if(digit == '.'){
            addr += NUMROWS_SDP * DROW;
            for(j=0; j<SEGS_PY; j++){
                if(blank) aryptr[addr] &= ~segs_p[j];
                else aryptr[addr] |= segs_p[j];
                addr += DROW;
            }
        }else{
            if(digit == 'S'){
                addr += NUMROWS_SS * DROW;
                for(j=0; j<DIGITSS_Y-1; j++){
                    for(i=0; i<MSDWIDE; i++){
                        if(blank) aryptr[addr+i] &= ~(*(iptr+i));
                        else aryptr[addr+i] |= *(iptr+i);
                    }
                    iptr = (iptr + MSDWIDE);
                    addr += DROW;
                }

            }else{
                for(j=0; j<NUMROWS_S; j++){
                    for(i=0; i<SDWIDE; i++){
                        if(blank) aryptr[addr+i] &= ~(*(iptr+i));
                        else aryptr[addr+i] |= *(iptr+i);
                    }
                    iptr = (iptr + SDWIDE);
                    addr += DROW;
                }
            }
        }
    }
    return;
}

//-----------------------------------------------------------------------------
// wr_msym2() writes a 2d array symbol to display mem - main
//-----------------------------------------------------------------------------
void wr_msym2(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr, U8* mptr){
    U8  x;
    U8  y;
    U16 addr = daddr;
    U8* iptr = sptr;

    for(y=0; y<ylen; y++){
        for(x=0; x<xlen; x++){
            if(blank){
                mptr[addr+x] &= ~(*(iptr+x));
            }else{
                mptr[addr+x] |= *(iptr+x);
            }
        }
        iptr = (iptr + xlen);
        addr += DROW;
    }
    return;
}

//-----------------------------------------------------------------------------
// wr_msym() writes a 1d array symbol to display mem - main
//-----------------------------------------------------------------------------
void wr_msym(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr, U8* mptr){
    U8  y;
    U16 addr = daddr;
    U8* iptr = sptr;

    for(y=0; y<ylen; y++){
        if(blank){
            mptr[addr] &= ~(*(iptr++));
        }else{
            mptr[addr] |= *(iptr++);
        }
        addr += DROW;
    }
    return;
}

//-----------------------------------------------------------------------------
// sg_mmin() and sg_smin() set/clr the minus duplex segment
//-----------------------------------------------------------------------------
void sg_mmin(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		main7[MINMSYM_ADDR] |= gdash;
		break;

	case SEGNOT:
		main7[MINMSYM_ADDR] &= ~gdash;
		break;

	default:
		break;
	}
	return;
}

void sg_smin(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sub7[MINSSYM_ADDR] |= gdash;
		break;

	case SEGNOT:
		sub7[MINSSYM_ADDR] &= ~gdash;
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mdup() and sg_sdup() set/clr the duplex segment
//-----------------------------------------------------------------------------
void sg_mdup(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gdup, DUPSYM_X, DUPSYM_Y, 0, DUPMSYM_ADDR, main7);
		break;

	case SEGNOT:
		wr_msym2(*gdup, DUPSYM_X, DUPSYM_Y, 1, DUPMSYM_ADDR, main7);
		break;

	default:
		break;
	}
	return;
}

void sg_sdup(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gdup, DUPSYM_X, DUPSYM_Y, 0, DUPSSYM_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gdup, DUPSYM_X, DUPSYM_Y, 1, DUPSSYM_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mtne() and sg_stne() set/clr the tone segment
//-----------------------------------------------------------------------------
void sg_mtne(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gtone, TONESYM_X, TONESYM_Y, 0, TONMSYM_ADDR, main7);
		break;

	case SEGNOT:
		wr_msym2(*gtone, TONESYM_X, TONESYM_Y, 1, TONMSYM_ADDR, main7);
		break;

	default:
		break;
	}
	return;
}

void sg_stne(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gtone, TONESYM_X, TONESYM_Y, 0, TONSSYM_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gtone, TONESYM_X, TONESYM_Y, 1, TONSSYM_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mmem() and sg_smem() set/clr the Mem segment
//-----------------------------------------------------------------------------
void sg_mmem(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gmem, MEMSYM_X, MEMSYM_Y, 0, MEMMSYM_ADDR, main7);
		break;

	case SEGNOT:
		wr_msym2(*gmem, MEMSYM_X, MEMSYM_Y, 1, MEMMSYM_ADDR, main7);
		break;

	default:
		break;
	}
	return;
}

void sg_smem(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gmem, MEMSYM_X, MEMSYM_Y, 0, MEMSSYM_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gmem, MEMSYM_X, MEMSYM_Y, 1, MEMSSYM_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}



//-----------------------------------------------------------------------------
// sg_mskp() and sg_sskp() set/clr the Mem segment
//-----------------------------------------------------------------------------
void sg_mskp(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gskp, SKPSYM_X, SKPSYM_Y, 0, SKPMSYM_ADDR, main7);
		break;

	case SEGNOT:
		wr_msym2(*gskp, SKPSYM_X, SKPSYM_Y, 1, SKPMSYM_ADDR, main7);
		break;

	default:
		break;
	}
	return;
}

void sg_sskp(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gskp, SKPSYM_X, SKPSYM_Y, 0, SKPSSYM_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gskp, SKPSYM_X, SKPSYM_Y, 1, SKPSSYM_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mm0-7() and sg_ss0-7() set/clr the s-met tape segments
//-----------------------------------------------------------------------------
void sg_mm0(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet1, SM1SYM_X, SM1SYM_Y, 0, MSMET1_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet1, SM1SYM_X, SM1SYM_Y, 1, MSMET1_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_mm1(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet2, SM2SYM_X, SM2SYM_Y, 0, MSMET2_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet2, SM2SYM_X, SM2SYM_Y, 1, MSMET2_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_mm2(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet3, SM3SYM_X, SM3SYM_Y, 0, MSMET3_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet3, SM3SYM_X, SM3SYM_Y, 1, MSMET3_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_mm3(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet4, SM4SYM_X, SM4SYM_Y, 0, MSMET4_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet4, SM4SYM_X, SM4SYM_Y, 1, MSMET4_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_mm4(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet5, SM5SYM_X, SM5SYM_Y, 0, MSMET5_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet5, SM5SYM_X, SM5SYM_Y, 1, MSMET5_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_mm5(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet6, SM6SYM_X, SM6SYM_Y, 0, MSMET6_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet6, SM6SYM_X, SM6SYM_Y, 1, MSMET6_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_mm6(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet7, SM7SYM_X, SM7SYM_Y, 0, MSMET7_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet7, SM7SYM_X, SM7SYM_Y, 1, MSMET7_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm0(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet1, SM1SYM_X, SM1SYM_Y, 0, SSMET1_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet1, SM1SYM_X, SM1SYM_Y, 1, SSMET1_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm1(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet2, SM2SYM_X, SM2SYM_Y, 0, SSMET2_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet2, SM2SYM_X, SM2SYM_Y, 1, SSMET2_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm2(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet3, SM3SYM_X, SM3SYM_Y, 0, SSMET3_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet3, SM3SYM_X, SM3SYM_Y, 1, SSMET3_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm3(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet4, SM4SYM_X, SM4SYM_Y, 0, SSMET4_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet4, SM4SYM_X, SM4SYM_Y, 1, SSMET4_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm4(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet5, SM5SYM_X, SM5SYM_Y, 0, SSMET5_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet5, SM5SYM_X, SM5SYM_Y, 1, SSMET5_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm5(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet6, SM6SYM_X, SM6SYM_Y, 0, SSMET6_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet6, SM6SYM_X, SM6SYM_Y, 1, SSMET6_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

void sg_sm6(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsmet7, SM7SYM_X, SM7SYM_Y, 0, SSMET7_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gsmet7, SM7SYM_X, SM7SYM_Y, 1, SSMET7_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_ow() set/clr the OW segment
//-----------------------------------------------------------------------------
void sg_ow(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gow, OWSYM_X, OWSYM_Y, 0, OW_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*gow, OWSYM_X, OWSYM_Y, 1, OW_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_low() set/clr the LOW segment
//-----------------------------------------------------------------------------
void sg_low(U8 son){

	change_flag |= MAINSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*glow, LOWSYM_X, LOWSYM_Y, 0, LOW_ADDR, mainsm);
		break;

	case SEGNOT:
		wr_msym2(*glow, LOWSYM_X, LOWSYM_Y, 1, LOW_ADDR, mainsm);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_ts() set/clr the TS segment
//-----------------------------------------------------------------------------
void sg_ts(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gts, TSSYM_X, TSSYM_Y, 0, TS_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gts, TSSYM_X, TSSYM_Y, 1, TS_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mhz() set/clr the MHz segment
//-----------------------------------------------------------------------------
void sg_mhz(U8 son){

	change_flag |= SUBSM_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gmhz, MHZSYM_X, MHZSYM_Y, 0, MHZ_ADDR, subsm);
		break;

	case SEGNOT:
		wr_msym2(*gmhz, MHZSYM_X, MHZSYM_Y, 1, MHZ_ADDR, subsm);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_prg() set/clr the PROG segment
//-----------------------------------------------------------------------------
void sg_prg(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gprog, PROGSYM_X, PROGSYM_Y, 0, PROG_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gprog, PROGSYM_X, PROGSYM_Y, 1, PROG_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bnd() set/clr the BAND segment
//-----------------------------------------------------------------------------
void sg_bnd(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gband, BANDSYM_X, BANDSYM_Y, 0, BAND_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gband, BANDSYM_X, BANDSYM_Y, 1, BAND_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_sub() set/clr the SUB segment
//-----------------------------------------------------------------------------
void sg_sub(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gsub, SUBSYM_X, SUBSYM_Y, 0, SUB_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*gsub, SUBSYM_X, SUBSYM_Y, 1, SUB_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_lck() set/clr the LOCK segment
//-----------------------------------------------------------------------------
void sg_lck(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*glock, LOCKSYM_X, LOCKSYM_Y, 0, LOCK_ADDR, sub7);
		break;

	case SEGNOT:
		wr_msym2(*glock, LOCKSYM_X, LOCKSYM_Y, 1, LOCK_ADDR, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_vxo() set/clr the VXO segment
//-----------------------------------------------------------------------------
void sg_vxo(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gvxo, VXOSYM_X, VXOSYM_Y, 0, VXO_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*gvxo, VXOSYM_X, VXOSYM_Y, 1, VXO_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_rit() set/clr the RIT segment
//-----------------------------------------------------------------------------
void sg_rit(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*grit, RITSYM_X, RITSYM_Y, 0, RIT_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*grit, RITSYM_X, RITSYM_Y, 1, RIT_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mss() set/clr the opta up segment
//-----------------------------------------------------------------------------
void sg_mss(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 0, OPTA_ADDR+UP_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 1, OPTA_ADDR+UP_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_tss() set/clr the opta dn segment
//-----------------------------------------------------------------------------
void sg_tss(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 0, OPTA_ADDR+DN_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 1, OPTA_ADDR+DN_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_tsq() set/clr the opta segment
//-----------------------------------------------------------------------------
void sg_tsq(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gopttq, OPTASYM_X, OPTASYM_Y, 0, OPTA_ADDR+OPT_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*gopttq, OPTASYM_X, OPTASYM_Y, 1, OPTA_ADDR+OPT_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_dsm() set/clr the optb up segment
//-----------------------------------------------------------------------------
void sg_dsm(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 0, OPTB_ADDR+UP_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 1, OPTB_ADDR+UP_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_dss() set/clr the optb dn segment
//-----------------------------------------------------------------------------
void sg_dss(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 0, OPTB_ADDR+DN_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 1, OPTB_ADDR+DN_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_dsq() set/clr the optb segment
//-----------------------------------------------------------------------------
void sg_dsq(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptdq, OPTBSYM_X, OPTBSYM_Y, 0, OPTB_ADDR+OPT_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptdq, OPTBSYM_X, OPTBSYM_Y, 1, OPTB_ADDR+OPT_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_op1s() set/clr the optc up segment
//-----------------------------------------------------------------------------
void sg_op1m(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 0, OPTC_ADDR+UP_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 1, OPTC_ADDR+UP_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_op1s() set/clr the optc dn segment
//-----------------------------------------------------------------------------
void sg_op1s(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 0, OPTC_ADDR+DN_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 1, OPTC_ADDR+DN_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_op1() set/clr the optc segment
//-----------------------------------------------------------------------------
void sg_op1(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gopt1, OPTCSYM_X, OPTCSYM_Y, 0, OPTC_ADDR+OPT_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*gopt1, OPTCSYM_X, OPTCSYM_Y, 1, OPTC_ADDR+OPT_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_op2m() set/clr the optd up segment
//-----------------------------------------------------------------------------
void sg_op2m(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 0, OPTD_ADDR+UP_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptup, OPTUPSYM_X, OPTUPSYM_Y, 1, OPTD_ADDR+UP_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_op2s() set/clr the optd dn segment
//-----------------------------------------------------------------------------
void sg_op2s(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 0, OPTD_ADDR+DN_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*goptdn, OPTDNSYM_X, OPTDNSYM_Y, 1, OPTD_ADDR+DN_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_op2() set/clr the optd segment
//-----------------------------------------------------------------------------
void sg_op2(U8 son){

	change_flag |= OPTROW_FL;
	switch(son){
	case SEGOR:
		wr_msym2(*gopt2, OPTDSYM_X, OPTDSYM_Y, 0, OPTD_ADDR+OPT_ADDR, optrow);
		break;

	case SEGNOT:
		wr_msym2(*gopt2, OPTDSYM_X, OPTDSYM_Y, 1, OPTD_ADDR+OPT_ADDR, optrow);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mdp() set/clr the mdp segment
//-----------------------------------------------------------------------------
void sg_mdp(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_mseg(CSEGP, 0, MDADDR3);
		break;

	case SEGNOT:
		wr_mseg(CSEGP, 1, MDADDR3);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mdp2() set/clr the mdp2 segment
//-----------------------------------------------------------------------------
void sg_mdp2(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_mseg(CSEGP, 0, MDADDR0);
		break;

	case SEGNOT:
		wr_mseg(CSEGP, 1, MDADDR0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_sdp() set/clr the sdp segment
//-----------------------------------------------------------------------------
void sg_sdp(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_sseg(CSEGP, 0, SDADDR3, sub7);
		break;

	case SEGNOT:
		wr_sseg(CSEGP, 1, SDADDR3, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_sdp2() set/clr the sdp2 segment
//-----------------------------------------------------------------------------
void sg_sdp2(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_sseg(CSEGP, 0, SDADDR0, sub7);
		break;

	case SEGNOT:
		wr_sseg(CSEGP, 1, SDADDR0, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_m00() set/clr the m00 segment
//-----------------------------------------------------------------------------
void sg_m00(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		wr_mdigit('S', 0, MDADDRS);
		break;

	case SEGNOT:
		wr_mdigit('S', 1, MDADDRS);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_s00() set/clr the s00 segment
//-----------------------------------------------------------------------------
void sg_s00(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		wr_sdigit('S', 0, SDADDRS, sub7);
		break;

	case SEGNOT:
		wr_sdigit('S', 1, SDADDRS, sub7);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_mbcd() set/clr bcd segments - main
//	bcdd = bcd data [efdg a-cb]
//	Loop through set bits and clear not set bits
//-----------------------------------------------------------------------------
void sg_mbcd(U8 bcdd, U16 digitaddr, U8 blank, U8 main_sel){
	U8	i;

	change_flag |= MAIN7_FL;
//	wr_mdigit('8', 1, digitaddr);
	for(i=0x80; i!=0; i>>=1){
		wr_mseg(bcdd & i, blank, digitaddr);
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_sbcd() set/clr bcd segments - sub
//	bcdd = bcd data [efdg a-cb]
//	Loop through set bits and clear not set bits
//-----------------------------------------------------------------------------
void sg_sbcd(U8 bcdd, U16 digitaddr, U8 blank, U8 main_sel){
	U8	i;

	if(main_sel){
		change_flag |= MAIN7_FL;
//		wr_sdigit('8', 1, digitaddr, main7);
		for(i=0x80; i!=0; i>>=1){
			wr_sseg(bcdd & i, blank, digitaddr, main7);
		}
	}else{
		change_flag |= SUB7_FL;
//		wr_sdigit('8', 1, digitaddr, sub7);
		for(i=0x80; i!=0; i>>=1){
			wr_sseg(bcdd & i, blank, digitaddr, sub7);
		}
	}
	return;
}

//-----------------------------------------------------------------------------
// null_fn() is a fail-safe dummy fn to fill empty segments in the segmem bitmaps
//-----------------------------------------------------------------------------
void null_fn(U8 son){
	volatile	U8	i=son;

	return;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//			BCD segment Fns
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// main digit _m0 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_m0a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m0b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m0c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m0d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m0e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m0f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m0g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m0g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR0, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR0, 1, 1);
		break;

	default:
		break;
	}
	return;
}

// main digit _m1 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_m1a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m1b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m1c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m1d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m1e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m1f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m1g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m1g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR1, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR1, 1, 1);
		break;

	default:
		break;
	}
	return;
}
// main digit _m2 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_m2a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m2b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m2c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m2d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m2e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m2f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m2g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m2g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR2, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR2, 1, 1);
		break;

	default:
		break;
	}
	return;
}
// main digit _m3 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_m3a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m3b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m3c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m3d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m3e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m3f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m3g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m3g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR3, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR3, 1, 1);
		break;

	default:
		break;
	}
	return;
}
// main digit _m4 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_m4a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m4b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m4c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m4d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m4e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m4f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m4g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m4g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR4, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR4, 1, 1);
		break;

	default:
		break;
	}
	return;
}
// main digit _m5 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_m5a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m5b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m5c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m5d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m5e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m5f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m5g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m5g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR5, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR5, 1, 1);
		break;

	default:
		break;
	}
	return;
}
// main digit _m6 ///////////////////////////////////////
/*//-----------------------------------------------------------------------------
// sg_bcd_m6a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGA, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGA, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}*/

//-----------------------------------------------------------------------------
// sg_bcd_m6bc() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6bc(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGB|CSEGC, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGB|CSEGC, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}
/*
//-----------------------------------------------------------------------------
// sg_bcd_m6c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGC, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGC, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m6d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGD, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGD, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m6e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGE, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGE, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m6f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGF, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGF, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_m6g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_m6g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_mbcd(CSEGG, MDADDR6, 0, 1);
		break;

	case SEGNOT:
		sg_mbcd(CSEGG, MDADDR6, 1, 1);
		break;

	default:
		break;
	}
	return;
}*/

//////////////////////////////              ////////////////////////////////
// sub_digit _s0 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s0a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s0b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s0c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s0d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s0e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s0f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s0g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s0g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR0, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR0, 1, 0);
		break;

	default:
		break;
	}
	return;
}

// sub_digit _s1 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s1a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s1b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s1c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s1d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s1e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s1f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s1g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s1g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR1, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR1, 1, 0);
		break;

	default:
		break;
	}
	return;
}
// sub_digit _s2 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s2a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s2b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s2c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s2d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s2e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s2f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s2g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s2g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR2, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR2, 1, 0);
		break;

	default:
		break;
	}
	return;
}
// sub_digit _s3 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s3a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s3b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s3c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s3d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s3e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s3f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s3g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s3g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR3, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR3, 1, 0);
		break;

	default:
		break;
	}
	return;
}
// sub_digit _s4 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s4a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s4b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s4c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s4d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s4e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s4f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s4g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s4g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR4, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR4, 1, 0);
		break;

	default:
		break;
	}
	return;
}
// sub_digit _s5 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s5a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s5b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s5c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s5d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s5e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s5f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s5g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s5g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR5, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR5, 1, 0);
		break;

	default:
		break;
	}
	return;
}
// sub_digit _s6 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_s6a() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
/*void sg_bcd_s6a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}*/

//-----------------------------------------------------------------------------
// sg_bcd_s6bc() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s6bc(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB|CSEGC, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB|CSEGC, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}
/*
//-----------------------------------------------------------------------------
// sg_bcd_s6c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s6c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s6d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s6d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s6e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s6e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s6f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s6f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_s6g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_s6g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, SDADDR6, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, SDADDR6, 1, 0);
		break;

	default:
		break;
	}
	return;
}
*/
/////////////////////////                              ///////////////////////////////

//-----------------------------------------------------------------------------
// sg_digit_mem_s7 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_mem_s7g() set/clr mem bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7a(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_s7b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7b(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_s7c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7c(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_s7d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7d(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_s7e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7e(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_s7f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7f(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_s7g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_s7g(U8 son){

	change_flag |= SUB7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, MEMSCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, MEMSCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

/////////////////////////////////                                  //////////////////////////////
//-----------------------------------------------------------------------------
// sg_digit_mem_m7 ///////////////////////////////////////
//-----------------------------------------------------------------------------
// sg_bcd_mem_m7g() set/clr mem bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7a(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGA, MEMSCH_ADDR, 0, 1);
//		sg_mbcd(CSEGA, MEMMCH_ADDR, 0, 0);
		break;

	case SEGNOT:
		sg_sbcd(CSEGA, MEMSCH_ADDR, 1, 1);
//		sg_mbcd(CSEGA, MEMMCH_ADDR, 1, 0);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_m7b() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7b(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGB, MEMSCH_ADDR, 0, 1);
		break;

	case SEGNOT:
		sg_sbcd(CSEGB, MEMSCH_ADDR, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_m7c() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7c(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGC, MEMSCH_ADDR, 0, 1);
		break;

	case SEGNOT:
		sg_sbcd(CSEGC, MEMSCH_ADDR, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_m7d() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7d(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGD, MEMSCH_ADDR, 0, 1);
		break;

	case SEGNOT:
		sg_sbcd(CSEGD, MEMSCH_ADDR, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_m7e() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7e(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGE, MEMSCH_ADDR, 0, 1);
		break;

	case SEGNOT:
		sg_sbcd(CSEGE, MEMSCH_ADDR, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_m7f() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7f(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGF, MEMSCH_ADDR, 0, 1);
		break;

	case SEGNOT:
		sg_sbcd(CSEGF, MEMSCH_ADDR, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// sg_bcd_mem_m7g() set/clr bcd seg (main/sub, digit addr, segid)
//-----------------------------------------------------------------------------
void sg_bcd_mem_m7g(U8 son){

	change_flag |= MAIN7_FL;
	switch(son){
	case SEGOR:
		sg_sbcd(CSEGG, MEMSCH_ADDR, 0, 1);
		break;

	case SEGNOT:
		sg_sbcd(CSEGG, MEMSCH_ADDR, 1, 1);
		break;

	default:
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
// disp_fn() set/clr bcd seg using function array (debug??)
//-----------------------------------------------------------------------------
void disp_fn(U8 csnum, U8 son, U8 maddr, U8 saddr){

	if(csnum == 1){
		(*cs1_fn[maddr][saddr])(son);
	}else{
		(*cs2_fn[maddr][saddr])(son);
	}
}

//-----------------------------------------------------------------------------
// trig_fill() place test date into trig array
//-----------------------------------------------------------------------------
void trig_fill(U8 idx, U8 data, U8 chsel){

	if(chsel == 1){
		CS1_trig[idx] = data;
	}else{
		CS2_trig[idx] = data;
	}
	return;
}

// uPD7225 memory arrays
// segment change memory array
//U8	CS1_trig[LCD_MEMLEN];	// trigger
// segment on/off memory array
//U8	CS1_dmem[LCD_MEMLEN];	// display mem
// blink on/off memory array
//U8	CS1_bmem[LCD_MEMLEN];	// blink mem

//-----------------------------------------------------------------------------
// trig_scan() scans through trig arrays and dispatches segment updates
//	copies new pixel status into dmem or bmem arrays
//	clears trig array as processed
//	mode is OR, AND, CLR, or write
//-----------------------------------------------------------------------------
void trig_scan1(U8 mode){
	U8	i;
	U8	j;
	U8	k;
	U8	m;
	U8*	memptr;

	for(i=0; i<LCD_MEMLEN; i++){
		m = CS1_trig[i];
		if(m){
			if(m & BLINKFL){
				memptr = CS1_bmem;
			}else{
				memptr = CS1_dmem;
			}
			k = memptr[i];
			switch(m & MODE_MASK){
			case MODE_OR:
				k |= (m & BIT_MASK);
				break;

			case MODE_AND:
				k &= (m & BIT_MASK);
				break;

			case MODE_WR:
				k = (m & BIT_MASK);
				break;

			default:	// clear
				k &= (~m & BIT_MASK);
				break;
			}
//			if(!(m & BLINKFL)){
				if(k != memptr[i]){
					memptr[i] = k;
					for(j=0; j<3; j++){
						(*cs1_fn[i][j])(k & 0x01);
						k >>= 1;
					}
				}
				CS1_trig[i] = 0;
//			}else{
//				memptr[i] = k;
//				CS1_trig[i] = MODE_OR | CS1_dmem[i];
//			}
		}
	}
 }

void trig_scan2(U8 mode){
	U8	i;
	U8	j;
	U8	k;
	U8	m;
	U8*	memptr;

	for(i=0; i<LCD_MEMLEN; i++){
		m = CS2_trig[i];
		if(m){
			memptr = CS2_dmem;
			k = memptr[i];
			switch(m & MODE_MASK){
			case MODE_OR:
				k |= (m & BIT_MASK);
				break;

			case MODE_AND:
				k &= (m & BIT_MASK);
				break;

			case MODE_WR:
				k = (m & BIT_MASK);
				break;

			default:	// clear
				k &= (~m & BIT_MASK);
				break;
			}
//			if(!(m & BLINKFL)){
				if(k != memptr[i]){
					memptr[i] = k;
					for(j=0; j<3; j++){
						(*cs2_fn[i][j])(k & 0x01);
						k >>= 1;
					}
				}
				CS2_trig[i] = 0;
//			}else{
//				memptr[i] = k;
//				CS2_trig[i] = MODE_OR | CS2_dmem[i];
//			}
		}
	}
}

// EOF
