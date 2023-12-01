/********************************************************************
 ************ COPYRIGHT (c) 2021 by ke0ff, Taylor, TX   *************
 *
 *  File name: init.h
 *
 *  Module:    Control, IC-900 RDU Controller Clone
 *
 *  Summary:   defines and global declarations for main.c
 *  			Also defines GPIO pins, timer constants, and other
 *  			GPIO system constants
 *
 *******************************************************************/

#include "typedef.h"
#include <stdint.h>

#ifndef INIT_H
#define INIT_H
#endif

#define	DEBUG 1
//#define	USE_QSPI 1

//////////////////////////////////////////////////////////////////////////////////////
//#define	USE_QSPI 1																	//
//#define	LA_ENABLE					// define if logic analyzer is enabled (debug)	//
//////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLKL 10000L
#define PIOCLK	(16000000L)			// internal osc freq (in Hz)
#define EXTXTAL           			// un-comment if external xtal is used
									// define EXTXTAL #def to select ext crystal
									// do not define for int osc
#ifdef EXTXTAL
#define XTAL 1
//#define XTAL_FREQ	SYSCTL_RCC_XTAL_16MHZ	// set value of external XTAL
#define XTAL_FREQ	SYSCTL_RCC_XTAL_20MHZ	// set value of external XTAL
#define SYSCLK	(60000000L)					// sysclk freq (bus clk)
//#define SYSCLK	(66667000L)				// sysclk freq (bus clk)
#else
#define XTAL 0
#define SYSCLK	PIOCLK						// sysclk freq (bus clk) same as PIOCLK
#endif

#define OSC_LF 4            				// osc clock selects
#define OSC_HF 0
#define OSC_EXT 1

// process_xx() defines
#define	PROC_INIT	0xff					// IPL command for process calls

#define	MS_PER_TIC	1
#define SEC10MS    (10/MS_PER_TIC)          // timer constants (ms)
#define SEC33MS    (33/MS_PER_TIC)
#define SEC50MS    (50/MS_PER_TIC)
#define SEC75MS    (74/MS_PER_TIC)
#define SEC100MS  (100/MS_PER_TIC)
#define SEC250MS  (250/MS_PER_TIC)
#define SEC300MS  (300/MS_PER_TIC)
#define SEC400MS  (400/MS_PER_TIC)
#define SEC500MS  (500/MS_PER_TIC)
#define SEC750MS  (750/MS_PER_TIC)
#define SEC1     (1000/MS_PER_TIC)
#define ONESEC   SEC1
#define MS1500   (1500/MS_PER_TIC)
#define SEC2     (2000/MS_PER_TIC)
#define SEC3     (3000/MS_PER_TIC)
#define SEC5     (5000/MS_PER_TIC)
#define SEC10   (10000/MS_PER_TIC)
#define SEC15   (15000/MS_PER_TIC)
#define SEC30   (30000/MS_PER_TIC)
#define SEC60   (60000/MS_PER_TIC)
#define SEC300 (300000L/MS_PER_TIC)
#define	ONEMIN	(SEC60)
#define	REG_WAIT_DLY 		200				// 200ms wait limit for register action
#define RESP_SAMP 			SEC100MS		// sets resp rate
#define	SOUT_PACE_TIME		9				// 33/4800 s for one SOUT msg
#define	SIN_PACE_TIME		50				// wait a little over 2 word times (2 x 32/4800 s for one SIN word)
#define	CLI_BUFLEN	100						// CLI buffer length
#define	RE_BUFLEN	40						// buffer mem lengths

// prescaled timer constants
#define	PS_PER_TIC	10
#define	PRESCALE_TRIGGER (PS_PER_TIC)		// establishes 10ms/lsb prescaled timer rate
#define PSEC10MS    (10/PS_PER_TIC)
#define	PSEC33MS    (33/PS_PER_TIC)
#define	PSEC50MS    (50/PS_PER_TIC)
#define	PSEC75MS    (74/PS_PER_TIC)
#define	PSEC100MS  (100/PS_PER_TIC)
#define	PSEC250MS  (250/PS_PER_TIC)
#define	PSEC300MS  (300/PS_PER_TIC)
#define	PSEC400MS  (400/PS_PER_TIC)
#define	PSEC500MS  (500/PS_PER_TIC)
#define	PSEC750MS  (750/PS_PER_TIC)
#define	PSEC1     (1000/PS_PER_TIC)
#define	PSEC2     (2000/PS_PER_TIC)
#define	PSEC3     (3000/PS_PER_TIC)
#define	PSEC5     (5000/PS_PER_TIC)
#define	PSEC10   (10000/PS_PER_TIC)
#define	PSEC15   (15000/PS_PER_TIC)
#define	PSEC30   (30000/PS_PER_TIC)
#define	PSEC60   (60000/PS_PER_TIC)
#define	PSEC300 (300000L/PS_PER_TIC)

