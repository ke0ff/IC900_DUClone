/****************************************************************************************
 ****************** COPYRIGHT (c) 2023 by Joseph Haas (DBA FF Systems)  *****************
 *
 *  File name: graphics.c
 *
 *  Module:    Control
 *
 *  Summary:   graphic symbols for IC900
 *
 *  File scope revision history:
 *    11-24-26 jmh:  creation date
 *
 ***************************************************************************************/
#define	GRAPHINCL

#include "typedef.h"
#include "lcd_db.h"
#include "graphics.h"

U8	gtone[TONESYM_Y][TONESYM_X] = {
		{0x7F,0xFF,0xE0},
		{0x80,0x00,0x10},
		{0xB9,0x25,0xD0},
		{0x92,0xB5,0x10},
		{0x92,0xAd,0x90},
		{0x92,0xA5,0x10},
		{0x91,0x25,0xD0},
		{0x80,0x00,0x10},
		{0x7F,0xFF,0xE0}
};

U8	gdup[DUPSYM_Y][DUPSYM_X] = {
		{ 0x7F , 0xFC , 0x00},
		{ 0x80 , 0x02 , 0x00},
		{ 0xB2 , 0xBA , 0x00},
		{ 0xAA , 0xAA , 0x00},
		{ 0xAA , 0xBA , 0x00},
		{ 0xAA , 0xA2 , 0x00},
		{ 0xB3 , 0xA2 , 0x00},
		{ 0x80 , 0x02 , 0x00},
		{ 0x7F , 0xFC , 0x00}
};

U8	gmem[MEMSYM_Y][MEMSYM_X] = {
		{ 0xC0 , 0x80},
		{ 0xC1 , 0x80},
		{ 0xC1 , 0x80},
		{ 0xE3 , 0x80},
		{ 0xE3 , 0x80},
		{ 0xD5 , 0x80},
		{ 0xD5 , 0x80},
		{ 0xC9 , 0x80},
		{ 0xC9 , 0x80},
		{ 0xC1 , 0x80},
		{ 0xC1 , 0x80}
};

U8	gskp[SKPSYM_Y][SKPSYM_X] = {
		{ 0x7F , 0x00},
		{ 0x80 , 0x80},
		{ 0x9C , 0x80},
		{ 0xA2 , 0x80},
		{ 0xA0 , 0x80},
		{ 0x9C , 0x80},
		{ 0x82 , 0x80},
		{ 0xA2 , 0x80},
		{ 0x9C , 0x80},
		{ 0x80 , 0x80},
		{ 0x7F , 0x00}
};

U8	gsmet1[SM1SYM_Y][SM1SYM_X] = {
		{ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x07 , 0xFF , 0xFE},
		{ 0x00 , 0x00 , 0xFF , 0xFF , 0xFF , 0xFE},
		{ 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFE},
		{ 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFE}
};

U8	gsmet2[SM2SYM_Y][SM2SYM_X] = {
		{0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x1F , 0xFF , 0xF1},
		{0xFF , 0xFF , 0xFF , 0xFF , 0xFD},
		{0xFF , 0xFF , 0xFF , 0xFF , 0xF9},
		{0xFF , 0xFF , 0xFF , 0xFF , 0xFD},
		{0xFF , 0xFF , 0xFF , 0xFF , 0xF1}
};

U8	gsmet3[SM3SYM_Y][SM3SYM_X] = {
		{ 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x00 , 0x00 , 0x00 , 0x00},
		{ 0x7F , 0xFF , 0xFF , 0x10},
		{ 0x7F , 0xFF , 0xFF , 0x70},
		{ 0x7F , 0xFF , 0xFF , 0x10},
		{ 0x7F , 0xFF , 0xFF , 0xD0},
		{ 0x7F , 0xFF , 0xFF , 0x10}
};

U8	gsmet4[SM4SYM_Y][SM4SYM_X] = {
		{0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00 , 0x00},
		{0x07 , 0xFF , 0xFC , 0x40},
		{0x07 , 0xFF , 0xFF , 0x40},
		{0x07 , 0xFF , 0xFF , 0x40},
		{0x07 , 0xFF , 0xFF , 0x40},
		{0x07 , 0xFF , 0xFF , 0x40}
};

U8	gsmet5[SM5SYM_Y][SM5SYM_X] = {
		{0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00},
		{0x00 , 0x00 , 0x00},
		{0x0F , 0xFF , 0xC0},
		{0x1F , 0xFC , 0x40},
		{0x1F , 0xFD , 0x40},
		{0x1F , 0xFC , 0x40},
		{0x1F , 0xFF , 0x40},
		{0x1F , 0xFF , 0x40}
};

