// ------------------------------------------------------------------------------------------ INCLUDES

#include <Arduino.h>
#include "BaseFunctions.h"
#include "LineFollow.h"
#include "Calibration.h"

// ------------------------------------------------------------------------------------------ VARIABLES (GLOBAL)

const int DMotors[4] = {0, 1, 2, 3};						// Drive Motors, pinOut for 16 Channel PWM Servo Driver
const int StraightWeight = 32;								// The right side drives slightly slower than that of the left side, hence the right needs a constant increase in PWM cycles
const int MID = 1278;										// The midpoint for the motors with a PWM frequence of 200
const int MIN = MID-700;									// The minimum PWM HIGH time for the motor to start spinning
const int MAX = MID+700;									// The maximum PWM HIGH time for the motor to continue spinning
const int CSensors[6] = {A0, A1, A2, A3, A4, A5};			// Colour Sensors, pinIn for the Arduino
int CSensorV[6];											// Variable to hold colour sensor readings
const int CLBound = 0;										// ColourLowerBound for the calibration
const int CUBound = 100;									// ColuourUpperBound for the calibration

const int RunOut = 0;										// The motor PWM Duty cycles values for debugging
const int ReadCSensorOut = 1;								// The colour sensor values for debugging
const int PIDOut = 1;										// The PID Control Algorithm values for debugging

int mode = 0;												// Variable for the mode we are in

// ------------------------------------------------------------------------------------------ MAIN PROGRAM

void loop() {
	Mode();									// Update the mode we are in

	if(mode == 0) {							// Stop the robot

		Serial.print("Stopped");
		Run(0, 0);

	} else if(mode == 1) {					// The real program for the entire run

		Serial.print("Running");
		ReadCSensorV();
		PID(275, 8, 0, 0);

	} else {								// Calibrate the colour sensors

		Serial.print("Calibrating");
		Run(0, 0);
		Calibrate();

	}
	
	Serial.println();						// End of a program cycle
	delay(2);								// Allow motors to reach PWM speed produced by PID algorithm
}