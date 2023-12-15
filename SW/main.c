/********************************************************************
 ************ COPYRIGHT (c) 2023 by ke0ff, Taylor, TX   *************
 *
 *  File name: main.c
 *
 *  Module:    Control
 *
 *  Summary:
 *  This is the main code file for the IC-900 DU/LCD Clone application.
 *
 *******************************************************************/

/********************************************************************
 *  Project scope rev notes:
 *  !! The TI datasheet lies !! UART: configuring 2 stop bits requires 2 stop bits for RX also
 *
 *  Summary
 *  This project produces an LCD display that is form-fit-function interchangeable with the LCD system used
 *  on the IC-900 Remote Controller.  The Tiva captures SPI messages from the host to the uPD7225 LCD interface
 *  ICs (two are used for the IC-900), interprets the commands/data, and uses them to drive graphic patterns
 *  on a dot-matrix LCD display.
 *
 *  The LCD used here is a 240x128, parallel bus, monochrome display offered by NewHaven, P/N NHD-240128WG-ATMI-VZ#
 *
 *
 *
 *    Project scope rev History:
 *    In-process code-n-test.
 *    12-08-23:		Added blink code to process_LCD().  Includes refresh of original segments when blink mem is written
 *    					(allows system to recover the original symbol when the blink segment is "unblinked").
 *    					Trapped BLINKOFF SPI cmd to re-trigger any blinking segments.
 *    				Some preliminary SSI RX testing: The way to get an ISR hit for every byte received is to use the RX timeout
 *    					feature (this basically bypasses the FIFO).  This requires the SSI clock to be set to a value that will
 *						give a favorable result.  32 SSI clocks are required for the timeout.  So, the SSI clock needs to be
 *						set to 1/(32*(tword+tclearance)). tword is the minimum time between SSI bytes and tclearance is the
 *						overhead time needed to service the SSI ISR before the next SSI word starts.  The RTIM mask bit needs
 *						to be set, and the timeout needs to be cleared manually inside the SSI ISR.
 *						FSS does not seem to be used by the SSI as a slave.  Only the SCLK and data seem to be needed.
 *    12-07-23:		Ran a full segment unit test from a simulated SPI stream capture to LCD display.  Debugged
 *    					a couple of issues to get the CS1 and CS2 paths functional and all segments tested/verified.
 *    12-06-23:		Added CMD/DATA gpio to capture status of this "address" signal by SPI.
 *    				Preliminary unit test of SPI buffer data flow to LCD display was successful (after some debug).
 *	  12-04-23		Pieces are coming together.  Got basic LCD ram->display mechanism working.  Starting to add
 *						SPI stuff. Need to code LCD commands and work out the processing of the the SPI buffer.
 *					Added process_SPI with most of a dispatch switch to process SPI cmds/data (need a scrub and DVT run)
 *
 *    11-21-23 jmh:  creation date
 *    				 <VERSION 0.0>
 *
 *******************************************************************/

//-----------------------------------------------------------------------------
// main.c
//  Interfaces IC-900 LCD SPI interface to a dot-matrix LCD
//  UART0 is used for debug I/O.
//
//  Debug CLI is a simple maintenance/debug port (via UART1 @PB[1:0]) with the following core commands:
//		? - help
//		VERS - interrogate SW version
//		LT - LCD Test cmd
//		See "cmd_fn.c" for CLI details
//
//	Uses GPIO to drive LCD 8-bit bus I/O
//
//	SSI0 inputs LCD SPI data (RX only)
//
//	GPIO is used for the LCD parallel interface.  PB = bi-directional 8-bit port, PE = interface lines for 8b port,
//		and 2 GPIOs on port D to drive mode signals to the LCD.
//
//  Interrupt Resource Map:
//	*	Timer3A			--			app timer
//	*	SSI0			PA2/4:		LCD interface ICs (uPD7225) SPI
//	*	UART0 			PA[1:0]:	ISR(RX/TX) debug serial port
//	*	M1PWM (4)		PF1:		LED PWM to drive LCD backlight
//		ADC0			PD2:		Ambient light sensor
//
//
//  I/O Resource Map:
//      See "init.h"
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
// compile defines

#define MAIN_C
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>
#include "init.h"
#include "typedef.h"
#include "version.h"
#include "serial.h"
#include "cmd_fn.h"
#include "tiva_init.h"
#include "eeprom.h"
#include "lcd_db.h"
//#include "radio.h"
#include "uxpll.h"
#include "spi.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

//  see init.h for main #defines
#define	MAX_REBUF	4		// max # of rebufs
#define	MAX_BARS	7		// max # of led bars
#define	GETS_INIT	0xff	// gets_tab() static initializer signal
#define DAC_LDAC 20			// dac spi defines
#define DAC_PWRUP 21
#define DAC_CLR 22