U8	gsmet6[SM6SYM_Y][SM6SYM_X] = {
		{0x00 , 0x00},
		{0x00 , 0x00},
		{0x0F , 0xFC},
		{0x1F , 0xFC},
		{0x1F , 0xFC},
		{0x1F , 0xFC},
		{0x1F , 0xFC},
		{0x1F , 0xFC},
		{0x1F , 0xFC}
};

U8	gsmet7[SM7SYM_Y][SM7SYM_X] = { //0x3E, 0x7E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE };
		{0x00 , 0xEF},
		{0x01 , 0xC7},
		{0x01 , 0xEF},
		{0x01 , 0xFF},
		{0x01 , 0x1B},
		{0x01 , 0xD5},
		{0x01 , 0x15},
		{0x01 , 0x75},
		{0x01 , 0x1B}
};

U8	goptup[OPTUPSYM_Y][OPTUPSYM_X] = {
		{ 0x3F , 0x80},
		{ 0x3F , 0x80},
		{ 0x1F , 0x00},
		{ 0x1F , 0x00},
		{ 0x1F , 0x00},
		{ 0x0E , 0x00},
		{ 0x0E , 0x00},
		{ 0x0E , 0x00},
		{ 0x04 , 0x00},
		{ 0x04 , 0x00}

/*		{ 0xFF , 0x80},
		{ 0x7F , 0x00},
		{ 0x7F , 0x00},
		{ 0x3E , 0x00},
		{ 0x3E , 0x00},
		{ 0x1C , 0x00},
		{ 0x1C , 0x00},
		{ 0x1C , 0x00},
		{ 0x1C , 0x00},
		{ 0x08 , 0x00},
		{ 0x08 , 0x00}*/
};

U8	goptdn[OPTDNSYM_Y][OPTDNSYM_X] = {
		{0x00 , 0x00},
		{0x01 , 0x00},
		{0x01 , 0x00},
		{0x03 , 0x80},
		{0x03 , 0x80},
		{0x03 , 0x80},
		{0x07 , 0xC0},
		{0x07 , 0xC0},
		{0x0F , 0xE0},
		{0x0F , 0xE0},
		{0x0F , 0xE0}

/*		{0x00 , 0x80},
		{0x00 , 0x80},
		{0x01 , 0xC0},
		{0x01 , 0xC0},
		{0x01 , 0xC0},
		{0x01 , 0xC0},
		{0x03 , 0xE0},
		{0x03 , 0xE0},
		{0x07 , 0xF0},
		{0x07 , 0xF0},
		{0x0F , 0xF8}*/
};

U8	gopttq[OPTASYM_Y][OPTASYM_X] = {
		{ 0x1F , 0xFF , 0xFF , 0xF0},
		{ 0x20 , 0x00 , 0x00 , 0x08},
		{ 0x2F , 0x8C , 0x64 , 0x08},
		{ 0x42 , 0x12 , 0x94 , 0x08},
		{ 0x42 , 0x10 , 0x94 , 0x08},
		{ 0x42 , 0x0C , 0x94 , 0x10},
		{ 0x82 , 0x02 , 0xD4 , 0x10},
		{ 0x82 , 0x12 , 0xB4 , 0x10},
		{ 0x82 , 0x4C , 0x77 , 0xA0},
		{ 0x80 , 0x00 , 0x08 , 0x20},
		{ 0x7F , 0xFF , 0xFF , 0xC0}
};

U8	goptdq[OPTBSYM_Y][OPTBSYM_X] = {
		{0x1F , 0xFF , 0xFF , 0xF0},
		{0x20 , 0x00 , 0x00 , 0x08},
		{0x2E , 0x0C , 0x64 , 0x08},
		{0x49 , 0x12 , 0x94 , 0x08},
		{0x49 , 0x10 , 0x94 , 0x08},
		{0x49 , 0x0C , 0x94 , 0x10},
		{0x89 , 0x02 , 0xD4 , 0x10},
		{0x89 , 0x12 , 0xB4 , 0x10},
		{0x8E , 0x4C , 0x77 , 0xA0},
		{0x80 , 0x00 , 0x08 , 0x20},
		{0x7F , 0xFF , 0xFF , 0xC0}
};

