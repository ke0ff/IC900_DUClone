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

#undef SWAPEVEN	// undefine if production build.  This corrects the even/odd swap mistake in the prototype bus wiring

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

#define	SUB7_LEN	(43*30)
#define	SUB7_OFFS	(SUBSM_OFFS+SUBSM_LEN)

#define	ERR_OFFS	(SUB7_OFFS+SUB7_LEN)
#define	ERR2_OFFS	(ERR_OFFS + (6))

#define	MAIN7_FL	0x80
#define	SUB7_FL		0x40
#define	MAINSM_FL	0x20
#define	SUBSM_FL	0x10
#define	OPTROW_FL	0x08

// LCD digit addresses - Main
#define	MDWIDE		3
#define	MSDWIDE		2
#define	MDADDR6		0
#define	MDADDR5		(MDADDR6+MDWIDE)
#define	MDADDR4		(MDADDR5+MDWIDE)
#define	MDADDR3		(MDADDR4+MDWIDE)
#define	MDADDR2		(MDADDR3+MDWIDE)
#define	MDADDR1		(MDADDR2+MDWIDE)
#define	MDADDR0		(MDADDR1+MDWIDE)
#define	MDADDRS		(MDADDR0+MDWIDE)

#define	DROW		(240/8)
#define	NUMROWS_M	38
#define	NUMROWS_MDP	35
#define	NUMROWS_MS	20

#define	SDWIDE		2
#define	SDADDR6		(7+(DROW*4))
#define	SDADDR5		(SDADDR6+SDWIDE)
#define	SDADDR4		(SDADDR5+SDWIDE)
#define	SDADDR3		(SDADDR4+SDWIDE)
#define	SDADDR2		(SDADDR3+SDWIDE)
#define	SDADDR1		(SDADDR2+SDWIDE)
#define	SDADDR0		(SDADDR1+SDWIDE)
#define	SDADDRS		(SDADDR0+SDWIDE)

#define	OW_ADDR		(0)
#define	LOW_ADDR	(OW_ADDR+OWSYM_X-1)

#define	MSMET1_ADDR	(LOW_ADDR+4)
#define	MSMET2_ADDR	(MSMET1_ADDR+SM1SYM_X)
#define	MSMET3_ADDR	(MSMET2_ADDR+SM2SYM_X)
#define	MSMET4_ADDR	(MSMET3_ADDR+SM3SYM_X-1)
#define	MSMET5_ADDR	(MSMET4_ADDR+SM4SYM_X-1)
#define	MSMET6_ADDR	(MSMET5_ADDR+SM5SYM_X-1)
#define	MSMET7_ADDR	(MSMET6_ADDR+SM6SYM_X-1)

#define	DN_ADDR		0
#define	OPT_ADDR	(DN_ADDR+1)
#define	UP_ADDR		(OPT_ADDR+4-1)
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

#define	CLUSTER1_M			(MDADDRS+1)
#define	TONMSYM_ADDR		(CLUSTER1_M)
#define	DUPMSYM_ADDR		(CLUSTER1_M+4)
#define	MINMSYM_ADDR		((DUPMSYM_ADDR-1)+((DROW*4)))
#define	MEMMSYM_ADDR		(TONMSYM_ADDR+2+(DROW*14))
#define	SKPMSYM_ADDR		(MEMMSYM_ADDR+(DROW*13))
#define	MEMMCH_ADDR			(DUPMSYM_ADDR+(DROW*13))

#define	CLUSTER1_S			((SDADDRS+1)-(DROW*2))
#define	TONSSYM_ADDR		CLUSTER1_S
#define	DUPSSYM_ADDR		(CLUSTER1_S+4)
#define	MINSSYM_ADDR		((DUPSSYM_ADDR-1)+((DROW*4)))
#define	MEMSSYM_ADDR		(TONSSYM_ADDR+2+(DROW*14))
#define	SKPSSYM_ADDR		(MEMSSYM_ADDR+(DROW*13))
#define	MEMSCH_ADDR			(DUPSSYM_ADDR+(DROW*11))

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

// LCD parallel port defines
// macro to swap even/odd bits -- temp patch until hardware can get fixed
#ifdef SWAPEVEN
#define	swapeo(data) (((data << 1) & 0xaa) | ((data >> 1) & 0x55))
#else
#define	swapeo(data) (data)
#endif

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

#define	STA01	(STA1|STA0)	// composite OK
#define	STA23	STA3		// composite OK, auto

//////////////////////////////////////////////////////////////////////////
// SPI/LCD defines
// LCD chip commands

