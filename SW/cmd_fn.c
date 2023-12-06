/********************************************************************
 ************ COPYRIGHT (c) 2022 by ke0ff, Taylor, TX   *************
 *
 *  File name: cmd_fn.c
 *
 *  Module:    Control
 *
 *  Summary:
 *  CLI Command Interpreter
 *  
 *******************************************************************/

// generally required includes
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include "typedef.h"

// application-specific includes
#include "inc/tm4c123gh6pm.h"
#include "init.h"						// App-specific SFR Definitions
#include "cmd_fn.h"						// fn protos, bit defines, rtn codes, etc.
#include "serial.h"
#include "version.h"
#include "eeprom.h"
#include "tiva_init.h"
#include "spi.h"
#include "lcd_db.h"
//#include "lcd_test.h"

//===================================================================================================
// Command token syntax defines and instantiations.
//
// This is the define list for the command line tokens used in this CLI.  Each "CMD" define is a quoted string that
//	corresponds to the minimum typed command line token.  The "ENUM" define sets the token used in the parsing
//	switch.  The "CMD" defines are entered into an array of arrays with a terminating array of 0xff, eg:
//		char* cmd_list[] = { CMD_1, CMD_2, ... , "\xff" };
//	The "ENUM" defines are likewise entered into an "enum" statement, eg:
//		enum cmd_enum{ ENUM_1, ENUM_2, ... , ENUM_LAST };
//	The primary requirement is for the subscript numbers for the "CMD" and "ENUM" must be in matching order.
//		The list order of the CMD/ENUM defines establishes the search order.  Lower-indexed (left-most) items are
//		searched first.  This means that the least-ambiguous items should be included earlier in the list to prevent
//		situations whereby a command of "BT" is entered, but matches on "B" in the list because it was the first
//		encountered.  So, for this example, "BT" should be first in the
//
// cmd_type is adjusted to the number of tokens in the list:

#define	cmd_type	char	// define as char for list < 255, else define as int

#define	CMD_1		"bu"			// bus test command ID
#define	ENUM_1		bus_cmd
#define	CMD_2A		"bank"			// Bluetooth debug command
#define	ENUM_2A		set_bank
#define	CMD_2		"bt"			// Bluetooth debug command
#define	ENUM_2		bttest
#define	CMD_3		"b"				// beeper debug
#define	ENUM_3		beeper
#define	CMD_31		"help"			// Alternate help syntax
#define	ENUM_31		help2
#define	CMD_32		"hi"			// hi/lo power debug
#define	ENUM_32		hilo
#define	CMD_4		"h"				// HM-133 data debug
#define	ENUM_4		hm_data
#define	CMD_41		"key"			// HM-133 key signal
#define	ENUM_41		set_hmkey
#define	CMD_5		"k"				// KPU debug data
#define	ENUM_5		kp_data
#define	CMD_6		"at"
#define	ENUM_6		tst_att
#define	CMD_7		"as"
#define	ENUM_7		tst_asc
#define	CMD_8		"a"
#define	ENUM_8		adc_tst
#define	CMD_81		"dup"
#define	ENUM_81		dup
#define	CMD_9		"d"
#define	ENUM_9		dis_la
#define	CMD_10		"lt"
#define	ENUM_10		lcd_tst
#define	CMD_101		"o"
#define	ENUM_101	set_offset
#define	CMD_102		"ptts"
#define	ENUM_102	pttsub
#define	CMD_11		"p"
#define	ENUM_11		tst_pwm
#define	CMD_12		"e"
#define	ENUM_12		tst_enc
#define	CMD_13		"frq"
#define	ENUM_13		tst_freq
#define	CMD_131		"f"
#define	ENUM_131	set_freq
#define	CMD_14		"info"
#define	ENUM_14		info
#define	CMD_15		"mstr"
#define	ENUM_15		mstr
#define	CMD_160		"nvall"
#define	ENUM_160	setnvall
#define	CMD_16		"nr"
#define	ENUM_16		nvrd
#define	CMD_17		"nw"
#define	ENUM_17		nvwr
#define	CMD_18		"nc"
#define	ENUM_18		nvcmd
#define	CMD_19		"u"
#define	ENUM_19		tstuart1
#define	CMD_20		"scan"
#define	ENUM_20		scan_cmd
#define	CMD_21		"sto"			// store memory data
#define	ENUM_21		sto_mem
#define	CMD_21A		"sav"			// send memory data
#define	ENUM_21A	sav_mem
#define	CMD_210		"s"				// squelch
#define	ENUM_210	squc
#define	CMD_211		"to"
#define	ENUM_211	tone
#define	CMD_212		"tsa"
#define	ENUM_212	tsac
#define	CMD_213		"tsb"
#define	ENUM_213	tsbc
#define	CMD_22		"ti"
#define	ENUM_22		timer_tst
#define	CMD_23		"t"
#define	ENUM_23		trig_la
#define	CMD_24		"?"				// help list
#define	ENUM_24		help1
#define	CMD_26		"vers"			// version info
#define	ENUM_26		vers
#define	CMD_27		"v"			// version info
#define	ENUM_27		volc
#define	ENUM_LAST	lastcmd


char* cmd_list[] = { CMD_1, CMD_2A, CMD_3, CMD_31, CMD_32, CMD_4, CMD_41, CMD_5, CMD_6, CMD_7, CMD_81, CMD_10, CMD_102, CMD_11, \
				     CMD_12, CMD_13, CMD_131, CMD_14, CMD_15, CMD_160, CMD_16, CMD_17, CMD_18, CMD_20, CMD_21, CMD_21A, CMD_210, CMD_211, CMD_212, CMD_213, \
				     CMD_22, CMD_24, CMD_26, CMD_27, "\xff" };

enum       cmd_enum{ ENUM_1, ENUM_2A, ENUM_3, ENUM_31, ENUM_32, ENUM_4, ENUM_41, ENUM_5, ENUM_6, ENUM_7, ENUM_81, ENUM_10, ENUM_102, ENUM_11, \
	   	   	   	   	 ENUM_12, ENUM_13, ENUM_131, ENUM_14, ENUM_15, ENUM_160, ENUM_16, ENUM_17, ENUM_18, ENUM_20, ENUM_21, ENUM_21A, ENUM_210, ENUM_211, ENUM_212, ENUM_213, \
					 ENUM_22, ENUM_24, ENUM_26, ENUM_27, ENUM_LAST };

// enum error message ID
enum err_enum{ no_response, no_device, target_timeout };

//===================================================================================================