// quick beep macro
/*#define	q_beep   beep_counter = beep_count; \
				 TIMER0_CTL_R |= (TIMER_CTL_TAEN);

// dial beep macro
#define	d_beep   beep_counter = DIAL_BEEP_COUNT; \
				 TIMER0_CTL_R |= (TIMER_CTL_TAEN);*/

//-----------------------------------------------------------------------------
// Local Variables
//-----------------------------------------------------------------------------

// Processor I/O assignments
//bitbands
#define DUT_ON  (*(volatile uint32_t *)(0x40058000 + (0x04 * 4)))
#define DUT_OFF (*(volatile uint32_t *)(0x40058000 + (0x08 * 4)))
#define DUT_SCK (*(volatile uint32_t *)(0x40058000 + (0x10 * 4)))
#define DUT_D   (*(volatile uint32_t *)(0x40058000 + (0x20 * 4)))
#define DUT_Q   (*(volatile uint32_t *)(0x40058000 + (0x40 * 4)))
#define DUT_CS  (*(volatile uint32_t *)(0x40058000 + (0x80 * 4)))

//-----------------------------------------------------------------------------
// Global variables (extern conditionals are in init.h)
//-----------------------------------------------------------------------------
char	bchar;							// break character trap register - traps ESC ascii chars entered at terminal
char	swcmd;							// software command flag
S8		handshake;						// xon/xoff enable
S8		xoffsent;						// xoff sent

//-----------------------------------------------------------------------------
// Local variables in this file
//-----------------------------------------------------------------------------
char	got_cmd;						// first valid cmd flag (freezes baud rate)
U32		abaud;							// 0 = 115.2kb (the default)
U8		iplt2;							// timer2 ipl flag
//U8		btredir;						// bluetooth cmd re-direct flag
U16		waittimer;						// gp wait timer
U16		waittimer2;						// gp wait timer
U16		ipl_timer;						// ipl timeout timer
U8		cmdtimer;						// cmd_ln GP timer
U16		blinktimer;						// blink regs
U8		blinkfl;

U32		free_32;						// free-running ms timer
S8		err_led_stat;					// err led status
U8		idx;
U16		ipl;							// initial power on state
char	btbuf[100];						// temp buffer

U8		led_5_on;
U8		led_6_on;
//U8		led_2_level;					// led level regs (0-100%)
//U8		led_3_level;
//U8		led_4_level;
U8		led_5_level;
U8		led_6_level;
//U16		pwm2_reg;						// led pwm regs
//U16		pwm3_reg;
//U16		pwm4_reg;
U16		pwm5_reg;
U16		pwm6_reg;
U8		pwm_master;						// led master level
U8		sw_state;
U8		sw_change;
//char	dbbuf[30];	// debug buffer
//U8		dbug8;
U8		hib_access;						// HIB intrpt lock-out flag

//-----------------------------------------------------------------------------
// Local Prototypes
//-----------------------------------------------------------------------------

void Timer_Init(void);
void Timer_SUBR(void);
char *gets_tab(char *buf, char *save_buf[3], int n);
//char kp_asc(U16 keycode);