U8	gopt1[OPTCSYM_Y][OPTCSYM_X] = {
		{0x1F , 0xFF , 0xFF , 0xF0},
		{0x20 , 0x00 , 0x00 , 0x08},
		{0x26 , 0x73 , 0xE1 , 0x08},
		{0x49 , 0x48 , 0x83 , 0x08},
		{0x49 , 0x48 , 0x81 , 0x08},
		{0x49 , 0x70 , 0x81 , 0x10},
		{0x89 , 0x40 , 0x81 , 0x10},
		{0x89 , 0x40 , 0x81 , 0x10},
		{0x86 , 0x40 , 0x83 , 0xA0},
		{0x80 , 0x00 , 0x00 , 0x20},
		{0x7F , 0xFF , 0xFF , 0xC0}
};

U8	gopt2[OPTDSYM_Y][OPTDSYM_X] = {
		{ 0x1F , 0xFF , 0xFF , 0xF0},
		{ 0x20 , 0x00 , 0x00 , 0x08},
		{ 0x26 , 0x73 , 0xE3 , 0x08},
		{ 0x49 , 0x48 , 0x84 , 0x88},
		{ 0x49 , 0x48 , 0x80 , 0x88},
		{ 0x49 , 0x70 , 0x83 , 0x10},
		{ 0x89 , 0x40 , 0x84 , 0x10},
		{ 0x89 , 0x40 , 0x84 , 0x10},
		{ 0x86 , 0x40 , 0x87 , 0xA0},
		{ 0x80 , 0x00 , 0x00 , 0x20},
		{ 0x7F , 0xFF , 0xFF , 0xC0}
};

U8	gow[OWSYM_Y][OWSYM_X] = {
		{ 0x3F , 0xFF , 0xFF , 0x00},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x40 , 0xC8 , 0x20 , 0x80},
		{ 0x41 , 0x28 , 0x20 , 0x80},
		{ 0x41 , 0x29 , 0x20 , 0x80},
		{ 0x41 , 0x29 , 0x20 , 0x80},
		{ 0x41 , 0x29 , 0x20 , 0x80},
		{ 0x41 , 0x25 , 0x40 , 0x80},
		{ 0x40 , 0xC2 , 0x80 , 0x80},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x3F , 0xFF , 0xFF , 0x00}
};

U8	glow[LOWSYM_Y][LOWSYM_X] = {
		{0x07 , 0xFF , 0xFF , 0xE0},
		{0x08 , 0x00 , 0x00 , 0x10},
		{0x08 , 0x83 , 0x20 , 0x90},
		{0x08 , 0x84 , 0xA0 , 0x90},
		{0x08 , 0x84 , 0xA4 , 0x90},
		{0x08 , 0x84 , 0xA4 , 0x90},
		{0x08 , 0x84 , 0xA4 , 0x90},
		{0x08 , 0x84 , 0x95 , 0x10},
		{0x08 , 0xF3 , 0x0A , 0x10},
		{0x08 , 0x00 , 0x00 , 0x10},
		{0x07 , 0xFF , 0xFF , 0xE0}
};

U8	gvxo[VXOSYM_Y][VXOSYM_X] = {
		{ 0x3F , 0xFF , 0xFF , 0x00},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x44 , 0x51 , 0x30 , 0x80},
		{ 0x44 , 0x51 , 0x48 , 0x80},
		{ 0x44 , 0x4A , 0x48 , 0x80},
		{ 0x44 , 0x44 , 0x48 , 0x80},
		{ 0x44 , 0x4A , 0x48 , 0x80},
		{ 0x42 , 0x91 , 0x48 , 0x80},
		{ 0x41 , 0x11 , 0x30 , 0x80},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x3F , 0xFF , 0xFF , 0x00}
};

U8	grit[RITSYM_Y][RITSYM_X] = {
		{0x07 , 0xFF , 0xFF , 0xE0},
		{0x08 , 0x00 , 0x00 , 0x10},
		{0x08 , 0x71 , 0xDF , 0x10},
		{0x08 , 0x48 , 0x84 , 0x10},
		{0x08 , 0x48 , 0x84 , 0x10},
		{0x08 , 0x70 , 0x84 , 0x10},
		{0x08 , 0x60 , 0x84 , 0x10},
		{0x08 , 0x50 , 0x84 , 0x10},
		{0x08 , 0x49 , 0xC4 , 0x10},
		{0x08 , 0x00 , 0x00 , 0x10},
		{0x07 , 0xFF , 0xFF , 0xE0}
};

U8	gts[TSSYM_Y][TSSYM_X] = {
		{ 0x3F , 0xFF , 0xFF , 0x00},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x40 , 0xF9 , 0x80 , 0x80},
		{ 0x40 , 0x22 , 0x40 , 0x80},
		{ 0x40 , 0x22 , 0x00 , 0x80},
		{ 0x40 , 0x21 , 0x80 , 0x80},
		{ 0x40 , 0x20 , 0x40 , 0x80},
		{ 0x40 , 0x22 , 0x40 , 0x80},
		{ 0x40 , 0x21 , 0x80 , 0x80},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x3F , 0xFF , 0xFF , 0x00}
};

