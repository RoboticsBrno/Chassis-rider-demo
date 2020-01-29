#include "Arduino.h"
#include "functions.h"
#include "RBControl_pinout.hpp"
#include "RBControl.hpp"
#include <MPU6050_tockn.h>
#include <Wire.h>

/*

void encRead(int& left, int& right)
{
    static bool first = true;
    static int lastLeft, lastRight;
    int nowLeft, nowRight;
    if(first)
    {
        lastLeft = rb::Manager::get().motor(rb::MotorId::M3).encoder()->value();
        lastRight = -(rb::Manager::get().motor(rb::MotorId::RIGHT).encoder()->value());
        first  = false;
    } 
    
    nowLeft = rb::Manager::get().motor(rb::MotorId::LEFT).encoder()->value();
    nowRight = -(rb::Manager::get().motor(rb::MotorId::RIGHT).encoder()->value());
    
    if(nowLeft-lastLeft != 0)
    {
        left += (nowLeft-lastLeft);
        lastLeft = nowLeft;
    }

    if(nowRight-lastRight != 0)
    {
        right += (nowRight-lastRight);
        lastRight = nowRight;
    }
    //Serial.printf("Var -> L: %d, R: %d; Enc -> L: %d, R: %d\n", left, right, nowLeft, nowRight);
}

void encReset(int& left, int& right)
{
    left = 0;
    right = 0;
}



void encPrintValue(int& left, int& right)
{
    Serial.printf("Print enc -> L: %d, R: %d\n", left, right );
}



void motorStopAll()
{
    motorLeft(0);
    motorRight(0);
}


void driveLeft(int position, int power)
{
    rb::Manager::get().motor(rb::MotorId::LEFT).drive(position, inRange(power));
}

void driveRight(int position, int power)
{
    rb::Manager::get().motor(rb::MotorId::RIGHT).drive(-position, inRange(-power));
}


void corssRegulator( int power, float P)
{
    int encLeft = rb::Manager::get().motor(rb::MotorId::LEFT).encoder()->value();
    int encRight = -(rb::Manager::get().motor(rb::MotorId::RIGHT).encoder()->value());
    
    int leftPower = inRange(power - P * (encLeft - encRight));
    int rightPower = inRange(power + P * (encLeft - encRight));
    motorLeft(leftPower);
    motorRight(rightPower);
    encPrintValue(encLeft, encRight);
    printf("CrossRegulator -> power -> L: %d, R: %d Target -> L: %d, R: %d\n", leftPower, rightPower, (encLeft - encRight), (encLeft - encRight) );
}

//drive to absolite position 
bool driveToRegulator(int encLeft, int encRight, int driveTo, int power, float P)
{
    int targLeft = driveTo - encLeft;
    int targRight = driveTo - encRight;
    
    
    int leftPower = power * P * targLeft;
    int rightPower = power * P * targRight;
    

    motorLeft( inRange(leftPower, power) );
    motorRight( inRange(rightPower, power) );
    printf("Regulator power -> L: %d, R: %d Target -> L: %d, R: %d\n", leftPower, rightPower, targLeft, targLeft);

    if( (abs(targLeft) <  10) && (abs(targRight) < 10 ) )
    {
        return true;
    }
    return false;

}



bool driveRegulator(int drive, int maxPower, float P)
{
    static int sFirst, sEncLeft, sEncRight, sDrive;
    
    if(sFirst == 0)
    {
        sDrive = drive;
        sFirst = 1;
        encReset(sEncLeft, sEncRight);
        Serial.printf("---------------------------------------------------");
    }
    encRead(sEncLeft, sEncRight);
    int targLeft = sDrive - sEncLeft;
    int targRight = sDrive - sEncRight;
    
    
    int leftPower = maxPower * P * targLeft;
    int rightPower = maxPower * P * targRight;
    

    motorLeft( inRange(leftPower, maxPower) );
    motorRight( inRange(rightPower, maxPower) );
    printf("Regulator power -> L: %d, R: %d Target -> L: %d, R: %d\n", leftPower, rightPower, targLeft, targLeft);

    if( (abs(targLeft) <  10) && (abs(targRight) < 10 ) )
    {
        sFirst = 0;
        encReset(sEncLeft, sEncRight);
        return true;
    }
    return false;

}
/*
void ultraPrintValue()
{
    
    int a = UltraLeft.read();
    int b = UltraFront.read();
    int c = UltraRight.read();
    
    printf("%d, %d, %d\n", a, b, c);
}
*/