//=============================================================================
// local registers

#define MAX_PRESP_BUF 80
char bcmd_resp_buf[MAX_PRESP_BUF + 10];
char* bcmd_resp_ptr;
U16	device_eprom_start;					// device parameter regs
U16	device_eprom_end;
U16	device_eeprom_start;
U16	device_eeprom_end;
U16	string_addr;						// string-parse next empty address
U16 sernum;								// serial number utility register
S8	device_eprom;
S8	device_eeprom;
U8	device_type;
S8	boot_len_fixed;
U16	device_max_bootlen;
U16	device_pprog;
char device_valid;
// HM-133 MFmic support
U8	hm_buf[HM_BUFF_END];				// signal buffer
U8	hm_hptr;
U8	hm_tptr;
U8	shftm;								// fn-shift mem register (MFmic)
char srbuf[STAT_BUF_LEN];				// status sending array
U8	key_count;
char key_hold;


// test init messages
U8	lcd_test_01a[] = { 0x41, 0xe0 };				// CS1
//							0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    32    33    34    35
U8	lcd_test_01[] = { 0x5f, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf };				// CS1
U8	lcd_test_01b[] = { 0x42, 0xff, 0xdf };			// CS1

U8	lcd_test_02a[] = { 0x81, 0xe0 };				// CS2
U8	lcd_test_02[] = { 0x9f, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf };				// CS1
U8	lcd_test_02b[] = { 0x82, 0xff, 0xdf };			// CS2

U8	lcd_init_00[] = { 0x43, 0x49, 0x20, 0x00 };				// CS1
U8	lcd_init_00b[] = { 0x83, 0x49, 0x20, 0x00 };			// CS2
U8	lcd_init_01[] = { 0x44, 0xe5, 0xc0, 0xfc, 0x81 };		// ??
U8	lcd_init_02[] = { 0x44, 0xe5, 0xc0, 0xfc, 0x81 };		// ??

U8	lcd_init_tst0[] = { 0x42, 0xe0, 0xd4 };
U8	lcd_init_tst1[] = { 0x61, 1 };							// data


U8	lcd_init_03[] = { 0x43, 0xe9, 0xd4, 0x15 };
U8	lcd_init_04[] = { 0x66, 0, 2, 5, 6, 4, 1 };				// data
U8	lcd_init_05[] = { 0x41, 0xd4 };

U8	lcd_init_06[] = { 0x45, 0xe4, 0xd0, 0xd2, 0xe1, 0x15 };
U8	lcd_init_07[] = { 0x61, 0x02 };							// data
U8	lcd_init_08[] = { 0x87, 0xfe, 0xd0, 0xfe, 0xc0, 0xb2, 0xe1, 0xc0 }; //CS2
U8	lcd_init_09[] = { 0x44, 0xfd, 0x91, 0xff, 0xd0 };
U8	lcd_init_10[] = { 0x44, 0xe5, 0xc0, 0xfc, 0x81 };
U8	lcd_init_11[] = { 0x45, 0xe4, 0xd0, 0xd2, 0xe1, 0x15 };
U8	lcd_init_12[] = { 0x61, 0x02 };							// data
U8	lcd_init_13[] = { 0x82, 0xe0, 0xd6 }; 					//CS2
U8	lcd_init_14[] = { 0x43, 0xd0, 0xfe, 0xd0 };
U8	lcd_init_15[] = { 0x82, 0xe1, 0xd0 }; 					//CS2
U8	lcd_init_16[] = { 0x82, 0xe0, 0xd6 }; 					//CS2
U8	lcd_init_17[] = { 0x43, 0xd0, 0xfe, 0xd0 };
U8	lcd_init_18[] = { 0x82, 0xe1, 0xd0 }; 					//CS2
U8	lcd_init_19[] = { 0x82, 0xe8, 0x14 };					//CS2
U8	lcd_init_20[] = { 0xa5, 0x10, 0xd8, 0xd8, 0xeb, 0x10 };	// CS2 data

U8	lcd_init_21[] = { 0x42, 0x11, 0x1b };					//CS1 disp on
U8	lcd_init_21b[] = { 0x82, 0x11, 0x1b };					//CS2
U8	lcd_init_22[] = { 0x87, 0xfe, 0xd0, 0xfe, 0xc0, 0xb2, 0xe1, 0xc0 };					//CS2
U8	lcd_init_23[] = { 0x44, 0xfd, 0x91, 0xff, 0xd0 };

U8	lcd_init_L1[] = { 0x42, 0xe6, 0x14 };					// looping LCD data
U8	lcd_init_L2[] = { 0x61, 0x00 };
U8	lcd_init_L3[] = { 0x42, 0xe8, 0xb0 };

U8	lcd_init_L4[] = { 0x82, 0xfb, 0x14 };
U8	lcd_init_L5[] = { 0xa1, 0x00 };
U8	lcd_init_L6[] = { 0x82, 0xfd, 0xb0 };

//				          0123456789012345
const char un_ary[] =  { "RDU-900,ke0ff\0\0\0" };			// init User SN string
const char teststr[] = { "THIS IS KE0FF " };				// test string

char	band_str[6][5] = {
		{"10m \0"},
		{"6m  \0"},
		{"2m  \0"},
		{"220 \0"},
		{"440 \0"},
		{"1296\0"}
};

//=============================================================================
// local Fn declarations

void get_BCD32(char *sptr, U32 *bcdval);
U8 get_Dargs(U8 argsrt, U8 nargs, char* args[ARG_MAX], U32 params[ARG_MAX]);
cmd_type cmd_srch(char* string);
char do_cmd_help(U8 cmd_id);
char parm_srch(U8 nargs, char* args[ARG_MAX], char* parm_str);
void parse_ehex(char * sptr);
void disp_error(U8 errnum);
void disp_fail(char* buf, char* s, U16 c, U16 d);
void disp_wait_addr(char* buf);
U16 boot_se(U8* bptr, U16 start, U16 end, U16 ppaddr);
U8* log_error_byte(U8* lbuf, U8 d, U8 h, U16 a);
void disp_error_log(U8* lbuf, U16 len);
void do_help(void);
void disp_esc(char flag);
U8 sto_nvmem(U8 band, U8 memnum, char* sptr);
char* char_srch(char* sptr, char searchr);
U8 str_chks(char* sptr);
U32 dpl_calc(U16 dplcode);
U8 cadd(U16 dplcode, U8 index);
void putOK(U8 tf);

