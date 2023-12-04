#include <Arduino.h>

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

char	REScommand[]    = {'/', 0,  0, RES, SMOD,         		     0,   0, '\n'};
char	MODcommand[]    = {'/', 2,  0, MOD, SMOD,   SPEED_DUAL, 250, 0,   0, '\n'};
char	PWRcommand[]    = {'/', 2,  0, POW, SMOT,   50, 0, 		     0,   0, '\n'};
char	SPEcommand[]    = {'/', 2,  0, SPE, SMOT,   0, 0, 		     0,   0, '\n'};
char	XXXcommand[]    = {'/', 0,  0, XXX, SMOT,         		     0,   0, '\n'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600);
  delay(5000);
  Serial.println("Hello from setup!");
  Serial1.begin(115200, SERIAL_8N1, 18, 17);
  Serial.println("Hello after configure!");
  //Serial1.write(REScommand, sizeof(REScommand));
  //Serial1.write(MODcommand, sizeof(MODcommand));
}

void loop() {
  Serial.println("Sending speed command:");
  Serial1.write(MODcommand, sizeof(MODcommand));
  delay(200);
  Serial1.write(PWRcommand, sizeof(PWRcommand));
  // Serial.println(Serial1.readString());
  delay(50);
}

/*
void loop() {
  Serial.println("LOOP:");
  Serial.println(Serial1.readString());
  delay(1000);
  Serial1.println("Hello from A!");
  delay(1000);
}
*/