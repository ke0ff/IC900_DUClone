/****************************************************************************************
 ****************** COPYRIGHT (c) 2023 by Joseph Haas (DBA FF Systems)  *****************
 *
 *  File name: segments.c
 *
 *  Module:    Control
 *
 *  Summary:   This is the main code file for the ADF4351 PLL setup application
 *
 *  File scope revision history:
 *    11-24-23 jmh:  creation date
 *
 ***************************************************************************************/
#define	SEGINCL

#include "typedef.h"
#include "lcd_db.h"
#include "segments.h"

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////
//		 DIGITS - MAIN          // ***************************************************
//////////////////////////////////

U8 digit0[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xFA}, {0x0E,0xFF,0xF7},
	{0x0F,0x7F,0xEF}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x00,0x0F},
	{0x0E,0x00,0x07}, {0x04,0x00,0x02}, {0x00,0x00,0x00}, {0x04,0x00,0x02},
	{0x0E,0x00,0x07}, {0x0F,0x00,0x0F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x7F,0xEF}, {0x0E,0xFF,0xF7}, {0x05,0xFF,0xFA},
	{0x03,0xFF,0xFC}, {0x01,0xFF,0xF8}
};
	
U8 digit1[38][3] = {
	{0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x02}, {0x00,0x00,0x07},
	{0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x0F},
	{0x00,0x00,0x07}, {0x00,0x00,0x02}, {0x00,0x00,0x00}, {0x00,0x00,0x02},
	{0x00,0x00,0x07}, {0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x0F}, {0x00,0x00,0x07}, {0x00,0x00,0x02},
	{0x00,0x00,0x00}, {0x00,0x00,0x00}
};
	
U8 digit2[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA}, {0x00,0xFF,0xF7},
	{0x00,0x7F,0xEF}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x0F},
	{0x00,0xFF,0xF7}, {0x01,0xFF,0xFA}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xF8},
	{0x0E,0xFF,0xF0}, {0x0F,0x00,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x7F,0xE0}, {0x0E,0xFF,0xF0}, {0x05,0xFF,0xF8},
	{0x03,0xFF,0xFC}, {0x01,0xFF,0xF8}
};
	
U8 digit3[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA}, {0x00,0xFF,0xF7},
	{0x00,0x7F,0xEF}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x0F},
	{0x00,0xFF,0xF7}, {0x01,0xFF,0xFA}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA},
	{0x00,0xFF,0xF7}, {0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x7F,0xEF}, {0x00,0xFF,0xF7}, {0x01,0xFF,0xFA},
	{0x03,0xFF,0xFC}, {0x01,0xFF,0xF8}
};
	
U8 digit4[38][3] = {
	{0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x04,0x00,0x02}, {0x0E,0x00,0x07},
	{0x0F,0x00,0x0F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x00,0x0F},
	{0x0E,0xFF,0xF7}, {0x05,0xFF,0xFA}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA},
	{0x00,0xFF,0xF7}, {0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x0F}, {0x00,0x00,0x07}, {0x00,0x00,0x02},
	{0x00,0x00,0x00}, {0x00,0x00,0x00}
};
	
U8 digit5[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xF8}, {0x0E,0xFF,0xF0},
	{0x0F,0x7F,0xE0}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x00,0x00},
	{0x0E,0xFF,0xF0}, {0x05,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA},
	{0x00,0xFF,0xF7}, {0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x7F,0xEF}, {0x00,0xFF,0xF7}, {0x01,0xFF,0xFA},
	{0x03,0xFF,0xFC}, {0x01,0xFF,0xF8}
};
	
U8 digit6[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xF8}, {0x0E,0xFF,0xF0},
	{0x0F,0x7F,0xE0}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00},
	{0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x80,0x00}, {0x0F,0x00,0x00},
	{0x0E,0xFF,0xF0}, {0x05,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xFA},
	{0x0E,0xFF,0xF7}, {0x0F,0x00,0x0F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x7F,0xEF}, {0x0E,0xFF,0xF7}, {0x05,0xFF,0xFA},
	{0x03,0xFF,0xFC}, {0x01,0xFF,0xF8}
};
	
U8 digit7[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA}, {0x00,0xFF,0xF7},
	{0x00,0x7F,0xEF}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x0F},
	{0x00,0x00,0x07}, {0x00,0x00,0x02}, {0x00,0x00,0x00}, {0x00,0x00,0x02},
	{0x00,0x00,0x07}, {0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x0F}, {0x00,0x00,0x07}, {0x00,0x00,0x02},
	{0x00,0x00,0x00}, {0x00,0x00,0x00}
};
	
