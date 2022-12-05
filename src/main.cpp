#include <Arduino.h>
#include "BaseFunctions.h"
#include "LineFollow.h"
#include "Calibration.h"

const int DMotors[4] = {0, 1, 2, 3};
const int StraightWeight = 32;
const int MID = 1278;
const int MIN = MID-600;
const int MAX = MID+600;
const int CSensors[6] = {A0, A1, A2, A3, A4, A5};
int CSensorV[6];
const int CLBound = 0;
const int CUBound = 100;	
const int ButtonModePin = 2;

const int RunOut = 0;
const int ReadCSensorOut = 1;
const int PIDOut = 1;

int mode = 0;

void loop() {
	Mode();

	if(mode == 0) {

		Serial.print("Stopped");
		Run(0, 0);

	} else if(mode == 1) {

		Serial.print("Running");
		ReadCSensorV();
		PID(150, 6, 0, 10);

	} else {

		Serial.print("Calibrating");
		Run(0, 0);
		Calibrate();

	}
	
	Serial.println();
	delay(2);
}