//*****************************************************************************
// main()
//  The main function runs a forever loop in which the main application operates.
//	Prior to the loop, Main performs system initialization, boot status
//	announcement, and main polling loop.  The loop also calls CLI capture/parse fns.
//	The CLI maintains and processes re-do buffers (4 total) that are accessed by the
//	TAB key.  Allows the last 4 valid command lines to be recalled and executed
//	(after TAB'ing to desired recall command, press ENTER to execute, or ESC to
//	clear command line).
//	Autobaud rate reset allows user to select alternate baudarates after reset:
//		115,200 baud is default.  A CR entered after reset at 57600, 38400,
//		19200, or 9600 baud will reset the system baud rate and prompt for user
//		acceptance.  Once accepted, the baud rate is frozen.  If rejected, baud rate
//		returns to 115200.  The first valid command at the default rate will also
//		freeze the baud rate.  Once frozen, the baud rate can not be changed until
//		system is reset.
//*****************************************************************************
int main(void){
	volatile uint32_t ui32Loop;
//	uint32_t i;
//    uint8_t	tempi;			// tempi
    char	buf[CLI_BUFLEN];	// command line buffer
    char	rebuf0[RE_BUFLEN];	// re-do buffer#1
    char	rebuf1[RE_BUFLEN];	// re-do buffer#2
    char	rebuf2[RE_BUFLEN];	// re-do buffer#3
    char	rebuf3[RE_BUFLEN];	// re-do buffer#4
    U8		argn;				// number of args
    char*	cmd_string;			// CLI processing ptr
    char*	args[ARG_MAX];		// ptr array into CLI args
    char*	rebufN[4];			// pointer array to re-do buffers
    U16		offset;				// srecord offset register
    U16		cur_baud;			// current baud rate
    U8		fault_found;		// fault detected flag

    fault_found = FALSE;								// only trap one fault-restart
	got_cmd = FALSE;
	offset = 0;
	cur_baud = 0;
    iplt2 = 1;											// init timer1
    ipl = proc_init();									// initialize the processor I/O
 	while(iplt2);										// wait for timer to finish intialization
	dispSWvers(); 										// display reset banner
	init_ssi0();
	process_LCD(0xff);
    do{													// outer-loop (do forever, allows soft-restart)
        rebufN[0] = rebuf0;								// init CLI re-buf pointers
    	rebufN[1] = rebuf1;
    	rebufN[2] = rebuf2;
    	rebufN[3] = rebuf3;
    	rebuf0[0] = '\0';								// clear cmd re-do buffers
    	rebuf1[0] = '\0';
    	rebuf2[0] = '\0';
    	rebuf3[0] = '\0';
//    	bcmd_resp_init();								// init bcmd response buffer
    	wait(10);										// a bit more delay..
//    	GPIO_PORTB_DATA_R &= ~PTT7K;					// set PTT7K in-active
//    	GPIO_PORTD_DATA_R &= ~PTTb;						// set PTTb in-active
    	while(gotchrQ()) getchrQ();						// clear serial input in case there was some POR garbage
    	gets_tab(buf, rebufN, GETS_INIT);				// initialize gets_tab()
        set_led(0xff, 0x00);							// init LED levels
        set_pwm(0, 99);									// master = 100%
        set_led(5, 1);									// enable LEDs at 10% until the OS takes over
        set_pwm(5, 10);
        set_led(6, 1);
        set_pwm(6, 10);
    	process_IO(PROC_INIT);							// init process_io
//    	btredir = 0;
//    	sprintf(buf,"NVsrt %0x, NVend %0x", NVRAM_BASE, MEM_END);
//    	putsQ(buf);
    	// GPIO init
    	//...
    	// main loop
        do{
    		putchar0(XON);
    		buf[0] = '\0';
    		putssQ("rdu>");										// prompt
    		cmd_string = gets_tab(buf, rebufN, RE_BUFLEN); 		// get cmd line & save to re-do buf
    		if(!got_cmd){										// if no valid commands since reset, look for baud rate change
    			if(cur_baud != abaud){							// abaud is signal from gets_tab() that indicates a baud rate change
    				if(set_baud(abaud)){						// try to set new baud rate
    					putsQ("");								// move to new line
    					dispSWvers();							// display reset banner & prompt to AKN new baud rate
    					while(gotchrQ()) getchrQ();				// clear out don't care characters
    					putssQ("press <Enter> to accept baud rate change: ");
    					while(!gotchrQ());						// wait for user input
    					putsQ("");								// move to new line
    					if(getchrQ() == '\r'){					// if input = CR
    						cur_baud = abaud;					// update current baud = new baud
    						got_cmd = TRUE;						// freeze baud rate
    					}else{
    						set_baud(0);						// input was not a CR, return to default baud rate
    						cur_baud = abaud = 0;
    					}
    				}else{
    					abaud = cur_baud;						// new baud rate not valid, ignore & keep old rate
    				}
    			}else{
    				got_cmd = TRUE;								// freeze baud rate (@115.2kb)
    			}
    		}
    		argn = parse_args(cmd_string,args);					// parse cmd line
    		if(x_cmdfn(argn, args, &offset)) got_cmd = TRUE;	// process cmd line, set got_cmd if cmd valid
    		if((NVIC_FAULT_STAT_R) && !fault_found){			// test for initial fault
    	        putsQ("nvic FLT\n");							// fault message
    			swcmd = SW_ESC;									// abort to restart
    			fault_found = TRUE;
    		}
        }while(swcmd != SW_ESC);
// this is done in process IPL        swcmd = 0;												// re-arm while-loop and restart...
/*    	NVIC_DIS0_R = 0xFFFFFFFF;								// disable ISRs
    	NVIC_DIS1_R = 0xFFFFFFFF;
    	NVIC_DIS2_R = 0xFFFFFFFF;
    	NVIC_DIS3_R = 0xFFFFFFFF;
    	NVIC_DIS4_R = 0xFFFFFFFF;*/
    }while(1);
}

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// gets_tab puts serial input into buffer, UART0.
//-----------------------------------------------------------------------------
// Main loop for command line input.
// waits for a chr and puts into buf.  If 1st chr = \t, copy re-do buf into
//  cmdbuf and cycle to next re-do buf.  if more than n chrs input, nul term buf,
//	disp "line too long", and return.  if \n or \r, copy buf to save_buf & return
//  returns buf (pointer).
//	if n == 0xff, initialize statics and exit.
//
//	11/08/13: Modified to support 4 (MAX_REBUF) rolling cmd save buffers
//	11/15/13: Modified to support auto-baud detect on CR input
//		For the following, each bit chr shown is one bit time at 115200 baud (8.68056us).
//			s = start bit (0), p = stop bit (1), x = incorrect stop, i = idle (1), bits are ordered  lsb -> msb:
//	 the ascii code for CR = 10110000
//			At 115.2kb, CR = s10110000p = 0x0D
//
//			At 57.6 kb, CR = 00110011110000000011 (1/2 115.2kb)
//			@115.2, this is: s01100111ps00000001i = 0xE6, 0x80
//
//			At 38.4 kb, CR = 000111000111111000000000000111 (1/3 115.2kb)
//			@115.2, this is: s00111000p11111s00000000xxxiii = 0x1c, 0x00
//
//			At 19.2 kb, CR = 000000111111000000111111111111000000000000000000000000111111 (1/6 115.2kb)
//			@115.2, this is: s00000111piis00000111piiiiiiiis00000000xxxxxxxxxxxxxxxiiiiii = 0xE0, 0xE0, 0x00
//
//			At 9600 b,  CR = 000000000000111111111111000000000000111111111111111111111111000000000000000000000000000000000000000000000000111111111111 (1/12 115.2kb)
//			@115.2, this is: s00000000xxxiiiiiiiiiiiis00000000xxxiiiiiiiiiiiiiiiiiiiiiiiis00000000xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxiiiiiiiiiiii = 0x00, 0x00, 0x00
//
//		Thus, @ 57.6 kb, a CR = 0xE6 followed by 0x80
//			  @ 38.4 kb, a CR = 0x1C followed by 0x00
//			  @ 19.2 kb, a CR = 0xE0 followed by 0xE0 (plus a 0x00)
//			  @ 9600 b, a  CR = 0x00 followed by 0x00 (plus a 0x00)
//
//		NOTE: gets_tab is only used for command line input and thus should not
//		see non-ascii data under normal circumstances.
//		re-NOTE: The wired remote sees non-ASCII in the check byte by design.  Need to refurb this code...

