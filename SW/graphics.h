/*
 * graphics.h
 *
 *  Created on: Nov 26, 2023
 *      Author: KE0FF-Actual
 */

#define	TONESYM_X	3
#define	TONESYM_Y	9
#define	DUPSYM_X	3
#define	DUPSYM_Y	9
#define	MEMSYM_X	2
#define	MEMSYM_Y	11
#define	SKPSYM_X	2
#define	SKPSYM_Y	11

#define	SM1SYM_X	6
#define	SM1SYM_Y	9
#define	SM2SYM_X	5
#define	SM2SYM_Y	9
#define	SM2ADDR		6
#define	SM3SYM_X	4
#define	SM3SYM_Y	9
#define	SM3ADDR		(SM2ADDR+SM2SYM_X)
#define	SM4SYM_X	4
#define	SM4SYM_Y	9
#define	SM4ADDR		(SM3ADDR+SM3SYM_X-1)
#define	SM5SYM_X	3
#define	SM5SYM_Y	9
#define	SM5ADDR		(SM4ADDR+SM4SYM_X-1)

#define	SM6SYM_X	2
#define	SM6SYM_Y	9
#define	SM6ADDR		(SM5ADDR+SM5SYM_X-1)
#define	SM7SYM_X	2
#define	SM7SYM_Y	9
#define	SM7ADDR		(SM6ADDR+SM6SYM_X-1)

#define	OPTROW_ADDR			(0) // FILL IN
#define	OPTA_SYMADDR		8
#define	OPTAUP_SYMADDR		(OPTA_SYMADDR-1)
#define	OPTADN_SYMADDR		(OPTA_SYMADDR+3)

#define	OPTB_SYMADDR		(OPTA_SYMADDR+6)
#define	OPTBUP_SYMADDR		(OPTB_SYMADDR-1)
#define	OPTBDN_SYMADDR		(OPTB_SYMADDR+3)

#define	OPTC_SYMADDR		(OPTB_SYMADDR+6)
#define	OPTCUP_SYMADDR		(OPTC_SYMADDR-1)
#define	OPTCDN_SYMADDR		(OPTC_SYMADDR+3)

#define	OPTD_SYMADDR		(OPTC_SYMADDR+6)
#define	OPTDUP_SYMADDR		(OPTD_SYMADDR-1)
#define	OPTDDN_SYMADDR		(OPTD_SYMADDR+3)

#define	OPTUPSYM_X	2
#define	OPTUPSYM_Y	11
#define	OPTDNSYM_X	2
#define	OPTDNSYM_Y	11

#define	OPTASYM_X	4
#define	OPTASYM_Y	11
#define	OPTBSYM_X	4
#define	OPTBSYM_Y	11
#define	OPTCSYM_X	4
#define	OPTCSYM_Y	11
#define	OPTDSYM_X	4
#define	OPTDSYM_Y	11

#define	OWSYM_X		4
#define	OWSYM_Y		11

#define	LOWSYM_X	4
#define	LOWSYM_Y	11

#define	VXOSYM_X	4
#define	VXOSYM_Y	11

#define	RITSYM_X	4
#define	RITSYM_Y	11

#define	TSSYM_X		4
#define	TSSYM_Y		11

#define	MHZSYM_X	4
#define	MHZSYM_Y	11

#define	PROGSYM_X	4
#define	PROGSYM_Y	11

#define	BANDSYM_X	4
#define	BANDSYM_Y	11

#define	SUBSYM_X	5
#define	SUBSYM_Y	11

#define	LOCKSYM_X	7
#define	LOCKSYM_Y	11

/////////////////////////////////////
// Extern array refs
/////////////////////////////////////

#ifndef	GRAPHINCL

extern	U8	gtone[TONESYM_Y][TONESYM_X];
extern	U8	gdup[DUPSYM_Y][DUPSYM_X];
#define		gdash 0x7e
extern	U8	gmem[MEMSYM_Y][MEMSYM_X];
extern	U8	gskp[SKPSYM_Y][SKPSYM_X];
extern	U8	gsmet1[SM1SYM_Y][SM1SYM_X];
extern	U8	gsmet2[SM2SYM_Y][SM2SYM_X];
extern	U8	gsmet3[SM3SYM_Y][SM3SYM_X];
extern	U8	gsmet4[SM4SYM_Y][SM4SYM_X];
extern	U8	gsmet5[SM5SYM_Y][SM5SYM_X];
extern	U8	gsmet6[SM6SYM_Y][SM6SYM_X];
extern	U8	gsmet7[SM7SYM_Y][SM7SYM_X];
extern	U8	goptup[][];
extern	U8	goptdn[][];
extern	U8	gopttq[][];
extern	U8	goptdq[][];
extern	U8	gopt1[][];
extern	U8	gopt2[][];
extern	U8	gow[][];
extern	U8	glow[][];
extern	U8	gvxo[][];
extern	U8	grit[][];
extern	U8	gts[][];
extern	U8	gmhz[][];
extern	U8	gprog[][];
extern	U8	gband[][];
extern	U8	gsub[][];
extern	U8	glock[][];

#endif