#define	CLEAR_TIMER		0xff				// timer control Fn signals
#define	READ_TIMER		0x00
#define	SET_TIMER		0x01
#define	CATA_TIME		PSEC2				// CAT activity rate
#define	CATO_TIME		PSEC10				// CAT timeout rate
#define	CATZ_TIME		SEC50MS				// CAT pacing timer rate

// ssi bit rate defns
// BR = SYSCLK/(CPSR * (1 + SCR))
// SCR = (SYSCLK/(BR * CPSR)) - 1
// see SSICLK_calc.xls for minimum error values for CPSDVSR and SCR
// 4800 baud for SSI1
#define	SIO_BAUD		4800L				// IC900 serial I/O baud rate
#define	SSI1_BR			SIO_BAUD			// ssi1 clock rate (Hz)
#define SSI1_CPSDVSR	(248)
#define	SSI1_SCR		(41)				//((SYSCLK/(SSI1_BR * SSI1_CPSDVSR)) - 1)
// 100K baud for SSI3
#define	LCD_BAUD		25000L				// LCD serial I/O baud rate
#define	SSI3_BR			LCD_BAUD			// ssi3 clock rate (Hz)
#define SSI3_CPSDVSR	(200)
#define	SSI3_SCR		(2)					//((SYSCLK/(SSI3_BR * SSI3_CPSDVSR)) - 1)

// SIN bit time defines... assumes PS = 0 (SYSCLK/1 is used for timer)
#define TIMER2A_PS 0
#define	SIN_BIT_TIME		(SYSCLK/(SIO_BAUD * (TIMER2A_PS + 1)))		// 10416
#define	SIN_HALF_BIT_TIME	(SIN_BIT_TIME/2)							// 5208

// timer definitions
#define TIMER1_PS 31				// prescale value for timer1
#define	TIMER1_FREQ	9600			// timer1 intr freq
//#define TIMER3_ILR 0xffff			// timer 3 interval (24 bit)
//#define TIMER3_PS 0xff
//#define TIMER1_PS 32
#define TIMER3_ILR 0xffff			// timer 3 interval (24 bit)
#define TIMER3_PS 32

#define TIMER1B_PS 		1			// prescale value for timer1B
#define	BBSPICLK_FREQ	600000L		// edge freq for BB SPI clk

#define	TPULSE	(100L)				// in usec
#define TMIN	(((SYSCLK / 100) * TPULSE)/10000L)	// minimum pulse width


// Port A defines
#define RXD0			0x01		// in		uart0
#define TXD0			0x02		// in		uart0
#define SPCK			0x04		// in		IC900 LCD SPI (SSI0)
#define CS1				0x08		// in
#define MOSI			0x10		// in
#define CS2				0x20		// in
#define sparePA6		0x40		// in
#define sparePA7		0x80		// in
#define PORTA_DIRV		(0)
#define	PORTA_DENV		(0xff)
#define	PORTA_PURV		(SPCK|CS1|MOSI|CS2)
#define	PORTA_INIT		0

// Port B defines
// parallel port, no bitmap			LCD data I/O bus
#define PORTB_DIRV		(0)
#define	PORTB_DENV		(0xff)
#define	PORTB_PURV		(0xff)
#define	PORTB_INIT		0

// Port C defines
#define sparePC4		0x10		// in		
#define sparePC5		0x20		// in		
#define sparePC6		0x40		// in		
#define sparePC7		0x80		// in		
#define PORTC_DIRV		(0)
#define	PORTC_DENV		(0xf0)
#define	PORTC_PURV		(0xf0)
#define	PORTC_INIT		(0)

