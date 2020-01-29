#include <MPU6050_tockn.h>
#include <Wire.h>

// Objekt gyroskopu
MPU6050 mpu6050(Wire);

// Funkce setup se spusti na zacatku programu
void setup() {
  // Nastartovani seriove linky
  Serial.begin(115200);
  
  // Nastartuj sbernici I2C
  Wire.begin();
  // Nastartuj gyroskop
  mpu6050.begin();
  // Proved kalibraci
  mpu6050.calcGyroOffsets(true);
  
}

// Funkce loop se opakuje stale dokola
void loop() {
  // Ziskej data z akcelerometru
  mpu6050.update();
  // Vypis uhly naklonu do seriove linky
  // A oddel je mezerou pro zobrazeni v plotru
  Serial.print(mpu6050.getAngleX()); Serial.print(" ");
  Serial.print(mpu6050.getAngleY()); Serial.print(" ");
  Serial.println(mpu6050.getAngleZ());
} 