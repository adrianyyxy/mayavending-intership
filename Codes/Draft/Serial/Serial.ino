//#define RXD0 3
//#define TXD0 1
//or
#define RXD2 16
#define TXD2 17

void setup() {

  //computer serial
  Serial.begin(115200);

  //Machine Vending Serial       
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
}

void loop() { //Choose Serial1 or Serial2 as required
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
}