// Port D defines
#define LCD_FS			0x01		// out		font size
#define LCD_RVS			0x02		// out		inverse image
#define sparePD2		0x04		// in						(AIN5)
#define sparePD3		0x08		// in						ss13Tx
#define sparePD4		0x10		// in
#define sparePD5		0x20		// in		
#define sparePD6		0x40		// in						qei0A
#define sparePD7		0x80		// in						qei0B
#define PORTD_DIRV		(LCD_FS|LCD_RVS)
#define	PORTD_DENV		(0xff)
#define	PORTD_PURV		~(LCD_FS|LCD_RVS)
#define	PORTD_INIT		(LCD_FS|LCD_RVS)

// Port E defines
#define sparePE0		0x01		// in
#define nTRD			0x02		// out		LCD bus read
#define nTWR			0x04		// out		LCD bus write
#define	TADDR			0x08		// out		LCD ADDR/CMD (0 = data, 1 = cmd)
#define nTCS			0x10		// out		LCD bus enable
#define TRESET			0x20		// out		LCD reset
#define PORTE_DIRV		(nTRD|nTWR|TADDR|nTCS|TRESET)
#define	PORTE_DENV		(nTRD|nTWR|TADDR|nTCS|TRESET|sparePE0)
#define	PORTE_PURV		(sparePE0)
#define	PORTE_INIT		(nTRD|nTWR|nTCS|TRESET)


// Port F defines
#define LED_PWM			0x01		// out		M1PWM4	{beeper}
#define sparePF2		0x02		// in		(M1PWM5)
#define sparePF3		0x04		// in		M1PWM6	PBSW LED PWM
#define sparePF4		0x08		// in	M1PWM7	DU backlight PWM
#define sparePF5		0x10		// in		T2CCP0	remote serial in
#define PORTF_DIRV		(LED_PWM)
#define	PORTF_DENV		(LED_PWM|sparePF2|sparePF3|sparePF4|sparePF5)
#define	PORTF_PURV		(sparePF2|sparePF3|sparePF4|sparePF5)
#define	PORTF_INIT		0

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

#ifndef MAIN_C
extern U16	app_timer1ms;		// app timer
extern U16	xm_timer;			// xmodem timer
extern U16	ccmd_timer;			// ccmd timer
extern char	bchar;				// global bchar storage
extern char	swcmd;				// global swcmd storage
extern S8	handshake;			// xon/xoff enable
extern S8	xoffsent;			// xoff sent
#endif

// system error flag defines

#define	NOP				FALSE			// no-operation flag.  Passed to functions that have the option to perform an action or return a status
#define	CLR				TRUE			// perform read-and-clear operation flag.  Passed to functions that have the option to perform read status and clear
#define	KEY_NULL		'~'				// null chr for HMD keycode LUT
#define KEYP_IDLE		0x00			// keypad ISR state machine state IDs
#define KEYP_DBDN		0x01
#define KEYP_PRESSED	0x02
#define KEYP_HOLD		0x03
#define KEYP_DBUP		0x04
#define	KEY_PR_FL		0x01			// key-pressed bit field
#define	KEY_HOLD_FL		0x02			// key-hold bit field
#define	KEY_PRESCALE	10				// sets COL hold time (in ms +1)
#define HM_KEY_HOLD_TIME (PSEC1)		// keypad hold timer value (~~ 1 sec)
#define SHFT_HOLD_TIME	(PSEC10) 		// MFmic func-shift timeout (~~ 10 sec)
#define DFE_TO_TIME		(PSEC10)		// dfe timeout value
// keypad hold timer value (~~ 1 sec)
#define KEY_HOLD_TIME	((HM_KEY_HOLD_TIME * PS_PER_TIC)/KEY_PRESCALE)
#define	KEY_HOLD_KEY	0x8000			// set hi bit of key buffer entry to signal hold
#define	KEY_RELEASE_KEY	0x4000			// key release keycode
#define	KHOLD_FLAG		0x80			// flag bit for key hold character
#define	KREL_FLAG		0x40			// flag bit for key release character

