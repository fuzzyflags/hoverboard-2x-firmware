#ifndef DEFINES_H
#define DEFINES_H

#define NOP 0
#define RSP 1
#define RES 2
#define ENA 3
#define DIS 4
#define POW 5
#define SPE 6
#define ABS 7
#define REL 8
#define DOG 9
#define MOD 10
#define XXX 0xFF

#define SPEED_PF 0
#define SPEED_STEP 1
#define SPEED_DUAL 2

// Unmodified HOVER-1 Ultra Dimensions
#define WHEEL_BASE			 408
#define MM_PER_DEGREE 	   3.508
#define DEGREE_PER_MM 	  0.2851
#define	TOP_SPEED 			5000
#define	TOP_TURN			1425

#define NOR 0
#define SMOT 1
#define SPOW 2
#define SSPE 3
#define SPOS 4
#define SVOL 5
#define SAMP 6
#define SDOG 7
#define SMOD 8
#define SFPI 9
#define STOP 0xFF

unsigned char	REScommand[]    = {'/', 0,  0, RES, SMOD,         		     0,   0, '\n'};
unsigned char	MODcommand[]    = {'/', 2,  0, MOD, SMOD,   SPEED_DUAL, 250, 0,   0, '\n'};
unsigned char	PWRcommand[]    = {'/', 2,  0, POW, SMOT,   200, 0, 		     0,   0, '\n'};
unsigned char	SPEcommand[]    = {'/', 2,  0, SPE, SMOT,   0, 0, 		     0,   0, '\n'};
unsigned char	XXXcommand[]    = {'/', 0,  0, XXX, SMOT,         		     0,   0, '\n'};

#endif