U8 digit8[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xFA}, {0x0E,0xFF,0xF7},
	{0x0F,0x7F,0xEF}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x00,0x0F},
	{0x0E,0xFF,0xF7}, {0x05,0xFF,0xFA}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xFA},
	{0x0E,0xFF,0xF7}, {0x0F,0x00,0x0F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x7F,0xEF}, {0x0E,0xFF,0xF7}, {0x05,0xFF,0xFA},
	{0x03,0xFF,0xFC}, {0x01,0xFF,0xF8}
};
	
U8 digit9[38][3] = {
	{0x01,0xFF,0xF8}, {0x03,0xFF,0xFC}, {0x05,0xFF,0xFA}, {0x0E,0xFF,0xF7},
	{0x0F,0x7F,0xEF}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F},
	{0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x80,0x1F}, {0x0F,0x00,0x0F},
	{0x0E,0xFF,0xF7}, {0x05,0xFF,0xFA}, {0x03,0xFF,0xFC}, {0x01,0xFF,0xFA},
	{0x00,0xFF,0xF7}, {0x00,0x00,0x0F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F}, {0x00,0x00,0x1F},
	{0x00,0x00,0x1F}, {0x00,0x00,0x0F}, {0x00,0x00,0x07}, {0x00,0x00,0x02},
	{0x00,0x00,0x00}, {0x00,0x00,0x00}
};

//////////////////////////////////
//		 DIGITS - SUB           // ***************************************************
//////////////////////////////////

U8 digits0[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x05 , 0xFA }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x04 , 0x02 },
		{ 0x00 , 0x00 }, { 0x04 , 0x02 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x05 , 0xFA }, { 0x03 , 0xFC },
		{ 0x01 , 0xF8 }
};

U8 digits1[25][2] = {
		{ 0x00, 0x00 }, { 0x00, 0x00 },
		{ 0x00, 0x02 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x02 },
		{ 0x00, 0x00 }, { 0x00, 0x02 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x07 }, { 0x00, 0x07 },
		{ 0x00, 0x02 }, { 0x00, 0x00 },
		{ 0x00, 0x00 }
};

U8 digits2[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x01 , 0xFA }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x01 , 0xFA },
		{ 0x03 , 0xFC }, { 0x05 , 0xF8 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x05 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x01 , 0xF8 }
};

U8 digits3[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x01 , 0xFA }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x01 , 0xFA },
		{ 0x03 , 0xFC }, { 0x01 , 0xFA },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x01 , 0xFA }, { 0x03 , 0xFC },
		{ 0x01 , 0xF8 }
};

U8 digits4[25][2] = {
		{ 0x00 , 0x00 }, { 0x00 , 0x00 },
		{ 0x04 , 0x02 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x05 , 0xFA },
		{ 0x03 , 0xFC }, { 0x01 , 0xFA },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x02 }, { 0x00 , 0x00 },
		{ 0x00 , 0x00 }
};

U8 digits5[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x05 , 0xF8 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x05 , 0xF8 },
		{ 0x03 , 0xFC }, { 0x01 , 0xFA },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x01 , 0xFA }, { 0x03 , 0xFC },
		{ 0x01 , 0xF8 }
};

U8 digits6[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x05 , 0xF8 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x0E , 0x00 },
		{ 0x0E , 0x00 }, { 0x05 , 0xF8 },
		{ 0x03 , 0xFC }, { 0x05 , 0xFA },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x05 , 0xFA }, { 0x03 , 0xFC },
		{ 0x01 , 0xF8 }
};

U8 digits7[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x01 , 0xFA }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x02 },
		{ 0x00 , 0x00 }, { 0x00 , 0x02 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x02 }, { 0x00 , 0x00 },
		{ 0x00 , 0x00 }
};

U8 digits8[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x05 , 0xFA }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x05 , 0xFA },
		{ 0x03 , 0xFC }, { 0x05 , 0xFA },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x05 , 0xFA }, { 0x03 , 0xFC },
		{ 0x01 , 0xF8 }
};

U8 digits9[25][2] = {
		{ 0x01 , 0xF8 }, { 0x03 , 0xFC },
		{ 0x05 , 0xFA }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x0E , 0x07 },
		{ 0x0E , 0x07 }, { 0x05 , 0xFA },
		{ 0x03 , 0xFC }, { 0x01 , 0xFA },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x07 }, { 0x00 , 0x07 },
		{ 0x00 , 0x02 }, { 0x00 , 0x00 },
		{ 0x00 , 0x00 }
};

