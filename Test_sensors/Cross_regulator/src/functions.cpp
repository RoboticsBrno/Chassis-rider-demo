#include "Arduino.h"
#include "functions.h"
#include "RBControl_pinout.hpp"
#include "RBControl.hpp"



void encRead(int& left, int& right)
{
    static bool first = true;
    static int lastLeft, lastRight;
    int nowLeft, nowRight;
    if(first)
    {
        lastLeft = rb::Manager::get().motor(rb::MotorId::LEFT).encoder()->value();
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
    Serial.printf("Var -> L: %d, R: %d; Enc -> L: %d, R: %d\n", left, right, nowLeft, nowRight);
}

void encReset(int& left, int& right)
{
    left = 0;
    right = 0;
}

int inRange(int power)
{
    if(power>100)
        return 100;
    if(power<-100)
        return -100;
    return power;
}

void encPrintValue(int& left, int& right)
{
    Serial.printf("Left: %d, Right: %d\n", left, right );
}

void motorLeft(int power)
{
    
    rb::Manager::get().motor(rb::MotorId::LEFT).power(inRange(power));
}

void motorRight(int power)
{
    rb::Manager::get().motor(rb::MotorId::RIGHT).power(inRange(-power));
}


void driveLeft(int position, int power)
{
    rb::Manager::get().motor(rb::MotorId::LEFT).drive(position, inRange(power));
}

void driveRight(int position, int power)
{
    rb::Manager::get().motor(rb::MotorId::RIGHT).drive(-position, inRange(-power));
}


void corssRegulator(int encLeft, int encRight, int power, float P)
{
    int leftPower = inRange(power - P * (encLeft - encRight));
    int rightPower = inRange(power + P * (encLeft - encRight));
    motorLeft(leftPower);
    motorRight(rightPower);
    printf("L: %d, R: %d\n", leftPower, rightPower);
}

//drive to absolite position 
void driveToRegulator(int start, int stop, float P)
{

}