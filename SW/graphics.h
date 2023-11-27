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

#endif

void wr_msym(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr, U8* mptr);
void wr_msym2(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr, U8* mptr);

void wr_ssym(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr);
void wr_ssym2(U8* sptr, U8 xlen, U8 ylen, U8 blank, U16 daddr);
