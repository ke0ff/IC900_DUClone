// OpenSCAD 2019.05
// LCD Case - enclosure, REV-001
// Joe Haas, KE0FF, 10/19/2023
// This is the IC900 controller clone enclosure which uses an NHD-240128WG LCD module
//	and custom button PCB
//
// Rev-001, 12/25/2023
//	initial code
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
// User defined parameters.  Modify these to suit a particular application
// NOTE: All data in this file is in mm
//----------------------------------------------------------------------------------------------------------------------
// parametric variables:

// LCD screen panel thickness
scrnthk = 0.08*25.4;

// hole expansion to compensate 3D print overage
dskew = 0.3;
// interface PCB
pcbz = 14.3-scrnthk+.6;
pcbthk = 1.6;
// LCD mtg hole baselines
lbx = 4+162;
lbdx = 6.38 * 25.4;
lby = (95-4.5-1.5)-85;
lbdy = 3.35 * 25.4;

wthick = 3.5;
wthickt = 1.5;

bswid = 57;
bslen = 95;
bsoffs = 49;
bsoffsy = 41;

bsz = 30;
splitwid = 171 + (2*wthick);
width = 171 + (4*wthick)+bswid;
height = 14+13 + (wthickt) +2;
length = 95 + (2*wthick);
fht_in = 30;					// front inside depth
bheight = wthick;

bossht = 7.8;
crad = 5;
crad2 = crad/2;
chamfer = .8;
lcdz = height-14+4.5; //13+4.5;
scham = 4;

num1_thru = (.09*25.4);
num1_thrd = (.05*25.4)+.3;

// buttons
bwid = 7;				// y
blen = 12;				// x
bht = 6;				// z
bclr = .5;				// clearance
bapron = 2;				// apron clearance around periphery of button base
capron = bapron;		// raw apron around periphery of button base
bapht = 2;				// apron thickness
bnib = 2+.6;			// nib thickness
bcham = 3;				// corner radius
bsep = 1.5;
bpitchx = blen+bsep+(2*bapron);
bpitchy = bwid+bsep+(2*bapron);
bqwid = 25;				// y
bqlen = 6.5;			// x
brelief = 3;			// button protrusion
brocker = 1;			// rocker button lengthwise relief

dialr = (6.5)/2;		// dial thread rad
dialnutr = 12.6/2;		// dial nut
nthick = 2.5;			// dial mounting surface thickness
opvoidr = 25.4/2;
opv_deep = 4;			// sets depth of dial optical void
dial_deep = 10;			// vertical placement of dial mounting plane
shaft_len = 20;			// dial shaft length

hole1 = .06*25.4;

capronx = blen+(2*(bapron+bclr));
caprony = bwid+(2*(bapron+bclr));
capcham = 3;
stcham = 2.5;
stht = 6.4+bapht+.6+.6;

piezx = 85;
piezo_dia = 34.8+.5;
piezo_id = 26;
piezo_ht = 8;

// light dome
domex = 3.5+wthick+12.55;
domey = 3.5+wthick+26.44;
domedia = 25.4;

// contrast pot knob
cpotx = 3.5+wthick+149.9;
cpoty = 3.5+wthick+.7;
pknobdia = 14;
pknobht = 6;

echo("cpotx", cpotx);

subrx = 198.6;
subry = 68.2;
trledia = 4.32;				// no larger than 4mm dia here !!! 4.32mm = 11/64 clearance drill (4.366mm)

///////////////////////////////////////////////////////////////////////////////
// plot directive processing //////////////////////////////////////////////////
		
button = 0;			// button caps, 1 = normal, 2 = short (power)
vqbtn = 0;			// v/q button
dopivot = 0;		// v/q pivot wedge
sidesel = 0;		// main case, 1 = lcd side, 2 = button side, 3 = both
backsel = 0;		// back cover, 1 = lcd side, 2 = button side, 3 = both
dopcb = 0;			// display PCB for fit-check
doeclip = 0;		// back-side clip piece
dopiezo = 0;		// piezo cover
dordu = 0;			// RDU PCB for fit-check
dosunwin = 0;		// sunlight window
dopot = 1;			// pot knob

//////////////////////

//mainpcb();

// ruler
//translate([183,22,0]) cylinder(r=3, h=12.3, $fn=8);

if(button==1){
	cap(model=1);
}
if(button==2){
	cap(model=1, capht=4);
}

if(vqbtn){
	vq();
}

if(sidesel){
	caset(side=sidesel);
}

if(dopcb){
	difference(){
		mainpcb();
//		translate([175,-10,-10]) cube([200,150, 40]);		// debug section
	}
}
//pbsw();


if(dordu){
	rdu();
}

if(doeclip){
	edgeclip();
}

if((backsel==1)||(backsel==3)){
	difference(){
		back1();
//		translate([82,-10,-10]) cube([200,150, 40]);		// debug section
	}
}

if((backsel==2)||(backsel==3)){
	back2();
}


if(dopiezo){
	translate([piezx,77,2*wthick]) rotate([180,0,0]) piezo(model = 1, depth=3*wthick, film = 1);
}

if(dopivot){
	pivot();
}

if(dosunwin){
	sunwin();
}

if(dopot){
	cpknob(model=1);
}

///////////////////////////////////////////////////////////////////////////////

// test instantiations

//deskstand();

// spacer
//cube([3.2,6,4]);

//translate([cpotx,cpoty,pcbz]) cpknob(model = 1);

		
//translate([105,50,2*wthick]) rotate([180,0,0]) piezo(model = 1, depth=3*wthick, film = 1);

//backlight(model = 0);

//hdr20(model=0);

//translate([3.5,3.5,lcdz-scrnthk-.6])
//lcdpcb();


//translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) dial(model=1);

//spilot();
//translate([width-(bsoffs),length-bsoffsy,height]) cap(model=1);
//translate([width-(bsoffs)-3,length-19,height]) vq(model=1);

//lcd();
//lid();

//bcaps();

//dial();
//translate([210,12,15.25]) cylinder(r1=2, r2=0, h=12, $fn=16);

// ruler
//translate([160,0,13.4-5.4]) cylinder(r=4, h=17.1+5.4, $fn=16);

////////////////////////////////////////////////////////////////////////////////////
module mainpcb(mode=0){

pgmz = .2*25.4;
jlcdz = 8.9;
rj45z = 13.2;

	translate([10-5.8,3.5,pcbz]){
		if(mode==0){
		// RJ45 test fit
//			translate([0,0,-(13.5)]) cube([10,10,13.5]);
		}
		// pgm conn
		color("white") translate([116,63,-pgmz]) cube([7.4,14.6,pgmz]);
		// lcd conn
		color("brown") translate([164.5-3.5,22.5-4.2,-jlcdz]) cube([7.4,23.1,jlcdz]);
		// rj45 conn
		color("white") translate([14.25,77.8,-rj45z]) cube([15.8,15.8,rj45z]);

		difference(){
			// pcb substrate
			color("green")cube([160+74,94.5,1.6]);
			translate([width-(bsoffs)+bpitchx-4.2,length-17-3.5,-10]) cylinder(r=31/2, h=30, $fn=64, center=false);
			translate([width-(bsoffs)+bpitchx-4.2,length-17-3.5+(31/2),0]) cube([31,31,10],center=true);
			translate([174,4,0]) cube([11,17,10],center=true);
			translate([174,92,0]) cube([11,17,10],center=true);
			// 20p header
			translate([170-5.8-.7,18.5,0]) hdr20(model=0);
			// backlight -
			translate([170-2.7-.7,58,0]) backlight(model=0);
			translate([-.7,0,0]) lcd(mtg=true);
			if(mode==0){
				translate([-10+5.8,-3.5,-10.95-14.3]){
					// button spacers
					translate([width-(bsoffs),length-bsoffsy,height]) pbsw(model=mode);
					translate([width-(bsoffs)+bpitchx,length-bsoffsy,height]) pbsw(model=mode);
					translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) pbsw(model=mode);
					
					translate([width-(bsoffs),length-bsoffsy-bpitchy,height]) pbsw(model=mode);
					translate([width-(bsoffs)+bpitchx,length-bsoffsy-bpitchy,height]) pbsw(model=mode);
					translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-bpitchy,height]) pbsw(model=mode);
					
					translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) pbsw(model=mode);
					translate([width-(bsoffs)+bpitchx,length-bsoffsy-(2*bpitchy),height]) pbsw(model=mode);
					translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(2*bpitchy),height]) pbsw(model=mode);
					
					translate([width-(bsoffs),length-bsoffsy-(3*bpitchy),height]) pbsw(model=mode);
					translate([width-(bsoffs)+bpitchx,length-bsoffsy-(3*bpitchy),height]) pbsw(model=mode);
					translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(3*bpitchy),height]) pbsw(model=mode);
					
					translate([width-(bsoffs),length-bsoffsy-(4*bpitchy),height]) pbsw(model=mode);
					translate([width-(bsoffs)+bpitchx,length-bsoffsy-(4*bpitchy),height]) pbsw(model=mode);
					translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(4*bpitchy),height]) pbsw(model=mode);
				
					// v/q toggles
					bqswoffs = (bqwid/2)-1;
					translate([width-(bsoffs)-3,length-19-bqswoffs,height]) pbsw(model=mode);
					translate([width-(bsoffs)-3,length-19+bqswoffs,height]) pbsw(model=mode);
					translate([width-11,length-19-bqswoffs,height]) pbsw(model=mode);
					translate([width-11,length-19+bqswoffs,height]) pbsw(model=mode);
				}
			}
			// spacer screw holes
			translate([width-(bsoffs)-4.2,length-bsoffsy-3.5,15]) cap(model=12);
	//		translate([width-(bsoffs)-4.2,length-bsoffsy-3.5,15]) cap(model=13);
			
			translate([width-(bsoffs)+(2*bpitchx)-4.2,length-bsoffsy-3.5,15]) cap(model=10);
			translate([width-(bsoffs)+(2*bpitchx)-4.2,length-bsoffsy-3.5,15]) cap(model=13);
	
			translate([width-(bsoffs)-4.2,length-bsoffsy-(2*bpitchy)-3.5,15]) cap(model=10);
	
