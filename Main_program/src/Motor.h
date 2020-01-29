#pragma once

#include "roboruka.h"
#include "RBControl_pinout.hpp"
//#include "Variables.h"


int motorUpdate();

//Set motor power and run
void motorPowerL(int power);

//Set motor power and run
void motorPowerR(int power);

//Stop left motor
void motorStopL();

//Stop Right motor
void motorStopR();

//Stop both motors
void motorStopAll();

//Drive motor to set position (according relative value)
bool motorDriveL(int position, int power, float P = 2);

//Drive motor to set position (according relative value)
bool motorDriveR(int position, int power, float P = 2);

void motorPrintValues();

void XmotorLeft(int power);

void XmotorRight(int power);

int inRange(int value, int max);

bool onPosition();
