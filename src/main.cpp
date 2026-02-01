//Arduino Nano Transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DEBUG false //Debug Variable for Logging

#define DEFAULT_MIN_VALUE 50 //Stanardwert für Min/Max auslesen
#define DEFAULT_MAX_VALUE 974
#define CUTOFF 50

//Radio - Pins NRF24 
RF24 radio(7, 8); // CE, CSN

//Adresse 
const byte address[6] = "00100";

//Data Package 
struct Data_Package {
  uint16_t lx;
  uint16_t ly;
  uint8_t  lz;

  uint16_t rx;
  uint16_t ry;
  uint8_t  rz;
};
//Datenpacket als "data" festlegen
Data_Package data;

//Helper Function to Constrain input between upper and lower limit
void normalizeInput(uint16_t &input) {
  const short lowerCap = CUTOFF; 
  const short higherCap = 1024 - CUTOFF; 
  input = constrain(input, lowerCap, higherCap);
}

void mapFrom0To1024(uint16_t &input) {
  const short lowerCap = CUTOFF; 
  const short higherCap = 1024 - CUTOFF;
  input = map(input, lowerCap, higherCap, 0, 1024); 
}

//Debug Logging
void debugAnalogInputs(const String msg) {
   Serial.print(
    msg + ":  lx: " + data.lx +
    "   ly: " + data.ly +
    "   lz: " + data.lz +
    "   rx: " + data.rx +
    "   ry: " + data.ry +
    "   rz: " + data.rz +
    "   |   "
  );
}

//---------------------------------------------------------------
//SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP
//----------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Program: RC-Transmitter");
  Serial.println("Setup - Start");

  //Radio Starten 
  bool init_status = radio.begin();
  Serial.print("Radio Initialization: ");
  Serial.println(init_status ? "Success" : "Failed");

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  Serial.println("Setup - End");
}

//---------------------------------------------------------------------
//LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP
//--------------------------------------------------------------------- 
void loop() {
  //Pins Auslesen 
  data.lx = analogRead(0);
  data.ly = analogRead(1);
  data.lz = digitalRead(A2);
  data.rx = analogRead(3);
  data.ry = analogRead(4);
  data.rz = digitalRead(A5);

  if (DEBUG) debugAnalogInputs("Raw Input"); 

  normalizeInput(data.lx); 
  normalizeInput(data.ly); 
  normalizeInput(data.rx); 
  normalizeInput(data.ry);
  
  if (DEBUG) debugAnalogInputs("Normalized Input");

  mapFrom0To1024(data.lx); 
  mapFrom0To1024(data.ly);
  mapFrom0To1024(data.rx); 
  mapFrom0To1024(data.ry); 

  if (DEBUG) {
    debugAnalogInputs("Mapped Input");
    Serial.println();
  }
  //Daten senden 
  radio.write(&data, sizeof(Data_Package));
  //Serial.println("Sende ..");
}