//			translate([width-(bsoffs)-4.2,length-bsoffsy-(2*bpitchy)-3.5,15]) cap(model=12);
			translate([width-(bsoffs)+(2*bpitchx)-4.2,length-bsoffsy-(2*bpitchy)-3.5,15]) cap(model=10);	
			translate([width-(bsoffs)+(2*bpitchx)-4.2,length-bsoffsy-(4*bpitchy)-3.5,15]) cap(model=11);
	
			translate([width-(bsoffs)-4.2,length-bsoffsy-(4*bpitchy)-3.5,15]) cap(model=10);

			// debug hog-off
			translate([200,-.1,-.1]) cube([splitwid+.1,length+5,height+5]);
		}
	}
	if(mode==1){
		translate([0,0,-10.95]){
			// button spacers
			translate([width-(bsoffs),length-bsoffsy,height]) pbsw(model=mode);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy,height]) pbsw(model=mode);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) pbsw(model=mode);
			
			translate([width-(bsoffs),length-bsoffsy-bpitchy,height]) pbsw(model=mode);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-bpitchy,height]) pbsw(model=mode);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-bpitchy,height]) pbsw(model=mode);
			
			translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) pbsw(model=mode);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-(2*bpitchy),height]) pbsw(model=mode);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(2*bpitchy),height]) pbsw(model=mode);
			
			translate([width-(bsoffs),length-bsoffsy-(3*bpitchy),height]) pbsw(model=mode);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-(3*bpitchy),height]) pbsw(model=mode);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(3*bpitchy),height]) pbsw(model=mode);
			
			translate([width-(bsoffs),length-bsoffsy-(4*bpitchy),height]) pbsw(model=mode);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-(4*bpitchy),height]) pbsw(model=mode);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(4*bpitchy),height]) pbsw(model=mode);
		
			// v/q toggles
			bqswoffs = (bqwid/2)-1;
			translate([width-(bsoffs)-3,length-19-bqswoffs,height]) pbsw(model=mode);
			translate([width-(bsoffs)-3,length-19+bqswoffs,height]) pbsw(model=mode);
			translate([width-11,length-19-bqswoffs,height]) pbsw(model=mode);
			translate([width-11,length-19+bqswoffs,height]) pbsw(model=mode);
		}
	}
}

module bcaps(mode=1){
	translate([0,0,0]){
		// button caps
		translate([width-(bsoffs),length-bsoffsy,height]) cap(model=mode, colr=2);
		translate([width-(bsoffs)+bpitchx,length-bsoffsy,height]) cap(model=mode);
		translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) cap(model=mode);
		
		translate([width-(bsoffs),length-bsoffsy-bpitchy,height]) cap(model=mode);
		translate([width-(bsoffs)+bpitchx,length-bsoffsy-bpitchy,height]) cap(model=mode);
		translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-bpitchy,height]) cap(model=mode);
		
		translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) cap(model=mode);
		translate([width-(bsoffs)+bpitchx,length-bsoffsy-(2*bpitchy),height]) cap(model=mode);
		translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(2*bpitchy),height]) cap(model=mode);
		
		translate([width-(bsoffs),length-bsoffsy-(3*bpitchy),height]) cap(model=mode);
		translate([width-(bsoffs)+bpitchx,length-bsoffsy-(3*bpitchy),height]) cap(model=mode);
		translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(3*bpitchy),height]) cap(model=mode);
		
		translate([width-(bsoffs),length-bsoffsy-(4*bpitchy),height]) cap(model=mode);
		translate([width-(bsoffs)+bpitchx,length-bsoffsy-(4*bpitchy),height]) cap(model=mode);
		translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(4*bpitchy),height]) cap(model=mode);
	
		// v/q toggles
		translate([width-(bsoffs)-3,length-19,height]) vq(model=mode);
		translate([width-11,length-19,height]) vq(model=mode);
	}
}
////////////////////////////////////////////////////////////////////////////////


//
//
//**************************************************
				//****************\\
				//    modules     \\
				//    follow      \\
				//****************\\
//**************************************************
//
//
///////////////
// top shell

