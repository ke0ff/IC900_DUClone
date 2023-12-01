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

// uPD7225 registers
U8	CS1_REG;				// CS1 status register
U8	CS2_REG;				// CS2 status register

U8	CS1_trig[LCD_MEMLEN];	// trigger
U8	CS1_dmem[LCD_MEMLEN];	// display mem
U8	CS1_bmem[LCD_MEMLEN];	// blink mem
U8	CS2_trig[LCD_MEMLEN];	// trigger
U8	CS2_dmem[LCD_MEMLEN];	// display mem
U8	CS2_bmem[LCD_MEMLEN];	// blink mem

//-----------------------------------------------------------------------------
// Local Fn Declarations
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// clear_main7() clears LCD memory - main
// clear_sub7() clears LCD memory - sub
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
/*		if(i&STA6){
			i |= 0x10;
		}*/
		i &= mask;
	}while(i != mask);
	GPIO_PORTB_DIR_R = 0xff;				// port = out
	GPIO_PORTB_DATA_R = swapeo(data); //data; // store data to port
	if(addr) css |= TADDR;
	GPIO_PORTE_DATA_R = css;				// store addr, bus idle
	GPIO_PORTE_DATA_R = css & ~(nTWR|nTCS);	// set OE & WR
//	for(i=0; i<TLEN; i++);
	GPIO_PORTE_DATA_R = css & ~(nTWR);		// store bus idle
//	for(i=0; i<TLEN; i++);
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
//	U8	i;

	GPIO_PORTB_DIR_R = 0;					// port = in
	if(addr) css |= TADDR;
	GPIO_PORTE_DATA_R = css;				// store addr, bus idle
	GPIO_PORTE_DATA_R = css & ~(nTRD|nTCS);	// set OE & RD
//	for(i=0; i<TLEN; i++);
	data = GPIO_PORTB_DATA_R;				// get data from port
	GPIO_PORTE_DATA_R = css;				// store bus idle
//	for(i=0; i<TLEN; i++);
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
void wr_sseg(U8 seg, U8 blank, U16 daddr){
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
                    sub7[addr+i] &= ~(*iptr++);
                }else{
                    sub7[addr+i] |= *iptr++;
                }
            }
            addr += DROW;
        }
        break;

    case 1:
        for(j=0; j<12; j++){
            if(blank){
                sub7[addr] &= ~(*iptr++);
            }else{
                sub7[addr] |= *iptr++;
            }
            addr += DROW;
        }
        break;

    case 2:
        for(j=0; j<2; j++){
            if(blank){
                sub7[addr] &= ~(*iptr++);
            }else{
                sub7[addr] |= *iptr++;
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
    if(i){
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
                        if(blank) main7[addr+i] &= ~(*iptr++);
                        else main7[addr+i] = *(iptr+i);
                    }
                    iptr = (iptr + MSDWIDE);
                    addr += DROW;
                }
            }else{
                for(j=0; j<NUMROWS_M; j++){
                    for(i=0; i<MDWIDE; i++){
                        if(blank) main7[addr+i] &= ~(*iptr++);
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
// sg_op2s() set/clr the optd up segment
//-----------------------------------------------------------------------------
void sg_op2m(U8 son){

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
// sg_mbcd() set/clr bcd segments - main
//	bcdd = bcd data [efdg a-cb]
//	Loop through set bits and clear not set bits
//-----------------------------------------------------------------------------
void sg_mbcd(U8 bcdd, U16 digitaddr){
	U8	i;

	wr_mdigit('8', 1, digitaddr);
	for(i=0x80; i!=0; i>>=1){
		wr_mseg(bcdd & i, 0, digitaddr);
	}
	return;
}

/*
//-----------------------------------------------------------------------------
// wr_ssym2() writes a 2d array symbol to display mem - sub
//-----------------------------------------------------------------------------
void wr_ssym2(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr){
    U8  x;
    U8  y;
    U16 addr = daddr;
    U8* iptr = sptr;

    for(y=0; y<ylen; y++){
        for(x=0; x<xlen; x++){
            if(blank){
                sub7[addr+x] &= ~(*(iptr+x));
            }else{
                sub7[addr+x] |= *(iptr+x);
            }
        }
        iptr = (iptr + xlen);
        addr += DROW;
    }
    return;
}

//-----------------------------------------------------------------------------
// wr_ssym() writes a 1d array symbol to display mem - sub
//-----------------------------------------------------------------------------
void wr_ssym(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr){
    U8  y;
    U16 addr = daddr;
    U8* iptr = sptr;

    for(y=0; y<ylen; y++){
        if(blank){
            sub7[addr] &= ~(*(iptr++));
        }else{
            sub7[addr] |= *(iptr++);
        }
        addr += DROW;
    }
    return;
}*/

//eof
