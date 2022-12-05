#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "BaseFunctions.h"
#include "Calibration.h"

Adafruit_PWMServoDriver motor = Adafruit_PWMServoDriver(0x40);

void setup() {
	Serial.begin(115200);
	Serial.println("Starting Program");

  	motor.begin();
	motor.setPWMFreq(220);
	Run(0, 0);

	for(int i=0; i<6; i++) {
		pinMode(CSensors[i], INPUT);
	}

	pinMode(ButtonModePin, INPUT);
}

void Mode() {
	String IncomingByte = "";

	if(Serial.available() > 0) {
    	Serial.print("Reading BYTE");
		IncomingByte = Serial.readString();

		if(IncomingByte == "s") mode = 0;
		if(IncomingByte == "g") mode = 1;
		if(IncomingByte == "c") mode = 2;
	}
}

void Run(int v1, int v2) {
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
	}

	CalibrateCSensorV(CLBound, CUBound);

	if(ReadCSensorOut == 1) {
		char Buffer[100];
		sprintf(Buffer, "\t\tTL:%d\tCL:%d\tBL:%d\tTR:%d\tCR:%d\tBR:%d", CSensorV[0], CSensorV[1], CSensorV[2], CSensorV[3], CSensorV[4], CSensorV[5]);
		Serial.print(Buffer);
	}
}