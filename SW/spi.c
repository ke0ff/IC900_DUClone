/********************************************************************
 ************ COPYRIGHT (c) 2023 by KE0FF, Taylor, TX   *************
 *
 *  File name: spi.c
 *
 *  Module:    Control
 *
 *  Summary:
 *  ssi driver for RDU LCD controller IC
 *
 *******************************************************************/

#include <ctype.h>
#include <math.h>
#include "inc/tm4c123gh6pm.h"
#include "typedef.h"
#include "init.h"						// App-specific SFR Definitions
#include "version.h"
#include "spi.h"
#include "serial.h"
#include "lcd_db.h"

// ******************************************************************
// defines

// ******************************************************************
// declarations

// BBSPI regs
#ifdef BBSPI
			U8	spidr;
			U8	spimsk;
#endif
// SPI input buffer
			U8	ssi0_buf[SPI_LEN];		// data buff
			U8	ssi0_status[SPI_LEN];	// cs/status buff
			U8	ssi0_h;					// head
			U8	ssi0_t;					// tail
			U8	ssi0_statreg;			// SPI global status
			U8	ssi0_meta;				// SPI meta dat capture register
volatile	U32	ssitimer;				// ssi timer


// ******************************************************************
// ***** START OF CODE ***** //

//////////////////
// init_ssi0 initializes SSI0 as an 8-bit slave
//
void init_ssi0(void)
{
//	volatile U8	i;

	ssi0_statreg = 0;
	ssi0_h = 0;
	ssi0_t = 0;
	ssi0_meta = 0;

#ifdef BBSPI
	spidr = 0;
	spimsk = 0x80;
	// config SCLK rising edge ISR, CSS falling edge flag
	GPIO_PORTA_IM_R = 0;
	GPIO_PORTA_IEV_R = SCLK;								// rising edge for SCLK (falling edge for CSS)
	GPIO_PORTA_IBE_R &= ~(SCLK|CSS);						// one edge
	GPIO_PORTA_IS_R &= ~(SCLK|CSS);							// edge ints
	GPIO_PORTA_ICR_R = 0xff;								// clear int flags
	GPIO_PORTA_IM_R = SCLK;									// only interrupt on SCLK
	ssitimer = 0;
	NVIC_EN0_R = NVIC_EN0_GPIOA;							// enable GPIOA intr in the NVIC_EN regs
	kick_ssito();
#endif

#ifndef BBSPI
	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;					// activate SSI0
	GPIO_PORTA_AFSEL_R |= SCLK|MOSI|CSS;					// enable alt funct on PA2/4
	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA2_SSI0CLK | GPIO_PCTL_PA4_SSI0RX;
	GPIO_PORTA_AMSEL_R &= ~(SCLK|CSS|CS1|CS2|MOSI);			// disable analog functionality on PQ*/
	SSI0_CR1_R = 0;											// disable SSI, master mode
	SSI0_CPSR_R = CPSDVSR;
	// SCR = [15:8], SPH[7] = 1, SPO[6] = 1, Freescale, DSS = 7 (8-bit data)
	SSI0_CR0_R = (U32)((SSI_CR0_SCR_M & (SCRVAL << 8))) | (U32)SSI_CR0_DSS_8 | (U32)SSI_CR0_SPO | (U32)SSI_CR0_SPH;
	SSI0_IM_R = SSI_IM_RXIM | SSI_IM_RTIM;					// enable ISR
	SSI0_CR1_R = SSI_CR1_MS;								// slave
	SSI0_CC_R = 0;
	SSI0_CR1_R = SSI_CR1_SSE | SSI_CR1_MS;					// enable SSI, slave
	while(SSI0_MIS_R | (SSI0_SR_R & SSI_SR_RNE)){			// flush data buffer
		i = SSI0_DR_R;
	}
	// config SCLK edge ISR
	i = GPIO_PORTF_IM_R;									// disable edge intr
	GPIO_PORTF_IM_R = 0;
	GPIO_PORTF_IEV_R |= SCLKE;								// rising edge
	GPIO_PORTF_IBE_R &= ~SCLKE;								// one edge
	GPIO_PORTF_IS_R &= ~SCLKE;								// edge ints
	GPIO_PORTF_ICR_R = 0xff;								// clear int flags
	i |= SCLKE;												// enable sclk edge intr
	GPIO_PORTF_IM_R = i;
	ssitimer = 0;
	NVIC_EN0_R = NVIC_EN0_GPIOF;							// enable GPIOC intr in the NVIC_EN regs
	NVIC_EN0_R = NVIC_EN0_SSI0;								// enable SSI0 isr
#endif

	return;
}

void dbg_spirx(U8* sptr, U8 len, U8* buf){
	U8	i;

	for(i=0; i<len; i++){
		ssi0_buf[i] = *sptr++;
		ssi0_status[i] = *buf++;
	}
//	ssi0_t = 0;
	ssi0_h += len;
}

//////////////////
// get_csseg returns true if CSSEG set.  Clears CCSEG
//
U8 get_csseg(void){

	if(ssi0_statreg & SPI_CSSEG){
		NVIC_DIS0_R = NVIC_EN0_GPIOA;							// disable GPIOA intr in the NVIC_EN regs
		ssi0_statreg &= ~SPI_CSSEG;								// clear edge
		NVIC_EN0_R = NVIC_EN0_GPIOA;							// re-enable GPIOA intr in the NVIC_EN regs
		return 1;
	}
	return 0;
}

