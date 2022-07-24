#define RXD2 16
#define TXD2 17

int i;

byte buffstore[20];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Computer Serial
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //Machine Vending Serial
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial2.write(0x05);
  delay(1000);

}