// key press character defines
// No keys can be greater than 0x3F (63)  Also, ESC (27) is not allowed
#define	FDchr			23				// FUNC-D cmd chr
#define	DUPchrM			24				// force minus duplex
#define	DUPchrP			25				// force plus duplex
#define	DUPchrS			26				// force simplex
#define	RMAINchr		28				// ersatz key -- restore from force main mode
#define	MMAINchr		29				// ersatz key -- force main mode
#define	RSUBchr			30				// ersatz key -- restore from force sub mode
#define	SSUBchr			31				// ersatz key -- force sub mode
										// !! no hold or release codes for ersatz keys !!

#define	SMUTEchr		32
#define	SUBchr			33
#define	TONEchr			34
#define	ENTchr			35
#define	CALLchr			36
#define	MWchr			37
#define	TSchr			38
#define	Tchr			39
#define	Vupchr			40
#define	Vdnchr			41
#define	HILOchr			42
#define	CHKchr			43
#define	errorchr		44
#define	MSchr			45
#define	DOTchr			46
#define	DUPchr			47

#define	ZEROchr			48				//////////////////////////
#define	ONEchr			49				//						//
#define	TWOchr			50				//						//
#define	THREEchr		51				//						//
#define	FOURchr			52				// these MUST be		//
#define	FIVEchr			53				// ascii digit codes	//
#define	SIXchr			54				//						//
#define	SEVENchr		55				//						//
#define	EIGHTchr		56				//						//
#define	NINEchr			57				//////////////////////////

#define	VFOchr			58
#define	MRchr			59
#define	MHZchr			60
#define	SETchr			61
#define	Qupchr			62
#define	Qdnchr			63

// HM MFmic character defines
#define	LOKEY		'L'
#define	CALLKEY		'T'
#define	XFCKEY		'X'
#define	UPKEY		'/'
#define	VMKEY		'V'
#define	MWKEY		'M'
#define	DNKEY		'\\'
#define	F1KEY		'F'
#define	F2KEY		'G'
// shifted keys
#define	SH_LOKEY	'p'
#define	SH_CALLKEY	'o'
#define	SH_XFCKEY	'n'
#define	SH_UPKEY	'k'
#define	SH_VMKEY	'm'
#define	SH_MWKEY	'l'
#define	SH_DNKEY	'j'
#define	SH_F1KEY	'|'
#define	SH_F2KEY	'!'
#define	SH_1		'a'
#define	SH_2		'b'
#define	SH_3		'c'
#define	SH_A		'q'
#define	SH_4		'd'
#define	SH_5		'e'
#define	SH_6		'f'
#define	SH_B		'r'
#define	SH_7		'g'
#define	SH_8		'h'
#define	SH_9		'i'
#define	SH_C		's'
#define	SH_STR		'+'
#define	SH_0		'`'
#define	SH_PND		'$'
#define	SH_D		't'

// key hold chr codes
#define	FDchr_H			(FDchr    | KHOLD_FLAG)				// FUNC-D cmd chr
#define	SUBchr_H		(SUBchr   | KHOLD_FLAG)
#define	TONEchr_H		(TONEchr  | KHOLD_FLAG)
#define	CALLchr_H		(CALLchr  | KHOLD_FLAG)
#define	MWchr_H			(MWchr    | KHOLD_FLAG)
#define	TSchr_H			(TSchr    | KHOLD_FLAG)
#define	Tchr_H			(Tchr     | KHOLD_FLAG)
#define	Vupchr_H		(Vupchr   | KHOLD_FLAG)
#define	Vdnchr_H		(Vdnchr   | KHOLD_FLAG)
#define	HILOchr_H		(HILOchr  | KHOLD_FLAG)
#define	CHKchr_H		(CHKchr   | KHOLD_FLAG)
#define	errorchr_H		(errorchr | KHOLD_FLAG)