char *gets_tab(char *buf, char *save_buf[], int n){
	char	*cp;
	char	*sp;
	char	c;
volatile	char	q = 0;
	int		i = 0;
	U8		j;
	U8		se = 0;						// status enable reflection.  If enabled, suppress echo
	static	U8   rebuf_num;
//	static	U8	 last_chr;

//	if((rebuf_num >= MAX_REBUF) || (n == GETS_INIT)){ // n == 0xff is static initializer signal
	if(n == GETS_INIT){ // n == 0xff is static initializer signal
		rebuf_num = 0;									// init recall buffer pointer
//		last_chr = 0xff;								// init to 0xff (not a valid baud select identifier chr)
		return buf;										// skip rest of Fn
	}
    cp = buf;
    sp = save_buf[rebuf_num];
    do{
    	do{
    		// run the process loops and capture UART chars
    		process_IO(0);
    		c = getchQ();
//    		if(c){
//    			getss(btbuf);
//    			c = *btbuf;
//    			if(*btbuf){
//        			putssQ("{");
//        			putssQ(btbuf);
//        			putsQ("}");
//    			}
//    		}
    	}while(!c && !q);
    	if(!q){
 /*   		if(!got_cmd){
                switch(c){
        			case 0xE0:									// look for 19.2kb autoselect
        				if(last_chr == 0xE0){
        					abaud = 19200L;
        					c = '\r';
        				}
        				break;

        			case 0x00:									// look for 38.4kb or 9600b autoselect
        				if(last_chr == 0x1C){
        					abaud = 38400L;
        					c = '\r';
        				}else{
        					if(last_chr == 0x00){
        						abaud = 9600L;
        						c = '\r';
        					}
        				}
        				break;

        			case 0x80:									// look for 57.6kb autoselect
        				if(last_chr == 0xE6){
        					abaud = 57600L;
        					c = '\r';
        				}
        				break;
                }
    		}*/
            switch(c){
/*    			case 0xE0:									// look for 19.2kb autoselect
    				if(last_chr == 0xE0){
    					abaud = 19200L;
    					c = '\r';
    				}
    				break;

    			case 0x00:									// look for 38.4kb or 9600b autoselect
    				if(last_chr == 0x1C){
    					abaud = 38400L;
    					c = '\r';
    				}else{
    					if(last_chr == 0x00){
    						abaud = 9600L;
    						c = '\r';
    					}
    				}
    				break;

    			case 0x80:									// look for 57.6kb autoselect
    				if(last_chr == 0xE6){
    					abaud = 57600L;
    					c = '\r';
    				}
    				break;*/

                case '\t':
    				if(i != 0){								// if tab, cycle through saved cmd buffers
    					do{
    						i--;							// update count/point
    						cp--;
    						if((*cp >= ' ') && (*cp <= '~')){
    							if(!se){
        							putchar0('\b');		// erase last chr if it was printable
        							putchar0(' ');
        							putchar0('\b');
        							kickuart0();
    							}
    						}
    					}while(i != 0);
    					cp = buf;							// just in case we got out of synch
    				}
    				//copy saved string up to first nul, \n, or \r
    				j = rebuf_num;
    				do{
    					if(--rebuf_num == 0xff){
    						rebuf_num = MAX_REBUF - 1;
    					}
    					if(*save_buf[rebuf_num]) j = rebuf_num;
    				}while(j != rebuf_num);
    				sp = save_buf[rebuf_num];
    				while((*sp != '\0') && (*sp != '\r') && (*sp != '\n')){
    					putdchQ(*sp);
    					*cp++ = *sp++;
    					i++;
    				}
    				kickuart0();
                    break;

                case '\b':
                case 0x7f:
                    if(i != 0){								// if bs & not start of line,
                        i--;								// update count/point
                        cp--;
                        if((*cp >= ' ') && (*cp <= '~')){
							if(!se){
    							putchar0('\b');			// erase last chr if it was printable
    							putchar0(' ');
    							putchar0('\b');
    							kickuart0();
							}
                        }
                    }
                    break;

                case '\r':									// if cr, nul term buf & exit
                case '\n':									// if nl, nul term buf & exit
                    i++;
                    *cp++ = c;
                    putchar_bQ('\n');						// no cntl chrs here
                    break;

                case ESC:									// if esc, nul buf & exit
                    cp = buf;
                    c = '\r';								// set escape condition
    				i = 0;
    				putchar_bQ('\n');						// no cntl chrs here
                    break;

                case BT_LN:									// if bluetooth cmd, do nothing
                    break;

                default:
                    i++;
                    *cp++ = c;								// put chr in buf
                    if(!se){
                        putdchQ(c);							// no cntl chrs here
//						kickuart0();
                    }
                    break;
            }
    	}
//		if(c != BT_LN) last_chr = c;					// set last chr
    } while((c != '\r') && (c != '\n') && (i < CLI_BUFLEN) && (c != BT_LN));		// loop until c/r or l/f or buffer full
    if(c == BT_LN){
//    	if(btredir){
//    		buf = get_btptr();
//    	}else{
//        		process_CCMD(0);
//    	}
    }else{
    	if(i >= CLI_BUFLEN){
    		putsQ("#! buffer overflow !$");
    		*buf = '\0';								// abort line
    	}else{
    		if(!se){
        		putsQ("");								// echo end of line to screen
    		}
    		*cp = '\0';									// terminate command line
    		if((*buf >= ' ') && (*buf <= '~')){			// if new buf not empty (ie, 1st chr = printable),
    			strncpy(save_buf[rebuf_num], buf, n);	// copy new buf to save
    			if(++rebuf_num >= MAX_REBUF) rebuf_num = 0;
    		}
    	}
    }
    return buf;
}

