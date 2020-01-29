#pragma once

#include "roboruka.h"
#include "RBControl_pinout.hpp"

//Define motor pins
#define LEFT M3
#define RIGHT M5

void encRead(int& left, int& right);
void encReset(int& left, int& right);
void encPrintValue(int& left, int& right);
void motorLeft(int power);
void motorRight(int power);
int inRange(int power);
void driveLeft(int position, int power);
void driveRight(int position, int power);
void corssRegulator(int encLeft, int encRight, int power, float P = 0.5);
void driveToRegulator(int start, int stop, float P = 0.5);