module caset(side=3){
	if((side==1)||(side==3)){
		translate([166,20,23.3]) rotate([0,0,270]) rotate([0,180,0]) linear_extrude(1.2) text("KE0FF V1.0", size=6);					// version & PN texts
	}
	difference(){
		union(){
			difference(){
				union(){
					cube([width,length,height]);
//					translate([19.1,88,0]) cube([2,2,23.6]); // test-ruler
				}
				cube([crad,crad,3*height], center=true);										// corner notch
				translate([width,0,0]) cube([crad,crad,3*height], center=true);				// corner notch
				translate([0,length,0]) cube([crad,crad,3*height], center=true);				// corner notch
				translate([width,length,0]) cube([crad,crad,3*height], center=true);			// corner notch
//				translate([3.5+6.5,3.5,4.5-2.5]) cube([60.5-13,80.2,5]);				// "Y" under PCB hog-out
//				translate([3.5,3.5+6+3,4.5-2.5]) cube([60.5,80.2-12-6,5]);				// "X" under PCB hog-out
			}
			if((side==1)||(side==3)){
				translate([crad2,crad2,0]) corner(height);								// corner radius (x4)
				translate([crad2,length-crad2,0]) corner(height);
			}
			translate([width-crad2,crad2,0]) corner(height);
			translate([width-crad2,length-crad2,0]) corner(height);
		}

//		translate([15,25,1.5]) linear_extrude(1) text("TivaLP bot", size=6);

		if((side==2)||(side==3)){
			// edge clip receivers
			ec_placed();
			// t/r led pipes
			translate([subrx, subry+2, height]) trled(model=0);
			translate([subrx+22.6+1.2, subry+2, height]) trled(model=0);
			// dim/lock button
			translate([width,42.3-24.9,pcbz+.53-5.5]) cube([20,4.5,4.5], center=true);
			translate([width-.9,42.3-24.9,pcbz+.53-5.5]) rotate([0,270,0]) rotate([0,0,45]) cylinder(r1=7, r2=0, h=2, $fn=4, center=true);
			// groove for sw stud
			translate([width-wthick-.5,42.3-24.9,pcbz+.53-5.5-4]) cube([4.5,4.5,10], center=true);
		}
		// bottom edge chams
		translate([0,chamfer,-.01]) rotate([135,0,0]) cube([width+10,5,5]);
		translate([0,length-chamfer,-.01]) rotate([-45,0,0]) cube([width+10,5,5]);
		if((side==1)||(side==3)){
			translate([chamfer,0,-.01]) rotate([0,360-135,0]) cube([5,length+10,5]);
		}
		translate([width-chamfer,0,-.01]) rotate([0,45,0]) cube([5,length+10,5]);
		// top edge chams
		translate([0,chamfer,height]) rotate([135,0,0]) cube([width+10,5,5]);
		translate([0,length-chamfer,height]) rotate([-45,0,0]) cube([width+10,5,5]);
		if((side==1)||(side==3)){
			translate([chamfer,0,height]) rotate([0,360-135,0]) cube([5,length+10,5]);
		}
		translate([width-chamfer,0,height]) rotate([0,45,0]) cube([5,length+10,5]);
		// split-chams
		translate([splitwid,-1,height-sqrt(scham)]) rotate([0,-45,0]) cube([scham,length+10,scham]);
		translate([splitwid,sqrt(scham),-1]) rotate([90,0,0]) rotate([0,-45,0]) cube([scham,length+10,scham]);
		translate([splitwid,length-sqrt(scham),-1]) rotate([90,0,0]) rotate([0,180-45,0]) cube([scham,length+10,scham]);

		if((side==1)||(side==3)){
			// main hog-out
			translate([3.5,3.5,lcdz]) lcd();
		}
		// split mtg holes, #2
		translate([splitwid,8,5]) spilot(csdeep=4.6);
		translate([splitwid,length-8,5]) spilot(csdeep=4.6);

		translate([splitwid,length-15,height-4.8]) spilot(csdeep=13.1-4.5, size=2);
		translate([splitwid,(15+19.5)/2,height-4.8]) spilot(csdeep=13.1-4.5);
		translate([splitwid,17.2+((length-26)/3),height-4.8]) spilot(csdeep=13.1-4.5);
		translate([splitwid,15.2+(2*(length-26)/3),height-4.8]) spilot(csdeep=13.1-4.5);
		
		// split line, pcb hog-out
		translate([splitwid-10,14,-.1]) cube([20,length-28,18]);

		// side 1 hog-off
		if((side != 1) && (side !=3)){
			translate([-.1,-.1,-.1]) cube([splitwid+.1,length+5,height+5]);
		}
		if((side != 2) && (side !=3)){
			// side 2 hog-off
			translate([splitwid,-.1,-.1]) cube([splitwid+.1,length+5,height+5]);
		}
		// RJ-45 opening
		translate([lbx+3.5-(.645*25.4)-((5.3+.04)*25.4)+1.3,lby+lbdy,pcbz+pcbthk+(-.511*25.4)+((.48-.425)*25.4)-(scrnthk/2)-(.08*25.4)]) cube([.587*25.4,30,.425*25.4]);
		// contrast pot knob opening
		translate([cpotx,cpoty,pcbz-5.4]) cpknob();
		translate([cpotx,cpoty,pcbz-5.4-4]) cube([10.5,30,10], center=true);
		
		// debug hog-off
//		translate([100,-.1,-.1]) cube([splitwid+.1,length+5,height+5]);

	////// dial/button artifacts //////

		if((side==2)||(side==3)){
			// dial
			translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) dial(model=0);
			translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) rotate([0,0,8]) dial(model=0);
			translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) rotate([0,0,-8]) dial(model=0);
			// button hog-out
			difference(){
				// button chamber
				translate([width-bswid-(wthick)+1,wthick,height-bsz-wthickt-bht+1.5]) cube([bswid-.5, bslen, bsz]);
				// dial pedestal
				translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) cylinder(r=29/2, h=30, $fn=64, center=false);
			}
			// buttons
			translate([width-(bsoffs),length-bsoffsy,height]) cap(model=0);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy,height]) cap(model=0);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) cap(model=0);
	
			translate([width-(bsoffs),length-bsoffsy-bpitchy,height]) cap(model=0);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-bpitchy,height]) cap(model=0);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-bpitchy,height]) cap(model=0);
	
			translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) cap(model=0);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-(2*bpitchy),height]) cap(model=0);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(2*bpitchy),height]) cap(model=0);
	
			translate([width-(bsoffs),length-bsoffsy-(3*bpitchy),height]) cap(model=0);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-(3*bpitchy),height]) cap(model=0);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(3*bpitchy),height]) cap(model=0);
	
			translate([width-(bsoffs),length-bsoffsy-(4*bpitchy),height]) cap(model=0);
			translate([width-(bsoffs)+bpitchx,length-bsoffsy-(4*bpitchy),height]) cap(model=0);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(4*bpitchy),height]) cap(model=0);
	
			// v/q toggles
			translate([width-(bsoffs)-3,length-19,height]) vq(model=0);
			translate([width-11,length-19,height]) vq(model=0);
		}
	}
	if((side==2) || (side==3)){
		translate([0,0,-scrnthk]) difference(){
			union(){
				// button spacers
				translate([width-(bsoffs),length-bsoffsy,height]) cap(model=2);
				translate([width-(bsoffs)+bpitchx,length-bsoffsy,height]) cap(model=2);
				translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) cap(model=2);
			
				translate([width-(bsoffs),length-bsoffsy-bpitchy,height]) cap(model=2);
				translate([width-(bsoffs)+bpitchx,length-bsoffsy-bpitchy,height]) cap(model=2);
				translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-bpitchy,height]) cap(model=2);
			
				translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) cap(model=2);
				translate([width-(bsoffs)+bpitchx,length-bsoffsy-(2*bpitchy),height]) cap(model=2);
				translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(2*bpitchy),height]) cap(model=2);
			
				translate([width-(bsoffs),length-bsoffsy-(3*bpitchy),height]) cap(model=2);
				translate([width-(bsoffs)+bpitchx,length-bsoffsy-(3*bpitchy),height]) cap(model=2);
				translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(3*bpitchy),height]) cap(model=2);
			
				translate([width-(bsoffs),length-bsoffsy-(4*bpitchy),height]) cap(model=2);
				translate([width-(bsoffs)+bpitchx,length-bsoffsy-(4*bpitchy),height]) cap(model=2);
				translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(4*bpitchy),height]) cap(model=2);

				// v/q spacers
				translate([width-(bsoffs)-3,length+2.2,height]){
					translate([-(capronx/2)+2.5,-(caprony/2),-bht+(bapht/2)-stht-.465]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+3]);
					translate([-(capronx/2)+14.5,-(caprony/2),-bht+(bapht/2)-stht-.465]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+3]);
				}
				translate([width-(bsoffs)+38,length+2.2,height]){
					translate([-(capronx/2)+2.5,-(caprony/2),-bht+(bapht/2)-stht-.465]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+3]);
					translate([-(capronx/2)+14.5,-(caprony/2),-bht+(bapht/2)-stht-.465]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+3]);
				}
			}
			// spacer screw holes
			translate([width-(bsoffs),length-bsoffsy,height]) cap(model=12);
	//		translate([width-(bsoffs),length-bsoffsy,height]) cap(model=13);
			
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) cap(model=10);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy,height]) cap(model=13);
	
			translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) cap(model=10);
	
			translate([width-(bsoffs),length-bsoffsy-(2*bpitchy),height]) cap(model=12);
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(2*bpitchy),height]) cap(model=10);
	
			translate([width-(bsoffs)+(2*bpitchx),length-bsoffsy-(4*bpitchy),height]) cap(model=11);
	
			translate([width-(bsoffs),length-bsoffsy-(4*bpitchy),height]) cap(model=10);
		}
	}
}
/////////////////////////////////////////////// BACK1 ///////////////////////////////////////////////////////////////////////////

module back1(){
cenx = 100;
ceny = length/2;
tabx = (142+178)/2;
taby = length/2;