//-----------------------------------------------------------------------------
// process_IO() processes system I/O
//-----------------------------------------------------------------------------
void process_IO(U8 flag){

	// perform periodic process updates					// ! SOUT init must execute before SIN init !
	process_CMD(flag);									// process CMD_FN state (primarily, the MFmic key-entry state machine)
	process_LCD(flag);									// lcd updates and blink machine
	process_SPI(flag);									// spi input processing
//	process_ERR(flag);									// error reporting
	return;
}

//-----------------------------------------------------------------------------
// set_pwm() sets the specified PWM to the percent value
//	pwmnum specifies the LED number.  LED 0 is the master setting.  This only sets the master,
//		it does not update any of the LED pwm registers.
//	percent is the percent level, 0 - 100.  If percent > 100, the value is unchanged
//		and the PWM settings are calculated based on stored led and master values
//-----------------------------------------------------------------------------
void set_pwm(U8 pwmnum, U8 percent){
	U32	kk;				// temp U32

	if(percent <= 100){
		switch(pwmnum){						// store level % value
		case 0:								// set master value
			pwm_master = percent;
			break;

/*		case 2:
			led_2_level = percent;
			break;

		case 3:
			led_3_level = percent;
			break;

		case 4:
			led_4_level = percent;
			break;*/

		case 5:
			led_5_level = percent;
			break;

		case 6:
			led_6_level = percent;
			break;
		}
	}
	switch(pwmnum){
/*	case 2:								// LED2
		// calc PWM value in U32 to avoid overflow
		kk = PWM_ZERO - ((PWM_ZERO - PWM_MAX) * (U32)led_2_level * (U32)pwm_master / 10000L) - 1L;
		// store PWM value
		pwm2_reg = (U16)kk;
		// update pwm if led is on
		if(led_2_on) PWM1_1_CMPB_R = pwm2_reg;
		break;

	case 3:								// LED3
		kk = PWM_ZERO - ((PWM_ZERO - PWM_MAX) * (U32)led_3_level * (U32)pwm_master / 10000L) - 1L;
		pwm3_reg = (U16)kk;
		if(led_3_on) PWM1_2_CMPA_R = kk;
		break;

	case 4:								// LED4
		kk = PWM_ZERO - ((PWM_ZERO - PWM_MAX) * (U32)led_4_level * (U32)pwm_master / 10000L) - 1L;
		pwm4_reg = (U16)kk;
		if(led_4_on) PWM1_2_CMPB_R = kk;
		break;*/

	case 5:								// LED5
		kk = PWM_ZERO - (((U32)(PWM_ZERO - PWM_MAX) * (U32)led_5_level * (U32)pwm_master) / 10000L) - 1L;
		pwm5_reg = (U16)kk;
		if(led_5_on) PWM1_3_CMPA_R = kk;
		break;

	case 6:								// LED6
		kk = PWM_ZERO - ((PWM_ZERO - PWM_MAX) * (U32)led_6_level * (U32)pwm_master / 10000L) - 1L;
		pwm6_reg = (U16)kk;
		if(led_6_on) PWM1_3_CMPB_R = kk;
		break;
	}
}