#define	LCD_MEMLEN	32				// chip memory length
// bit fields
#define	MEMD0		0x01			// segment bitfields
#define	MEMD1		0x02			// [2:0] correspond to uPD7225 mem bits
#define	MEMD2		0x04
#define	MEMD3		0x08
#define	BIT_MASK	(MEMD0|MEMD1|MEMD2|MEMD3)
#define	DATA_RDY	0x10			// data ready
#define	MEM_MODE0	0x20			// (trig) mem mode, 0x20==OR, 0x40==AND, 0x60==WR, 00==CLR
#define	MEM_MODE1	0x40
#define	MODE_OR		0x20
#define	MODE_AND	0x40
#define	MODE_WR		0x60
#define	MODE_MASK	0x60
#define	BLINKFL		0x80			// (trig) blink data

// chip status reg defines
#define	CS_DECODE7	0x01			// 7-seg decoder enabled
#define	CS_FBLINK	0x02			// fast blink
#define	CS_SBLINK	0x04			// slow blink
#define	CS_DISPON	0x08			// disp enable
#define	CS_NEBLINK	(CS_FBLINK | CS_SBLINK)	// any blink

// chip commands
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
#define	DMASK0		0x07			// data mask, addr+0
#define	DSHFT0		0x00			// data shift, addr+0
#define	DMASK1		0x38			// data mask, addr+1
#define	DSHFT1		0x03			// data shift, addr+1
#define	DMASK2		0xc0			// data mask, addr+2
#define	DSHFT2		0x06			// data shift, addr+2

#define	WR_DMEM		0xD0			// write with (0x0f masked data)
#define	OR_DMEM		0xB0			// OR with (0x0f masked data)
#define	AND_DMEM	0x90			// AND with (0x0f masked data)
#define	CLR_DMEM	0x20			// clear disp. mem
#define	WR_BMEM		0xC0			// write with (0x0f masked data)
#define	OR_BMEM		0xA0			// OR with (0x0f masked data)
#define	AND_BMEM	0x80			// AND with (0x0f masked data)
#define	CLR_BMEM	0x00			// clear blink. mem*/
// internal command aliases
#define	WR_DDEC		0x70			// write disp data with decode
#define	WR_BDEC		0x60			// write blink data with decode

#define	MAX_SRF		7				// #srf bargraph segments
#define	MSMET_ADDR	0x06
#define	SSMET_ADDR	0x1b

// LCD segment bit defines (addr and segment map)
//	CS1
#define	MMEM_ADDR		0x01
#define	MDUP		0x1
#define	MDUP_ADDR		0x04
#define	MMIN		0x2
#define	MMIN_ADDR		0x04
#define	MSKP		0x4
#define	MSKP_ADDR		0x04
#define	MTNE		0x1
#define	MTNE_ADDR		0x05
#define	MM			0x2
#define	MM_ADDR			0x05

#define	MSRF6		0x4
#define	MSRF6_ADDR		0x06
#define	MSRF3		0x1
#define	MSRF4		0x2
#define	MSRF5		0x4
#define	MSRF345_ADDR	0x07
#define	MSRF0		0x1
#define	MSRF1		0x2
#define	MSRF2		0x4
#define	MSRF012_ADDR	0x08
#define	M00			0x2
#define	M00_ADDR	0x09
#define	M0_ADDR		0x0a

#define	M6			0x01
#define	MDP2		0x02
#define	MDP			0x04
#define	MDP_ADDR		0x1c

#define	AOW			0x1
#define	AOW_ADDR		0x1d
#define	ALOW		0x2
#define	ALOW_ADDR		0x1d

#define	ARIT		0x1
#define	ARIT_ADDR		0x1f
#define	AVXO		0x2
#define	AVXO_ADDR		0x1f
#define	ATS			0x4
#define	ATS_ADDR		0x1f

//	CS2
#define	SMEM_ADDR	0x02
#define	SDUP		0x1
#define	SDUP_ADDR	0x05
#define	SMIN		0x2
#define	SMIN_ADDR	0x05
#define	SSKP		0x4
#define	SSKP_ADDR	0x05
#define	STNE		0x1
#define	STNE_ADDR	0x06
#define	SM			0x2
#define	SM_ADDR		0x06
#define	S00			0x2
#define	S00_ADDR	0x07

#define	S0_ADDR		0x08
#define	S6			0x1
#define	S6_ADDR		0x1a

#define	SDP2		0x02
#define	SDP			0x04
#define	SDP_ADDR		0x1a

