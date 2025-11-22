#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Radio - Pins NRF24 
RF24 radio(7, 8); // CE, CSN

//Adresse 
const byte address[6] = "00100";

//Datenpaket zum Senden 
struct Data_Package {
  int lx = 0;
  int ly = 0; 
  bool lz = 0; 

  int rx = 0; 
  int ry = 0; 
  bool rz = 0; 
};
//Datenpacket als "data" festlegen
Data_Package data;


//Initializer to find the physical limits of the analogue joysticks of the remote
struct Min_Values {
  int lx = 0; 
  int ly = 0; 
  int rx = 0; 
  int ry = 0; 
};
Min_Values minValues; 

struct Max_Values {
  int lx = 0; 
  int ly = 0; 
  int rx = 0; 
  int ry = 0; 
};
Max_Values maxValues; 

//---------------------------------------------------------------
//SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP
//----------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println();
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

//--------------------------------------------------------------
//LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP
//--------------------------------------------------------------------- 
void loop() {
  //Pins Auslesen 
  data.lx = analogRead(0);
  data.ly = analogRead(1);
  data.lz = digitalRead(A2);
  data.rx = analogRead(3);
  data.ry = analogRead(4);
  data.rz = digitalRead(A5);

  //Daten senden 
  radio.write(&data, sizeof(Data_Package));
  //Serial.println("Sende ..");
}