	difference(){
		translate([0,0,-bheight]){
			difference(){
				// inside panel
				translate([(wthick+.5),(wthick+.5),bheight-.01]) cube([splitwid-(2*wthick)-.5-2,length-(2*wthick)-1,bheight]);
				// corner chams
				translate([(wthick+.5),(wthick+.5),bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				translate([(wthick+.5),(wthick+.5)+length-(2*wthick)-1,bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				translate([(wthick+.5)+splitwid-(2*wthick)-.5-2,(wthick+.5),bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				translate([(wthick+.5)+splitwid-(2*wthick)-.5-2,(wthick+.5)+length-(2*wthick)-1,bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				// side2 notch
				translate([(wthick+.5)+splitwid-(2*wthick)-.5-2,length/2,bheight+wthick]) cube([60,60,10], center=true);
				// thru mtg
				translate([10-5.8-.7,3.5,0]){
					translate([lbx,lby,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby,0]) stoff(delz=wthick);
					translate([lbx,lby+lbdy,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby+lbdy,0]) stoff(delz=wthick);
				}
			}
			difference(){
				// outside panel
				union(){
					// contrast pot
					translate([cpotx,wthick/2,pcbz-5.4-1]) cube([10.25,wthick,6], center=true);
					cube([splitwid,length,bheight]);
					// standoff bodies
					translate([10-5.8-.7,3.5,0]){
						translate([lbx,lby,0]) stoff(delz=wthick, model=1);
						translate([lbx-lbdx,lby,0]) stoff(delz=wthick, model=1);
						translate([lbx,lby+lbdy,0]) stoff(delz=wthick, model=1);
						translate([lbx-lbdx,lby+lbdy,0]) stoff(delz=wthick, model=1);
					}
				}
				// corner notches and radii
				cube([crad,crad,3*height], center=true);										// corner notch
				translate([0,length,0]) cube([crad,crad,3*height], center=true);				// corner notch
				translate([10-5.8-.7,3.5,0]){
					translate([lbx,lby,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby,0]) stoff(delz=wthick);
					translate([lbx,lby+lbdy,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby+lbdy,0]) stoff(delz=wthick);
				}
			}
		}
		// debug hog-off
//		translate([100,-.1,-(height+5)/2]) cube([splitwid+.1,length+5,height+5]);

		// contrast pot
		translate([cpotx,cpoty,pcbz-5.4]) cpknob();
//		translate([cpotx,cpoty,pcbz-5.4-4]) cube([10.5,30,10], center=true);

		// light dome
		translate([domex,domey,0]) ltdome();
		// light dome mtg
		translate([domex,domey+15,-2]) cylinder(r=num1_thrd/2, h=7, $fn=16);
		translate([domex,domey-15,-2]) cylinder(r=num1_thrd/2, h=7, $fn=16);
		// piezo spkr
		translate([piezx,77,bheight+.25]) piezo(model = 0, depth=2*wthick-1, film = 1);
//		translate([138,77,2*wthick]) rotate([180,0,0]) piezo(model = 1, depth=3*wthick, film = 1);
		// RDU pcb hogouts
		rduho();
		// RJ-45 opening
		translate([lbx+3.5-(.645*25.4)-((5.3+.04)*25.4)+1.3-((.67-.587)*25.4/2),lby+lbdy+12.5-21,-.05*25.4]) cube([.67*25.4,30,.4*25.4]);
		// piezo conn hogout
		translate([100,29,-bheight+1]) cube([16,22.5,10]);
		// base mounting pattern
		translate([cenx-(70.2/2), ceny-(30.1/2),0]) cylinder(r=3.3/2, h=20, $fn=16, center=true);
		translate([cenx-(70.2/2), ceny+(30.1/2),0]) cylinder(r=3.3/2, h=20, $fn=16, center=true);
		translate([cenx+(70.2/2), ceny-(30.1/2),0]) cylinder(r=3.3/2, h=20, $fn=16, center=true);
		translate([cenx+(70.2/2), ceny+(30.1/2),0]) cylinder(r=3.3/2, h=20, $fn=16, center=true);
		// side 2 tab mtg
		translate([tabx-(25/2), taby-(50/2),-bheight]) stoff(delz=wthick);
		translate([tabx+(25/2), taby-(50/2),-bheight]) stoff(delz=wthick);
		translate([tabx-(25/2), taby+(50/2),-bheight]) stoff(delz=wthick);
		translate([tabx+(25/2), taby+(50/2),-bheight]) stoff(delz=wthick);
		// bottom edge chams
		translate([0,chamfer,-.01]) rotate([135,0,0]) cube([width+10,5,5]);
		translate([0,length-chamfer,-.01]) rotate([-45,0,0]) cube([width+10,5,5]);
		translate([chamfer,0,-.01]) rotate([0,360-135,0]) cube([5,length+10,5]);
		// top edge chams
		translate([0,chamfer,.01-bheight]) rotate([135,0,0]) cube([width+10,5,5]);
		translate([0,length-chamfer,.01-bheight]) rotate([-45,0,0]) cube([width+10,5,5]);
		translate([chamfer,0,.01-bheight]) rotate([0,360-135,0]) cube([5,length+10,5]);
	}
	// corner radius (x3)
	translate([crad2,crad2,-bheight]) cornerb(bheight);
	translate([crad2,length-crad2,-bheight]) cornerb(bheight);
	// nomenclature
	translate([40,18,bheight-.01]) linear_extrude(1) text("IC-900F RCU", size=10);
	translate([40,10,bheight-.01]) linear_extrude(1) text("REV-   KE0FF   FEB-2024", size=6);
}

/////////////////////////////////////// BACK2 ///////////////////////////////////////////////////////////////////////////
module back2(){
cenx = 100;
ceny = length/2;
tabx = (142+178)/2;
taby = length/2;
pgmz = .2*25.4;
jlcdz = 8.9;

	difference(){
		translate([0,0,-bheight]){
			difference(){
				union(){
					// inside panel
					translate([(wthick+1.5)+splitwid,(wthick+.5),bheight-.01]) cube([splitwid-80-40-wthick+.5,length-(2*wthick)-1,bheight]);
					// side2 tab/notch
					translate([(wthick+.5)+splitwid-(2*wthick)-.5-2,length/2,bheight-.01+(wthick/2)]) cube([58,58,wthick], center=true);
				}
				// corner chams
				translate([(wthick+.5),(wthick+.5),bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				translate([(wthick+.5),(wthick+.5)+length-(2*wthick)-.5,bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				translate([(wthick+.5)+splitwid-(2*wthick)-.5-2,(wthick+.5),bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				translate([(wthick+.5)+splitwid-(2*wthick)-.5-2,(wthick+.5)+length-(2*wthick)-.5,bheight+wthick]) cylinder(r=1, h=10, $fn=4, center=true);
				// thru mtg
				translate([10-5.8-.7,3.5,0]){
					translate([lbx,lby,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby,0]) stoff(delz=wthick);
					translate([lbx,lby+lbdy,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby+lbdy,0]) stoff(delz=wthick);
				}
			}
			difference(){
				// outside panel
				union(){
					translate([splitwid+.25,0,0]) cube([width-splitwid-.25,length,bheight]);
					// button spacers
					translate([0,0,bheight]){
						translate([splitwid+6.5,length-bsoffsy+6,-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+6.5,length-bsoffsy+6-(2*bpitchy),-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+6.5,length-bsoffsy+6-(4*bpitchy),-bheight+.01]) bstoff(delz=5.1-1.6);
					
						translate([splitwid+6.25+(2*bpitchx),length-bsoffsy+6,-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+6.25+(2*bpitchx),length-bsoffsy+6-(2*bpitchy),-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+6.25+(2*bpitchx),length-bsoffsy+6-(4*bpitchy),-bheight+.01]) bstoff(delz=5.1-1.6);
					
						translate([splitwid+5.8+(3*bpitchx),length-bsoffsy+6-(1*bpitchy),-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+5.8+(3*bpitchx),length-bsoffsy+6-(3*bpitchy),-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+5.8+(3*bpitchx),length-bsoffsy+6-(5*bpitchy)+1.25,-bheight+.01]) bstoff(delz=5.1-1.6);

						translate([splitwid+12,length-7,-bheight+.01]) bstoff(delz=5.1-1.6);
						translate([splitwid+52.8,length-7,-bheight+.01]) bstoff(delz=5.1-1.6);
					}
				}
				// corner notches and radii
				translate([width,0,0]) cube([crad,crad,3*height], center=true);										// corner notch
				translate([width,length,0]) cube([crad,crad,3*height], center=true);				// corner notch
				translate([10-5.8-.7,3.5,0]){
					translate([lbx,lby,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby,0]) stoff(delz=wthick);
					translate([lbx,lby+lbdy,0]) stoff(delz=wthick);
					translate([lbx-lbdx,lby+lbdy,0]) stoff(delz=wthick);
				}
			}
		}
		// lcd conn hog-out
		translate([10-5.8,3.5,pcbz-.6-1.5]) translate([164.5-3.5-2,22.5-4.2-2,-jlcdz]) cube([7.4+4,23.1+4,jlcdz]);
		// mid hog-out
		translate([190,6,-3.5+2]) cube([24,83,20]);
		translate([196,73,-3.5+2]) cube([28,23.5,20]);
		// tab hog-out
//		translate([147,32,1.5]) cube([28,38,20]);
		// edge clip holes
		translate([width-17+3,35,wthick]) edgeclip(model=0);
		// inside panel trims
		translate([width-wthick-.5-.01,0,+.1]) cube([width, length, 20]);
		translate([0,-1-.01,+.1]) cube([width, wthick+1.5, 20]);
		translate([splitwid-40+.5+wthick+1.01,-1-.01,+.1]) cube([40, 23, 20]);
		translate([0,length-wthick-.5-.01,+.1]) cube([width, wthick+1.5, 20]);
		// RJ-45 opening
		translate([lbx+3.5-(.645*25.4)-((5.3+.04)*25.4)+1.3-((.67-.587)*25.4/2),lby+lbdy+12.5-21,-.05*25.4]) cube([.67*25.4,30,.4*25.4]);
		// side 2 tab mtg
		translate([tabx-(25/2), taby-(50/2),-bheight]) stoff(delz=wthick);
		translate([tabx+(25/2), taby-(50/2),-bheight]) stoff(delz=wthick);
		translate([tabx-(25/2), taby+(50/2),-bheight]) stoff(delz=wthick);
		translate([tabx+(25/2), taby+(50/2),-bheight]) stoff(delz=wthick);
		// bottom edge chams
		translate([0,chamfer,-.01]) rotate([135,0,0]) cube([width+10,5,5]);
		translate([0,length-chamfer,-.01]) rotate([-45,0,0]) cube([width+10,5,5]);
		translate([width-chamfer,0,-.01]) rotate([0,360-135-180,0]) cube([5,length+10,5]);
		// top edge chams
		translate([0,chamfer,.01-bheight]) rotate([135,0,0]) cube([width+10,5,5]);
		translate([0,length-chamfer,.01-bheight]) rotate([-45,0,0]) cube([width+10,5,5]);
		translate([width-chamfer,0,.01-bheight]) rotate([0,360-135-180,0]) cube([5,length+10,5]);
	}
	// corner radius (x3)
	translate([width-crad2,crad2,-bheight]) cornerb(bheight);
	translate([width-crad2,length-crad2,-bheight]) cornerb(bheight);
	// edge clips
//	translate([width-17+3,35,wthick]) edgeclip(model=1);		// for fit check
}

/////////////////////////////////////// edge-clip: placed ///////////////////////////////////////////////////////////////////////////
module ec_placed(){

	translate([width-17+3,35,wthick]) edgeclip(model=1, exp = 0.5);
}
///////////////////////////////////////////////////////////////

module stoff(delz=1, model=0){
	if(model){
		translate([0,0,delz]) cylinder(r=3, h=12.85-.14-.86, $fn=16);
	}else{
		cylinder(r=.13*25.4/2, h=50, $fn=16, center=true);
		translate([0,0,-.1]) cylinder(r1=6/2, r2=0, h=6/2, $fn=32);
	}
}

module bstoff(delz=1){
	translate([0,0,delz]) cylinder(r=2, h=12.4, $fn=16);
	translate([0,0,delz+2]) cylinder(r1=12/2, r2=0, h=9.8, $fn=16);
}

module edgeclip(model=1, exp=0){		// model==1 is the part, model==0 is the threaded hole.  exp is used for the negative into the case

	if(model){
		difference(){
			union(){
				// main sections
				cube([9,14,5]);
				translate([0,26,0]) cube([9,14,5]);
				// tie-section
				cube([3,30,5]);
				if(exp==0){
					// part tongues
					translate([0,0,2.5]) cube([11+(wthick/2),14,2.5]);
					translate([0,26,2.5]) cube([11+(wthick/2),14,2.5]);
				}else{
					// tongue hog-out
					translate([0,-exp/2,2.5-(exp/2)]) cube([11+(wthick/2)+(exp/2),14+exp,2.5+exp]);
					translate([0,26-exp/2,2.5-exp/2]) cube([11+(wthick/2)+(exp/2),14+exp,2.5+exp]);
				}
			}
			// mtg holes
			translate([5,7,-wthick]) cylinder(r=.09*25.4, h=10, $fn=16);
			translate([5,7+26,-wthick]) cylinder(r=.09*25.4, h=10, $fn=16);
			// mtg c-sinks
			translate([5,7,5.1-(6.5/2)-.5]) cylinder(r1=0, r2=6.5/2, h=6.5/2, $fn=16);
			translate([5,7,5.1-.51]) cylinder(r=6.5/2, h=6.5/2, $fn=16);

			translate([5,7+26,5.1-(6.5/2)-.5]) cylinder(r1=0, r2=6.5/2, h=6.5/2, $fn=16);
			translate([5,7+26,5.1-.51]) cylinder(r=6.5/2, h=6.5/2, $fn=16);

		}
	}else{
		// mtg hole negatives (for mating part diff)
		translate([5,7,-wthick-2.9]) cylinder(r=.05*25.4, h=10, $fn=16);
		translate([5,7+26,-wthick-2.9]) cylinder(r=.05*25.4, h=10, $fn=16);
	}
}

module piezo(model = 0, depth=10, film = 1){
	rotate([0,0,0]){
		if(model == 1){
			difference(){
				union(){
					translate([0,0,(3/2)+(piezo_ht/2)-.01]) cylinder(r=piezo_dia/2, h=piezo_ht, $fn=64, center=true);
					translate([0,0,1.2]) rotate([0,0,45]) cube([piezo_dia,piezo_dia,3], center=true);
				}
				// spkr center
				translate([0,0,(3/2)+(piezo_ht/2)+.01]) cylinder(r=piezo_id/2, h=piezo_ht+1, $fn=64, center=true);
				// mtg holes, #1 thru
				translate([(piezo_dia/2)+3,0,0]) cylinder(r=num1_thru/2, h=20, $fn=16, center=true);
				translate([(piezo_dia/2)+3,0,-.35]) cylinder(r1=3.4/2, r2=0, h=3.4/2, $fn=24);

				translate([-(piezo_dia/2)-3,0,0]) cylinder(r=num1_thru/2, h=20, $fn=16, center=true);
				translate([-(piezo_dia/2)-3,0,-.35]) cylinder(r1=3.4/2, r2=0, h=3.4/2, $fn=24);
				// lop-offs
				translate([0,(piezo_dia/2)+(30/2),0]) cube([30,30,30], center=true);
				translate([0,-(piezo_dia/2)-(30/2),0]) cube([30,30,30], center=true);
				// wire-gap
				translate([0,0,depth-2]) cube([4.5,piezo_dia+5,4], center=true);
			}
		}else{
			difference(){
				union(){
					// mtg holes, #1 thread
					translate([(piezo_dia/2)+3,0,-depth]) cylinder(r=num1_thrd/2, h=depth*2, $fn=16);
					translate([-(piezo_dia/2)-3,0,-depth]) cylinder(r=num1_thrd/2, h=depth*2, $fn=16);
					// body clearance
					translate([0,0,-depth]) cylinder(r=(piezo_dia+.6)/2, h=piezo_ht+10, $fn=64);
					translate([0,0,-depth-.5]) cylinder(r=(piezo_id)/2, h=piezo_ht+10, $fn=64);
					// wire clearance
					translate([0,-piezo_dia/2,((piezo_ht+10)/2)-depth]) cube([4,8,piezo_ht+10], center=true);
				}
			}
		}
	}
}

///////////////////////////////////////////////

module lcd(mtg=false, holes=false, csnkz = -5, hdia = .11*25.4){
	bezelz = 8;
	bezy = 1.5;

	if(mtg){
		// mtg holes only
		translate([lbx,lby,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([lbx-lbdx,lby,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([lbx,lby+lbdy,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([lbx-lbdx,lby+lbdy,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
	}else{
		if(!holes){
			// pcb+backing
			translate([0-1,0-.25,-fht_in+.1-scrnthk]) cube([170+1.5,95+.5,fht_in]);
			// LCD frame
			translate([9-.25,6.9-.25,-scrnthk]) cube([152.5+.5,88+.5,7.3+scrnthk]);
			// bezel opening
			translate([(170-(53))/2,(95/2)+bezy,bezelz]) rotate([0,0,45]) cylinder(r1=sqrt(2)*77/2, r2=(sqrt(2)*(77/2))+10, h=5, $fn=4);
			translate([(170+(53))/2,(95/2)+bezy,bezelz]) rotate([0,0,45]) cylinder(r1=sqrt(2)*77/2, r2=(sqrt(2)*(77/2))+10, h=5, $fn=4);
			// bezel, to depth
			translate([170/2,(95/2)+bezy,bezelz-3]) scale([130/77,1,1]) rotate([0,0,45]) cylinder(r=sqrt(2)*77/2, h=4, $fn=4);
		}
		// mtg holes
		translate([lbx,lby,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([lbx-lbdx,lby,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([lbx,lby+lbdy,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([lbx-lbdx,lby+lbdy,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		if(holes){
			// csinks
			translate([4,95-4.5,csnkz]) cylinder(r2=hdia/2, r1=(hdia/2)+2, h= 2, $fn=32);
			translate([4,(95-4.5)-85,csnkz]) cylinder(r2=hdia/2, r1=(hdia/2)+2, h= 2, $fn=32);
			translate([4+162,95-4.5,csnkz]) cylinder(r2=hdia/2, r1=(hdia/2)+2, h= 2, $fn=32);
			translate([4+162,(95-4.5)-85,csnkz]) cylinder(r2=hdia/2, r1=(hdia/2)+2, h= 2, $fn=32);
		}
		if(!holes){
			// connector/wire holes clearance
			translate([179-18,14,-8]) cube([9,67,10]);
		}
	}
}

///////////////////////////////////////////////
//

module lcdpcb(csnkz = -5, hdia = .11*25.4){
	bezelz = 8;

	difference(){
		// pcb+backing
		translate([0,0,0]) cube([170+.5,93+.5,1.6]);
		// mtg holes
		translate([4,88.5,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([4,3.6,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([165.8,88.5,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([165.8,3.6,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		// edge notches
		translate([15.9,-.1,-.1]) cube([5.9,5.2+.1,10]);
		translate([27.9,-.1,-.1]) cube([113.8,5.2+.1,10]);
		translate([148,-.1,-.1]) cube([5.9,5.2+.1,10]);
		// pin1 mark
		translate([170-2.5,44.3,-(2.5-1.6)/2]) cylinder(r=.5, h=2.5, $fn=16);
	}
	difference(){
		// LCD frame
		translate([9-.25,6.5+.8,0]) cube([152.5+.5,87,7.3]);
		translate([21,12,7.3-.7]) cube([127.8,73.8,5]);
	}
	// 20p header
	translate([170-5.8,18.5,0]) hdr20(model=1);
	// backlight -
	translate([170-2.7,58,0]) backlight(model=1);
	// small frame tabs
	translate([14.6,6.4,-3.1]) cube([5.8,4.9,3.1]);
	translate([150,6.4,-3.1]) cube([5.8,4.9,3.1]);
	translate([8.4,17.9,-3.1]) cube([4.9,5.8,3.1]);
	translate([8.4,47.4,-3.1]) cube([4.9,5.8,3.1]);
	translate([8.4,77.4,-3.1]) cube([4.9,5.8,3.1]);
	translate([156,17.9,-3.1]) cube([4.9,5.8,3.1]);
	translate([156,47.4,-3.1]) cube([4.9,5.8,3.1]);
	translate([156,77.4,-3.1]) cube([4.9,5.8,3.1]);
	// lg frame tabs
	translate([10.5+(5.8/2),93.5,0]) frmtab();
	translate([38.8+(5.8/2),93.5,0]) frmtab();
	translate([66.7+(5.8/2),93.5,0]) frmtab();
	translate([97.8+(5.8/2),93.5,0]) frmtab(left=0);
	translate([125.9+(5.8/2),93.5,0]) frmtab(left=0);
	translate([153.5+(5.8/2),93.5,0]) frmtab(left=0);
	// lcd ICs
	translate([18.3,14.8,-1.5]) cube([13.8,13.8,1.5]);
	translate([18.3,54.5,-1.5]) cube([13.8,13.8,1.5]);
	translate([46.1,66.5,-1.5]) cube([13.8,13.8,1.5]);
	translate([82.6,66.5,-1.5]) cube([13.8,13.8,1.5]);
	translate([118.6,66.5,-1.5]) cube([13.8,13.8,1.5]);
	// RAM
	translate([105.5,8.1+5.5,-2.9]) cube([10.2,18.2,2.9]);
	// RAIO
	translate([126.7,18.4,-1.5]) cube([14,20,1.5]);
	// xtal
	translate([139.5,47,-1.9]) cube([7.2,2.7,1.9]);
	// L1
	translate([54.1,13.2+5.4,-3.3]) cylinder(r=4.7/2, h=3.3, $fn=16);
	// caps
	translate([40.8,27.1+5.4,-2]) cube([3.6,2.9,2]);
	translate([40.8,8.4+27.1+5.4,-2]) cube([3.6,2.9,2]);
	translate([69.1,9.8+5.4,-2.8]) cube([6.1,3.4,2.8]);
	translate([69.1,13.1+9.8+5.4,-2.8]) cube([6.1,3.4,2.8]);
	// D1
	translate([58.6,9.7+5.4,-2.8]) cube([6.9,3.3,2.8]);
	// misc ICs
	translate([63,16.1+5.4,-1.5]) cube([5,5,1.5]);
	translate([67.1,29.7+5.4,-1.5]) cube([5,8.7,1.5]);
	translate([85.1,19.5+5.4,-1.5]) cube([5,8.7,1.5]);

	
}

module frmtab(left=1){
	translate([0,0,-2.4]){
		if(left){
			difference(){
				cylinder(r=5.8/2, h=2.4, $fn=16);
				translate([-3,0,-.1]) cube([6,6,4]);
			}
			translate([5.8/2,0,0]) cylinder(r=1, h=2.4, $fn=16);
			translate([(5.6/2),0,0]) cube([9.1,1,2.4]);
		}else{
			difference(){
				cylinder(r=5.8/2, h=2.4, $fn=16);
				translate([-3,0,-.1]) cube([6,6,4]);
			}
			translate([-5.8/2,0,0]) cylinder(r=1, h=2.4, $fn=16);
			translate([-9.1-(5.6/2),0,0]) cube([9.1,1,2.4]);
		}
	}
}

module pin(){
	translate([-.3,-.3,-9.1]) cube([.6,.6,11.5]);
	translate([-1.3,-1.3,-3]) cube([2.6,2.6,3]);

	translate([-.3+(0.1*25.4),-.3,-9.1]) cube([.6,.6,11.5]);
	translate([-1.3+(0.1*25.4),-1.3,-3]) cube([2.6,2.6,3]);
}

module pin1(){
	translate([-.3,-.3,-9.1]) cube([.6,.6,11.5]);
	translate([-1.3,-1.3,-3]) cube([2.6,2.6,3]);
}

module hdr20(model=1){

	if(model == 1){
		for(y = [0 : 0.1*25.4 : 0.9*25.4]) translate([0,y,0]) pin();
	}else{
		for(y = [0 : 0.1*25.4 : 0.9*25.4]) translate([0,y,0]) cylinder(r=0.045*25.4/2, h=20, $fn=16, center=true);
		for(y = [0 : 0.1*25.4 : 0.9*25.4]) translate([0.1*25.4,y,0]) cylinder(r=0.045*25.4/2, h=20, $fn=16, center=true);
	}
}

module backlight(model = 1){
	if(model==1){
		translate([0,0,0]) pin1();
		translate([0,6.1,0]) pin1();
		translate([0,11,0]){
			translate([0,0,0]) pin1();
			translate([0,6.1,0]) pin1();
		}
	}else{
		translate([0,0,0]) cylinder(r=0.045*25.4/2, h=20, $fn=16, center=true);
		translate([0,6.1,0]) cylinder(r=0.045*25.4/2, h=20, $fn=16, center=true);
		translate([0,11,0]){
			translate([0,0,0]) cylinder(r=0.045*25.4/2, h=20, $fn=16, center=true);
			translate([0,6.1,0]) cylinder(r=0.045*25.4/2, h=20, $fn=16, center=true);
		}
	}
}

module dial(model=2, sl=20){

	if(model == 0){
		// dial main body
		translate([0,0,(-6.5/2)+.1]) cube([13,14,6.5], center=true);
		// thread section clearance
		cylinder(r=dialr+dskew, h=30, $fn=32);
		translate([0,0,nthick]) cylinder(r=dialnutr, h=30-nthick, $fn=32);
		// optical void
		translate([0,0,opv_deep]) cylinder(r=opvoidr+0.5, h=10, $fn=64);
		// light holes
		translate([.8*opvoidr,0,0]) cylinder(r=3, h=40, $fn=32, center=true);
		translate([-.8*opvoidr,0,0]) cylinder(r=3, h=40, $fn=32, center=true);
		translate([0,.8*opvoidr,0]) cylinder(r=3, h=40, $fn=32, center=true);
		translate([0,-.8*opvoidr,0]) cylinder(r=3, h=40, $fn=32, center=true);
	}
	if(model == 2){
		// dial optical diffuser insert piece
		difference(){
			// main body
			union(){
				translate([0,0,opv_deep]) cylinder(r2=opvoidr-0.5+(1.3/2), r1=opvoidr-0.5-.5+(1.3/2), h=.51, $fn=64);
				translate([0,0,opv_deep+.5]) cylinder(r=opvoidr-0.5+(1.3/2), h=5.5-.5, $fn=64);
			}
			// central opening for dial shaft
			translate([0,0,opv_deep-.1]) cylinder(r1=dialnutr, r2=dialnutr+2, h=6, $fn=32);
			// concave shaping of top surface
			translate([0,0,opv_deep+5.51-1.5]) cylinder(r2=opvoidr-.5-1, r1=0, h=1.5, $fn=64);
		}
	}
	if(model==1){
		// dial main body
		translate([0,0,(-6.5/2)+.1]) cube([13,14,6.5], center=true);
		// shaft length
		if(sl<15.1){
			cylinder(r=dialr, h=5, $fn=32);
		}
		else{
			cylinder(r=dialr, h=7, $fn=32);
		}
		cylinder(r=6/2, h=sl, $fn=32);
		// knob
		color("black") translate([0,0,sl-11.7+2]) cylinder(r=21/2, h=11.7, $fn=64, center=false);
		color("black") translate([0,0,sl-11.7+2]) cylinder(r=24/2, h=2, $fn=64, center=false);
	}
}

/*
bwid = 9;		// y
blen = 15;		// x
bht = 6;		// z
bclr = .5;		// clearance
bapron = 2;		// apron around periphery of button base
bcham = .5;
*/

module cap(model=0, colr=1, capht=6){
walldr = 2;		// sets thickness of cap wall
topdr = .7;		// sets thickness of cap top

	// ruler
//	translate([1.5,-3,-7.5-5.48]) cylinder(r=1, h=7.5, $fn=16);

	if(model==0 || model==2 || model>9){
		capb(modelb=model, colrb=colr, caphtb=capht);
	}else{
		difference(){
			capb(modelb=model, colrb=colr, caphtb=capht);
			difference(){
				translate([0,0,-topdr]) scale([(blen-walldr)/blen,(bwid-walldr)/bwid,1]) capb(modelb=model, colrb=colr, caphtb=capht);
			}
			// debug section
//			translate([0,10,0]) cube([20,20,20],center=true);
		}
		// contact nib
		difference(){
			translate([0,0,-capht-.35-.3-.8]) cube([1.5, 8, bnib+scrnthk+1.6], center=true);
			translate([0,0,-capht+1.5]) rotate([0,90,0]) cylinder(r=bwid*(bwid-2)/(2*bwid), h=4, $fn=16, center=true);
			translate([0,-6.7,-capht]) rotate([20,0,0]) cube([4,5,10],center=true);
			translate([0,6.7,-capht]) rotate([-20,0,0]) cube([4,5,10],center=true);

			translate([0,5.14,-capht-8.85]) cube([4,5,10],center=true);
			translate([0,-5.14,-capht-8.85]) cube([4,5,10],center=true);
		}
	}
}

module capb(modelb=0, colrb=1, caphtb=6){

	if(modelb == 0){
		translate([0,0,(-caphtb/2)+(2*bclr)]) cube([blen+bclr, bwid+bclr, caphtb+(4*bclr)], center=true);
		translate([0,0,-caphtb+(bapht/2)-.1]) cube([capronx, caprony, bapht], center=true);
	}
	if(modelb == 1){
		difference(){
			union(){
				if(colrb==1) color("yellow"){
					translate([0,0,(-caphtb+brelief)/2]) minkowski(){
						cube([blen-(bcham), bwid-(bcham), caphtb-(bcham)+brelief], center=true);
						sphere(r=bcham/2, $fn=32);
					}
				}
				if(colrb==2) color("red"){
					translate([0,0,(-caphtb+brelief)/2]) minkowski(){
					cube([blen-(bcham), bwid-(bcham), caphtb-(bcham)+brelief-4], center=true);
					sphere(r=bcham/2, $fn=32);
					}
				}
				// apron
				translate([0,0,-caphtb+(bapht/2)]) cube([blen+(2*capron), bwid+(2*capron), bapht], center=true);
			}
			// corner chams
			translate([-capronx/2,-caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([capronx/2,-caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([-capronx/2,caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([capronx/2,caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
		}
	}
	// spacer supports
	if(modelb == 2){
	qoffs=0;
	qoffy=0;
		translate([-(capronx/2)-qoffs,-(caprony/2)-qoffy,-caphtb+(bapht/2)-stht-.468]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);
		translate([(capronx/2)+qoffs,-(caprony/2)-qoffy,-caphtb+(bapht/2)-stht-.468]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);
		translate([-(capronx/2)-qoffs,(caprony/2)+qoffy,-caphtb+(bapht/2)-stht-.468]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);
		translate([(capronx/2)+qoffs,(caprony/2)+qoffy,-caphtb+(bapht/2)-stht-.468]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);

//		translate([-(capronx/2)-.17,-(caprony/2)-.17,-caphtb+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
//		translate([capronx/2,-caprony/2,-caphtb+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
//		translate([-capronx/2,caprony/2,-caphtb+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
//		translate([capronx/2,caprony/2,-caphtb+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
	}
	if(modelb==10){
		// #1 holes
		translate([-(capronx/2)-.17,-(caprony/2)-.17,-caphtb+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
	if(modelb==11){
		// #1 holes
		translate([(capronx/2)-.17,-(caprony/2)-.17,-caphtb+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
	if(modelb==12){
		// #1 holes
		translate([-(capronx/2)-.17,(caprony/2)-.17,-caphtb+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
	if(modelb==13){
		// #1 holes
		translate([(capronx/2)-.17,(caprony/2)-.17,-caphtb+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
}

module vq(model=1){
walldr = 1.5;		// sets thickness of cap wall
topdr = .5;		// sets thickness of cap top

	// ruler
//	translate([0,11,-11]) cylinder(r=3, h=14.5, $fn=16);

	// model
	if(model==0){
		vqb(modelb=model);
	}else{
		difference(){
			vqb(modelb=1);
			difference(){
				translate([0,0,-topdr]) scale([(bqlen-walldr)/bqlen,(bqwid-walldr)/bqwid,1]) vqb(modelb=2);
			}
			// debug section
//			translate([10,13,0]) cube([20,20,20],center=true);
		}
		// contact nibs
		difference(){
			union(){
				translate([0,(-bqwid/2)+.5+(2.25/2)+1,-bht-bapron-(7.1/2)+2.1+4]) cube([1.5, 7.25/2, 7.1-2.6], center=true);
				translate([0,(-bqwid/2)+.5+(2.25/2),-bht-bapron-(7.1/2)+2.1+(2.6/2)-(1.1/2)+.25]) cube([1.5, 7.25, 7.1-2], center=true);
				translate([0,(+bqwid/2)-.5-(2.25/2)-1,-bht-bapron-(7.1/2)+2.1+4]) cube([1.5, 7.25/2, 7.1-2.6], center=true);
				translate([0,(bqwid/2)-.5-(2.25/2),-bht-bapron-(7.1/2)+2.1+(2.6/2)-(1.1/2)+.25]) cube([1.5, 7.25, 7.1-2], center=true);
			}
			rotate([0,90,0]) scale([1.5,1,1]) cylinder(r=13.5, h=4, $fn=4, center=true);
		}
	}
}

module vqb(modelb=1){
qcapronx = bqlen+(2*(bapron+bclr));
qcaprony = bqwid+(2*(bapron+bclr));
qcapcham = 2.5;

	if(modelb == 0){
		// main body
		translate([0,0,(-bht/2)+(2*bclr)]) cube([bqlen+bclr, bqwid+bclr, bht+(4*bclr)], center=true);
		// apron
		translate([0,0,-bht+(bapht/2)-.1]) cube([bqlen+(2*(bapron+bclr)), bqwid+(2*(bapron+bclr)), bapht], center=true);
		// pivot axle
		translate([0,0,-bht+1]) rotate([0,90,0]) cylinder(r=2, h=bqlen+(2*(bapron+bclr)), $fn=32, center=true);
	}else{
		difference(){
			union(){
				// main cap body
				color("yellow") translate([0,0,((-bht+brelief)/2)-.5]) minkowski(){
					cube([bqlen-(bcham), bqwid-(bcham)-brocker, bht-(bcham)+brelief+2], center=true);
					sphere(r=bcham/2, $fn=32);
				}
				// apron
				if(modelb==1){
					translate([0,0,-bht-.5]) cube([bqlen+(2*bapron), bqwid+(2*bapron), bapht], center=true);
				}else{
					translate([0,0,-bht-1]) cube([bqlen+(2*bapron), bqwid+(2*bapron), bapht], center=true);
				}
				// contact nib
//				translate([0,0,-bht-bapron+.4]) cube([2, bqwid+3.5, bnib+scrnthk-2], center=true);
				if(modelb!=2){
					// rocker axle
					translate([0,0,-bht+1]) rotate([0,90,0]) cylinder(r=2-.1, h=bqlen+(2*(bapron+bclr)), $fn=32, center=true);
				}
			}
			// apron chams
			translate([-qcapronx/2,-qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([qcapronx/2,-qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([-qcapronx/2,qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([qcapronx/2,qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
		}
	}
	
}

module pivot(){
pivot_ht = 8.5;
pivotx = 11.7;
pivoty = 4;

	difference(){
		cube([pivotx,pivoty,pivot_ht]); //5.9+2.7
		translate([-1,0,pivot_ht]) rotate([0,90,0]) cylinder(r=pivoty/2, h=15, $fn=4);
		translate([-1,4,pivot_ht]) rotate([0,90,0]) cylinder(r=pivoty/2, h=15, $fn=4);
	}
	translate([0,(-15.7/2)+2,0]) cube([2.5,15.7,1]);
}

module pbsw(model=1){
	color("red") translate([0,0,-1.6-.6]) rotate([0,0,0]){
		if(model==1){
			translate([0,0,(3.4/2)+.9+.9]) cube([2.9,1.4,3.4], center=true);
			translate([0,0,(3.4/2)+.9]) cube([6,3.5,3.4], center=true);
			translate([0,0,.9+.1]) cube([7.6,1,.2], center=true);
			translate([7.4/2,0,-(3.9/2)+.9]) cube([.2,1,3.9], center=true);
			translate([-7.4/2,0,-(3.9/2)+.9]) cube([.2,1,3.9], center=true);
		}else{
			translate([7.4/2,0,-(3.9/2)+.9]) cylinder(r=1.2/2, h=10, $fn=16, center=true);
			translate([-7.4/2,0,-(3.9/2)+.9]) cylinder(r=1.2/2, h=10, $fn=16, center=true);
		}
	}
}


module corner(height=5){
			translate([0,0,height-chamfer]) cylinder(r2=(crad/2)-chamfer, r1=crad/2, h=chamfer, $fn=32, center=false);
			translate([0,0,chamfer]) cylinder(r=crad/2, h=height-(2*chamfer), $fn=32, center=false);			// corner
			cylinder(r1=(crad/2)-chamfer, r2=crad/2, h=chamfer, $fn=32, center=false);
}

module cornerb(height=5){
			translate([0,0,height-chamfer]) cylinder(r2=(crad/2)-chamfer, r1=crad/2, h=chamfer, $fn=32, center=false);
			translate([0,0,chamfer]) cylinder(r=crad/2, h=height-(2*chamfer), $fn=32, center=false);			// corner
			cylinder(r1=(crad/2)-chamfer, r2=crad/2, h=chamfer, $fn=32, center=false);
}

module spilot(csdeep=5, size=2){
	if(size==2){
		rotate([0,90,0]){
			// threaded pilot
			cylinder(r=(1.63+(dskew/2))/2, h = 8, $fn=16);
			// thru-pilot
			translate([0,0,-20+.01]) cylinder(r=(2.18+dskew)/2, h = 20, $fn=16);
			// c-sink
			translate([0,0,-csdeep]) cylinder(r1=4.5/2, r2=0, h = 4.5/2, $fn=16);
			translate([0,0,-10-csdeep+.01]) cylinder(r=4.8/2, h = 10, $fn=16);
		}
	}
	if(size==4){
		rotate([0,90,0]){
			// threaded pilot
			cylinder(r=(1.96+dskew)/2, h = 6.2, $fn=16);
			// thru-pilot
			translate([0,0,-20+.01]) cylinder(r=(3.05+dskew)/2, h = 20, $fn=16);
			// c-sink
			translate([0,0,-csdeep]) cylinder(r1=5.4/2, r2=0, h = 5.4/2, $fn=16);
			translate([0,0,-10-csdeep+.01]) cylinder(r=5.6/2, h = 10, $fn=16);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
module rdu(mode=0){
brz = 1.6+(.2 * 25.4);
j4z = brz+(.2 * 25.4);
l2z = j4z;

	translate([6.4,13,pcbz-brz]) cube([145,44,brz]);
	translate([82.3,49.4,pcbz-j4z]) cube([14.6,7.4,j4z]);
	translate([38,36.2,pcbz-j4z]) cube([12.6,12.6,l2z]);
}


/////////////////////////////////////////////////////////////////////////////////////

module rduho(){

brz = 1.6+(.2 * 25.4);
j4z = brz+(.2 * 25.4);
l2z = j4z;

	translate([82.3-1.5,49.4-1.5,pcbz-j4z-1.5]) cube([14.6+3,7.4+3,j4z]);
	translate([38-1.5,36.2-1.5,pcbz-l2z-1.5]) cube([12.6+3,12.6+3,l2z]);
}


/////////////////////////////////////////////////////////////////////////////////////

module trled(model=0){


	if(model==0){
		cylinder(r=trledia/2, h=20, $fn=16, center=true);
	}else{
	}
}

module sunwin(){
	difference(){
		union(){
			translate([0,0,5.99]) cylinder(r1=(domedia/2)-.5, r2=(domedia/2)-1.5, h=1, $fn=64, center=false);
			cylinder(r=(domedia/2)-.5, h=6, $fn=64, center=false);
			translate([0,0,-2/2]) cube([domedia+10, 8, 2], center=true);
		}
		translate([0,0,-4]) cylinder(r=(domedia/2)-3, h=10, $fn=64, center=false);
		// mtg holes
		translate([15,0,-4]) cylinder(r=num1_thru/2, h=7, $fn=16);
		translate([15,0,-4+(3.8/2)]) cylinder(r1=3.8/2, r2=0, h=3.8/2, $fn=16);
		translate([-15,0,-4]) cylinder(r=num1_thru/2, h=7, $fn=16);
		translate([-15,0,-4+(3.8/2)]) cylinder(r1=3.8/2, r2=0, h=3.8/2, $fn=16);
	}
}

/////////////////////////////////////////////////////////////////////////////////////

module ltdome(){

	cylinder(r=(domedia-3)/2, h=20, $fn=64, center=true);
	translate([0,0,-2.5]) cylinder(r=domedia/2, h=20, $fn=64, center=false);
}

/////////////////////////////////////////////////////////////////////////////////////

module cpknob(model=0){

	if(model==0){
 		cylinder(r=(pknobdia+1)/2, h=pknobht+1, $fn=32, center=true);
		translate([0,-4,0]) rotate([90,0,0]) scale([1.5,.8,1]) rotate([0,0,45]) cylinder(r1=3.2, r2=9, h=4, $fn=4);
	}else{
		difference(){
			cylinder(r=((pknobdia)/2), h=4.9, $fn=64);
			translate([0,0,2]) cylinder(r=(9.65)/2, h=4.5, $fn=64);
			translate([0,0,2+2]) cylinder(r=(10.2)/2, h=4.5, $fn=64);
			for(a=[0:20:360]){
				rotate([0,0,a]) translate([(pknobdia)/2,0,0]) cylinder(r=.5, h=10, $fn=4, center=true);
			}
			translate([(10.4/2)+(10/2),0,0]) cube([10,20,20],center=true);
		}
//		translate([0,0,1.5]) cube([.8,3.6,1+2],center=true);
	}
}

/////////////////////////////////////////////////////////////////////////////////////

// pcb spacer
screenthk = 1.6;

module spacer(thk=.125*25.4){
//	difference(){
		cylinder(r=6.5/2, h=thk, $fn=32);
		cylinder(r=2.2/2, h=5+thk, $fn=16);
//	}
}

////////////
// Top cover

module lid(){
	difference(){
		translate([wthick+.5,wthick+.5,0]) cube([170-1,95-1,5]);
		translate([wthick+.5,wthick+.5,0]) translate([((170-1)-154)/2,-10,5-3.5+.01]) cube([154,120,3.5]);
		translate([3.5,3.5,4.5]) lcd(holes=true);													// main hog-out
		// cable opening
		translate([169,15,-1]) cube([5,60,20]);
	}
}

/////////////////////////////////////
// Light pipe (inserts into top cover)

module lid_lpipe(hole){
	translate([0,length,(height+heightl)]) rotate([180,0,0]) tiva_assy(10);			// "10" param creates stand-alone LP
}


/////////////////////////
// lid corner

module corner(height=5){
			translate([0,0,height-chamfer]) cylinder(r2=(crad/2)-chamfer, r1=crad/2, h=chamfer, $fn=32, center=false);
			translate([0,0,chamfer]) cylinder(r=crad/2, h=height-(2*chamfer), $fn=32, center=false);			// corner
			cylinder(r1=(crad/2)-chamfer, r2=crad/2, h=chamfer, $fn=32, center=false);
}

module deskstand(){
ww = 4;

	difference(){
		union(){
			cube([150,90,ww+1]);
			translate([150-ww,0,0]) cube([ww,90,110]);
			cube([ww,90,110]);
			translate([0,90-ww,0]) cube([150,ww,80]);
		}
		translate([-20,10,8]) rotate([53,0,0]) cube([200,150,150]);
	}
}

/////////////////////////
// debug artifacts
//
// X-rulers
//#translate([-.69,0,0]) cube([0.01,30,50]);	// ruler
//#translate([175.41,0,0]) cube([0.01,30,50]);	// inside main void ruler 1
//#translate([1.67,0,0]) cube([0.01,30,50]);	// inside main void ruler 2
//#translate([175.34,0,0]) cube([0.01,30,50]);	// outside shroud ruler 2
// Y-rulers
//#translate([0,23.81,0]) cube([180,0.01,50]);	// ruler
//#translate([0,22.36 ,0]) cube([180,0.01,50]);	// ruler
// Z-rulers
//#translate([0,0,0]) cube([10,40,.01]);	// ruler
//#translate([0,0,2.04]) cube([9,40,.01]);	// ruler

// EOF
