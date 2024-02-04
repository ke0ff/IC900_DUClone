// OpenSCAD 2019.05
// LCD Case - enclosure, REV-001
// Joe Haas, KE0FF, 1/1/2024
// This is the IC900 controller clone A-Unit enclosure
//	This houses the RJ-45 and DSUB-9 and MFMIC pcbs
//
// Rev-001, 1/1/2024
//	initial code

////////////////////////////////////////////////////////////////////////////////
// plot directives.  Enable/disable plot features:
		
button = 0;			// button caps, 1 = normal, 2 = short (power)
vqbtn = 1;			// v/q button
sidesel = 0;		// main case, 1 = lcd side, 2 = button side, 3 = both
backsel = 0;		// back cover, 1 = lcd side, 2 = button side, 3 = both
dopcb = 0;			// display PCB for fit-check
doeclip = 0;		// back-side clip piece
dopiezo = 0;		// piezo cover
dordu = 0;			// RDU PCB for fit-check

////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
// User defined parameters.  Modify these to suit a particular application
// NOTE: All data in this file is in mm
//----------------------------------------------------------------------------------------------------------------------
// parametric variables:

in2mm = 25.4;

rj45x = .625 * in2mm;
rj45y = .65 * in2mm;
rj45z = .53 * in2mm;
clr = 0.5;
ds9x = 1.22 * in2mm;
ds9y = .49 * in2mm;
ds9z = .49 * in2mm;
ds9ox = .75 * in2mm;
ds9oy = .22 * in2mm;
ds9oz = .49 * in2mm;
ds9mtgx = .99 * in2mm;
ds9mtgz = .99 * in2mm;

sipx = 23;
sipy = 2.5;
sipz = 11;

wally = 3;
wall = 3;
zwall = 2;
cham = 2;
bcham = 4;
tcham = 1.25;

pcb_x = (2.6 * in2mm);
pcb_y = (1 * in2mm);
pcb_z = rj45z + 5;
pcbthk = 1.6;

pcb2_x = 52;
pcb2_y = 20.5;
pcb2_z = 4;
pcb2thk = .8;

box_x = pcb_x+1 + (2*wall)+3;
box_y = pcb_y+1 + (2*wally);
box_z = rj45z + 0.5 + 4;

threadia = 0.065 * in2mm;
dia2 = .095*in2mm;
mtgdia = 0.125 * in2mm;
jackdia = 5.9;
jackwall = 1;
bosdia = .25*in2mm;
hddia = 4.1;
hdht = hddia/2;

topthk = 8;
cbldia = 5;
cbldia2 = 6;


///////////////////////////////////////////////////////////////////////////////
// plot directive processing //////////////////////////////////////////////////

bottom_on = 0;
top_on = 1;
clamp_top = 0;
clamp_bot = 0;

if(bottom_on){
	difference(){
		bottom();
//		translate([-15,30,-10]) cube([200,30,100]);	// debug section
	}
}

if(top_on){
	difference(){
		translate([0,0,box_z+.1]) top();
//		translate([-15,20,-10]) cube([200,100,100]);	// debug section
	}
}

if(clamp_top){
	difference(){
		translate([0,0,0]) clamp(top=1);
//		translate([-15,20,-10]) cube([200,100,100]);	// debug section
	}
}

if(clamp_bot){
	difference(){
		translate([0,0,0]) clamp(top=0);
//		translate([-15,20,-10]) cube([200,100,100]);	// debug section
	}
}

// TEST PRINTS ////////////////
//translate([wall+(.1*in2mm)+.5+1.5,wall+.5,zwall+1+4]) pcb(model=1);

//cable();
//screw();

//csink();

// corner spacer for a-unit PCB
//spacer();

////////////////////////////////////////////////////////////////////////////////////
module bottom(){