U8	gmhz[MHZSYM_Y][MHZSYM_X] = {
		{ 0x07 , 0xFF , 0xFF , 0xE0},
		{ 0x08 , 0x00 , 0x00 , 0x10},
		{ 0x08 , 0x45 , 0x20 , 0x10},
		{ 0x08 , 0x6D , 0x20 , 0x10},
		{ 0x08 , 0x55 , 0x2E , 0x10},
		{ 0x08 , 0x55 , 0xE2 , 0x10},
		{ 0x08 , 0x45 , 0x24 , 0x10},
		{ 0x08 , 0x45 , 0x28 , 0x10},
		{ 0x08 , 0x45 , 0x2E , 0x10},
		{ 0x08 , 0x00 , 0x00 , 0x10},
		{ 0x07 , 0xFF , 0xFF , 0xE0}
};

U8	gprog[PROGSYM_Y][PROGSYM_X] = {
		{ 0x3F , 0xFF , 0xFF , 0x00},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x5C , 0xE3 , 0x18 , 0x80},
		{ 0x52 , 0x94 , 0xA4 , 0x80},
		{ 0x52 , 0x94 , 0xA0 , 0x80},
		{ 0x5C , 0xE4 , 0xA0 , 0x80},
		{ 0x50 , 0xC4 , 0xAC , 0x80},
		{ 0x50 , 0xA4 , 0xA4 , 0x80},
		{ 0x50 , 0x93 , 0x18 , 0x80},
		{ 0x40 , 0x00 , 0x00 , 0x80},
		{ 0x3F , 0xFF , 0xFF , 0x00}
};

U8	gband[BANDSYM_Y][BANDSYM_X] = {
		{ 0x07 , 0xFF , 0xFF , 0xE0},
		{ 0x08 , 0x00 , 0x00 , 0x10},
		{ 0x0B , 0x8C , 0x8B , 0x90},
		{ 0x0A , 0x52 , 0xCA , 0x50},
		{ 0x0A , 0x52 , 0xAA , 0x50},
		{ 0x0B , 0x9E , 0xAA , 0x50},
		{ 0x0A , 0x52 , 0xAA , 0x50},
		{ 0x0A , 0x52 , 0x9A , 0x50},
		{ 0x0B , 0x92 , 0x8B , 0x90},
		{ 0x08 , 0x00 , 0x00 , 0x10},
		{ 0x07 , 0xFF , 0xFF , 0xE0}
};

U8	gsub[SUBSYM_Y][SUBSYM_X] = {
		{ 0x00 , 0x00 , 0x7F , 0xFF , 0xF0},
		{ 0x00 , 0x00 , 0xFF , 0xFF , 0xF8},
		{ 0x00 , 0x00 , 0xF3 , 0x74 , 0x78},
		{ 0x00 , 0x00 , 0xED , 0x75 , 0xB8},
		{ 0x00 , 0x00 , 0xEF , 0x75 , 0xB8},
		{ 0x00 , 0x00 , 0xF3 , 0x74 , 0x78},
		{ 0x00 , 0x00 , 0xFD , 0x75 , 0xB8},
		{ 0x00 , 0x00 , 0xED , 0x75 , 0xB8},
		{ 0x00 , 0x00 , 0xF3 , 0x8C , 0x78},
		{ 0x00 , 0x00 , 0xFF , 0xFF , 0xF8},
		{ 0x00 , 0x00 , 0x7F , 0xFF , 0xF0}
};

U8	glock[LOCKSYM_Y][LOCKSYM_X] = {
		{ 0x3F , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xE0},
		{ 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x10},
		{ 0x40 , 0x10 , 0x41 , 0x8C , 0x90 , 0x40 , 0x10},
		{ 0x40 , 0x18 , 0x42 , 0x52 , 0x90 , 0xC0 , 0x10},
		{ 0x40 , 0x1C , 0x42 , 0x50 , 0xA1 , 0xC0 , 0x10},
		{ 0x5F , 0xFE , 0x42 , 0x50 , 0xC3 , 0xFF , 0xD0},
		{ 0x40 , 0x1C , 0x42 , 0x50 , 0xA1 , 0xC0 , 0x10},
		{ 0x40 , 0x18 , 0x42 , 0x52 , 0x90 , 0xC0 , 0x10},
		{ 0x40 , 0x10 , 0x79 , 0x8C , 0x90 , 0x40 , 0x10},
		{ 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x10},
		{ 0x3F , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xE0}
};