//-----------------------------------------------------------------------------
// set_led() turns on/off the LED outputs
// if lednum == 0xff, do a POR init of the registers
// if lednum == 0x00, value = master %, reset all PWM values
//	else, value = 1/0 for on/off.  save to reg and update pwm reg to set on or off
//-----------------------------------------------------------------------------
void set_led(U8 lednum, U8 value){

	if(lednum == 0xff){
/*		led_2_on = 0;						// init registers all off
		led_3_on = 0;
		led_4_on = 0;*/
		led_5_on = 0;
		led_6_on = 0;
/*		led_2_level = 24;					// led level regs (0-100%)
		led_3_level = 16;
		led_4_level = 50;*/
		led_5_level = 22;
		led_6_level = 99;
		pwm_master = 100;					// led master level
		// led pwm regs
/*		pwm2_reg = (U16)(PWM_ZERO - ((PWM_ZERO - PWM_MAX) * 24L / 100L) - 1L);
		pwm3_reg = (U16)(PWM_ZERO - ((PWM_ZERO - PWM_MAX) * 16L / 100L) - 1L);
		pwm4_reg = (U16)(PWM_ZERO - ((PWM_ZERO - PWM_MAX) * 50L / 100L) - 1L);*/
		pwm5_reg = (U16)(PWM_ZERO - ((PWM_ZERO - PWM_MAX) * 22L / 100L) - 1L);
		pwm6_reg = (U16)(PWM_ZERO - ((PWM_ZERO - PWM_MAX) * 99L / 100L) - 1L);
	}else{
		// process led settings
		if(value < 2){
			switch(lednum){
/*			case 2:
				led_2_on = value;
				if(led_2_on) PWM1_1_CMPB_R = pwm2_reg;
				else PWM1_1_CMPB_R = PWM_ZERO - 1;
				break;

			case 3:
				led_3_on = value;
				if(led_3_on) PWM1_2_CMPA_R = pwm3_reg;
				else PWM1_2_CMPA_R = PWM_ZERO - 1;
				break;

			case 4:
				led_4_on = value;
				if(led_4_on) PWM1_2_CMPB_R = pwm4_reg;
				else PWM1_2_CMPB_R = PWM_ZERO - 1;
				break;*/

			case 5:
				led_5_on = value;
				if(!led_5_on) PWM1_3_CMPA_R = PWM_ZERO - 1;
				break;

			case 6:
				led_6_on = value;
				if(led_6_on) PWM1_3_CMPB_R = pwm6_reg;
				else PWM1_3_CMPB_R = PWM_ZERO - 1;
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// wrwhib_ram() writes a word to the HIBRAM array
//-----------------------------------------------------------------------------
void wrwhib_ram(U32 data, volatile U8 addr){
	volatile uint32_t* pii; // Vu32 pointer

	pii = &HIB_DATA_R;
	pii += addr;
	hib_access = HIB_APPL;						// lock out intr access
	while(!(HIB_CTL_R & HIB_CTL_WRC));			// pause until HIBRAM is ready
	*pii = data;								// write new data
	hib_access = HIB_INTR;						// enable intr access
}

//-----------------------------------------------------------------------------
// wrhib_ram() writes a databyte to the HIBRAM array using a byte address
//-----------------------------------------------------------------------------
void wrhib_ram(uint8_t data, U8 addr){
				U8			i;						// temp
	volatile	uint32_t*	pii;					// Vu32 pointer
				uint32_t	dd = (uint32_t)data;	// temp data
				uint32_t	ee;						// temp data
				uint32_t	maskdd = 0x000000ff;	// mask

	while(!(HIB_CTL_R & HIB_CTL_WRC));			// pause until HIBRAM is ready
	pii = &HIB_DATA_R;							// get base pointer (assumed to be word aligned)
	pii += (addr >> 2);							// point to desired word addr
	ee = *pii;									// get existing data
	i = addr & 0x03;							// get byte addr
	while(i){									// align byte and mask
		dd <<= 8;
		maskdd <<= 8;
		i--;
	}
	ee &= ~maskdd;								// mask existing data
	ee |= dd;									// combine new daata
	hib_access = HIB_APPL;						// lock out intr access
//	dd = HIB_CTL_R;								// do a dummy read
	*pii = ee;									// write new data
	hib_access = HIB_INTR;						// enable intr access
}

//-----------------------------------------------------------------------------
// rdwhib_ram() reads a word from the HIBRAM array using a word address
//-----------------------------------------------------------------------------
uint32_t rdwhib_ram(U8 addr){
	volatile 	uint32_t* pii;		// Vu32 pointer
				uint32_t	ee;		// temp data

	while(!(HIB_CTL_R & HIB_CTL_WRC));			// pause until HIBRAM is ready
	pii = &HIB_DATA_R;							// get base pointer
	pii += (addr);								// point to desired word addr
	ee = *pii;									// get existing data
	return ee;
}

//-----------------------------------------------------------------------------
// rdhib_ram() reads a databyte from the HIBRAM array using a byte address
//-----------------------------------------------------------------------------
uint8_t rdhib_ram(U8 addr){
				U8	i;				// temp
	volatile 	uint32_t* pii;		// Vu32 pointer
				uint32_t	ee;		// temp data

	while(!(HIB_CTL_R & HIB_CTL_WRC));			// pause until HIBRAM is ready
	pii = &HIB_DATA_R;							// get base pointer
	pii += (addr >> 2);							// point to desired word addr
	ee = *pii;									// get existing data
	i = addr & 0x03;							// get byte addr
	while(i){									// align byte to uint8_t
		ee >>= 8;
		i--;
	}
	ee &= 0xff;
	return (uint8_t)ee;
}

//-----------------------------------------------------------------------------
// waitpio() uses a dedicated ms timer to establish a defined delay (+/- 1LSB latency)
//	loops through process_IO during wait period.
//-----------------------------------------------------------------------------
void waitpio(U16 waitms){
//	U32	i;

//	i = 545 * (U32)waitms;
    waittimer = waitms;
//    for(;i!=0;i--);		// patch
    while(waittimer != 0) process_IO(0);
    return;
}

//-----------------------------------------------------------------------------
// wait() uses a dedicated ms timer to establish a defined delay (+/- 1LSB latency)
//-----------------------------------------------------------------------------
void wait(U16 waitms){
//	U32	i;

//	i = 545L * (U32)waitms;
    waittimer2 = waitms;
//    for(;i!=0;i--);		// patch
    while(waittimer2 != 0);
    return;
}

//-----------------------------------------------------------------------------
// set_wait() uses a dedicated ms timer to establish a defined delay (+/- 1LSB latency)
// is_wait() returns status of wait timer
//-----------------------------------------------------------------------------
void set_wait(U16 waitms){

    waittimer2 = waitms;
    return;
}

U8 is_wait(void){
	U8	i;	// rtrn

    if(waittimer2) i = 1;
    else i = 0;
    return i;
}

//-----------------------------------------------------------------------------
// wait2() does quick delay pace =  (5.6us * waitms)
//		Emperical measurements with SYSCLK = 50 MHz give the following
//			timing value: waitms = 2 gives a time delay of about 11.2 us
//			(about 560ns per for() cycle) {+/- interrupt variations}
//-----------------------------------------------------------------------------
void wait2(U16 waitms)
{
	U32	i;

	i = 20 * (U32)waitms;
    waittimer = waitms;
    for(;i!=0;i--);		// patch
//    while(waittimer != 0);
    return;
}

//-----------------------------------------------------------------------------
// wait_busy0() waits for (delay timer == 0) or (LCD BUSY == 0)
//	if delay expires, return TRUE, else return FALSE
//-----------------------------------------------------------------------------
U8 wait_busy0(U16 delay){
	U8 loopfl = TRUE;

    waittimer = delay;
    while(loopfl){
    	if(!waittimer) loopfl = FALSE;
//    	if(!(GPIO_PORTE_DATA_R & BUSY_N)) loopfl = FALSE;
//    	if(GPIO_PORTE_RIS_R & (BUSY_N)) loopfl = FALSE;
    }
//	GPIO_PORTE_ICR_R = (BUSY_N);						// clear edge flag
    return !waittimer;
}

//-----------------------------------------------------------------------------
// wait_busy1() waits for (delay timer == 0) or (LCD BUSY == 0)
//	then wait for BUSY_N == 1
//	if delay expires, return TRUE, else return FALSE
//-----------------------------------------------------------------------------
U8 wait_busy1(U16 delay){
	U8 loopfl = TRUE;

	wait_busy0(delay);
	waittimer = delay;
    while(loopfl){
    	if(!waittimer) loopfl = FALSE;
//    	if(GPIO_PORTE_DATA_R & BUSY_N) loopfl = FALSE;
    }
    return !waittimer;
}

//-----------------------------------------------------------------------------
// wait_reg0() waits for (delay timer == 0) or (regptr* & clrmask == 0)
//	if delay expires, return TRUE, else return FALSE
//-----------------------------------------------------------------------------
U8 wait_reg0(volatile uint32_t *regptr, uint32_t clrmask, U16 delay){
	U8 timout = FALSE;

    waittimer = delay;
    while((waittimer) && ((*regptr & clrmask) != 0));
    if(waittimer == 0) timout = TRUE;
    return timout;
}

//-----------------------------------------------------------------------------
// wait_reg1() waits for (delay timer == 0) or (regptr* & setmask == setmask)
//	if delay expires, return TRUE, else return FALSE
//-----------------------------------------------------------------------------
U8 wait_reg1(volatile uint32_t *regptr, uint32_t setmask, U16 delay){
	U8 timout = FALSE;

    waittimer = delay;
    while((waittimer) && ((*regptr & setmask) != setmask));
    if(waittimer == 0) timout = TRUE;
    return timout;
}

//-----------------------------------------------------------------------------
// getipl() returns current ipl flags value
//-----------------------------------------------------------------------------
U16 getipl(void){

	return ipl;
}

//-----------------------------------------------------------------------------
// warm_reset() triggers primary while-loop in main() to re-start.
//-----------------------------------------------------------------------------
void warm_reset(void){
	swcmd = SW_ESC;				// trigger restart
}

//-----------------------------------------------------------------------------
// free_run() returns value of free-running timer
//-----------------------------------------------------------------------------
U32 free_run(void){
	return free_32;				// return timer value
}

//-----------------------------------------------------------------------------
// get_free() returns value of free_32
//-----------------------------------------------------------------------------
U32 get_free(void){

	return free_32;
}

//-----------------------------------------------------------------------------
// is_blink() returns true if blink timer lsb is true
//-----------------------------------------------------------------------------
U8 is_blink(void){

	if(blinkfl){
		blinkfl = 0;
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Timer3A_ISR
// Called when timer3 A overflows (NORM mode):
//	intended to update app timers @ 1ms per lsb
//	also drives RGB "I'm alive" LED.  The LED transitions through the color
//	wheel in a way that can be modified by the #defines below. RATE, PERIOD,
//	and START values may be adjusted to taylor the color transitions,
//	transition rate, and cycle period.
//-----------------------------------------------------------------------------
void Timer3A_ISR(void){
static	U32	prescale;				// prescales the ms rate to the slow timer rates

//	GPIO_PORTB_DATA_R |= LOCK;		// toggle debug pin -- 2.25 us ISR exec time at 1.0003ms rate
	if(iplt2){										// if flag is set, perform ipl initialization
		iplt2 = 0;
		free_32 = 0;
		waittimer = 0;				// gp wait timer
		prescale = 0;				// init resp led regs
		blinktimer = BLINK_TIME;
		blinkfl = 0;
	}
	// process app timers
	free_32++;											// update large free-running timer
	if(waittimer != 0){									// update wait timer
		waittimer--;
	}
	if(waittimer2 != 0){								// update wait2 timer
		waittimer2--;
	}
	if(--blinktimer == 0){								// update blink timer
		blinkfl = 1;
		blinktimer = BLINK_TIME;
	}
	// process 10ms timers
	if(++prescale >= PRESCALE_TRIGGER){
		prescale = 0;
		if (ipl_timer != 0){							// update ipl timer
			ipl_timer--;
		}
	}

//	GPIO_PORTB_DATA_R &= ~LOCK;			// toggle debug pin
	TIMER3_ICR_R = TIMERA_MIS_MASK;						// clear all A-intr
	return;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

