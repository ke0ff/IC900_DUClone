/********************************************************************
 ************ COPYRIGHT (c) 2015 by KE0FF, Taylor, TX   *************
 *
 *  File name: spi.h
 *
 *  Module:    Control
 *
 *  Summary:   defines and global declarations for spi.c
 *
 *******************************************************************/

#include "typedef.h"
#include <stdint.h>
#ifndef SPI_H_
#define SPI_H_


//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define	BBSPI			// enable bit-bang SPI

// spi defines
#define	SPI_LEN		256
#define	SPI_OVFLW	0x80
#define	SPI_CSSEG	0x40

// Processor I/O assignments
// ...see init.h

#define	BITRATE		2000000L
#define	CPSDVSR		64L
#define	SCRVAL		((SYSCLK/(CPSDVSR * BITRATE))-1)

// TIMER1B isr mask
#define TIMER_MIS_BMASK	(TIMER_MIS_TBMMIS | TIMER_MIS_CBEMIS | TIMER_MIS_CBMMIS | TIMER_MIS_TBTOMIS)

// putspi defines
#define	CS_OPEN			1
#define	CS_CLOSE		2
#define	CS_OPENCLOSE	(CS_OPEN | CS_CLOSE)
#define	CS_IDLE			0
#define	CS_READ			0
#define	CS_WRITE		0x80

// BUSY WAIT TIMEOUT
#define	BUSY_WAT	5					// max delay to wait for UART2 TX to clear (ms)


//-----------------------------------------------------------------------------
// Global Fns
//-----------------------------------------------------------------------------

void dbg_spirx(U8* sptr, U8 len, U8* buf);


void init_ssi0(void);
U8 get_csseg(void);
U8 got_ssi0(void);
U16 get_ssi0(void);
void ssi0_isr(void);
void gpiof_isr(void);
U8 is_ssito(void);
void kick_ssito(void);
void Timer1B_ISR(void);

/*uint8_t send_spi3(uint8_t data);
int put_spi(const U8 *string, U8 mode);
void open_spi(uint8_t addr);
void close_spi(void);
void spi1_clean(void);
void lcd_cmd(U8 cmd);
void send_so(uint32_t data);
// NVRAM Fns
void close_nvr(void);
void wen_nvr(void);
void storecall_nvr(U8 tf_fl);
U8 rws_nvr(U8 dataw, U8 mode);
U8 rw8_nvr(U32 addr, U8 dataw, U8 mode);
U16 rw16_nvr(U32 addr, U16 dataw, U8 mode);
U32 rw32_nvr(U32 addr, U32 dataw, U8 mode);
void rwusn_nvr(U8* dptr, U8 mode);
void Timer1B_ISR(void);*/

#endif /* SPI_H_ */
