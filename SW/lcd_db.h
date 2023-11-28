/********************************************************************
 ************ COPYRIGHT (c) 2023 by ke0ff, Taylor, TX   *************
 *
 *  File name: lcd_db.h
 *
 *  Module:    Control
 *
 *  Summary:   This is the header file for parallel I/O.
 *
 *******************************************************************/


/********************************************************************
 *  File scope declarations revision history:
 *    11-22-23 jmh:  creation date
 *
 *******************************************************************/

//------------------------------------------------------------------------------
// extern defines
//------------------------------------------------------------------------------
#define	MAIN7_LEN	(39*30)
#define	MAIN7_OFFS	(0x4000)
#define	MAINSM_LEN	(12*30)
#define	MAINSM_OFFS	(MAIN7_OFFS+MAIN7_LEN+(3*30))

#define	OPTROW_LEN	(12*30)
#define	OPTROW_OFFS	(MAINSM_OFFS+MAINSM_LEN)

#define	SUBSM_LEN	(12*30)
#define	SUBSM_OFFS	(OPTROW_OFFS+OPTROW_LEN)

#define	SUB7_LEN	(45*30)
#define	SUB7_OFFS	(SUBSM_OFFS+SUBSM_LEN)

// LCD digit addresses - Main
#define	MDWIDE		3
#define	MSDWIDE		2
#define	MDADDR0		0
#define	MDADDR1		(MDADDR0+MDWIDE)
#define	MDADDR2		(MDADDR1+MDWIDE)
#define	MDADDR3		(MDADDR2+MDWIDE)
#define	MDADDR4		(MDADDR3+MDWIDE)
#define	MDADDR5		(MDADDR4+MDWIDE)
#define	MDADDR6		(MDADDR5+MDWIDE)
#define	MDADDR7		(MDADDR6+MDWIDE)

#define	DROW		(240/8)
#define	NUMROWS_M	38
#define	NUMROWS_MDP	35
#define	NUMROWS_MS	20

#define	SDWIDE		2
#define	SDADDR0		(7+(DROW*4))
#define	SDADDR1		(SDADDR0+SDWIDE)
#define	SDADDR2		(SDADDR1+SDWIDE)
#define	SDADDR3		(SDADDR2+SDWIDE)
#define	SDADDR4		(SDADDR3+SDWIDE)
#define	SDADDR5		(SDADDR4+SDWIDE)
#define	SDADDR6		(SDADDR5+SDWIDE)
#define	SDADDR7		(SDADDR6+SDWIDE)


#define	OW_ADDR		(0)
#define	LOW_ADDR	(OW_ADDR+OWSYM_X-1)

#define	MSMET1_ADDR	(LOW_ADDR+4)
#define	MSMET2_ADDR	(MSMET1_ADDR+SM1SYM_X)
#define	MSMET3_ADDR	(MSMET2_ADDR+SM2SYM_X)
#define	MSMET4_ADDR	(MSMET3_ADDR+SM3SYM_X-1)
#define	MSMET5_ADDR	(MSMET4_ADDR+SM4SYM_X-1)
#define	MSMET6_ADDR	(MSMET5_ADDR+SM5SYM_X-1)
#define	MSMET7_ADDR	(MSMET6_ADDR+SM6SYM_X-1)

#define	UP_ADDR		0
#define	OPT_ADDR	(UP_ADDR+1)
#define	DN_ADDR		(OPT_ADDR+4-1)
#define	VXO_ADDR	(0)
#define	RIT_ADDR	(VXO_ADDR+OWSYM_X-1)
#define	OPTA_ADDR	(RIT_ADDR+4)
#define	OPTB_ADDR	(OPTA_ADDR+OPTASYM_X+1)
#define	OPTC_ADDR	(OPTB_ADDR+OPTBSYM_X+1)
#define	OPTD_ADDR	(OPTC_ADDR+OPTCSYM_X+1)

#define	TS_ADDR		(0)
#define	MHZ_ADDR	(TS_ADDR+TSSYM_X-1)

