#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    Serial.begin(115200);
    // Upravte nastavení, například:
    cfg.motor_max_power_pct = 40; // limit výkonu motorů na 30%
    rkSetup(cfg);
    Serial.printf("\nBT1 - calibration \nBT2 - line follow\n");

    // Napište setup kód, provede se jednou po startu robota
}

static bool gFollowing = false;
static bool gReading = false;
static float gPreviousPos = 0.f;


void loop() {
    // Obsah této metody se provádí neustále dokola

    if(rkButtonIsPressed(1)) {         // Tlačítko 1: Kalibruj senzory
        delay(500);
        rkLineCalibrate();
    } else if(rkButtonIsPressed(2)) {  // Tlačítko 2: Sleduj čáru
        if(gFollowing) {
            rkMotorsSetPower(0, 0); // zastavit robota, aby přestal sledovat
            gFollowing = false;
        } else {
            gFollowing = true;
        }
        delay(500); // prodleva, abyste stihli uhnout rukou
    } else if(rkButtonIsPressed(3)) {  // Tlačítko 2: Sleduj čáru
        if(gReading) {
            rkMotorsSetPower(0, 0); // zastavit robota, aby přestal sledovat
            gReading = false;
        } else {
            rkMotorsSetPower(0, 0); // zastavit robota, aby přestal sledovat
            gReading = true;
        }
        delay(500); // prodleva, abyste stihli uhnout rukou
    }

    // Načti pozici čáry
    float pos = rkLinePosition(false, 25);
    //Serial.printf("Pos: %f\n", pos);
    

    // Rozsvítíme ledky podle toho, kde čára zrovna je
    int ledId = int(round((pos + 1.f)/2*3)); // převod z rozsahu <-1;1> na číslo od 0 do 3,
    rkLedRed(ledId == 0);
    rkLedYellow(ledId == 1);
    rkLedGreen(ledId == 2);
    rkLedBlue(ledId == 3);

    // Pokud jsme mimo čáru, použijeme pozici z poslední iterace,
    // jinak si pozici uložíme.
    if(isnan(pos)) {
        // Pokud předchozí pozice byla vlevo (<0), zatočíme co nejvíc doleva (-1),
        // v opačném případě doprava (1).
        if(gPreviousPos < 0) {
            pos = -1;
        } else {
            pos = 1;
        }
    } else {
        gPreviousPos = pos;
    }

    // Sledujeme teď čáru?
    if(gFollowing) {
        Serial.println(pos);
        // Zatáčení doleva
        if(pos < -0.75) {
            rkMotorsSetPower(0, 100);
        } else if(pos < -0.25) {
            rkMotorsSetPower(50, 100);
        }

        // Rovně
        if(pos > -0.25 && pos < 0.25) {
            rkMotorsSetPower(100, 100);
        }

        if(pos > 0.75) {
            rkMotorsSetPower(100, 0);
        } else if(pos > 0.25) {
            rkMotorsSetPower(100, 50);
        }        
    }

    if(gReading)
    {
        for(int i = 0; i <=7; i++)
        {
        Serial.printf("%d, ", map(rkLineGetSensor(i), 1, 1023, 1, 255));    
        }
        Serial.printf("-> ");
        Serial.println(pos);
        Serial.printf("\n");
        delay(500);
        
    }

    usleep(10000); // mezi iteracemi čekáme 10ms == 10000us
}
