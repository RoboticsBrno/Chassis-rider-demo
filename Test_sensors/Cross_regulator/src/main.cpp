#include <Arduino.h>
#include "roboruka.h"
#include "functions.h"
#include "RBControl.hpp"



bool gRun = false;
int gEncLeft = 0;
int gEncRight = 0;

void setup() {
    rkConfig cfg;
    Serial.begin(115200);
    cfg.motor_max_power_pct = 50; 
    rkSetup(cfg);

    while(1)
    {
        //int lastLeft = rb::Manager::get().motor(rb::MotorId::LEFT).encoder()->value();
        //int lastRight = (rb::Manager::get().motor(rb::MotorId::RIGHT).encoder()->value());
        Serial.printf("%d %d %d %d %d %d %d %d\n", rb::Manager::get().motor(rb::MotorId::M1).encoder()->value(), rb::Manager::get().motor(rb::MotorId::M2).encoder()->value(), rb::Manager::get().motor(rb::MotorId::M3).encoder()->value(), rb::Manager::get().motor(rb::MotorId::M4).encoder()->value(), rb::Manager::get().motor(rb::MotorId::M5).encoder()->value(), rb::Manager::get().motor(rb::MotorId::M6).encoder()->value() , rb::Manager::get().motor(rb::MotorId::M7).encoder()->value() , rb::Manager::get().motor(rb::MotorId::M8).encoder()->value());
        delay(100);
    }
}



void loop() {
    encRead(gEncLeft, gEncRight); 

    if(rkButtonIsPressed(1)) {         
        delay(500);
        
    } else if(rkButtonIsPressed(2)) {  
        if(gRun) {
            rkMotorsSetPower(0, 0); 
            gRun = false;
            rkLedBlue(false);
        } else {
            gRun = true;
            rkLedBlue(true);
        }
        delay(500);  

    }

    encPrintValue(gEncLeft, gEncRight);

    if(gRun)
    {
        corssRegulator(gEncLeft, gEncRight, 100);
    }

    usleep(10000); 
}
    