#define	MSchr_H			(MSchr    | KHOLD_FLAG)
#define	DUPchr_H		(DUPchr   | KHOLD_FLAG)
#define	VFOchr_H		(VFOchr   | KHOLD_FLAG)
#define	MRchr_H			(MRchr    | KHOLD_FLAG)
#define	MHZchr_H		(MHZchr   | KHOLD_FLAG)
#define	SETchr_H		(SETchr   | KHOLD_FLAG)
#define	Qupchr_H		(Qupchr   | KHOLD_FLAG)
#define	Qdnchr_H		(Qdnchr   | KHOLD_FLAG)
#define	SMUTEchr_H		(SMUTEchr | KHOLD_FLAG)
// key release chr codes
#define	FDchr_R			(FDchr    | KREL_FLAG)				// FUNC-D cmd chr
#define	SUBchr_R		(SUBchr   | KREL_FLAG)
#define	TONEchr_R		(TONEchr  | KREL_FLAG)
#define	CALLchr_R		(CALLchr  | KREL_FLAG)
#define	MWchr_R			(MWchr    | KREL_FLAG)
#define	TSchr_R			(TSchr    | KREL_FLAG)
#define	Tchr_R			(Tchr     | KREL_FLAG)
#define	Vupchr_R		(Vupchr   | KREL_FLAG)
#define	Vdnchr_R		(Vdnchr   | KREL_FLAG)
#define	HILOchr_R		(HILOchr  | KREL_FLAG)
#define	CHKchr_R		(CHKchr   | KREL_FLAG)
#define	errorchr_R		(errorchr | KREL_FLAG)

#define	MSchr_R			(MSchr    | KREL_FLAG)
#define	DUPchr_R		(DUPchr   | KREL_FLAG)
#define	VFOchr_R		(VFOchr   | KREL_FLAG)
#define	MRchr_R			(MRchr    | KREL_FLAG)
#define	MHZchr_R		(MHZchr   | KREL_FLAG)
#define	SETchr_R		(SETchr   | KREL_FLAG)
#define	Qupchr_R		(Qupchr   | KREL_FLAG)
#define	Qdnchr_R		(Qdnchr   | KREL_FLAG)
#define	SMUTEchr_R		(SMUTEchr | KREL_FLAG)

#define	HIB_APPL	0					// HIB is being accessed by application
#define	HIB_INTR	1					// HIB may be accessed by intrpt

//-----------------------------------------------------------------------------
// main.c Fn prototypes
//-----------------------------------------------------------------------------

void Init_Device(void);
char process_IO(U8 flag);
void  process_chg(U8 chg, char* buf, U8* key_mem, U8* keyh_mem);
U8 get_status(U8* keyh_mem);
U8 set_pttmode(U8 value);
void set_pwm(U8 pwmnum, U8 percent);
void set_led(U8 lednum, U8 value);

void wrwhib_ram(U32 data, volatile U8 addr);
void wrhib_ram(uint8_t data, U8 addr);
uint32_t rdwhib_ram(U8 addr);
uint8_t rdhib_ram(U8 addr);

void waitpio(U16 waitms);
void wait(U16 waitms);
void set_wait(U16 waitms);
U8 is_wait(void);
void wait2(U16 waitms);
U16 getipl(void);
U32 get_syserr(U8 opr);
U8 wait_busy0(U16 delay);
U8 wait_busy1(U16 delay);
U8 wait_reg0(volatile uint32_t *regptr, uint32_t clrmask, U16 delay);
U8 wait_reg1(volatile uint32_t *regptr, uint32_t setmask, U16 delay);

U8 got_key(void);
//U8 not_key(U8 flag);
char get_key(void);

void warm_reset(void);
U32 free_run(void);
void set_dial(S8 value);
S8 get_dial(U8 tf);
void do_dial_beep(void);
void do_1beep(void);
void do_2beep(void);
void do_3beep(void);
void do_4beep(void);
U8 sin_time(U8 cmd);
U8 sout_time(U8 cmd);
U8 mhz_time(U8 tf);
U8 v_time(U8 tf);
U8 q_time(U8 tf);
U8 set_time(U8 tf);
U8 offs_time(U8 tf);
U8 hmk_time(U8 tf);
U8 dfe_time(U8 tf);
U8 shft_time(U8 tf);
U8 cato_time(U8 tf);
U8 cata_time(U8 tf);
U8 catz_time(U8 tf);
U8 cmd_time(U8 value);
U8 sub_time(U8 tf);
U8 mic_time(U8 tf);
U8 micdb_time(U8 tf);
U8 mute_time(U8 tf);
U8 ts_time(U8 tf);
U8 slide_time(U8 tf);
U8 scan_time(U8 focus, U8 tf);
U8 ipl_time(U8 tf);
U32 get_free(void);

void set_beep(U16 beep_frq, U16 b_count);
void do_beep(U16 beep_cycles);
void Timer0A_ISR(void);

void gpiob_isr(void);
void gpioc_isr(void);
void Timer3A_ISR(void);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------