#define	SSRF6		0x4
#define	SSRF6_ADDR		0x1b
#define	SSRF3		0x1
#define	SSRF4		0x2
#define	SSRF5		0x4
#define	SSRF345_ADDR	0x1c
#define	SSRF0		0x1
#define	SSRF1		0x2
#define	SSRF2		0x4
#define	SSRF012_ADDR	0x1d

#define	ASUB		0x1
#define	ASUB_ADDR	0x1e
#define	ALCK		0x2
#define	ALCK_ADDR	0x1e

#define	APRG		0x1
#define	APRG_ADDR	0x1f
#define	AMHZ		0x2
#define	AMHZ_ADDR	0x1f
#define	ABND		0x4
#define	ABND_ADDR	0x1f

#define	CSEGA		0x08			// uPD7225 BCD seg map
#define	CSEGB		0x01
#define	CSEGC		0x02
#define	CSEGD		0x20
#define	CSEGE		0x80
#define	CSEGF		0x40
#define	CSEGG		0x10
#define	CSEGP		0xff

//------------------------------------------------------------------------------
// public Function Prototypes
//------------------------------------------------------------------------------
void process_LCD(U8 iplfl);
void process_SPI(U8 iplfl);
void process_ERR(U8 iplfl);
void set_csn(U8 m, U8 n);


void clear_main7(void);
void clear_sub7(void);
void clear_optrow(void);
void clear_mainsm(void);
void clear_subsm(void);
void clear_all(void);
void lcdClear(void);
void wrlcd_str(U8 *lcd_string, U16 strlen, U16 lcd_addr);
void lcd_setup(void);
void disp_err(U8 mnum);

void wr_mseg(U8 seg, U8 blank, U16 daddr);
void wr_sseg(U8 seg, U8 blank, U16 daddr, U8* aryptr);
void wr_mdigit(char digit, U8 blank, U16 daddr);
void wr_sdigit(char digit, U8 blank, U16 daddr, U8* aryptr);

void wrdb(U8 data, U8 addr, U8 mask);
U8 rddb(U8 addr);
void debug_db(U8 data);
void lcd_cntl(U8 cmd);
U8 lcd_stat(U8 mask);

void wr_msym(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr, U8* mptr);
void wr_msym2(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr, U8* mptr);

#define	SEGOR	1
#define	SEGNOT	0

void sg_mmin(U8 son);
void sg_smin(U8 son);
void sg_mdup(U8 son);
void sg_sdup(U8 son);
void sg_mtne(U8 son);
void sg_stne(U8 son);
void sg_mmem(U8 son);
void sg_smem(U8 son);
void sg_mskp(U8 son);
void sg_sskp(U8 son);
void sg_mm0(U8 son);
void sg_mm1(U8 son);
void sg_mm2(U8 son);
void sg_mm3(U8 son);
void sg_mm4(U8 son);
void sg_mm5(U8 son);
void sg_mm6(U8 son);
void sg_sm0(U8 son);
void sg_sm1(U8 son);
void sg_sm2(U8 son);
void sg_sm3(U8 son);
void sg_sm4(U8 son);
void sg_sm5(U8 son);
void sg_sm6(U8 son);
void sg_ow(U8 son);
void sg_low(U8 son);
void sg_ts(U8 son);
void sg_mhz(U8 son);
void sg_prg(U8 son);
void sg_bnd(U8 son);
void sg_sub(U8 son);
void sg_lck(U8 son);
void sg_vxo(U8 son);
void sg_rit(U8 son);
void sg_mss(U8 son);
void sg_tss(U8 son);
void sg_tsq(U8 son);
void sg_dsm(U8 son);
void sg_dss(U8 son);
void sg_dsq(U8 son);
void sg_op1m(U8 son);
void sg_op1s(U8 son);
void sg_op1(U8 son);
void sg_op2m(U8 son);
void sg_op2s(U8 son);
void sg_op2(U8 son);
void sg_mdp(U8 son);
void sg_mdp2(U8 son);
void sg_sdp(U8 son);
void sg_sdp2(U8 son);
void sg_m00(U8 son);
void sg_s00(U8 son);

void sg_mbcd(U8 bcdd, U16 digitaddr, U8 blank, U8 main_sel);
void sg_sbcd(U8 bcdd, U16 digitaddr, U8 blank, U8 main_sel);

void null_fn(U8 son);

