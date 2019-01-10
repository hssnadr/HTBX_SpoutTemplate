#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro(0x68);

//--------------------------------
//----------- MPU6050 ------------
//--------------------------------
// MPU6050 is the motion sensor used to get the acceleration and angular speed from the punching bag
// reference: https://playground.arduino.cc/Main/MPU-6050
int16_t ax, ay, az; // store acceleration data (XYZ)
int16_t gx, gy, gz; // store gyroscope data (XYZ)

//--------------------------------
//------------ TOUCH -------------
//--------------------------------
// TOUCH GRID CONFIGURATION (here is a 24x24 sensors grid)
#define  ROWS  24
#define  COLS  24

// DIGITAL MULTIPLEXER
// used to set individual columns states (HIGH or LOW)
// reference: (French) https://www.zem.fr/decouverte-du-composant-74hc595-8-bit-shift-register/
//            (English) https://learn.adafruit.com/adafruit-arduino-lesson-4-eight-leds/overview
#define DATAPIN 2              // data pin to send new state     (pin 14 on the 75HC595)
#define CLOCKPIN 4             // clock pin for synchronization  (pin 11 on the 75HC595)
#define REGPIN 3               // register pin to set new states (pin 12 on the 75HC595)
#define number_of_74hc595s 3   // 3 multiplexers are needed to control 24 columns (8 outputs per multiplexer)
#define numOfRegisterPins number_of_74hc595s * 8 // do not change (hardware configuration)
boolean registers[numOfRegisterPins];

// ANALOG MULTIPLEXER (receive analog signal from touch grid)
// used to read individual rows electric potential (directly related to pressure applied on each sensor points)
// reference: http://bildr.org/2011/02/cd74hc4067-arduino/
#define S0 8 // control pin used to root incoming analog signal
#define S1 7 // ...
#define S2 6
#define S3 5 
int controlPin[] = {S0, S1, S2, S3};
#define numOfmuxChannel 16 // max number of analog signals rooted on each multiplexer
int muxChannel[numOfmuxChannel][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0},   // states control table: each combination correspond to one analog input (from 1 to 16)
                                      {1, 1, 0, 0}, {0, 0, 1, 0}, {1, 0, 1, 0},
                                      {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1},
                                      {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
                                      {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1},
                                      {1, 1, 1, 1}};
#define SIGPIN0 9 // analog pin receiving pressure data for 1st analog multiplexer (rows from 1 to 16)
#define SIGPIN1 8 // analog pin receiving pressure data for 2nd analog multiplexer (rows from 17 to 24)

void setup() {
  // Initialize communication
  Wire.begin();         // for MPU6050
  Serial.begin(38400);  // for serial (to computer)

  // Initialize MPU6050 sensors
  delay(100);
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // Initialize digital multiplexer pins
  pinMode(DATAPIN, OUTPUT);
  pinMode(REGPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  clearRegisters(); // reset all register pins
  writeRegisters(); // reset all register pins

  // Initialize analog multiplexer pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
}

void loop() {
  //--------------------------------
  //------------ TOUCH -------------
  //--------------------------------
  // Scan each column
  for (int i = 0; i < COLS; i++) {
    setRegisterPin(i, HIGH); // set current column state to HIGH (while others columns remain to LOW)
    writeRegisters();

    // Write row reference on serial (zCCxAAAxAAAxAAAxAAA...xAAAq)
    Serial.print("z");
    Serial.printf("%02X", i); // (CC) two characters corresponding to the current column index in hexadecimal format
    
    // Read electric potential on each points (rows) of the current columns
    for (int j = 0; j < ROWS; j++) {
      Serial.print("x"); // "x" used as separator
      if(j < numOfmuxChannel){
        Serial.printf("%03X", readMux(SIGPIN0, j));                 // (AAA) read and write analog value from the analog multiplexer
      }
      else{
        Serial.printf("%03X", readMux(SIGPIN1, j%numOfmuxChannel)); // (AAA) use the second analog multiplexer for rows out of range from the first one
      }
    }
    Serial.print('q');     // "q" used as end line character

    // Set column pin in high-impedance state
    setRegisterPin(i, LOW); // set current row back to LOW state (at this step each rows are supposed to be at LOW)
    writeRegisters();
  }

  //--------------------------------
  //----------- MPU6050 ------------
  //--------------------------------
  accelgyro.getAcceleration(&ax, &ay, &az); // read acceleration data and store into corresponding variables

  // Format and write data on serial: aXXXcYYYcZZZq (with XXX acceleration X written on 3 characters in hexadecimal, YYY acceleration Y...)
  Serial.print("a");                                    // a
  Serial.printf("%03X", (int)((ax+32768)*4095/65536.)); // XXX
  Serial.print("c");                                    // c
  Serial.printf("%03X", (int)((ay+32768)*4095/65536.)); // YYY
  Serial.print("c");                                    // c
  Serial.printf("%03X", (int)((az+32768)*4095/65536.)); // ZZZ
  Serial.print('q');                                    // q
}
