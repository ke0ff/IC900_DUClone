// OpenSCAD 2019.05
// LCD Case - enclosure, REV-001
// Joe Haas, KE0FF, 10/19/2023
// This is the IC900 controller clone enclosure which uses an NHD-240128WG LCD module
//	and custom button PCB
//
// Rev-001, 12/17/2023
//	initial code

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

bossht = 7.8;
crad = 5;
crad2 = crad/2;
chamfer = .8;
lcdz = height-14+4.5; //13+4.5;
scham = 4;

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
stht = 6.4+bapht+.6;

////////////////////////////////////////////////////////////////////////////////

//translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) cylinder(r=6.9/2, h=40, $fn=32, center=true);

/*translate([width-(bsoffs)+bpitchx-45,length-17,21.1]) cylinder(r=6.9/2, h=9.41, $fn=32);
translate([width-(bsoffs)+bpitchx-45,length-14,28.3]) cylinder(r=6.9/2, h=9.41, $fn=32);
translate([width-(bsoffs)+bpitchx-45,length-22,28.3-(.08*25.4)]) cylinder(r=6.9/2, h=9.41, $fn=32);*/

//translate([160,6,15.3]) 
//spacer();
//	translate([30,0,0]) cap(model=1);
		
button = 0;
vqbtn = 0;
sidesel = 1;
dopcb = 0;

if(button){
	cap(model=1);
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
//		translate([175,-10,-10]) cube([200,150, 40]);
	}
}
//pbsw();

//backlight(model = 0);

//hdr20(model=0);

//translate([3.5,3.5,lcdz-scrnthk-.6]) lcdpcb();


//translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) dial(model=1);

//spilot();
//translate([width-(bsoffs),length-bsoffsy,height]) cap(model=1);
//translate([width-(bsoffs)-3,length-19,height]) vq(model=1);

//lcd();
//lid();

//bcaps();

//dial(model=0);
//translate([210,12,15.25]) cylinder(r1=2, r2=0, h=12, $fn=16);