//////////////////
// get_hptr returns value of hptr
//
U8 get_tptr(void){

	return ssi0_t;
}

//////////////////
// get_spiovf returns true if SPI_OVFLW set.
//
U8 get_spiovf(void){

	if(ssi0_statreg & SPI_OVFLW){
		return 1;
	}
	return 0;
}

//////////////////
// got_ssi0 returns true if buffer has data
//
U8 got_ssi0(void){

	if(ssi0_h != ssi0_t) return 1;
	return 0;
}

//////////////////
// get_ssi0 returns true if buffer has data
//
U16 get_ssi0(void){
	U16	ii;

	ii = ((U16)ssi0_status[ssi0_t]) << 8;
	ii |= ((U16)ssi0_buf[ssi0_t]) & 0xff;
	ssi0_t++;
	return ii;
}

//////////////////
// ssi0_isr pushes data into SPI buffer
//

void ssi0_isr(void){
	U8	si;

	while(SSI0_SR_R & SSI_SR_RNE){							// check if data available
		si = SSI0_DR_R;										// get data, place in buff
		ssi0_buf[ssi0_h] = si;								// get data, place in buff
		if((si == 0xd0) && (ssi0_meta & CS2)){
			if(ssi0_buf[ssi0_h-1] == 0xfe){
				GPIO_PORTD_DATA_R ^= sparePD7;
				GPIO_PORTD_DATA_R ^= sparePD7;
				si++;
			}
		}
		// CS1/CS2/CMD_DATA are inverted when placed in buffer
		ssi0_status[ssi0_h] = ssi0_meta;					// get CS status, place in stat buff
		GPIO_PORTF_ICR_R = SCLKE;							// pre-clear int flags
		GPIO_PORTF_IM_R |= SCLKE;							// enable edge intr
		if(ssi0_h++ == (SPI_LEN)){
			ssi0_h = 0;
		}
	}
	if(ssi0_h == ssi0_t){
		ssi0_statreg |= SPI_OVFLW;
		if(ssi0_t++ == (SPI_LEN)){
			ssi0_t = 0;
		}
	}
//	ssitimer = SSITO_TIME;
	SSI0_ICR_R = SSI_ICR_RTIC;
	return;
}

//-----------------------------------------------------------------------------
// gpiof_isr
// GPIO_PORTF isr, processes first falling edge of SCLK to capture meta data
//		on Port A.
//		SSI0 init & SSI0 ISR enables this ISR
//		This ISR disables itself after 1 call.
//-----------------------------------------------------------------------------
void gpiof_isr(void){

#ifndef BBSPI
	// clear int flags
	GPIO_PORTF_ICR_R = SCLKE;
	// get CS status & cmd, place in stat buff
	ssi0_meta = ~GPIO_PORTA_DATA_R & (CS2|CS1|CMD_DATA) | DRFF;
	// disable gpiof
	GPIO_PORTF_IM_R &= ~SCLKE;
#endif

#ifdef BBSPI
	U8	pd;

	if(GPIO_PORTA_RIS_R & CSS){
		spimsk = 0x80;									// if there is a CSS falling edge, reset rx regs
		spidr = 0;
		ssi0_statreg |= SPI_CSSEG;
	}
	if(GPIO_PORTA_DATA_R & MOSI){
		spidr |= spimsk;
	}
	spimsk >>= 1;
	if(!spimsk){
		// get CS status & cmd, place in stat buff
		pd = ~GPIO_PORTA_DATA_R;
		pd &= CS2|CS1|CMD_DATA;
		ssi0_status[ssi0_h] = pd;
		ssi0_buf[ssi0_h] = spidr;						// get data, place in buff
		kick_ssito();									// reset serial timeout
		spimsk = 0x80;									// reset data rx regs for next byte
		spidr = 0;
		ssi0_h++;										// advance buffer pointer (auto rollover)
		if(ssi0_h == ssi0_t){
			ssi0_statreg |= SPI_OVFLW;					// process buffer overflow
			ssi0_t++;
		}
	}
	GPIO_PORTA_ICR_R = SCLK|CSS;
#endif

	return;
}

//-----------------------------------------------------------------------------
// is_ssito() returns true if ssitimer == 0
//-----------------------------------------------------------------------------
U8 is_ssito(void){

	if(ssitimer == 0){
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// kick_ssito() resets ssitimer
//-----------------------------------------------------------------------------
void kick_ssito(void){

	ssitimer = SSITO_TIME;
	return;
}

//-----------------------------------------------------------------------------
// Timer1B_ISR() drives SPI TO timer
//-----------------------------------------------------------------------------
//
// Called when timer 1B overflows (NORM mode):
//	Timer1_B runs as 16 bit reload timer set to 10ms interrupt rate
//	Provides a time-out timer resource to the SSI driver
//
//-----------------------------------------------------------------------------

void Timer1B_ISR(void){

//	GPIO_PORTF_DATA_R ^= sparePF3;
	// set flag to trigger bit
	if(TIMER1_MIS_R & TIMER_MIS_TBTOMIS){
		if(ssitimer){										// update ssi timer
			ssitimer--;
		}
	}
	// clear ISR flags
	TIMER1_ICR_R = TIMER1_MIS_R & TIMERB_MIS_MASK;
//	GPIO_PORTF_DATA_R ^= sparePF3;
	return;
}

