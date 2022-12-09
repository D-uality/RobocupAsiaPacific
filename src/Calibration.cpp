// ------------------------------------------------------------------------------------------ INLCUDES

#include <Arduino.h>
#include <EEPROM.h>

/*
	The EEPROM, Electrically Erasable Programmable Read-Only-Memory is included on the Arduino Mega. Th
	is chip is ROM that can be easily changed through the use of code. The Arduino Mega has an EEPROM t
	hat has a size of 4 KB, more than enough for storing a couple numbers. 
	
	However, the EEPROM can only store 8-bit numbers, and our analog colour sensors returns 10-bit numb
	ers. This means that we will need to use 2 addresses to store 1 calibration variable.

	We have 2 calibration numbers (Minimum, Maximum), and 6 sensors in total.
*/

#include "BaseFunctions.h"

// ------------------------------------------------------------------------------------------ FUNCTIONS	

void CalibrateCSensorV(int LBound, int UBound) {
	int InNum[6][2][2];
	int CalibrationNum[6][2];

	for(int SensorNum=0; SensorNum<6; SensorNum++) {													// For each sensor
		for(int Extremity=0; Extremity<2; Extremity++) {												// For each extremity
			for(int i=0; i<2; i++){
				InNum[SensorNum][Extremity][i] = EEPROM.read(4*SensorNum + 2*Extremity + i);			// Read from EEPROM with algorithm to expand from an array (linear addresses), to a 4d array
			}

			InNum[SensorNum][Extremity][1] = InNum[SensorNum][Extremity][1] * 256;						// Multiply the second part of each number by 256, as the 8th bit is 2^8 is 256
																										// We do not need to worry about the 9th bit, as our values will never be max value 

			CalibrationNum[SensorNum][Extremity] = InNum[SensorNum][Extremity][0] + InNum[SensorNum][Extremity][1];
																										// Add both 8-bit numbers together to receive the original 10-bit number
		}
		CSensorV[SensorNum] = map(CSensorV[SensorNum], CalibrationNum[SensorNum][1], CalibrationNum[SensorNum][0], LBound, UBound);
																										// Using these values, we can map and create consistency
	}
}