//=============================================================================
// CLI cmd processor entry point
//	Somewhere in the application, there must be a constant polling for input characters or end of
//		line signal.  The goal is to produce a command line array that was terminated by an EOL
//		(ASCII 0x0d).  This array is passed through parse_args() before calling x_cmdfn().  The
//		base command line array and the args arrays are maintained elsewhere in the application
//		(usually in main.c).
//	Uses number of args (nargs) and arg pointer array (args[]) to lookup command and dispatch to
//		a command-specific code code block for execution.
//
//	offset is srecord offset value which is maintained in main() and passed
//	forward for the srecord functions (upload), if used.
//
//	Note that the original command line array is "chopped up" by parse_args() by replacing
//		whitespace characters with null-terminators (unless brackets by quotes, as described below).
//
//	"Switch" operands, such as "-C", are processed here.  These switches set flags and are then
//		removed from the parameter arrays, so that the subsequent command code doesn't have to
//		try and filter them out.  If a switch is included in a parameter field for a command that
//		does not interpret it, the switch flag is simply ignored.
//
//	get_Dargs() (get decimal args) converts ASCII numbers into integer values and is called from
//		each individual parser so that "no-parameter-entered" defaults can be established.  To
//		enforce these defaults, first initialize the params[] array with the default values, then
//		call get_Dargs().
//
//	whitespace() defines what ASCII characters are white-space parameter separators used to separate
//		parameters within the command line array.  Multiple, consecutive whitespaces are allowed and
//		are filtered out by parse_args().
//	quotespace() defines what ASCII characters are to be used as quotes inside the parameter field.
//		Quotes allow character strings which contain whitespace to be contained within a single
//		arg[] array.  Quotes effectively suspend whitespace delimiting within quote pairs.
//		The system doesn't differentiate opening or closing quotes.  As long as it is defined in
//		quotespace(), any character can be used to open and close a quoted parameter string.
//			e.g.: if a double (") and single (') quote character are defined in quotespace(),
//			any of the following are valid quoted strings:
//				"Now is the time on schprockets when we dance'
//				'No monkeys allowed!"
//				"Go for launch"
//
//	Parameter ordinal management is specified by the command-specific parser code blocks and Fns.
//		Generally, a fixed order is easiest to manage.  Non-entered values in a list of 2 or more
//		parameters are indicated by a "-" character.  Negation of decimal values must also be
//		addressed in the cmd-specific code.  Other management schemes are possible but would be
//		up to the cmd-specific code to execute.
//
//		Parametric switches ("-x" in the command line) do not have any ordinal requirements nor can
//		any be enforced under the current architecture.
//
//=============================================================================

