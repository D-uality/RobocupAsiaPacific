#include <Arduino.h>
#include "BaseFunctions.h"
#include "LineFollow.h"

int Error, LastError, Derivative, Integral;
int Turn, LMotor, RMotor;

void SpeedGuard(int v) {
	if (LMotor > MAX-MID) LMotor = MAX-MID;
 	if (RMotor > MAX-MID) RMotor = MAX-MID;
 	if (LMotor < MIN-MID) LMotor = MIN-MID;
  	if (RMotor < MIN-MID) RMotor = MIN-MID;
}

void PID(int v, float kp, float ki, float kd) {
	Error = (CSensorV[0] + CSensorV[1] + CSensorV[2]) - (CSensorV[3] + CSensorV[4] + CSensorV[5]);
	Integral = Error + Integral;
	Derivative = Error - LastError;

	Turn = kp*Error + ki*Integral + kd*Derivative;
	LMotor = v + Turn;
	RMotor = v - Turn;
	SpeedGuard(v);

	Run(LMotor, RMotor);
	LastError = Error;

	if(PIDOut == 1) {
		char Buffer[100];
		sprintf(Buffer, "\t\tError:%d\tIntegral:%d\tDerivative:%d\tLMotor:%d\tRMotor:%d", Error, Integral, Derivative, LMotor, RMotor);
		Serial.print(Buffer);
	}
}