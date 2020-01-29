#include <Arduino.h>

#include "roboruka.h"

void handleMessage(const std::string& cmd, rbjson::Object *pkt) {
    if(cmd == "joy") {
        const rbjson::Array *data = pkt->getArray("data");
        if(data->size() >= 1) {
            const rbjson::Object *joy = data->getObject(0);
            int x = joy->getInt("x");
            int y = joy->getInt("y");

            rkMotorsJoystick(x, y);
        }
    } else if(cmd == "arm") {
        const double x = pkt->getDouble("x");
        const double y = pkt->getDouble("y");
        rkArmMoveTo(x, y);
    } else if(cmd == "grab") {
        rkArmSetGrabbing(!rkArmIsGrabbing());
    }
}

void setup() {
    rkConfig cfg;
    cfg.owner = "Robotiada";
    cfg.name = "Demo";

    // BUĎTO připojit na WiFi...
    //cfg.wifi_name = "Technika";
    //cfg.wifi_password = "materidouska";

    // A NEBO vytvořit vlastní WiFi (odkomentovat další dva řádky)
    cfg.wifi_default_ap = true;
    cfg.wifi_ap_password = "Chassis-demo";

    cfg.motor_enable_failsafe = true;
    cfg.rbcontroller_app_enable = true;
    cfg.rbcontroller_message_callback = handleMessage;
    rkSetup(cfg);


    printf("%s's roboruka '%s' started!\n", cfg.owner, cfg.name);
}

static int gIter = 0;

void loop() {
    // Send text to the android application
    rkControllerSendLog("Tick #%d, battery at %d%%, %dmv %d\n", gIter++, rkBatteryPercent(),
        rkBatteryVoltageMv(), rkButtonIsPressed(1, false));
    delay(1000);
}