int x_cmdfn(U8 nargs, char* args[ARG_MAX], U16* offset){

#define	OBUF_LEN 110				// buffer length
#define	MAX_LOG_ERRS (OBUF_LEN / 4)	// max number of errors logged by verify cmd
	char	obuf[OBUF_LEN];			// gp output buffer
//	char	abuf[30];				// temp string buffer
//	char	bbuf[6];				// temp string buffer
	U32		params[ARG_MAX];		// holding array for numeric params
	char	c;						// char temp
//	char	d;						// char temp
//	volatilechar	pm = FALSE;				// M flag, MAIN (set if "-M" found in args)
volatile	char	ph = FALSE;				// H flag, (set if "-h" found in args)
volatile	char	ps = FALSE;				// S flag, SUB (set if "-S" found in args)
volatile	char	pc = FALSE;				// C flag (set if "-C" found in args)
volatile	char	pw = FALSE;				// W flag (set if "-W" found in args)
volatile	char	px = FALSE;				// X flag (set if "-X" found in args)
volatile	char	pv = FALSE;				// V flag (set if <wsp>-V<wsp> found in args)
volatile	char	pr = FALSE;				// R flag (set if <wsp>-R<wsp> found in args)
	int		cmd_found = TRUE;		// default to true, set to false if no valid cmd identified
//	char*	q;						// char temp pointer
	char*	s;						// char temp pointer
	char*	t;						// char temp pointer
	char	cmd_id;					// command enumerated id
//	U8		g;						// temp
//	U8		h;						// temp
	U8		i;						// temp
	U8		j;						// temp
	U8		k;						// temp
//	S32		mem_buf8[7];			// U8 mem buffer
//	U8*		ptr0;					// U8 mem pointer
//	U16		ki;						// U16 temp
//	U16		kk;						// U16 temp
//	U32		hh;						// U16 temp
//	U16		adc_buf[8];				// adc buffer
//	U16*	ip;						// U16 pointer
//	U16		tadc_buf[8];			// adc buffer
//	uint32_t ii;
//	uint32_t jj;
//	volatile uint32_t* pii;					// u32 pointer
//	S32		si;
//	float	fa;
//	char	gp_buf[25];				// gen-purpose buffer

#ifdef DEBUG
//	U8		m;						// temp
//	U8		dbuf[10];				// U8 disp buf
//	S8		si;						// temp s
//	S8		sj;
//	float	fb;
#endif

	bchar = '\0';																// clear global escape
    if (nargs > 0){
    	if((args[0][0] != '~')){
//    		for(i = 0; i <= nargs; i++){										//upcase all args !!! might just upcase command token ???
//    			s = args[i];
//    			str_toupper(s);
//    		}
    	}
		t = args[0];															// point to first arg (cmd)
		cmd_id = cmd_srch(t);													// search for command
		s = args[1];															// point to 2nd arg (1st param)
		if(*s == '?'){
			if((cmd_id == help1) || (cmd_id == help2)){
				do_help();														// set to list all cmd helps
				putsQ("");
				for(i = 0; i < lastcmd; i++){
					if(do_cmd_help(i)) putsQ("");
				}
			}else{
				do_cmd_help(cmd_id);											// do help for cmd only
			}
		}else{
			c = parm_srch(nargs, args, "-s");									// capture sub floater
			if(c){
				ps = TRUE;
				nargs--;
			}
/*			c = parm_srch(nargs, args, "-m");									// capture main floater
			if(c){
				pm = TRUE;
				nargs--;
			}*/
			c = parm_srch(nargs, args, "-h");									// capture main floater
			if(c){
				ph = TRUE;
				nargs--;
			}
			c = parm_srch(nargs, args, "-v");									// capture v-flag floater (verbose)
			if(c){
				pv = TRUE;
				nargs--;
			}
			c = parm_srch(nargs, args, "-c");									// capture c-flag floater
			if(c){
				pc = TRUE;
				nargs--;
			}
			c = parm_srch(nargs, args, "-w");									// capture w-flag floater
			if(c){
				pw = TRUE;
				nargs--;
			}
			c = parm_srch(nargs, args, "-x");									// capture x-flag select floater
			if(c){
				px = TRUE;
				nargs--;
			}
			c = parm_srch(nargs, args, "-r");									// capture x-flag select floater
			if(c){
				pr = TRUE;
				nargs--;
			}
			gas_gage(2);														// init gas gauge to disabled state

// ====== PARSING SWITCH ==========================================================================================================================================

			switch(cmd_id){														// dispatch command
				case help1:
				case help2:														// MAIN HELP
					do_help();
					break;

				case info:														// info + version
					break;

				case vers:														// SW VERSION CMD
					dispSWvers();
					break;

				case set_hmkey:
					break;

				case set_freq:
					break;

				case set_bank:
					break;

				case sto_mem:													// store mem: p[0] = band, p[1] = mem#, p[2] = chks, a[4] = mem_param_string
					break;

				case sav_mem:
					break;

				case tone:														// tone: p[0] = mid, p[1] = tone freq, p[2] = on/off
					break;

				case dup:														// duplex: p[0] = mid, p[1] = "+", "-", or "S"
					break;

				case hilo:														// hilo: p[0] = mid, p[1] = "h", "l", "1", or "0"
					break;

// ==  DEBUG Fns FOLLOW +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

				case beeper:													// beeper test
					break;

				case scan_cmd:
					break;

				case bus_cmd:													// process LCD bus test cmd
					// init if -R
					if(pr){
						lcd_cntl(LDSC_INIT);
					}
					// send test data or cycle bus bit ("-C" = 1of8)
					if(pc){
						i = 0x01;
						j = 0;
						GPIO_PORTB_DIR_R = 0xff;				// port = out
						do{
							bchar = '\0';
//							wrdb(i,j);
							GPIO_PORTB_DATA_R = swapeo(i);				// port = i
							if(j) k = TADDR|nTRD|nTWR;
							else k = nTRD|nTWR;
							debug_db(k);
							sprintf(obuf,"1of8: %02x  %d", i, j);
							putsQ(obuf);
							i <<= 1;
							if(!i){
								i = 0x01;
								j ^= 0x01;
							}
							while(!bchar);										// wait for any chr
						}while(bchar != ESC);
						putsQ("Done.\n");
					}else{
						params[0] = 0;
						params[1] = 0;
						get_Dargs(1, nargs, args, params);						// parse param numerics into params[] array
						if(pw){
							// write LCD
							if(params[0]) i = TADDR;
							else i = 0;
							wrdb((U8)params[1], i, STA01);
							sprintf(obuf,"wr: %02x  %d", params[0], params[1]);
							putsQ(obuf);
						}else{
							// read LCD
							i = rddb((U8)params[0]);
							sprintf(obuf,"rd: %02x  %d", i, params[0]);
							putsQ(obuf);
						}
					}
					break;

				case lcd_tst:													// debug, LCD test
//					lcd_test();
					if(ps){
						obuf[0] = LOAD_PTR | 0x0a;
						obuf[1] = WITH_DECODE;
						obuf[2] = 0;
						obuf[3] = 0;
						obuf[4] = 0;
						obuf[5] = 2;
						obuf[6] = 9;
						obuf[7] = 2;
						obuf[8] = WITHOUT_DECODE;
						obuf[9] = LOAD_PTR | 0x1c;
						obuf[10] = WR_DMEM | 0x01;
						dbg_spirx((U8*)obuf, 11, 1);
						putsQ("spi dvt, ESC to quit...");
						for(i=0; i<15; i++){
							process_SPI(0);
						}
						while(bchar != ESC){
							process_LCD(0);
						}
					}else{
						if(pr || pv){
							if(pr){
								disp_err(1);
								putsQ("LCD err1");
							}
							if(pv){
								disp_err(2);
								putsQ("LCD err2");
							}
						}else{
							params[0] = 0xffff;
							params[1] = 0xffff;
							params[2] = 0;
							get_Dargs(1, nargs, args, params);						// parse param numerics into params[] array
							i = 1;
							if(params[0]>31){
								params[0] -= 32;
								i = 2;
							}
							switch(params[2]){
							case 1:
								j = MODE_OR;
								break;

							case 2:
								j = MODE_AND;
								break;

							default:
								j = MODE_WR;
							}
							trig_fill((U8)params[0], j | (U8)params[1], i);							// 1

							trig_scan1(MODE_OR);
							trig_scan2(MODE_OR);
							wrlcd_str(main7, MAIN7_LEN, MAIN7_OFFS);
							wrlcd_str(sub7, SUB7_LEN, SUB7_OFFS);
							wrlcd_str(mainsm, MAINSM_LEN, MAINSM_OFFS);
							wrlcd_str(optrow, OPTROW_LEN, OPTROW_OFFS);
							wrlcd_str(subsm, SUBSM_LEN, SUBSM_OFFS);
							putsQ("LCD test.");
						}
					}
					break;

				case nvrd:
					break;

				case nvwr:
					break;

				case nvcmd:
					break;

				case kp_data:													// debug, disp T3 capture buf
					break;

				case tst_att:
					break;

				case timer_tst:
					break;

				case tst_pwm:
					break;

				case tst_asc:
					break;

				case tst_freq:
					break;

				case tst_enc:													// pwr-off (sleep)
					break;

				default:
				case lastcmd:													// not valid cmd
					cmd_found = FALSE;
					break;

			} //=========== END PARSING SWITCH ======================================================================================
		}
    }
	if(bchar == ESC) while(gotchrQ()) getchrQ();									// if ESC, clear CLI input chrs
	return cmd_found;
}

//=============================================================================
// putOK() displays OK status response if true, else err
//=============================================================================
void putOK(U8 tf){

	if(tf){
		putsQ("#-OK$");
	}else{
		putsQ("#-ERR$");
	}
	return;
}

//=============================================================================
// do_help() displays main help screen
//=============================================================================
void do_help(void){
		//          1         2         3         4         5         6         7         8
		// 12345678901234567890123456789012345678901234567890123456789012345678901234567890
	putsQ("IC-900 DU-Clone CMD List:");
	putsQ("Syntax: <cmd> <arg1> <arg2> ... args are optional depending on cmd.");
	putsQ("\t<arg> order is critical except for floaters.");
	putsQ("\"?\" as first <arg> gives cmd help,  \"? ?\" lists all cmd help lines. When");
	putsQ("selectively entering <args>, use \"-\" for <args> that keep default value.");
	putsQ("\"=\" must precede decimal values w/o spaces. Floating <args>: these non-number");
	putsQ("<args> can appear anywhere in <arg> list: \"W\" = wait for operator\n");
//	putsQ("\tFreq set\tofp VERSion");

	putsQ("Supports baud rates of 115.2, 57.6, 38.4, 19.2, and 9.6 kb.  Press <Enter>");
	putsQ("as first character after reset at the desired baud rate.");
}