U8 digitmS[18][2] = {
		{ 0x03 , 0xFE }, { 0x07 , 0xFC },
		{ 0x06 , 0x00 }, { 0x06 , 0x00 },
		{ 0x06 , 0x00 }, { 0x06 , 0x00 },
		{ 0x06 , 0x00 }, { 0x06 , 0x00 },
		{ 0x07 , 0xFC }, { 0x03 , 0xFE },
		{ 0x00 , 0x06 }, { 0x00 , 0x06 },
		{ 0x00 , 0x06 }, { 0x00 , 0x06 },
		{ 0x00 , 0x06 }, { 0x00 , 0x06 },
		{ 0x03 , 0xFE }, { 0x07 , 0xFC },
};

U8 digitsS[12][2] = {
		{ 0x07 , 0xE0 }, { 0x07 , 0xF0 },
		{ 0x06 , 0x00 }, { 0x06 , 0x00 },
		{ 0x06 , 0x00 }, { 0x07 , 0xE0 },
		{ 0x07 , 0xF0 }, { 0x00 , 0x30 },
		{ 0x00 , 0x30 }, { 0x00 , 0x30 },
		{ 0x07 , 0xF0 }, { 0x03 , 0xE0 },
};

//////////////////////////////////
// SEGMENTS - MAIN              // ***************************************************
//////////////////////////////////

U8 segm_a[5][3] = {
	{0x01,0xFF,0xF8},
	{0x03,0xFF,0xFC},
	{0x01,0xFF,0xF8},
	{0x00,0xFF,0xF0},
	{0x00,0x7F,0xE0}
};

U8 segm_b[19] = {
	 0x00,0x00,0x02,0x07,
	 0x0F,0x1F,0x1F,0x1F,
	 0x1F,0x1F,0x1F,0x1F,
	 0x1F,0x1F,0x1F,0x0F,
	 0x07,0x02,0x00
};

U8 segm_c[19] = {
	 0x02,0x07,0x0F,0x1F,
	 0x1F,0x1F,0x1F,0x1F,
	 0x1F,0x1F,0x1F,0x1F,
	 0x1F,0x1F,0x0F,0x07,
	 0x02,0x00,0x00
};

U8 segm_d[5][3] = {
	{0x00,0x7F,0xE0},
	{0x00,0xFF,0xF0},
	{0x01,0xFF,0xF8},
	{0x03,0xFF,0xFC},
	{0x01,0xFF,0xF8}
};

U8 segm_e[19][2] = {
	{0x04,0x00}, {0x0E,0x00},
	{0x0F,0x00}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x00}, {0x0E,0x00},
	{0x04,0x00}, {0x00,0x00},
	{0x00,0x00}
};

U8 segm_f[19][2] = {
	{0x00,0x00}, {0x00,0x00},
	{0x04,0x00}, {0x0E,0x00},
	{0x0F,0x00}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x80},
	{0x0F,0x80}, {0x0F,0x00},
	{0x0E,0x00}, {0x04,0x00},
	{0x00,0x00}
};

U8 segm_g[5][3] = {
	{0x00,0xFF,0xF0},
	{0x01,0xFF,0xF8},
	{0x03,0xFF,0xFC},
	{0x01,0xFF,0xF8},
	{0x00,0xFF,0xF0}
};

U8 segm_p[3] = {0xE0,0xE0,0xE0};

/////////////////////////////////
//  	SEGMENTS - SUB         // ***************************************************
/////////////////////////////////

U8 segs_a[3][2] = {
		{0x01,0xF8},
		{0x03,0xFC},
		{0x01,0xF8}
};

U8 segs_b[12] = {
		 0x00,0x00,0x02,0x07,
		 0x07,0x07,0x07,0x07,
		 0x07,0x07,0x07,0x02};

U8 segs_c[12] = {
		 0x02,0x07,0x07,0x07,
		 0x07,0x07,0x07,0x07,
		 0x07,0x02,0x00,0x00};

U8 segs_d[3][2] = {
		{0x01,0xF8},
		{0x03,0xFC},
		{0x01,0xF8}
};

U8 segs_e[12] = {
		 0x04,0x0E,0x0E,0x0E,
		 0x0E,0x0E,0x0E,0x0E,
		 0x0E,0x04,0x00,0x00};

U8 segs_f[12] = {
		 0x00,0x00,0x04,0x0E,
		 0x0E,0x0E,0x0E,0x0E,
		 0x0E,0x0E,0x0E,0x04};

U8 segs_g[3][2] = {
		{0x01,0xF8},
		{0x03,0xFC},
		{0x01,0xF8}
};

U8 segs_p[2] = {0x60,0x60};