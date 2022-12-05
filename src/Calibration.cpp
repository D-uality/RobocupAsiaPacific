#include <Arduino.h>
#include <EEPROM.h>
#include "BaseFunctions.h"

void CalibrateCSensorV(int LBound, int UBound) {
	int InNum[6][2][2];
	int CalibrationNum[6][2];

	for(int SensorNum=0; SensorNum<6; SensorNum++) {
		for(int Extremity=0; Extremity<2; Extremity++) {
			for(int i=0; i<2; i++){
				InNum[SensorNum][Extremity][i] = EEPROM.read(4*SensorNum + 2*Extremity + i);
			}

			InNum[SensorNum][Extremity][1] = InNum[SensorNum][Extremity][1] * 256;

			CalibrationNum[SensorNum][Extremity] = InNum[SensorNum][Extremity][0] + InNum[SensorNum][Extremity][1];
		}
		CSensorV[SensorNum] = map(CSensorV[SensorNum], CalibrationNum[SensorNum][1], CalibrationNum[SensorNum][0], LBound, UBound);
	}
}

void Calibrate() {
	int CSensorExtremity[6][2] = {{500, 500}, {500, 500}, {500, 500}, {500, 500}, {500, 500}, {500, 500}};
	int bits[6][2][10] = {{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
	int bytes[6][2][2][8] = {{{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}, };
	int OutNum[6][2][2] = {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}};
	char Buffer[100];
	String IncomingByte = "";

	while(mode == 2) {
		Serial.print("Calibrating");

		if(Serial.available() > 0) {
			Serial.print("Reading BYTE");
			IncomingByte = Serial.readString();

			if(IncomingByte == "e") {
				// Exit Calibration
				mode = 0;
			} else if(IncomingByte == "w") {
				Run(0, 0);
				Serial.println();

				for(int SensorNum=0; SensorNum<6; SensorNum++) {
					for(int Extremity=0; Extremity<2; Extremity++) {

						// Convert into 10 bit binary
						for(int bit=9; bit>=0; bit--) {
							bits[SensorNum][Extremity][9-bit] = (CSensorExtremity[SensorNum][Extremity] & (1 << bit)) != 0;
						}

						// Assign into 2 x bytes
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
					
						// Convert into decimal
						for(int i=0; i<2; i++) {
							for(int j=0; j<8; j++) {
								OutNum[SensorNum][Extremity][i] += (1<<j) * bytes[SensorNum][Extremity][i][7-j];
							}
						}

						sprintf(Buffer, "\t\t\t%d = %d%d%d%d%d%d%d%d%d%d = %d%d%d%d%d%d%d%d + %d%d%d%d%d%d%d%d = %d + %d", CSensorExtremity[SensorNum][Extremity],bits[SensorNum][Extremity][0], bits[SensorNum][Extremity][1], bits[SensorNum][Extremity][2], bits[SensorNum][Extremity][3], bits[SensorNum][Extremity][4], bits[SensorNum][Extremity][5], bits[SensorNum][Extremity][6], bits[SensorNum][Extremity][7], bits[SensorNum][Extremity][8], bits[SensorNum][Extremity][9], bytes[SensorNum][Extremity][0][0], bytes[SensorNum][Extremity][0][1], bytes[SensorNum][Extremity][0][2], bytes[SensorNum][Extremity][0][3], bytes[SensorNum][Extremity][0][4], bytes[SensorNum][Extremity][0][5], bytes[SensorNum][Extremity][0][6], bytes[SensorNum][Extremity][0][7], bytes[SensorNum][Extremity][1][0], bytes[SensorNum][Extremity][1][1], bytes[SensorNum][Extremity][1][2], bytes[SensorNum][Extremity][1][3], bytes[SensorNum][Extremity][1][4], bytes[SensorNum][Extremity][1][5], bytes[SensorNum][Extremity][1][6], bytes[SensorNum][Extremity][1][7], OutNum[SensorNum][Extremity][0], OutNum[SensorNum][Extremity][1]);
						Serial.println(Buffer);
					}
				}

				// Write to EEPROM
				for(int SensorNum=0; SensorNum<6; SensorNum++) {
					for(int Extremity=0; Extremity<2; Extremity++) {
						for(int i=0; i<2; i++) {
							EEPROM.write(4*SensorNum + 2*Extremity + i, OutNum[SensorNum][Extremity][i]);
						}
					}
					Serial.println();
				}

				mode = 0;
				delay(3000);
			}
		} else {
			// Read Colour Sensors

			for(int SensorNum=0; SensorNum<6; SensorNum++) {
				CSensorV[SensorNum] = analogRead(CSensors[SensorNum]);
				if(SensorNum==0 || SensorNum==1 || SensorNum==2) CSensorV[SensorNum] = map(CSensorV[SensorNum], 0, 1023, 1023, 0);

				if(CSensorV[SensorNum] > CSensorExtremity[SensorNum][0]) CSensorExtremity[SensorNum][0] = CSensorV[SensorNum];
				if(CSensorV[SensorNum] < CSensorExtremity[SensorNum][1]) CSensorExtremity[SensorNum][1] = CSensorV[SensorNum];
			}

			sprintf(Buffer, "\t\t%d %d %d %d %d %d", CSensorExtremity[0][0], CSensorExtremity[1][0], CSensorExtremity[2][0], CSensorExtremity[3][0], CSensorExtremity[4][0], CSensorExtremity[5][0]);
			Serial.print(Buffer);
			sprintf(Buffer, "\t\t%d %d %d %d %d %d", CSensorExtremity[0][1], CSensorExtremity[1][1], CSensorExtremity[2][1], CSensorExtremity[3][1], CSensorExtremity[4][1], CSensorExtremity[5][1]);
			Serial.println(Buffer);

			Run(-100, 100);
		}
	}
} 