/********************************************************************
 ************ COPYRIGHT (c) 2022 by KE0FF, Taylor, TX   *************
 *
 *  File name: spi.c
 *
 *  Module:    Control
 *
 *  Summary:
 *  spi for RDU LCD controller IC
 *
 *******************************************************************/

//#include <stdint.h>
//#include <string.h>
//#include <stdio.h>
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
#define	SPI_LEN		250
#define	SPI_OVFLW	0x80

// ******************************************************************
// declarations
//U8 shift_spi(U8 dato);
//void open_nvr(void);

// SPI input buffer
			U8	ssi0_buf[SPI_LEN];		// data buff
			U8	ssi0_status[SPI_LEN];	// cs/status buff
			U8	ssi0_h;					// head
			U8	ssi0_t;					// tail
			U8	ssi0_statreg;			// SPI global status
			U8	ssi0_meta;				// SPI meta dat capture register
volatile	U32	ssitimer;				// ssi timer

#define	SSITO_TIME	(3)

// ******************************************************************
// ***** START OF CODE ***** //

//////////////////
// init_ssi0 initializes SSI0 as an 8-bit slave
//
void init_ssi0(void)
{
	volatile U8	i;

	SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;					// activate SSI0
	GPIO_PORTA_AFSEL_R |= SCLK|MOSI|CSS;					// enable alt funct on PA2/4
	GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA2_SSI0CLK | GPIO_PCTL_PA4_SSI0RX;
	GPIO_PORTA_AMSEL_R &= ~(SCLK|CSS|CS1|CS2|MOSI);			// disable analog functionality on PQ*/
	ssi0_statreg = 0;
	ssi0_h = 0;
	ssi0_t = 0;
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
	ssi0_meta = 0;
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

	ii = (U16)ssi0_status[ssi0_t] << 8;
	ii |= (U16)ssi0_buf[ssi0_t] & 0xff;
	if(ssi0_t++ == (SPI_LEN)) ssi0_t = 0;
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

	// clear int flags
	GPIO_PORTF_ICR_R = SCLKE;
	// get CS status & cmd, place in stat buff
	ssi0_meta = ~GPIO_PORTA_DATA_R & (CS2|CS1|CMD_DATA) | DRFF;
	// disable gpiof
	GPIO_PORTF_IM_R &= ~SCLKE;
	return;
}