module mainpcb(mode=0){
	translate([10-5.8,3.5,pcbz]){
		if(mode==0){
		// RJ45 test fit
//			translate([0,0,-(13.5)]) cube([10,10,13.5]);
		}
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
	difference(){
		union(){
			difference(){
				union(){
					cube([width,length,height]);
//					translate([19.1,88,0]) cube([2,2,23.6]); // test-ruler
				}
				cube([crad,crad,60], center=true);										// corner notch
				translate([width,0,0]) cube([crad,crad,60], center=true);				// corner notch
				translate([0,length,0]) cube([crad,crad,60], center=true);				// corner notch
				translate([width,length,0]) cube([crad,crad,60], center=true);			// corner notch
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

//		translate([15,15,1.5]) linear_extrude(1) text("REV 4", size=6);					// version & PN texts
//		translate([15,25,1.5]) linear_extrude(1) text("TivaLP bot", size=6);

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
			translate([splitwid,-.1,-.1]) cube([splitwid+.1,length+5,height+5]);
		}
		// RJ-45 opening
		translate([lbx+3.5-(.645*25.4)-((5.3+.04)*25.4)+1.3,lby+lbdy,pcbz+pcbthk+(-.511*25.4)+((.48-.425)*25.4)-(scrnthk/2)]) cube([.587*25.4,30,.425*25.4]);
		
		// debug hog-off
		translate([230,-.1,-.1]) cube([splitwid+.1,length+5,height+5]);

	////// dial/button artifacts //////

		if((side==2)||(side==3)){
			// dial
			translate([width-(bsoffs)+bpitchx,length-17,height-dial_deep]) dial(model=0);
			// button hog-out
			difference(){
				// button chamber
				translate([width-bswid-(wthick)+1,wthick,height-bsz-wthickt-bht+1.5]) cube([bswid-1, bslen, bsz]);
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
					translate([-(capronx/2)+2.5,-(caprony/2),-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht]);
					translate([-(capronx/2)+14.5,-(caprony/2),-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht]);
				}
				translate([width-(bsoffs)+38,length+2.2,height]){
					translate([-(capronx/2)+2.5,-(caprony/2),-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht]);
					translate([-(capronx/2)+14.5,-(caprony/2),-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht]);
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
			translate([0-.25,0-.25,-fht_in+.1-scrnthk]) cube([170+.5,95+.5,fht_in]);
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
		translate([0,0,0]) cube([170+.5,95+.5,1.6]);
		// mtg holes
		translate([4,95-4.5-.6,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([4,(95-4.5-.8)-85,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([4+162+.7,95-4.5+.3,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		translate([4+162,(95-4.5-1.5)-85,-20]) cylinder(r=hdia/2, h= 20+bossht-1, $fn=16);
		// edge notches
		translate([15.9,-.1,-.1]) cube([5.9,5.2+.1,10]);
		translate([27.9,-.1,-.1]) cube([113.8,5.2+.1,10]);
		translate([148,-.1,-.1]) cube([5.9,5.2+.1,10]);
		// pin1 mark
		translate([170-2.5,44.3,-(2.5-1.6)/2]) cylinder(r=.5, h=2.5, $fn=16);
	}
	difference(){
		// LCD frame
		translate([9-.25,6.9-.25,0]) cube([152.5+.5,88+.5,7.3]);
		translate([21,12,7.3-.7]) cube([127.8,73.8,5]);
	}
	// 20p header
	translate([170-5.8,18.5,0]) hdr20(model=1);
	// backlight -
	translate([170-2.7,58,0]) backlight(model=1);
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

module dial(model=0, sl=20){

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

	}else{
		// dial main body
		translate([0,0,(-6.5/2)+.1]) cube([13,14,6.5], center=true);
		// shaft length
		if(sl<15.1){
			cylinder(r=dialr, h=5, $fn=32);
		}
		else{
			cylinder(r=dialr, h=7, $fn=32);
		}
#		cylinder(r=6/2, h=sl, $fn=32);
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


module cap(model=0, colr=1){

	if(model == 0){
		translate([0,0,(-bht/2)+(2*bclr)]) cube([blen+bclr, bwid+bclr, bht+(4*bclr)], center=true);
		translate([0,0,-bht+(bapht/2)-.1]) cube([capronx, caprony, bapht], center=true);
	}
	if(model == 1){
		difference(){
			union(){
				if(colr==1) color("yellow"){
					translate([0,0,(-bht+brelief)/2]) minkowski(){
						cube([blen-(bcham), bwid-(bcham), bht-(bcham)+brelief], center=true);
						sphere(r=bcham/2, $fn=32);
					}
				}
				if(colr==2) color("red"){
					translate([0,0,(-bht+brelief)/2]) minkowski(){
					cube([blen-(bcham), bwid-(bcham), bht-(bcham)+brelief-4], center=true);
					sphere(r=bcham/2, $fn=32);
					}
				}
				// apron
				translate([0,0,-bht+(bapht/2)]) cube([blen+(2*capron), bwid+(2*capron), bapht], center=true);
				// contact nib
				translate([0,0,-bht-.35-.3]) cube([2, 8, bnib+scrnthk], center=true);
			}
			// corner chams
			translate([-capronx/2,-caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([capronx/2,-caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([-capronx/2,caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([capronx/2,caprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
		}
	}
	// spacer supports
	if(model == 2){
	qoffs=0;
	qoffy=0;
		translate([-(capronx/2)-qoffs,-(caprony/2)-qoffy,-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);
		translate([(capronx/2)+qoffs,-(caprony/2)-qoffy,-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);
		translate([-(capronx/2)-qoffs,(caprony/2)+qoffy,-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);
		translate([(capronx/2)+qoffs,(caprony/2)+qoffy,-bht+(bapht/2)-stht]) rotate([0,0,45]) translate([-(1.5*stcham)/2,-(1.5*stcham)/2,0]) cube([1.5*stcham,1.5*stcham,stht+2+scrnthk]);

//		translate([-(capronx/2)-.17,-(caprony/2)-.17,-bht+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
//		translate([capronx/2,-caprony/2,-bht+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
//		translate([-capronx/2,caprony/2,-bht+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
//		translate([capronx/2,caprony/2,-bht+(bapht/2)-stht]) cylinder(r=stcham, h=stht, $fn=4, center=false);
	}
	if(model==10){
		// #1 holes
		translate([-(capronx/2)-.17,-(caprony/2)-.17,-bht+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
	if(model==11){
		// #1 holes
		translate([(capronx/2)-.17,-(caprony/2)-.17,-bht+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
	if(model==12){
		// #1 holes
		translate([-(capronx/2)-.17,(caprony/2)-.17,-bht+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
	if(model==13){
		// #1 holes
		translate([(capronx/2)-.17,(caprony/2)-.17,-bht+(bapht/2)-stht]) cylinder(r=hole1/2, h=2*(5/16)*25.4, $fn=10, center=true);
	}
}

module vq(model=1){
qcapronx = bqlen+(2*(bapron+bclr));
qcaprony = bqwid+(2*(bapron+bclr));
qcapcham = 2.5;

	if(model == 0){
		// main body
		translate([0,0,(-bht/2)+(2*bclr)]) cube([bqlen+bclr, bqwid+bclr, bht+(4*bclr)], center=true);
		// apron
		translate([0,0,-bht+(bapht/2)-.1]) cube([bqlen+(2*(bapron+bclr)), bqwid+(2*(bapron+bclr)), bapht], center=true);
		// pivot axle
		translate([0,0,-bht]) rotate([0,90,0]) cylinder(r=2, h=bqlen+(2*(bapron+bclr)), $fn=32, center=true);
	}else{
		difference(){
			union(){
				// main cap body
				color("yellow") translate([0,0,(-bht+brelief)/2]) minkowski(){
					cube([bqlen-(bcham), bqwid-(bcham)-brocker, bht-(bcham)+brelief+1], center=true);
					sphere(r=bcham/2, $fn=32);
				}
				// apron
				translate([0,0,-bht]) cube([bqlen+(2*bapron), bqwid+(2*bapron), bapht], center=true);
				// contact nib
				translate([0,0,-bht-bapron+.4]) cube([2, bqwid+3.5, bnib+scrnthk-2], center=true);
				// rocker axle
				translate([0,0,-bht+1]) rotate([0,90,0]) cylinder(r=2-.1, h=bqlen+(2*(bapron+bclr)), $fn=32, center=true);
			}
			// apron chams
			translate([-qcapronx/2,-qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([qcapronx/2,-qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([-qcapronx/2,qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
			translate([qcapronx/2,qcaprony/2,0]) cylinder(r=capcham, h=20, $fn=4, center=true);
		}
	}
	
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

////////////
// pcb spacer
screenthk = 1.6;

module spacer(thk=.125*25.4){
	difference(){
		cylinder(r=6.5/2, h=thk, $fn=32);
		cylinder(r=3/2, h=20, $fn=16, center=true);
	}
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