//=============================================================================
// do_cmd_help() displays individual cmd help using cmd_id enum
//=============================================================================
char do_cmd_help(U8 cmd_id){
	char 	c = TRUE;	// temps
//	U16		k;
//	U8		i;
//	char	hbuf[10];

	switch(cmd_id){														// dispatch help line
		case bus_cmd:
			putsQ("BUs <addr> ; <data> -Cycle -Write");
			putsQ("\t-C does a 1of8 cycle from D0 to D7 then to the addr line.");
			putsQ("\t-W does a write, else op is a read");
			putsQ("\tdisplays result to serial term");
			break;

		case lcd_tst:
			putsQ("lt <addr 0-63> ; <data> ; <0/1/2> WR, OR, AND");
			putsQ("\tconfigures segments");
			break;

/*
//			code char fnkey_code[] = { 'p',   'o',   'n',   'k',   'm',   'l',   'j',  '|',
//								  '!',   'a',   'b',   'c',   'q',   'd',   'e',   'f',
//								  'r',   'g',   'h',   'i',   's',   '+',   '`',   '$',
//								  't',   '\0' };
//			// Normal mode keycodes for HM-151 and HM-133 (see above for HM-133 notes)
//			code char   key_code[] = { 'L',   'T',   'X',   '/',   'V',   'M',   '\\',  'F',
//								  'G',   '1',   '2',   '3',   'A',   '4',   '5',   '6',
//								  'B',   '7',   '8',   '9',   'C',   '*',   '0',   '#',
//								  'D',   '\0' };

		case set_freq:
			putsQ("F <frq Hz> ?");
			putsQ("\tSet freq (Hz).  Sets VFO in corresponding band module.");
			putsQ("\tERR if <frq> not in range of any module.");
			break;

		case set_offset:
			putsQ("O <frq Hz> <mid>?");
			putsQ("\tSet Offs freq (Hz) <bid> = 1 for UX-19 up to 6 for UX-129");
			break;

		case sto_mem:													// store mem: p[0] = band, p[1] = mem#, p[2] = chks, a[4] = mem_param_string
			putsQ("STO <mid> <mchr> <chks> 'str' ?");
			putsQ("\tStore mem, <mid> is module ID (Ux19 = 1, UX-129 = 6),");
			putsQ("\t<mchr> = the mem# chr ID, <chks> = checksum of 'str' ");
			putsQ("\t       Valid = AlphaNum & ( ) [ ] !! EXCEPT C, I, O, Q !!");
			putsQ("\t'str' = 'VFO + OFFS + DPLX/pwr/skip + CTCSS + SQ + VOL + XIT + RIT +");
			putsQ("\tBID + !MEM_NAME!");
			putsQ("\t\tVFO/OFFS = KHz");
			putsQ("\t\tDPLX/pwr/skip = bitmap");
			putsQ("\t\tCTCSS = bitmap");
			putsQ("\t\tSQ/VOL = 0-34");
			putsQ("\t\tXIT/RIT = bitmap");
			putsQ("\t\tBID = <mid>-1");
			putsQ("\t\t!MEM_NAME! = mem name str (15 bytes max)");
			break;
			// str = "VFO + OFFS + DPLX/pwr/skip + CTCSS + SQ + VOL + XIT + RIT + BID-1 + !MEM_NAME! + STR_CHKS"
			// if STR_CHKS omitted or is incorrect, an error is reported, but the cmd line processes anyway
			// STO 3 A XX "1296000 20000 nn 103.5 34 34 -7 +7 5 !1234567890123456!"
			// 1234567890123456789012345678901234567890123456789012345678901234567890
			//          1         2         3         4         5         6         7
			// store mem: p[0] = band, p[1] = mem#, p[2] = chks, a[4] = mem_param_string
			// band#		// STO <bnum> <mchr> <chks> "str"

		case mstr:														// mem string: p[0] = band+1, p[1] = mem#, p[2] = string
			putsQ("MSTR <mid> <mchr> 'str' ?");
			putsQ("\tStore MEM Name String, <mid> is module ID (Ux19 = 1, UX-129 = 6),");
			putsQ("\t<mchr> = the mem# chr ID");
			putsQ("\t       Valid = AlphaNum & ( ) [ ] !! EXCEPT C, I, O, Q !!");
			putsQ("\t'str' = mem name str (15 bytes max)");
			break;

		case tone:														// tone: p[0] = mid, p[1] = tone freq, p[2] = on/off
			putsQ("TONE <mid> <tfreq> <on/off> ?");
			putsQ("\tStore tone & set on/off, <mid> is module ID (Ux19 = 1, UX-129 = 6),");
			putsQ("\tIf <tfreq> omitted, just adjust on/off");
			putsQ("\tIf <on/off> omitted, just adjust tfreq");
			i = 0;
			do{
				k = pl_lookup(i);
				if(k != 0xffff){
					sprintf(hbuf,"\t%3.1f",(float)(k)/10.0);
					putssQ(hbuf);
					if((++i%6) == 0) putsQ("");
				}
			}while(k != 0xffff);
			putsQ("");
			break;

		case dup:														// duplex: p[0] = mid, p[1] = +/-/S
			putsQ("DUP <mid> <+/-/S> ?");
			putsQ("\tSet duplex, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

		case hilo:														// hilo: p[0] = mid, p[1] = H/L/1/0
			putsQ("HIlo <mid> <H/L/1/0> ?");
			putsQ("\tSet RF power, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

		case volc:														// vol: p[0] = mid, p[1] = vol
			putsQ("Vol <mid> <0-34> ?");
			putsQ("\tSet VOL, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

		case squc:														// squ: p[0] = mid, p[1] = squ
			putsQ("Squ <mid> <0-34> ?");
			putsQ("\tSet SQU, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

		case tsac:														// tsa: p[0] = mid, p[1] = ts value
			putsQ("TSA <mid> <0-1000 KHz> ?");
			putsQ("\tSet TSA, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

		case tsbc:														// tsb: p[0] = mid, p[1] = ts value
			putsQ("TSB <mid> <0-1000 KHz> ?");
			putsQ("\tSet TSB, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

		case pttsub:													// pttsub: p[0] = mid, p[1] = 1-4, none, smute, scall, mcall
			putsQ("PTT-SUB <mid> <mode> ?");
			putsQ("\tSet PTTSub, <mid> is module ID (Ux19 = 1, UX-129 = 6),");
			putsQ("\t<mode> = '1', no-action at PTT xsitions");
			putsQ("\t<mode> = '2', toggle SMUTE at PTT xsitions");
			putsQ("\t<mode> = '3', toggle SUB-CALL at PTT xsitions");
			putsQ("\t<mode> = '4', toggle MAIN-CALL at PTT xsitions");
			break;

		case setnvall:													// nvall: p[0] = start_bid, p[1] = stop_bid
			putsQ("NVALL <start_mid> <stop_mid> ?");
			putsQ("\tSave VFO(s) to NVRAM, <mid> is module ID (Ux19 = 1, UX-129 = 6)");
			break;

/////////////////////////////////////
		case hm_data:													// debug
			putsQ("Hm data: ?");
			putsQ("\tdisplay HM-151 key data, ESC to exit");
			break;

//		case kp_data:													// debug
//			putsQ("Kp data: ?");
//			putsQ("\tdisplay key-pad data, ESC to exit");
//			break;
//
//		case tstuart1:													// debug UART1 tst CMD
//			// Test UART1:  TSTU1 ?/<string>/W: loop output of <string>
//			putsQ("TSTU1 (Test UART1): ?/<string>/W (loop until ESC)");
//			break;
//
//		case adc_tst:													// debug ADC tst CMD
//			// Test ADC:  Adc test ?
//			putsQ("Adc test: ?");
//			break;
//
//		case tst_enc:													// debug ADC tst CMD
//			// Test Enc:  Enc ?
//			putsQ("Enc test: ?");
//			putsQ("\tDisp encoder changes, ESC to exit.");
//			break;
//
//		case tst_pwm:													// debug ADC tst CMD
//			// Pwm debug:  P ?
//			putsQ("Pwm test: <pwm#>/<value>/?");
//			putsQ("\tDisplay min/max PWM and set PWM values.");
//			putsQ("\t<pwm#> = 2 - 6");
//			break;
//
//		case timer_tst:													// debug ADC tst CMD
//			// Test timer:  TI ?
//			putsQ("TImer test: ?");
//			putsQ("\tToggles PC4 @200ms period");
//			break;
*/
		default:
			c = FALSE;
			break;
	}
	return c;
}
/*
//=============================================================================
// disp_error() displays standard error messages & device off status
//=============================================================================
void disp_error(U8 errnum){

	do_red_led(LED_BLINK);
	switch(errnum){
		case no_response:
			putsQ("!! ERR !! No response from DUT.");
			break;

		case no_device:
			putsQ("!! ERR !! No DUT selected.");
			break;

		case target_timeout:
			putsQ("!! ERR !! DUT timeout.");
			break;

		default:
			break;
	}
	if(!dut_pwr(TARGET_STAT)) putsQ("DEVICE OFF");		// signal device off
}*/
/*
//=============================================================================
// disp_fail() displays pgm/read error message with params
//	this is a debug function that displays the HC11 fn comms status
//	Also, this fn sets the status LED to blink to indicate a failure was encountered
//=============================================================================
void disp_fail(char* buf, char* s, U16 c, U16 d){

	do_red_led(LED_BLINK);
	sprintf(buf,"%s Failed!! errs: %02x %02x", s, (U16)c, (U16)d);
	putsQ(buf);
}*/

