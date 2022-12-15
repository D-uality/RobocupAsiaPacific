// ------------------------------------------------------------------------------------------ INCLUDES

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "BaseFunctions.h"
#include "Calibration.h"

Adafruit_PWMServoDriver motor = Adafruit_PWMServoDriver(0x40);

// ------------------------------------------------------------------------------------------ FUNCTIONS

void setup() {
	Serial.begin(115200);
	Serial.println("Starting Program");						// Used to check if there is a program reset that was unintentionally caused by voltage drops from too much current being drawn from the motors

  	motor.begin();
	motor.setPWMFreq(220);
	Run(0, 0);

	for(int i=0; i<6; i++) {
		pinMode(CSensors[i], INPUT);
	}
}

void Mode() {												// Change the mode we are in
	String IncomingByte = "";

	if(Serial.available() > 0) {
    	Serial.print("Reading BYTE");
		IncomingByte = Serial.readString();

		if(IncomingByte == "s") mode = 0;
		if(IncomingByte == "g") mode = 1;
		if(IncomingByte == "c") mode = 2;
		if(IncomingByte == "r") mode = 3;
	}
}

void Run(int v1, int v2) {
	/*
		A simplistic view for the movement of the robot, to help with the readibility of the code.

		This is used as we do not need to account for direction (PWM Duty Cycle), or the orientati
		on of the motor.

		This is simply a speed for the left and right side, with a positive and negative number, r
		epresenting a forwards or backwards spin respectively.
	*/
	motor.setPin(DMotors[0], MID+v1);
	motor.setPin(DMotors[1], MID-v2);
	motor.setPin(DMotors[2], MID+v1);
	motor.setPin(DMotors[3], MID-v2);

	if(RunOut == 1) {
		char Buffer[100];
		sprintf(Buffer, "\t\t %d %d", MID+v1, MID-v1);
		Serial.print(Buffer);
	}
}

void ReadCSensorV() {
	for(int SensorNum=0; SensorNum<6; SensorNum++) {
		CSensorV[SensorNum] = analogRead(CSensors[SensorNum]);
		if(SensorNum==0 || SensorNum==1 || SensorNum==2) CSensorV[SensorNum] = map(CSensorV[SensorNum], 0, 1023, 1023, 0);

		CalibrateCSensorV(SensorNum, CLBound, CUBound);
	}

	if(ReadCSensorOut == 1) {
		char Buffer[100];
		sprintf(Buffer, "\t\tTL:%d\tCL:%d\tBL:%d\tTR:%d\tCR:%d\tBR:%d", CSensorV[0], CSensorV[1], CSensorV[2], CSensorV[3], CSensorV[4], CSensorV[5]);
		Serial.print(Buffer);
	}
}