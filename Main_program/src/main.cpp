//Other H files
#include <Arduino.h>
#include <Ultrasonic.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

//My H files
//#include "Variables.h"
#include "Motor.h"
#include "roboruka.h"
#include "functions.h"
#include "RBControl.hpp"




//Button variables
bool gSecond = false;
bool gThird = false;
bool gFirst = false;

//Ultrasonic structure
struct ultrasonic
{
    int L;
    int F;
    int R;
};
ultrasonic us;

int state = 0;
int gState = 0;


Ultrasonic UltraLeft(18, 19);
Ultrasonic UltraFront(4, 13);
Ultrasonic UltraRight(15, 16);
//MPU6050 Gyr(Wire);

void setup() {
    rkConfig cfg;
    Serial.begin(115200);
    Serial.printf("START!!!!!!!!!!\n");
    //Wire.begin();
    //Gyr.begin();
    //Gyr.calcGyroOffsets(true);
    cfg.motor_max_power_pct = 50; 
    rkSetup(cfg);
    rkLedGreen(true);
    
    


}


void loop() {
    us.L = UltraLeft.readRound();
    usleep(10000); 
    us.F = UltraFront.readRound();
    usleep(10000); 
    us.R = UltraRight.readRound();
    usleep(10000); 
    //Gyr.update();
    int onPos = motorUpdate();
    printf( "On Pos [%d]\n",  onPos);

    //Serial.printf("Stav [%d]\n", state);

    if(rkButtonIsPressed(1)) {  
        if(gFirst) {
            motorStopAll(); 
            gFirst = false;
            rkLedBlue(false);
        } else {
            gFirst = true;
            rkLedBlue(true);
        }
        delay(500);  
          
    } else if(rkButtonIsPressed(2)) {  
        if(gSecond) {
            motorStopAll(); 
            gSecond = false;
            rkLedBlue(false);
        } else {
            gSecond = true;
            rkLedBlue(true);
            
        }
        delay(500);  
    } else if(rkButtonIsPressed(3)) {  
        if(gThird) {
            motorStopAll(); 
            gThird = false;
            rkLedBlue(false);
        } else {
            gThird = true;
            rkLedBlue(true);
        }
        delay(500);  
    } 


    if(gFirst)
    {
        //printf("US-> L:%d, F:%d, R:%d; Gyr-> X:%f, Y:%f, Z:%f \n", us.L, us.F, us.R, Gyr.getAngleX(), Gyr.getAngleY(), Gyr.getAngleZ());
        motorPrintValues();        
    }   

    if(gSecond)
    {
        motorPowerL(80);
        motorPowerR(80);

    }


    if(gThird)
    {
        Serial.printf("Case: %d\n", gState);

        switch (gState)
        {
        case 0:
            motorDriveL(100, 100 );
            motorDriveR(100, 100);
            gState += onPos;
            break;
        
        case 1:
            motorStopAll();
            break;

        case 2:
            break;
        
        default:
            break;
        }


                                                                             
    }   

    usleep(10000); 
}
    