//=============================================================================
// disp_wait_addr() dead code
//=============================================================================
void disp_wait_addr(char* buf){

	sprintf(buf,"disp_wait_addr = dead code\n");
	putsQ(buf);
}

//=============================================================================
// exec_bcmd() dead code
//=============================================================================
void exec_bcmd(char* bcmdbuf_ptr, char* obuf, U16* offset){

}

//***********************//
// bcmd functions follow //
//***********************//

//******************************//
// hosekeeping functions follow //
//******************************//



///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// process_CMD() handles periodic polling tasks for the cmd_fn.c file scope
//-----------------------------------------------------------------------------

char process_CMD(U8 flag){

	if(flag == PROC_INIT){
		// init file-local statics
		return 0;
	}
	return 0;
}

//=============================================================================
// get BCD param from string
//=============================================================================
void get_BCD32(char *sptr, U32 *bcdval){

	char*	s = sptr;

	*bcdval = 0;
	while(*s){
		*bcdval <<= 4;
		*bcdval |= asc_hex(*s++);
	}
	return;
}

//=============================================================================
// get numeric params from command line args
//	argsrt specifies the first item to be searched and aligns argsrt with params[0]
//	for multi-arg cmds, fields must be entered in order.
//=============================================================================
U8 get_Dargs(U8 argsrt, U8 nargs, char* args[ARG_MAX], U32 params[8]){

	char*	s;
	U32*	ptr1;
	S32		temp32;
	U8		i;
	U8		count = 0;

	if(argsrt < nargs){											// test for start in limit (abort if not)
		for(i = argsrt; i < nargs; i++){						// convert strings to values
			s = args[i];										// set pointers to array items
			ptr1 = &params[i - argsrt];
			switch(*s){
				case '-':										// skip if user specified default
				case '\0':										// or if arg is empty
					break;

				default:
					count += sscanf(s,"%d",&temp32);			// get decimal value
					*ptr1 = temp32;
					break;

				case '$':
					s++;
					count += sscanf(s,"%x",&temp32);			// get hex if leading "$"
					*ptr1 = temp32;
					break;

				case 'O':
				case 'o':
					s++;
					count += sscanf(s,"%o",&temp32);			// get octal if leading "O"
					*ptr1 = temp32;
					break;
			}
		}
	}
	return count;
}

