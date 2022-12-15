// ------------------------------------------------------------------------------------------ INCLUDES

#include <Arduino.h>
#include "BaseFunctions.h"
#include "LineFollow.h"

// ------------------------------------------------------------------------------------------ VARIABLES

int Error, LastError, Derivative, Integral;
int Turn, LMotor, RMotor;

// ------------------------------------------------------------------------------------------ FUNCTIONS

void SpeedGuard() {									// Stops the motors from exceeding the allowed PWM Duty cycle lengths
	if (LMotor > MAX-MID) LMotor = MAX-MID;	
 	if (RMotor > MAX-MID) RMotor = MAX-MID;
 	if (LMotor < MIN-MID) LMotor = MIN-MID;
  	if (RMotor < MIN-MID) RMotor = MIN-MID;

	if(LMotor > 0) 	LMotor = map(LMotor, 0, 800, 60, 800);
	else			LMotor = map(LMotor, -800, 0, -800, -60);
	if(RMotor > 0) 	RMotor = map(RMotor, 0, 800, 60, 800);
	else			RMotor = map(RMotor, -800, 0, -800, -60);
}

void PID(int v, float kp, float ki, float kd) {
	Error = (CSensorV[0]*1.2 + CSensorV[1] + CSensorV[2]) - (CSensorV[3]*1.2 + CSensorV[4] + CSensorV[5]);
													// Takes a weighted sum of the left and right side to determine a error
	Integral = Error + Integral;					// Adds to the running error, to calculate the size of the turn
	Derivative = Error - LastError;					// Finds the difference between the current error and last error, to see the sharpness of the turn

	Turn = kp*Error + ki*Integral + kd*Derivative;	// Takes the weighted sum of all values
	LMotor = v + Turn;
	RMotor = v - Turn;

	SpeedGuard();

	Run(LMotor, RMotor);
	LastError = Error;								// The last error becomes the current error at the end of this calculation

	if(PIDOut == 1) {								// Debugging
		char Buffer[100];
		sprintf(Buffer, "\t\tError:%d\tIntegral:%d\tDerivative:%d\tLMotor:%d\tRMotor:%d", Error, Integral, Derivative, LMotor, RMotor);
		Serial.print(Buffer);
	}
}