/*
///////////////////
// send_spi3 does bit-bang SPI for port1 <<< with the addition of the SPI NVRAM, the QSPI implementation is now deprecated -- !! DO NOT USE !! >>>
//
uint8_t send_spi3(uint8_t data)
{

#if (USE_QSPI == 1)

//	uint8_t di = 0;

    wait_reg1(&GPIO_PORTE_DATA_R, BUSY_N, BUSY_WAT);	// wait for LCD busy to set
	while((SSI3_CR1_R & SSI_CR1_EOT) == 1);				// wait for eot and not busy
//	SSI3_ICR_R = SSI_ICR_EOTIC;							// pre-clear EOT flag
	SSI3_DR_R = ~data;									// invert data to compensate for 74HCT04
//	di = SSI3_DR_R;
//	SSI3_ICR_R = SSI_ICR_EOTIC;							// clear EOT flag
	return 0;

#else
	// use bit-bang SSI (for LCD -- checks for LCD BUSY == 1 before sending)
	//

//    wait_reg1(&GPIO_PORTE_DATA_R, BUSY_N, BUSY_WAT);	// wait for LCD busy to set
    shift_spi(~data);									// invert LCD data because of the inverting level shifter driving the LCD chips (NVRAM doesn't get inverted)
	return 0;
#endif
}

//-----------------------------------------------------------------------------
// shift_spi() drives shift engine for bit-bang SSI
//	returns receive data (requires NVRAM CS to be low to receive NVRAM data,
//	else all you get is the lock switch status)
//	Uses ssiflag trigger from Timer1B_ISR to apply the clock period to each edge
//-----------------------------------------------------------------------------
U8 shift_spi(U8 dato){
	U8	i;
	U8	datain = 0;

	for(i=0x80;i;i >>= 1){
//		if(i & dato) GPIO_PORTD_DATA_R |= MOSI_N;		// set MOSI
//		else GPIO_PORTD_DATA_R &= ~MOSI_N;				// clear MOSI
//		GPIO_PORTD_DATA_R |= SCK;						// clr SCK (it is inverted before reaching the slave devices)
		ssiflag = 0;
		while(!ssiflag);								// delay 1/2 bit time
		ssiflag = 0;
//		if(GPIO_PORTB_DATA_R & MISO_LOCK) datain |= i;	// capture MISO == 1
//		GPIO_PORTD_DATA_R &= ~SCK;						// set SCK
		ssiflag = 0;
		while(!ssiflag);								// delay 1/2 bit time
	}
	ssiflag = 0;
	while(!ssiflag);									// delay 1/2 bit time
	return datain;
}

//-----------------------------------------------------------------------------
// put_spi() does puts to the SPI.  The first byte of the string specifies the
//	length, CS, and C/D settings per the bitmap:
//	CS2_MASK	0x80	= CS2
//	CS1_MASK	0x40	= CS1
//	DA_CM_MASK	0x20	= data/cmd
//	LEN_MASK	0x1f	= string length
//-----------------------------------------------------------------------------
int put_spi(const U8 *string, U8 mode){
//	U8	i;
//	U8	k;

	if(mode & CS_OPEN){
//		open_spi((*string) & CS2_MASK);			// activate CS
	}
//	wait2(100);
//	lcd_cmd((*string) & DA_CM_MASK);			// set cmd/data
//	k = (*string++) & LEN_MASK;
//	for(i=0; i<k; i++){							// send data
//		send_spi3(*string++);
//	}
	if(mode & CS_CLOSE){
		close_spi();							// close CS
	}
	return 0;
}

///////////////////
// spi3_clean clears out the rx fifo.
//
void spi3_clean(void)
{
	volatile uint8_t di = 0;

#if (USE_QSPI == 1)
	while(SSI1_SR_R & SSI_SR_RNE){						// repeat until FIFO is empty
	di = SSI1_DR_R;
	}
#endif
	return;
}

///////////////////
// open_spi starts an spi message by lowering CS to the addressed device (1 or 0)
//
void open_spi(uint8_t addr)
{
//	putchar_bQ('0');
	TIMER1_CTL_R |= (TIMER_CTL_TBEN);					// enable bit timer
//	GPIO_PORTE_ICR_R = (BUSY_N);						// pre-clear edge flag
	if(addr){
		GPIO_PORTD_DATA_R |= (CS2);						// open IC1 /CS
	}else{
		GPIO_PORTD_DATA_R |= (CS1);						// open IC2 /CS
	}
	// wait for busy == 0
	wait_busy0(3);										// wait 3ms (max) for busy to release
	return;
}

///////////////////
// close_spi closes an spi message by raising CS
//
void close_spi(void)
{
	wait_busy1(3);										// wait 3m (max) for busy to release
//	wait(3);											// delay busy
//    wait_reg0(&GPIO_PORTE_DATA_R, BUSY_N, BUSY_WAT);	// wait up for not busy
	GPIO_PORTD_DATA_R &= ~(CS1 | CS2);					// close all SPI /CS
	TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);					// disable timer
//	putchar_bQ('1');
	return;
}

///////////////////
// lcd_cmd activates updates the DA_CM GPIO
// if cdata != 0, DA_CM = 1; else DA_CM = 0
//
void lcd_cmd(U8 cdata)
{
	if(cdata){
//		GPIO_PORTE_DATA_R |= DA_CM;						// set data
	}else{
//		GPIO_PORTE_DATA_R &= ~DA_CM;					// set cmd
	}
//	wait(1);
	return;
}

///////////////////////////////////////////////////////////////////////////
// NVRAM support Fns
///////////////////////////////////////////////////////////////////////////

///////////////////
// open_nvr starts an spi message by lowering RAMCS_N
//
void open_nvr(void)
{

//	putchar_bQ('c');
	TIMER1_CTL_R |= (TIMER_CTL_TBEN);					// enable bit timer
//	GPIO_PORTD_DATA_R &= ~RAMCS_N;						// open NVRAM
	ssiflag = 0;
	while(!ssiflag);									// sync to bit timer ISR & setup time
	return;
}

///////////////////
// close_nvr closes an spi message by raising RAMCS_N
//
void close_nvr(void)
{

//	GPIO_PORTD_DATA_R |= RAMCS_N;						// close NVRAM
	ssiflag = 0;
	while(!ssiflag);									// hold time (1/2 bit)
	TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);					// disable timer
//	putchar_bQ('d');
	return;
}

///////////////////
// wen_nvr sends write enable cmd to the NVRAM
//
void wen_nvr(void)
{

	open_nvr();
	shift_spi(WREN);
	close_nvr();
	return;
}

///////////////////
// storecall_nvr sends store or recall cmd to the NVRAM
//	tf_fl == 1 is store, else recall
//
void storecall_nvr(U8 tf_fl)
{

	open_nvr();
	if(tf_fl) shift_spi(STORE);
	else shift_spi(RECALL);
	close_nvr();
	return;
}

///////////////////
// rws_nvr sends status-rw cmd to the NVRAM
//
//	set hi-bit of mode (CS_WRITE) to signal write
//
U8 rws_nvr(U8 dataw, U8 mode)
{
	U8	i;		// temp

	if(mode & CS_WRITE) i = WRSR;
	else i = RDSR;
	open_nvr();
	shift_spi(i);
	i = shift_spi(dataw);
	close_nvr();
	return i;
}

///////////////////
// rw8_nvr sends byte-rw cmd to the NVRAM
//
// Supports repeated write/read based on state of mode flag:
//	if mode = OPEN, assert CS and sent r/w cmd and address
//	if mode = CLOSE, de-assert CS at end
//	else, just send/rx data byte
//	set hi-bit of mode (CS_WRITE) to signal write
//
U8 rw8_nvr(U32 addr, U8 dataw, U8 mode)
{
	U8	i;		// temp

	if(mode & CS_OPEN){
		if(mode & CS_WRITE){
			i = WRITE;
			wen_nvr();							// have to enable writes for every cycle
		}else{
			i = READ;
		}
		open_nvr();
		shift_spi(i);
		shift_spi((U8)(addr >> 16));
		shift_spi((U8)(addr >> 8));
		shift_spi((U8)(addr));
	}
	i = shift_spi(dataw);
	if(mode & CS_CLOSE){
		close_nvr();
	}
	return i;
}

///////////////////
// rw16_nvr sends word-rw cmd to the NVRAM
//
// Supports same modes as rw8_nvr()
//
U16 rw16_nvr(U32 addr, U16 dataw, U8 mode)
{
	U8	i = 0;		// temp
	U16	ii;

	if(mode & CS_WRITE){
		i = (U8)dataw;
	}
	ii = (U16)rw8_nvr(addr, i, (mode&(CS_WRITE | CS_OPEN)) );
	if(mode & CS_WRITE) i = (U8)(dataw >> 8);
	ii |= ((U16)rw8_nvr(addr+1, i, mode & CS_CLOSE)) << 8;
	return ii;
}

///////////////////
// rw32_nvr sends wword-rw cmd to the NVRAM
//
// Supports same modes as rw8_nvr()
//
U32 rw32_nvr(U32 addr, U32 dataw, U8 mode)
{
	U8	i = 0;		// temp
//	U8	j;
	U32	ii;
	U32	jj = dataw;

	// first (low) byte:
	if(mode & CS_WRITE){
		i = (U8)jj;
	}
	ii = (U32)rw8_nvr(addr, i, (mode&(CS_WRITE | CS_OPEN)) );
	// 2nd byte:
	if(mode & CS_WRITE){
		jj >>= 8;
		i = (U8)jj;
	}
	ii |= (U32)rw8_nvr(addr+1, i, (mode&(CS_WRITE)) ) << 8;
	// 3rd byte:
	if(mode & CS_WRITE){
		jj >>= 8;
		i = (U8)jj;
	}
	ii |= (U32)rw8_nvr(addr+2, i, (mode&(CS_WRITE)) ) << 16;
	// 4th (hi) byte:
	if(mode & CS_WRITE){
		jj >>= 8;
		i = (U8)jj;
	}
	ii |= (U32)rw8_nvr(addr+3, i, (mode&(CS_WRITE | CS_CLOSE)) ) << 24;

	return ii;
}

///////////////////
// rwusn_nvr r/w NVRAM user seria#
//
//
void rwusn_nvr(U8* dptr, U8 mode)
{
	U8	i;
	U8	j;

	if(mode&CS_WRITE) wen_nvr();
	open_nvr();
	if(mode&CS_WRITE) shift_spi(WRSNR);
	else shift_spi(RDSNR);
	for(i=0; i<16; i++, dptr++){
		j = shift_spi(*dptr);
		if(!(mode&CS_WRITE)) *dptr = j;
	}
	close_nvr();
	return;
}
*/

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
//	Timer1_B runs as 16 bit reload timer set to 5us interrupt rate
//	This gives a 100KHz SPI clk
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

