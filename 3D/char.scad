// Stick-figure characters
//	Stock size is 6mm H x about 5mm W

// Punctuation
depth = 1;

module char_DASH(){
  union(){										// "-"
	translate([3,1,0]) cube([.5,2,depth]);
  }
}

module char_PLUS(){
  union(){										// "+"
	translate([3,1,0]) cube([.5,2,depth]);
	translate([2.25,1.75,0]) cube([2,.5,depth]);
  }
}

//Numbers

module char_0(){
   union(){										// "0"
	translate([1,0,0]) cube([4.3,.5,depth]);
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	 translate([.7,3.0,0]) rotate([0,0,-27]) cube([5.5,.5,depth]);
  }
}

module char_1(){
  union(){										// "1"
	cube([.5,3,depth]);
	translate([0,1.3,0]) cube([6,.5,depth]);
	translate([5.65,1.42,0]) rotate([0,0,45]) cube([.5,1.3,depth]);
  }
}

module char_2(){
   union(){										// "2"
	translate([0,3.5,0]) cube([2.5,.5,depth]);
	translate([4,0,0]) cube([1.3,.5,depth]);
	translate([0,0,0]) cube([.5,4,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	}
 }
}

module char_3(){
   union(){										// "3"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([4,0,0]) cube([1.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

module char_4(){
   union(){										// "4"
	translate([0,0,0]) cube([6,.5,depth]);
	translate([4,3,0]) cube([1.75,.5,depth]);
	translate([3,0,0]) cube([.5,2.5,depth]);
	translate([4,3.5,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
 }
}

module char_5(){
   union(){										// "5"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([4,3.5,0]) cube([2.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,.5,0]) cube([.5,3,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

module char_6(){
   union(){										// "6"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

module char_7(){
   union(){										// "7"
	translate([0,1.5,0]) cube([2.7,.5,depth]);
	translate([4,0,0]) cube([1.8,.5,depth]);
	translate([5.8,.5,0]) cube([.5,2.5,depth]);
	translate([5.95,.85,0]) rotate([0,0,225]) cube([.7,.5,depth]);
	translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([6.5,2.21,0]) rotate([0,0,180]) union(center=TRUE){
		translate([3,1,0]) rotate([0,0,315]) cube([1.1,.5,depth]);
	}
 }
}

module char_8(){
   union(){										// "8"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([1,3.5,0]) cube([1.5,.5,depth]);
	translate([4,0,0]) cube([1.3,.5,depth]);
	translate([4,3.5,0]) cube([1.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

module char_9(){
   translate([6.3,4,0]) rotate([0,0,180])union(){		// "9"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

// Letters

module char_A(){
   union(){										// "A"
	translate([0,0,0]) cube([5.3,.5,depth]);
	translate([0,3.5,0]) cube([5.3,.5,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([3,.1,0]) cube([.5,3.5,depth]);
 }
}

module char_B(){
   union(){										// "B"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([0,3.5,0]) cube([5.9,.5,depth]);
	translate([4,0,0]) cube([1.3,.5,depth]);
	translate([0,1,0]) cube([.5,2.9,depth]);
	translate([5.8,1,0]) cube([.5,3,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2.9,depth]);
	translate([3,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

module char_C(){
   union(){										// "C"
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
  }
}

module char_D(){
   union(){										// "D"
	translate([1,0,0]) cube([4.3,.5,depth]);
	translate([0,3.5,0]) cube([5.9,.5,depth]);
	translate([0,1,0]) cube([.5,2.9,depth]);
	translate([5.8,1,0]) cube([.5,3,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
 }
}

module char_E(){
  union(){										// "E"
	translate([0,3.5,0]) cube([6.1,.5,depth]);
	translate([6,.5,0]) cube([.5,3.5,depth]);
	translate([2.8,1.2,0]) cube([.5,2.8,depth]);
	translate([0,.5,0]) cube([.5,3.5,depth]);
  }
}

module char_F(){
  union(){										// "F"
	translate([0,3.5,0]) cube([6.1,.5,depth]);
	translate([6,.5,0]) cube([.5,3.5,depth]);
	translate([2.8,1.2,0]) cube([.5,2.8,depth]);
  }
}

module char_G(){
  union(){										// "G"
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([1,0,0]) cube([1.51,.5,depth]);
	translate([2.5,0,0]) cube([.5,2,depth]);
  }
}

module char_H(){
  union(){										// "H"
	translate([0,0,0]) cube([6,.5,depth]);
	translate([0,3.6,0]) cube([6,.5,depth]);
	translate([3,.1,0]) cube([.5,3.5,depth]);
  }
}

module char_I(){
  union(){										// "I"
	cube([.5,3,depth]);
	translate([0,1.3,0]) cube([6,.5,depth]);
	translate([5.5,0,0]) cube([.5,3,depth]);
  }
}

module char_CIRCLEI(){
  union(){										// ICOM "I"
	translate([0,1.3,0]) cube([5.5,.7,depth]);
	translate([8.6,1.45,0]) union(){
		difference(){
			cylinder(h=depth, d=5.5, center=false, $fn=16);
			translate([0,0,-.1]) cylinder(h=2, d=4.3, center=false, $fn=16);
		}
	}
  }
}

module char_II(){
  union(){										// "I" with no top/bot
	translate([0,1.3,0]) cube([6,.5,depth]);
  }
}

module char_J(){
   union(){										// "J"
	translate([1,0,0]) cube([5,.5,depth]);
	translate([0,1,0]) cube([.5,2.5,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4.5,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
 }
}

module char_K(){
  union(){										// "K"
	translate([0,3.5,0]) cube([6.1,.5,depth]);
//	translate([3.8,0,0]) cube([1.7,.5,depth]);
//	translate([6,1,0]) cube([.5,3,depth]);
	translate([2.8,2.4,0]) cube([.5,1.25,depth]);
	translate([2.8,2.4,0]) rotate([0,0,320]) cube([3.8,.5,depth]);
//	translate([6.1,1.33,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([.1,0,0]) rotate([0,0,35]) cube([3.8,.5,depth]);
  }
}

module char_L(){
  union(){										// "L"
	translate([0,3.5,0]) cube([6.1,.5,depth]);
	translate([0,.5,0]) cube([.5,3.5,depth]);
  }
}

module char_M(){
  union(){										// "M"
	cube([6,.5,depth]);
	translate([6,.5,0]) rotate([0,0,135]) cube([3.4,.5,depth]);
	translate([5.61,5.34,0]) rotate([0,0,225]) cube([3.65,.5,depth]);
	translate([0,5,0]) cube([6,.5,depth]);
  }
}

module char_N(){
  union(){										// "N"
	cube([6,.5,depth]);
//	translate([6,.5,0]) rotate([0,0,135]) cube([3.4,.5,depth]);
	translate([5.71,4.44,0]) rotate([0,0,215]) cube([6.97,.5,depth]);
	translate([0,4,0]) cube([6,.5,depth]);
  }
}

module char_O(){
   union(){										// "O"
	translate([1,0,0]) cube([4.3,.5,depth]);
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
  }
}

module char_P(){
  union(){										// "P"
	translate([0,3.5,0]) cube([6.1,.5,depth]);
	translate([3.8,0,0]) cube([1.7,.5,depth]);
	translate([6,1,0]) cube([.5,3,depth]);
	translate([2.8,1,0]) cube([.5,3,depth]);
	translate([2.8,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([6.1,1.33,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
//	translate([0,0,0]) rotate([0,0,20]) cube([3.3,.5,depth]);
  }
}

module char_Q(){
   union(){										// "Q"
	translate([1,0,0]) cube([4.3,.5,depth]);
	translate([1,3.5,0]) cube([4.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([1.2,1.6,0]) rotate([0,0,225]) cube([2,.5,depth]);
  }
}

module char_R(){
  union(){										// "R"
	translate([0,3.5,0]) cube([6.1,.5,depth]);
	translate([3.8,0,0]) cube([1.7,.5,depth]);
	translate([6,1,0]) cube([.5,3,depth]);
	translate([2.8,1,0]) cube([.5,3,depth]);
	translate([2.8,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([6.1,1.33,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([0,0,0]) rotate([0,0,20]) cube([3.3,.5,depth]);
  }
}

module char_S(){
   union(){										// "S"
	translate([1,0,0]) cube([1.5,.5,depth]);
	translate([4,3.5,0]) cube([1.3,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([5.8,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([5.95,1.35,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([3,1,0]) cube([.5,2,depth]);
	translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	translate([6.5,4,0]) rotate([0,0,180]) union(center=TRUE){
		translate([4,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
	}
 }
}

module char_T(){
  union(){										// "T"
	translate([0,2.25,0]) cube([6,.5,depth]);
	translate([5.5,0,0]) cube([.5,5,depth]);
  }
}

module char_U(){
   union(){										// "U"
	translate([1,0,0]) cube([5.2,.5,depth]);
	translate([1,3.5,0]) cube([5.2,.5,depth]);
	translate([0,1,0]) cube([.5,2,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([1,4,0]) rotate([0,0,225]) cube([1.4,.5,depth]);
  }
}

module char_V(){
   union(){										// "V"
	translate([1.5,0,0]) cube([4.2,.5,depth]);
	translate([2,3.5,0]) cube([4.2,.5,depth]);
	translate([0,2,0]) rotate([0,0,315]) cube([2.8,.5,depth]);
	translate([2,4,0]) rotate([0,0,225]) cube([2.8,.5,depth]);
  }
}

module char_W(){
   union(){										// "W"
	translate([1.5,0,0]) cube([4.5,.5,depth]);
	translate([1.5,2.5,0]) cube([3.5,.5,depth]);
	translate([1.5,4.88,0]) cube([4.5,.5,depth]);
	translate([0,1.5,0]) rotate([0,0,315]) cube([2.15,.5,depth]);
	translate([.35,1.14,0]) rotate([0,0,45]) cube([2.15,.5,depth]);
	translate([0,3.9,0]) rotate([0,0,315]) cube([2.15,.5,depth]);
	translate([.35,3.5,0]) rotate([0,0,45]) cube([2.15,.5,depth]);
  }
}

module char_X(){
   union(){										// "X"
	translate([0,0,0]) rotate([0,0,30]) cube([7,.5,depth]);
	translate([-.28,3.5,0]) rotate([0,0,-30]) cube([7,.5,depth]);
  }
}

module char_Y(){
   union(){										// "Y"
	translate([0,1.75,0]) cube([3.7,.5,depth]);
	translate([3,1.9,0]) rotate([0,0,320]) cube([3.5,.5,depth]);
	translate([5.70,4.30,0]) rotate([0,0,220]) cube([3.5,.5,depth]);
  }
}

module char_Z(){
   union(){										// "Z"
	translate([0,4,0]) rotate([0,0,325]) cube([7,.5,depth]);
	translate([0,1,0]) cube([.5,3,depth]);
	translate([5.8,0,0]) cube([.5,3,depth]);
	translate([0,1,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
	translate([4.95,3.66,0]) rotate([0,0,315]) cube([1.4,.5,depth]);
  }
}