//=============================================================================
// search for command keywords, return cmd ID if found
//	uses the cmd_list[] array which is constructed as an array of null terminated
//	strings compressed into a single string definition.  Commands are added by
//	placing the minimum required text from the command name with a '\0' terminating
//	null.  cmd_list[] is terminated by an $ff after all of the command names.
//	cmd_enum{} holds an enumerated, named list of all valid commands.  The enum
//	definition must be at the top of this file, so the commented-out version shown
//	below must be copied and pasted to the top of the file whan any changes are
//	made (commands added or deleted).
//
//	Some thought must be put into the order of command names.  Shorter matching
//	names (e.g., single chr entries) must follow longer names to allow the algortihm
//	to properly trap the shorter cmd name.
//	
//=============================================================================
cmd_type cmd_srch(char* string){
// dummy end of list, used to break out of search loop
const char end_list[] = {0xff};
// list of minimum length command words. cmd words are separated by '\0' and list terminated with 0xff
//const char cmd_list[] = {"H\0K\0A\0D\0L\0P\0E\0U\0S\0TI\0T\0?\0H\0VERS\0\xff"};
//enum cmd_enum{ hm_data,kp_data,adc_tst,log_data,tst_pwm,tst_enc,tstuart1,timer_tst,tst_tempa,help1,help2,vers,lastcmd,helpcmd };
//!!! make changes to cmd_enum here, move them to top of file, then un-comment !!!

	char*	ptr;							// temp ptr
	char	cmdid = 0;						// start at beginning of cmd_enum
	char	i;								// temp
	char	found = FALSE;					// cmd found flag (default to not found)

	ptr = cmd_list[cmdid];										// start at beginning of search list
	while((*ptr & 0x80) != 0x80){								// process until 0xff found in search list
		i = strncmp(string, ptr, strlen(ptr));					// inbound string match search list?
		if(i){
			cmdid++;											// no, advance to next cmdid
			ptr = cmd_list[cmdid];
//			while(*ptr++);										// skip to next item in search list
		}else{
			ptr = (char*)end_list;								// found match,
			found = TRUE;										// set break-out criteria
		}
	}
	if(!found) cmdid = lastcmd;									// not found, set error cmd id
	return cmdid;
}

/*	char*	ptr;							// temp ptr
	char	cmdid = 0;						// start at beginning of cmd_enum
	char	i;								// temp
	char	found = FALSE;					// cmd found flag (default to not found)

	ptr = (char*)cmd_list;										// start at beginning of search list
	while((*ptr & 0x80) != 0x80){								// process until 0xff found in search list
		i = strncmp(string, ptr, strlen(ptr));					// inbound string match search list?
		if(i){
			cmdid++;											// no, advance to next cmdid 
			while(*ptr++);										// skip to next item in search list
		}else{
			ptr = (char*)end_list;										// found match,
			found = TRUE;										// set break-out criteria
		}
	}
	if(!found) cmdid = lastcmd;									// not found, set error cmd id
	return cmdid;
}*/

//=============================================================================
// parm_srch() looks for a match of parm_str in any non-empty args[] strings
//	if found, remove the args entry from param list and return 1st chr of parm_str,
//	else return '\0'
//=============================================================================
char parm_srch(U8 nargs, char* args[ARG_MAX], char* parm_str){

	U8		i;								// counter temp
	char	c = '\0';						// first chr of matched parm_str (first time thru loop, there is no match)
	static char null_str[] = "";			// null string that persists

//	if(nargs > 1){
	    for(i = 1; i <= nargs; i++){							// search starting with first args[] item
			if(c){												// if(c!=null)...
				args[i] = args[i+1];							// if there was a match, move the subsequent pointers down one
			}else{
				if(strlen(parm_str) == strlen(args[i])){		// in order to match, the lengths have to be equal...
					if(strncmp(args[i], parm_str, strlen(parm_str)) == 0){ // look for match
						c = *parm_str;							// if match, capture 1st chr in matched string
						i--;									// back-up one to edit this item out of the list
					}
				}
			}
	    }
//	}
 	if(c != '\0'){
		args[ARG_MAX - 1] = null_str;							// if there was a match, the last pointer goes to null
		
	}
	return c;													// return first chr in matched string, or null if no match
}

//=============================================================================
// disp_esc() if param true, display "Press esc to exit" msg
//=============================================================================
void disp_esc(char flag){

	if(flag){
		putsQ("  Press <ESC> to exit.");
	}
	putsQ("");
}

//=============================================================================
// convert all chrs in string to upper case
//=============================================================================
void str_toupper(char *string){

    while(*string != '\0'){
        *string++ = toupper(*string);
    }
}

//=============================================================================
// parse command line array for delimited arguments.  Called prior to entry into x_cmdfn().
//  on exit, the args[] array (an array of char pointers) holds each delimited argument
//	from the command string input:
//  args[0] holds first arg (the command token)
//  args[1] holds next arg
//  args[2] etc...
//  up to args[ARG_MAX]
//
//  nargs holds number of arguments collected.  i.e., nargs = 3 specifies that args[0] .. args[3]
//      all hold arguments (three total, including the command).
//=============================================================================
int parse_args(char* cmd_string, char* args[ARG_MAX]){
	int i;
	char quote_c = 0;
	static char null_string[2] = "";

    // clear args pointers
    for (i=0; i<ARG_MAX; i++){
        args[i] = null_string;
    }
    i = 0;
    do{
        if(quotespace(*cmd_string, 0)){         // process quotes until end quote or end of string
            quote_c = *cmd_string;              // close quote must = open quote
            args[i++] = ++cmd_string;               // start args with 1st char after quote
            while(!quotespace(*cmd_string,quote_c)){
                if(*cmd_string == '\0'){
                    return i;                   // end of cmd string, exit
                }
                cmd_string++;
            }
            *cmd_string++ = '\0';               // replace end quote with null
        }
        if(*cmd_string == '\0'){
            return i;                           // end of cmd string, exit
        }
        if(!whitespace(*cmd_string)){
            args[i++] = cmd_string++;			// when non-whitespace encountered, assign arg[] pointer
            if(i > ARG_MAX){
                return i;						// until all args used up
            }
            do{
                if(*cmd_string == '\0'){
                    return i;                   // end of cmd string, exit
                }
                if(whitespace(*cmd_string)){
                    *cmd_string = '\0';			// then look for next whitespace and delimit (terminate) the arg[] string
                    break;
                }
                cmd_string++;					// loop until end of cmd_string or next whitespace
            } while (1);
        }
        cmd_string++;							// loop...
    } while (1);
    // no return here, we'll never get here anyway...
}

//=============================================================================
// parse_ehex() for embeded hex ($$) arguments
//  on exit, the string holds the original text with %xx replaced by a single
//	hex byte.
//=============================================================================
void parse_ehex(char * sptr){
	char* tptr;
	U8	i;

	while(*sptr){
		if((*sptr == '$') && (*(sptr+1) == '$')){
			i = asc_hex(*(sptr+2)) << 4;
			i |= asc_hex(*(sptr+3));
			*sptr++ = i;
			tptr = sptr;
			do{
				*tptr = *(tptr+3);
				tptr++;
			}while(*(tptr+2));
		}else{
			sptr++;
		}
	}
}
//=============================================================================
// test characer for whitespace
//=============================================================================
int whitespace(char c){

    switch (c){					// These are all valid whitespace:
        case '\n':          	// newline
        case '\r':          	// cr
        case '\t':          	// tab
        case 0x20:{         	// space
		case '/':				// slash is also wsp
            return TRUE;
        }
    }
    return FALSE;
}