	translate([.1,box_y/2,box_z-(cbldia2/2)]) cable(top=0);
	difference(){
		union(){
			difference(){
				cube([box_x, box_y, box_z]);
				translate([wall+1.5,wall,zwall]) cube([pcb_x+1,pcb_y+1,pcb_z]);
				// corner chams
				rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
				translate([box_x,0,0]) rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
				translate([0,box_y,0]) rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
				translate([box_x,box_y,0]) rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
				// bottom chams
				rotate([45,0,0]) cube([3*box_x,bcham,bcham], center=true);
				translate([0,box_y,0]) rotate([45,0,0]) cube([3*box_x,bcham,bcham], center=true);
				rotate([0,45,0]) cube([bcham,3*box_y,bcham], center=true);
				translate([box_x,0,0]) rotate([0,45,0]) cube([bcham,3*box_y,bcham], center=true);
			}
			// mtg bosses
			translate([wall+(.1*in2mm)+.5+1.5,wall+.5,-.1]){
				translate([.15*in2mm,(.15*in2mm)+.8,zwall]) cylinder(r=bosdia/2, h=5, $fn=32);
				translate([.15*in2mm,(.85*in2mm)+.8,zwall]) cylinder(r=bosdia/2, h=5, $fn=32);
				translate([1.820*in2mm,(0.606*in2mm)+1.5,zwall]) cylinder(r=bosdia/2, h=5, $fn=32);
				translate([.836*in2mm,(0.606*in2mm)+(bosdia/2)+2,zwall]) cylinder(r=bosdia/2, h=5, $fn=32);
			}
		}
		// cable opening
		translate([0,box_y/2,box_z-(cbldia2/2)]) cable(top=0, model=0);
		translate([0,box_y/2,box_z]) cube([20,cbldia2,cbldia2],center=true);
		// pcb features
		translate([wall+(.1*in2mm)+.5+1.5,wall+.5,zwall+1+4]) pcb(model=0, depth=5.95);
		translate([0,0,box_z]) top(model=0);
	}
}

module top(model=1){
mtgoffs = 3;

	if(model==0){
		// mtg holes
		translate([(wall+mtgoffs)/2,wall,0]) cylinder(r=threadia/2, h=20, $fn=16, center=true);
		translate([(wall+mtgoffs)/2,box_y-wall,0]) cylinder(r=threadia/2, h=20, $fn=16, center=true);
		translate([box_x-((wall+mtgoffs)/2),wall,0]) cylinder(r=threadia/2, h=20, $fn=16, center=true);
		translate([box_x-((wall+mtgoffs)/2),box_y-wall,0]) cylinder(r=threadia/2, h=20, $fn=16, center=true);

	}else{
		difference(){
			union(){
				translate([0,0,0]) cube([box_x, box_y, topthk]);
				translate([.1,box_y/2,-(cbldia2/2)]) cable(top=1);
			}
			// cable skirt trim
			translate([-.1,0,-box_z/2-.01]) cube([10,box_y,box_z/2]);
			// inside hog-out
			translate([wall+1.5,wally,-.1]) cube([pcb_x+1,pcb_y+1,topthk-2+.2]);
			// corner chams
			rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
			translate([box_x,0,0]) rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
			translate([0,box_y,0]) rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
			translate([box_x,box_y,0]) rotate([0,0,45]) cube([cham,cham,3*box_z], center=true);
			// top chams
			translate([0,0,topthk]) rotate([45,0,0]) cube([3*box_x,tcham,tcham], center=true);
			translate([0,box_y,topthk]) rotate([45,0,0]) cube([3*box_x,tcham,tcham], center=true);
			translate([0,0,topthk]) rotate([0,45,0]) cube([tcham,3*box_y,tcham], center=true);
			translate([box_x,0,topthk]) rotate([0,45,0]) cube([tcham,3*box_y,tcham], center=true);
			// csinks
			translate([(wall+mtgoffs)/2,wall,topthk-1.1]) csink();
			translate([(wall+mtgoffs)/2,box_y-wall,topthk-1.1]) csink();
			translate([box_x-((wall+mtgoffs)/2),wall,topthk-1.1]) csink();
			translate([box_x-((wall+mtgoffs)/2),box_y-wall,topthk-1.1]) csink();
			// mtg holes
			translate([(wall+mtgoffs)/2,wall,0]) cylinder(r=dia2/2, h=15, $fn=16, center=true);
			translate([(wall+mtgoffs)/2,box_y-wall,0]) cylinder(r=dia2/2, h=15, $fn=16, center=true);
			translate([box_x-((wall+mtgoffs)/2),wall,0]) cylinder(r=dia2/2, h=15, $fn=16, center=true);
			translate([box_x-((wall+mtgoffs)/2),box_y-wall,0]) cylinder(r=dia2/2, h=15, $fn=16, center=true);
			// pcb features
			translate([wall+(.1*in2mm)+.5+1.5,wall+.5,zwall+1+4-box_z]) pcb(model=0, depth=2.95);
		}
	}
}