#define	SSMET1_ADDR	(MHZ_ADDR+4)
#define	SSMET2_ADDR	(SSMET1_ADDR+SM1SYM_X)
#define	SSMET3_ADDR	(SSMET2_ADDR+SM2SYM_X)
#define	SSMET4_ADDR	(SSMET3_ADDR+SM3SYM_X-1)
#define	SSMET5_ADDR	(SSMET4_ADDR+SM4SYM_X-1)
#define	SSMET6_ADDR	(SSMET5_ADDR+SM5SYM_X-1)
#define	SSMET7_ADDR	(SSMET6_ADDR+SM6SYM_X-1)

#define	NUMROWS_S	25
#define	NUMROWS_SDP	23
#define	NUMROWS_SS	(25-12)

#define	SEGA	0
#define	SEGB	1
#define	SEGC	2
#define	SEGD	3
#define	SEGE	4
#define	SEGF	5
#define	SEGG	6
#define	SEGDP	7

#define	CLUSTER1_M			(MDADDR7+1)
#define	TONMSYM_ADDR		(CLUSTER1_M)
#define	DUPMSYM_ADDR		(CLUSTER1_M+4)
#define	MINMSYM_ADDR		((DUPMSYM_ADDR-1)+((DROW*4)))
#define	MEMMSYM_ADDR		(TONMSYM_ADDR+2+(DROW*14))
#define	SKPMSYM_ADDR		(MEMMSYM_ADDR+(DROW*13))
#define	MEMMCH_ADDR			(DUPMSYM_ADDR+(DROW*13))

#define	CLUSTER1_S			((SDADDR7+1)-(DROW*2))
#define	TONSSYM_ADDR		CLUSTER1_S
#define	DUPSSYM_ADDR		(CLUSTER1_S+4)
#define	MINSSYM_ADDR		((DUPSSYM_ADDR-1)+((DROW*4)))
#define	MEMSSYM_ADDR		(TONSSYM_ADDR+2+(DROW*14))
#define	SKPSSYM_ADDR		(MEMSSYM_ADDR+(DROW*13))
#define	MEMSCH_ADDR			(DUPSSYM_ADDR+(DROW*13))

#define	PROG_ADDR	0
#define	BAND_ADDR	(PROG_ADDR+PROGSYM_X-1)
#define	SUB_ADDR	(PROG_ADDR+(12*DROW))
#define	LOCK_ADDR	(SUB_ADDR+(12*DROW))

#ifndef	LCD_DB
extern	U8	main7[];		// main 7-seg DU memory
extern	U8	sub7[];			// sub 7-seg DU memory
extern	U8	mainsm[];		// main smet row
extern	U8	subsm[];		// sub smet row
extern	U8	optrow[];		// opt row
#endif

#define	swapeo(data) (((data << 1) & 0xaa) | ((data >> 1) & 0x55))

#define	LCDCMD		TADDR	//0x01
#define	LCDDATA		0x00
// LCD discrete cmds
#define	LDSC_RESET		1
#define	LDSC_FS68		11
#define	LDSC_FS88		10
#define	LDSC_RVS		21
#define	LDSC_NORM		20
#define	LDSC_INIT		0xff


#define	STA0	0x01		// command execute OK
#define	STA1	0x02		// data read/write OK
#define	STA2	0x04		// auto-mode read OK
#define	STA3	0x08		// auto-mode write OK
#define STA5	0x20		// controller OK
#define	STA6	0x40		// error flag (screen copy)
#define	STA7	0x80		// Blink - normal display

#define	STA01	(STA1|STA0)		// composite OK
#define	STA23	STA3		// composite OK, auto

//------------------------------------------------------------------------------
// public Function Prototypes
//------------------------------------------------------------------------------
void clear_main7(void);
void clear_sub7(void);
void wr_mseg(U8 seg, U8 blank, U16 daddr);
void wr_sseg(U8 seg, U8 blank, U16 daddr);
void wr_mdigit(char digit, U16 daddr);
void wr_sdigit(char digit, U16 daddr, U8* aryptr);

void wrdb(U8 data, U8 addr, U8 mask);
U8 rddb(U8 addr);
void debug_db(U8 data);
void lcd_cntl(U8 cmd);
U8 lcd_stat(U8 mask);

//U8 swapeo(U8 data);

//eof
