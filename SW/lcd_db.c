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

U8	main7[MAIN7_LEN];		// main 7-seg DU memory
U8	sub7[SUB7_LEN];			// sub 7-seg DU memory
U8	mainsm[MAINSM_LEN];		// main smet row
U8	subsm[SUBSM_LEN];		// main smet row
U8	optrow[OPTROW_LEN];		// main smet row

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
// swapeo() swaps even & odd bits - a temporary fix for mis-wired hardware
//-----------------------------------------------------------------------------
/*U8 swapeo(U8 data){
	U8	i;
	U8	j;

	i = (data << 1) & 0xaa;
	j = (data >> 1) & 0x55;

	return (i | j);
}*/
//eof