module clamp(top=0){
	difference(){
		// main body
		cube([20,22,(9.5*2)-2]);
		// corner cham
		translate([4.6,-.1,-5]) rotate([0,0,104.3]) cube([40,20,9.5*3]);
		// cable tunnel
		translate([4.6+8.8+1.4,-5,9.5]) rotate([0,0,14.3]) rotate([-90,0,0]) cylinder(r=5.5/2, h=40, $fn=16);
		// mtg holes
		translate([16,13,9.5]) rotate([180,0,0]) screw(csdeep=9.5, thread=4, sink=0);
		translate([3.5,17,9.5]) rotate([180,0,0]) screw(csdeep=9.5, thread=4, sink=0);
		translate([6,7,9.5]) rotate([180,0,0]) screw(csdeep=9.5, thread=4, sink=0);
		// far-notch
		translate([-.1,22.1-4.5,-5]) cube([.9,4.5,30]);
		// near-cham
		translate([20,0,0]) rotate([0,0,45]) cube([4.5,4.5,45],center=true);
		// top/bottom section
		if(top){
			translate([-1,-1,9.5]) cube([30,30,20]);
		}else{
			translate([-1,-1,-20+9.5]) cube([30,30,20]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

module csink(){

	cylinder(r2=hddia/2, r1=0, h=hdht, $fn=16, center=true);
	translate([0,0,(hdht/2)-.01]) cylinder(r=hddia/2, h=10, $fn=16);

}

module spacer(){
//	translate([0,0,-4.8-1.6])cube([3,3,12.5]);
	difference(){
		cylinder(r=7.5/2, h=11.1, $fn=32);
		translate([0,0,12.5-4.8-1.6-(hdht/2)]) csink();
		translate([0,0,0]) cylinder(r=dia2/2, h=15, $fn=16, center=true);
//		cube([10,10,20]);
	}
}


module cable(top=0, model=1){

	if(model){
		rotate([0,-90,0]) difference(){
			union(){
				cylinder(r2=7, r1=10, h=20, $fn=64);
				translate([0,0,20]) cylinder(r2=5, r1=7, h=5, $fn=64);
			}
			// cable opening
			cylinder(r=cbldia/2, h=60, $fn=16, center=true);
			translate([0,0,25.01]) cylinder(r2=(cbldia2+1)/2, r1=cbldia/2, h=2, $fn=16, center=true);
			// cable clearance
			translate([0,0,10-1.1]) cylinder(r1=cbldia2/2, r2=cbldia/2, h=2, $fn=16, center=true);
			translate([0,0,-2]) cylinder(r=cbldia2/2, h=20, $fn=16, center=true);
			// screws
			translate([0,4.8,12]) rotate([0,90,0]) screw(csdeep=4);
			translate([0,-4.8,12]) rotate([0,90,0]) screw(csdeep=4);
			// top/bot section
			if(top==0){
				translate([0,-25,-30]) cube([50,50,60]);
			}else{
				translate([-50,-25,-30]) cube([50,50,60]);
			}
		}
	}else{
		// cable hole for difference()
		rotate([0,-90,0]) cylinder(r=cbldia2/2, h=20, $fn=16, center=true);
		rotate([0,-90,0]) translate([0,0,-3.55]) cylinder(r1=(cbldia2+2)/2, r2=cbldia2/2, h=2, $fn=16, center=true);
	}
}

module screw(csdeep=4, thread=2, sink=1){
	tdia2 = (0.065 * in2mm) + .25;
	cdia2 = (.095*in2mm) + .5;
	hdia2 = 4.2;
	tdia4 = (0.08 * in2mm) + .25;
	cdia4 = .125*in2mm;
	hdia4 = 5.5;

	if(thread==2){
		cylinder(r=tdia2/2, h=30, $fn=16, center=true);
		translate([0,0,-.02]) cylinder(r=cdia2/2, h=15, $fn=16);
		if(sink){
			translate([0,0,csdeep]){
				cylinder(r=hdia2/2, h=15, $fn=16);
				translate([0,0,-hdht+.01]) cylinder(r1=0, r2=hdia2/2, h=hdht, $fn=16);
			}
		}
	}
	if(thread==4){
		cylinder(r=tdia4/2, h=30, $fn=16, center=true);
		translate([0,0,-.02]) cylinder(r=cdia4/2, h=15, $fn=16);
		if(sink){
			translate([0,0,csdeep]){
				cylinder(r=hdia4/2, h=15, $fn=16);
				translate([0,0,-hdht+.01]) cylinder(r1=0, r2=hdia4/2, h=hdht, $fn=16);
			}
		}
	}
}

module pcb(model=0, depth=2){
	ddeep = depth+1.62;
	if(model==0){
		// mtg holes
		translate([.15*in2mm,(.15*in2mm)+.8,pcbthk]) cylinder(r=threadia/2, h=2*ddeep, $fn=16, center=true);
		translate([.15*in2mm,(.85*in2mm)+.8,pcbthk]) cylinder(r=threadia/2, h=2*ddeep, $fn=16, center=true);
		translate([1.820*in2mm,(0.606*in2mm)+1.5,pcbthk]) cylinder(r=threadia/2, h=2*ddeep, $fn=16, center=true);
//		translate([.836*in2mm,0.606*in2mm,pcbthk]) cylinder(r=threadia/2, h=2*ddeep, $fn=16, center=true);
		// rj45
		translate([(2.001*in2mm)-clr,(.17*in2mm)-clr,pcbthk]) cube([rj45x+(2*clr)+10,rj45y+(2*clr),rj45z+(clr)]);
		translate([(2.001*in2mm)-clr,(.17*in2mm)-clr,-pcbthk]) cube([rj45x+(2*clr)-3.8,rj45y+(2*clr),rj45z+(clr)]);
		// rj45 chams
		translate([box_x-9.8,(box_y/2)-5.4,-.4+(box_z/2)]) rotate([0,90,0]) rotate([0,0,45]) cylinder(r1=1.15*rj45y/2, r2=1.15*(rj45y+4)/2, h=3, $fn=4);
		translate([box_x-9.8,(box_y/2)-1.9,-.4+(box_z/2)]) rotate([0,90,0]) rotate([0,0,45]) cylinder(r1=1.15*rj45y/2, r2=1.15*(rj45y+4)/2, h=3, $fn=4);
		// Dsub
		translate([0,1.2,0]){
			translate([(.712*in2mm)-clr,(.48*in2mm)-clr,pcbthk]) cube([ds9x+(2*clr),ds9y+(2*clr),ds9z+(clr)]);
			translate([1.3285*in2mm,(.488*in2mm)+(.48*in2mm)-.1,(.246*in2mm)+pcbthk]){
				translate([0,(ds9oy+10)/2,0]) cube([ds9ox,ds9oy+10,ds9oz], center=true);
				translate([ds9mtgx/2,0,0]) rotate([90,0,0]) cylinder(r=mtgdia/2, h=25, $fn=16, center=true);
				translate([-ds9mtgx/2,0,0]) rotate([90,0,0]) cylinder(r=mtgdia/2, h=25, $fn=16, center=true);
				translate([ds9mtgx/2,ds9y-(3*clr)+jackwall-.2,0]) rotate([90,0,0]) cylinder(r=jackdia/2, h=10, $fn=16);
				translate([-ds9mtgx/2,ds9y-(3*clr)+jackwall-.2,0]) rotate([90,0,0]) cylinder(r=jackdia/2, h=10, $fn=16);
			}
		}
		// pcb2 P3
		translate([-.1*in2mm+3.5,15-2,pcbthk+sipz+pcb2thk]) cube([17,9,5]);
		// pcb
		translate([-.1*in2mm,0,0]) cube([pcb_x,pcb_y,pcbthk+.02]);
	}else{
		difference(){
			union(){
				// rj45
				translate([(2.001*in2mm),(.17*in2mm),pcbthk]) cube([rj45x,rj45y,rj45z]);
				// Dsub
				translate([0,1.2,0]){
					translate([(.712*in2mm)-clr,(.48*in2mm)-clr+ds9y-4.1,pcbthk]) cube([ds9x+(2*clr),4.1+(2*clr),ds9z+(clr)]);
					translate([(.712*in2mm)-clr+6.5,(.48*in2mm)-clr,pcbthk]) cube([17.5,ds9y-4.1+(2*clr),ds9z+(clr)-2.2]);
					translate([1.3285*in2mm,(.488*in2mm)+(.48*in2mm)-.1,(.246*in2mm)+pcbthk]){
						translate([0,(ds9oy)/2,0]) cube([ds9ox-2,ds9oy,ds9oz-2], center=true);
						translate([ds9mtgx/2,0,0]) rotate([90,0,0]) cylinder(r=5.3/2, h=13, $fn=6, center=true);
						translate([-ds9mtgx/2,0,0]) rotate([90,0,0]) cylinder(r=5.3/2, h=13, $fn=6, center=true);
					}
				}
				// pcb
				translate([-.1*in2mm,0,0]) cube([pcb_x,pcb_y,pcbthk+.02]);
			}
			// mtg holes
			translate([.15*in2mm,.15*in2mm,pcbthk]) cylinder(r=mtgdia/2, h=2*ddeep, $fn=16, center=true);
			translate([.15*in2mm,.85*in2mm,pcbthk]) cylinder(r=mtgdia/2, h=2*ddeep, $fn=16, center=true);
			translate([1.820*in2mm,0.606*in2mm,pcbthk]) cylinder(r=mtgdia/2, h=2*ddeep, $fn=16, center=true);
			translate([.836*in2mm,0.606*in2mm,pcbthk]) cylinder(r=mtgdia/2, h=2*ddeep, $fn=16, center=true);
		}
		// pcb2 hdr
		translate([(.9-.1)*in2mm,.022*in2mm,pcbthk]) cube([sipx,sipy,sipz]);
		// pcb2
		translate([-.1*in2mm,0,pcbthk+sipz]) cube([pcb2_x,pcb2_y,pcb2thk+.02]);
		// pcb2 P3
		translate([-.1*in2mm+4.5,15,pcbthk+sipz+pcb2thk]) cube([14.3,4.6,4.5]);
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