//=============================================================================
// test characer for quote
//=============================================================================
int quotespace(char c, char qu_c){

    if(qu_c == '\0'){
        switch (c){				// if qu_c is null, these are valid quotes:
            case '\'':          // single
            case '\"':          // double
            case '\t':          // tab
                return TRUE;
            }
    } else {
        if(c == qu_c){			// else, only qu_c results in a TRUE match
            return TRUE;
        }
    }
    return FALSE;
}

//=============================================================================
// gas_gage() display up to 16 "*" chrs based on count rate.
//	Gauge appearance:
//	[****************]	all OK
//	[***.............]	errors detected
//
//	"len" cmds:
//	0: process gauge counter/display
//	1: set gauge error character = "."
//	2: disable gage counter/display (set clen = 0)
//	all others: set creset = count = len/16, display initial gauge characters
//	This calculation identifies how many bytes are in 1/16th of the total
//	byte count (len).  For count events (len == 0), this Fn decrements count, &
//	displays a gauge chr when count == 0.  count is then reloaded with creset.
//	process continues until 16 gauge chrs have been displayed.  After this,
//	any further count events result in no further change to the display.
//=============================================================================
U8 gas_gage(U16 len){

#define LENCMD_MAX 2		// max # of gas-gage() cmds

	static U16	creset;		// holding reg for data counter reset value
	static U16	count;		// data counter
	static U8	clen;		// gage chr counter
	static U8	gchr;		// gage chr storage
		   U8	c = 0;		// gage printed flag

	if(len <= LENCMD_MAX){
		if(!len && clen){
			if(--count == 0){ 
				putchar(gchr);					// disp gage chr
				count = creset;					// reset loop counters
				clen--;
				if(clen == 0) putchar(']');		// if end of gage, print end bracket
				c = 1;
			}
		}else{
			if(len == 1) gchr = '.';			// if error flag, change gauge chr to err mode
			if(len == 2) clen = 0;				// disable gauge
		}
	}else{
		creset = count = len >> 4;				// init count & count reset (creset) = len/16
		if(creset == 0) creset = 1;				// if overall length too short, set 1:1
		clen = 16;								// 16 gage chrs max
		gchr = '*';								// set * as gage chr
		putchar('[');							// print start bracket
		for(c = 0; c < 16; c++) putchar(' ');
		putchar(']');							// place end bracket for scale
		for(c = 0; c < 17; c++) putchar('\b');	// backspace to start of scale
		c = 1;
	}
	return c;
}

//=============================================================================
// log_error_byte() places error data into log buffer.  Log format is:
//	(device) (host) (addrH) (addrL).  Called by target verify fns to allow
//	a limited number of errors to be trapped (limit is the buffer used to
//	hold the error log).
//	returns updated pointer to next available log entry
//=============================================================================
U8* log_error_byte(U8* lbuf, U8 d, U8 h, U16 a){

	*lbuf++ = d;								// store device data
	*lbuf++ = h;								// store host data
	*lbuf++ = (U8)(a >> 8);						// store addr
	*lbuf++ = (U8)(a & 0xff);
	return lbuf;								// return updated pointer
}

//=============================================================================
// disp_error_log() displays errors logged into error string.  Log format is:
//	(device) (host) (addrH) (addrL)
//	Display format is:
//	nn: Dev ($xx) != $xx @$xxxx\n = 28 printed chrs
//	nn = err number (ordinal)
//	xx = data bytes
//	xxxx = error address
//=============================================================================
void disp_error_log(U8* lbuf, U16 len){

	char obuf[32];				// local buffer
	// use U16 type to simplify sprintf variable list
	U16  i;						// loop counter
	U16  d;						// device data
	U16  h;						// host data
	U16  a;						// addr

	len++;										// add 1 to end so that we can start loop at "1"
	for(i = 1; i < len; i++){					// loop from 1 to len+1 entries
		d = (U16)*lbuf++ & 0xff;				// format device data
		h = (U16)*lbuf++ & 0xff;				// format host data
		a = ((U16)*lbuf++ & 0xff) << 8;			// format addr
		a |= (U16)*lbuf++ & 0xff;
		sprintf(obuf,"%02u: Dev ($%02x) != $%02x @$%04x", i, d, h, a); // display err line
		putsQ(obuf);
	}
}

//=============================================================================
// bcmd_resp_init() inits bcmd_resp_ptr
//=============================================================================
void bcmd_resp_init(void){

	bcmd_resp_ptr = bcmd_resp_buf;
	*bcmd_resp_ptr = '\0';
}

//=============================================================================
// asc_hex() converts ascii chr to 4-bit hex.  Returns 0xff if error
//=============================================================================
U8 asc_hex(S8 c){

	U8 i;

	if((c >= 'a') && (c <= 'f')) c = c - ('a' - 'A');	// upcase
	if((c >= '0') && (c <= '9')){			// if decimal digit,
		i = (U8)(c - '0');					// subtract ASCII '0' to get hex nybble
	}else{
		if((c >= 'A') && (c <= 'F')){		// if hex digit,
			i = (U8)(c - 'A' + 0x0A);		// subtract ASCII 'A', then add 0x0A to get hex nybble
		}else{
			i = 0xff;						// if not valid hex digit, set error return
		}
	}
	return i;	
}

//=============================================================================
// temp_float() converts MCP9800 binary temp to a float (degrees C)
//=============================================================================
float temp_float(U16 k){
	U8		i;			// temp
	U8		j = 0;		// temp sign
	float	fa;			// temp float

	if(k & 0x8000){												// if negative,
		j = 1;													// preserve sign and
		k = ~k + 1;												// convert value to positive
	}
	i = k >> 8;													// get integer portion
	fa = (float)i;												// convert to float
	if(k & 0x0080) fa += 0.5;									// add fractional portion
	if(k & 0x0040) fa += 0.25;
	if(k & 0x0020) fa += 0.125;
	if(k & 0x0010) fa += 0.0625;
	if(j){														// if negative, convert
		fa *= -1;
	}
	return fa;
}

// eof
