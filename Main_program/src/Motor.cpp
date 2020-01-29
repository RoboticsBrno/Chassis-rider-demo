

#include <Arduino.h>
#include "RBControl.hpp"
#include <Ultrasonic.h>
#include <MPU6050_tockn.h>
#include "RBControl.hpp"

#include "Motor.h"


#define MotorLeft rb::MotorId::M8
#define MotorRight rb::MotorId::M6

//Motor global variables/////
bool gCross = false; //Cross regulator 0/1
int gMaxPowerL = 0, gMaxPowerR = 0; //max power to motor
int gPosL = 0, gPosR = 0; //Actual robot position
int gTargPosL = 0, gTargPosR = 0; //Target position of robot for motor regulator
float PL = 0.5, PR = 0.5; //P value fot P-regulator
int gInfin = 100000000; //Infinity position for motor regulator :) 
bool gOnPositionL = true;
bool gOnPositionR = true;
/////////////////////////////


int motorUpdate()
{
    gPosL = rb::Manager::get().motor(MotorLeft).encoder()->value();
    gPosR = -(rb::Manager::get().motor(MotorRight).encoder()->value());
 
    int powL = 0, powR = 0;
    if(!gCross)
    {
        powL = (gTargPosL - gPosL) * PL;
        powR = (gTargPosR - gPosR) * PR;
        XmotorLeft(inRange(powL,gMaxPowerL));
        XmotorRight(inRange(powR,gMaxPowerR));
    }
    else if(gCross)
    {
        //To do
        Serial.printf("TO DO\n");
    }

    if( abs(gTargPosL - gPosL) < 10 && abs(gTargPosR - gPosR) < 10 )
    {
        return 1;
        gOnPositionL = true;
        gOnPositionR = true;
    }
    else if( abs(gTargPosL - gPosL) < 10)
    {
        return 1;
        gOnPositionL = true;
    }    
    else if( abs(gTargPosR - gPosR) < 10)
    {
        return 1;
        gOnPositionR = true;
    }    
    else
    {
        return 0;

    }



}

//Set motor power and run
void motorPowerL(int power)
{
    gMaxPowerL = power;
    gCross = false;
    gTargPosL = gPosL + gInfin;
}

//Set motor power and run
void motorPowerR(int power)
{
    gMaxPowerR = power;
    gCross = false;
    gTargPosR = gPosR + gInfin;
}

//Stop left motor
void motorStopL()
{
    gMaxPowerL = 0;
    gTargPosL = gPosL;
    gCross = false;
}

//Stop Right motor
void motorStopR()
{
    gMaxPowerR = 0;
    gTargPosR = gPosR;
    gCross = false;
}

//Stop both motors
void motorStopAll()
{
    motorStopL();
    motorStopR();
}

//Drive motor to set position (according relative value)
bool motorDriveL(int position, int power, float P)
{
    if(gOnPositionL)
    {
        gCross = false;
        gTargPosL = gPosL + position;
        gMaxPowerL = power;
        PL = P;
        gOnPositionL = false;
    }
    return gOnPositionL;
}

//Drive motor to set position (according relative value)
bool motorDriveR(int position, int power, float P)
{
    if(gOnPositionR)
    {
        gCross = false;
        gTargPosR = gPosR + position;
        gMaxPowerR = power;
        PR = P;
        gOnPositionR = false;
    }
    return  gOnPositionR;
    

}

void motorPrintValues()
{
    int powL = (gTargPosL - gPosL) * PL;
    int powR = (gTargPosR - gPosR) * PR;
    
    Serial.printf("VAR -> Now [%d][%d], Tar [%d][%d], Mot [%d][%d], Pow [%d][%d], P [%g][%g], Cross[%d], OnPos[%d][%d]\n", gPosL, gPosR, gTargPosL, gTargPosR, powL, powR, gMaxPowerL, gMaxPowerR, PL, PL, gCross, gOnPositionL, gOnPositionR);
}

void XmotorLeft(int power)
{
    
    rb::Manager::get().motor(MotorLeft).power(inRange(power, 100));
}

void XmotorRight(int power)
{
    rb::Manager::get().motor(MotorRight).power(inRange(-power, 100));
}



int inRange(int value, int max)
{
    if(value>max)
        return max;
    if(value<-max)
        return -max;
    return value;
}

bool onPosition()
{
    if(gOnPositionL == 1 or gOnPositionR == 1)
    {
        return true;
    }
    if(gOnPositionL)
    {
        return true;
    }
    if(gOnPositionR)
    {
        return true;
    }

    return false;
    
}