void sg_bcd_m0a(U8 son);
void sg_bcd_m0b(U8 son);
void sg_bcd_m0c(U8 son);
void sg_bcd_m0d(U8 son);
void sg_bcd_m0e(U8 son);
void sg_bcd_m0f(U8 son);
void sg_bcd_m0g(U8 son);
void sg_bcd_m1a(U8 son);
void sg_bcd_m1b(U8 son);
void sg_bcd_m1c(U8 son);
void sg_bcd_m1d(U8 son);
void sg_bcd_m1e(U8 son);
void sg_bcd_m1f(U8 son);
void sg_bcd_m1g(U8 son);
void sg_bcd_m2a(U8 son);
void sg_bcd_m2b(U8 son);
void sg_bcd_m2c(U8 son);
void sg_bcd_m2d(U8 son);
void sg_bcd_m2e(U8 son);
void sg_bcd_m2f(U8 son);
void sg_bcd_m2g(U8 son);
void sg_bcd_m3a(U8 son);
void sg_bcd_m3b(U8 son);
void sg_bcd_m3c(U8 son);
void sg_bcd_m3d(U8 son);
void sg_bcd_m3e(U8 son);
void sg_bcd_m3f(U8 son);
void sg_bcd_m3g(U8 son);
void sg_bcd_m4a(U8 son);
void sg_bcd_m4b(U8 son);
void sg_bcd_m4c(U8 son);
void sg_bcd_m4d(U8 son);
void sg_bcd_m4e(U8 son);
void sg_bcd_m4f(U8 son);
void sg_bcd_m4g(U8 son);
void sg_bcd_m5a(U8 son);
void sg_bcd_m5b(U8 son);
void sg_bcd_m5c(U8 son);
void sg_bcd_m5d(U8 son);
void sg_bcd_m5e(U8 son);
void sg_bcd_m5f(U8 son);
void sg_bcd_m5g(U8 son);
void sg_bcd_m6bc(U8 son);

void sg_bcd_s0a(U8 son);
void sg_bcd_s0b(U8 son);
void sg_bcd_s0c(U8 son);
void sg_bcd_s0d(U8 son);
void sg_bcd_s0e(U8 son);
void sg_bcd_s0f(U8 son);
void sg_bcd_s0g(U8 son);
void sg_bcd_s1a(U8 son);
void sg_bcd_s1b(U8 son);
void sg_bcd_s1c(U8 son);
void sg_bcd_s1d(U8 son);
void sg_bcd_s1e(U8 son);
void sg_bcd_s1f(U8 son);
void sg_bcd_s1g(U8 son);
void sg_bcd_s2a(U8 son);
void sg_bcd_s2b(U8 son);
void sg_bcd_s2c(U8 son);
void sg_bcd_s2d(U8 son);
void sg_bcd_s2e(U8 son);
void sg_bcd_s2f(U8 son);
void sg_bcd_s2g(U8 son);
void sg_bcd_s3a(U8 son);
void sg_bcd_s3b(U8 son);
void sg_bcd_s3c(U8 son);
void sg_bcd_s3d(U8 son);
void sg_bcd_s3e(U8 son);
void sg_bcd_s3f(U8 son);
void sg_bcd_s3g(U8 son);
void sg_bcd_s4a(U8 son);
void sg_bcd_s4b(U8 son);
void sg_bcd_s4c(U8 son);
void sg_bcd_s4d(U8 son);
void sg_bcd_s4e(U8 son);
void sg_bcd_s4f(U8 son);
void sg_bcd_s4g(U8 son);
void sg_bcd_s5a(U8 son);
void sg_bcd_s5b(U8 son);
void sg_bcd_s5c(U8 son);
void sg_bcd_s5d(U8 son);
void sg_bcd_s5e(U8 son);
void sg_bcd_s5f(U8 son);
void sg_bcd_s5g(U8 son);
void sg_bcd_s6bc(U8 son);

void sg_bcd_mem_m7a(U8 son);
void sg_bcd_mem_m7b(U8 son);
void sg_bcd_mem_m7c(U8 son);
void sg_bcd_mem_m7d(U8 son);
void sg_bcd_mem_m7e(U8 son);
void sg_bcd_mem_m7f(U8 son);
void sg_bcd_mem_m7g(U8 son);

void sg_bcd_mem_s7a(U8 son);
void sg_bcd_mem_s7b(U8 son);
void sg_bcd_mem_s7c(U8 son);
void sg_bcd_mem_s7d(U8 son);
void sg_bcd_mem_s7e(U8 son);
void sg_bcd_mem_s7f(U8 son);
void sg_bcd_mem_s7g(U8 son);

void trig_fill(U8 idx, U8 data, U8 chsel);
void disp_fn(U8 csnum, U8 son, U8 maddr, U8 saddr);
void trig_scan1(U8 mode);
void trig_scan2(U8 mode);

//eof