void Calibrate() {
																										// Defined variables locally, as having them global would mean that they will take up RAM on the Arduino board.
																										// Force defined all the variables to have placeholder values, so calculations will not go wrong.
	int CSensorExtremity[6][2] = {{512, 512}, {512, 512}, {512, 512}, {512, 512}, {512, 512}, {512, 512}};
	int bits[6][2][10] = {{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
	int bytes[6][2][2][8] = {{{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, };
	int OutNum[6][2][2] = {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}};
	char Buffer[100];
	String IncomingByte = "";

	while(mode == 2) {																					// New subroutine
		Serial.print("Calibrating");

		if(Serial.available() > 0) {
			Serial.print("Reading BYTE");
			IncomingByte = Serial.readString();

			if(IncomingByte == "e") {																	// Exit Calibration
				
				
				mode = 0;


			} else if(IncomingByte == "w") {															// Write to EEPROM
				
				
				Run(0, 0);
				Serial.println();

				for(int SensorNum=0; SensorNum<6; SensorNum++) {										// For each sensor
					for(int Extremity=0; Extremity<2; Extremity++) {									// For each extremity

						for(int bit=9; bit>=0; bit--) {													// Convert into binary, stored in arrarys
							bits[SensorNum][Extremity][9-bit] = (CSensorExtremity[SensorNum][Extremity] & (1 << bit)) != 0;
						}

																										// Assign into 2 8-bit numbers
						bytes[SensorNum][Extremity][1][6] = bits[SensorNum][Extremity][0];
						bytes[SensorNum][Extremity][1][7] = bits[SensorNum][Extremity][1];
						bytes[SensorNum][Extremity][0][0] = bits[SensorNum][Extremity][2];
						bytes[SensorNum][Extremity][0][1] = bits[SensorNum][Extremity][3];
						bytes[SensorNum][Extremity][0][2] = bits[SensorNum][Extremity][4];
						bytes[SensorNum][Extremity][0][3] = bits[SensorNum][Extremity][5];
						bytes[SensorNum][Extremity][0][4] = bits[SensorNum][Extremity][6];
						bytes[SensorNum][Extremity][0][5] = bits[SensorNum][Extremity][7];
						bytes[SensorNum][Extremity][0][6] = bits[SensorNum][Extremity][8];
						bytes[SensorNum][Extremity][0][7] = bits[SensorNum][Extremity][9];
						
						for(int i=0; i<2; i++) {														// For both 8-bit numbers
							for(int j=0; j<8; j++) {													// Convert into decimal
								OutNum[SensorNum][Extremity][i] += (1<<j) * bytes[SensorNum][Extremity][i][7-j];
							}
						}
																										// Print for debugging
						sprintf(Buffer, "\t\t\t%d = %d%d%d%d%d%d%d%d%d%d = %d%d%d%d%d%d%d%d + %d%d%d%d%d%d%d%d = %d + %d", CSensorExtremity[SensorNum][Extremity], bits[SensorNum][Extremity][0], bits[SensorNum][Extremity][1], bits[SensorNum][Extremity][2], bits[SensorNum][Extremity][3], bits[SensorNum][Extremity][4], bits[SensorNum][Extremity][5], bits[SensorNum][Extremity][6], bits[SensorNum][Extremity][7], bits[SensorNum][Extremity][8], bits[SensorNum][Extremity][9], bytes[SensorNum][Extremity][0][0], bytes[SensorNum][Extremity][0][1], bytes[SensorNum][Extremity][0][2], bytes[SensorNum][Extremity][0][3], bytes[SensorNum][Extremity][0][4], bytes[SensorNum][Extremity][0][5], bytes[SensorNum][Extremity][0][6], bytes[SensorNum][Extremity][0][7], bytes[SensorNum][Extremity][1][0], bytes[SensorNum][Extremity][1][1], bytes[SensorNum][Extremity][1][2], bytes[SensorNum][Extremity][1][3], bytes[SensorNum][Extremity][1][4], bytes[SensorNum][Extremity][1][5], bytes[SensorNum][Extremity][1][6], bytes[SensorNum][Extremity][1][7], OutNum[SensorNum][Extremity][0], OutNum[SensorNum][Extremity][1]);
						Serial.println(Buffer);
					}
				}

				for(int SensorNum=0; SensorNum<6; SensorNum++) {										// Write to EEPROM
					for(int Extremity=0; Extremity<2; Extremity++) {
						for(int i=0; i<2; i++) {
							EEPROM.write(4*SensorNum + 2*Extremity + i, OutNum[SensorNum][Extremity][i]);		// Compression from a 3d array into a number, 1d array.
						}
					}
					Serial.println();
				}

				mode = 0;
				delay(3000);																			// Pause to check debugging
			}


		} else {																						// Read Colour Sensors


			for(int SensorNum=0; SensorNum<6; SensorNum++) {
				CSensorV[SensorNum] = analogRead(CSensors[SensorNum]);
				if(SensorNum==0 || SensorNum==1 || SensorNum==2) CSensorV[SensorNum] = map(CSensorV[SensorNum], 0, 1023, 1023, 0);

				if(CSensorV[SensorNum] > CSensorExtremity[SensorNum][0]) CSensorExtremity[SensorNum][0] = CSensorV[SensorNum];			// Check if there is a new maximum
				if(CSensorV[SensorNum] < CSensorExtremity[SensorNum][1]) CSensorExtremity[SensorNum][1] = CSensorV[SensorNum];			// Check if there is a new minimum
			}

																										// Print for debugging
			sprintf(Buffer, "\t\t%d %d %d %d %d %d", CSensorExtremity[0][0], CSensorExtremity[1][0], CSensorExtremity[2][0], CSensorExtremity[3][0], CSensorExtremity[4][0], CSensorExtremity[5][0]);
			Serial.print(Buffer);
			sprintf(Buffer, "\t\t%d %d %d %d %d %d", CSensorExtremity[0][1], CSensorExtremity[1][1], CSensorExtremity[2][1], CSensorExtremity[3][1], CSensorExtremity[4][1], CSensorExtremity[5][1]);
			Serial.println(Buffer);

			Run(-100, 100);

			
		}
	}
} 