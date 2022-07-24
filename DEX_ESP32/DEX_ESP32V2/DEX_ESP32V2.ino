/* ----------------------- GLOBAL CONSTANTS -------------------------- */

#define SUCCESS 0 // function will return 0 if successful
#define FAIL  -1  // Function will return -1 if unsuccessful
#define MILLI_SECONDS 15

#include <Ticker.h>

int i;

char DEXresponse;

size_t bytes_buffstore;
char buffstore[16]; //byte buffstore[16];

uint16_t   BCC, BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;
String CRC1 = "6150210000RR01L01";

uint16_t buff1;
uint16_t buff2;

Ticker TaskSerialPoll;

//------------------------- DEX Commands --------------------------- 

#define SOH 0x01
#define STX 0x02
#define ETX 0x03
#define EOT 0x04
#define ENQ 0x05

#define DLE 0x10
#define NAK 0x15
#define SYN 0x16
#define ETB 0x17

#define NUL 0x00
#define ACK 0x06

//------------------------- ESP Pin --------------------------- 

#define RXD2 16
#define TXD2 17

// ------------------------- Functions --------------------------- 

void alternate_answer(int j)
{
  delay(MILLI_SECONDS);
  Serial2.write(DLE);
  delay(MILLI_SECONDS);
  Serial2.write(0x30+(j));
}

void crc_16(unsigned char data)
{
  uint8_t j;
  for (j=0;j<8;j++)
  {
    DATA_0 = (data >> j) & 0x0001;
    BCC_0 = (BCC & 0x0001);
    BCC_1 = (BCC >> 1) & 0x0001;
    BCC_14 = (BCC >> 14) & 0x0001;
    X16 = (BCC_0 ^ DATA_0) & 0x0001;   // bit15 of BCC after shift
    X15  = (BCC_1 ^ X16) & 0x0001;   // bit0 of BCC after shift
    X2  = (BCC_14 ^ X16) & 0x0001;   // bit13 of BCC after shift
    BCC = BCC >> 1;
    BCC = BCC & 0x5FFE;
    BCC = BCC | (X15);
    BCC = BCC | (X2 << 13);
    BCC = BCC | (X16 << 15);
  }
}

int calculateMSB(int CRCmsb) {
    int msb = CRCmsb>>8;
    return msb;
}

int calculateLSB(int CRClsb) {
    int lsb = CRClsb&0x00ff;
    return  lsb;
}

//void store_hex(int n)
void store_hex()
{
  for(i=0;i<2;i++)
  {
    buffstore[i]=Serial2.read();
  }
  Serial.println(buffstore[i],HEX);
  Serial.println("Space");
}

//agregar un return para confirmar
void first_handshake()
{
  Serial2.write(0x05);
  Serial.println("05h");

  /////////////////////////////////////////////////////////
  
  /*bytes_buffstore = Serial2.readBytesUntil('0', buffstore, 5);
  if(bytes_buffstore == 5){
    Serial.println("I received two bytes");
    Serial.println(buffstore);
  }
  Serial.print("Bytes Received :");
  Serial.println(buffstore);*/
  /////////////////////////////////////////////////////////
  //if(bytes_buffstore == 5){ //if(Serial2.available()>0){

    while(i == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '0' ){i=1;}
    }
    
    if(i=1 )//&& buffstore[5] == 0x30)
    {

      delay(15);
      Serial.println("complete");
      //10h 01h
      Serial2.write(0x10);
      Serial2.write(0x01);

      //6150210000 
      Serial2.write(0x36);
      Serial2.write(0x31);
      Serial2.write(0x35);
      Serial2.write(0x30);
      Serial2.write(0x32);
      Serial2.write(0x31);
      Serial2.write(0x30);
      Serial2.write(0x30);
      Serial2.write(0x30);
      Serial2.write(0x30);

      //R 
      Serial2.write(0x52);
      //R01 
      Serial2.write(0x52);
      Serial2.write(0x30);
      Serial2.write(0x31);
      //L01
      Serial2.write(0x4C);
      Serial2.write(0x30);
      Serial2.write(0x31);

      //10h 03h
      Serial2.write(0x10);
      Serial2.write(0x03);

      //------------------------- CRC ---------------------------
      byte buf[CRC1.length()+1 ];
      CRC1.getBytes(buf, CRC1.length()+1 );

      BCC=0;
      for(i=0;i<=17;i++)
      {
        crc_16(buf[i]);
      }

      buff1 = calculateMSB(BCC);
      buff2 = calculateLSB(BCC);

      Serial2.write(buff2);
      Serial2.write(buff1);
      //---------------------------------------------------------
      //REINCIIAR
      i=0;

      while(i == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '1' ){i=1;}
      }
    
      if(i=1)
      {
        //04h
        Serial2.write(0x04);
        Serial.println("First Handshake passed");
      }

      i=0;

      while(i == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){i=1;}
      }

      if(i=1)
      {
        //04h
        Serial2.write(0x10);
        Serial2.write(0x30);
      }

      i=0;

      while(i == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){i=1;}
      }

      if(i=1)
      {
        //04h
        Serial2.write(0x10);
        Serial2.write(0x31);
      }

      i=0;

      while(i == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){i=1;}
      }

      if(i=1)
      {
        //04h
        Serial2.write(0x10);
        Serial2.write(0x30);
      }

    //}
  }
}

void second_handshake()
{
  if(Serial2.read() == 0x05)
  {
    Serial2.write(0x30);
    Serial2.write(0x31);

    if(buffstore[0] == 0x10 && buffstore[1] == 0x01)
    {
      //Okay (30h 30h)
      //store_hex();

      if(buffstore[0] == 0x30 && buffstore[1] == 0x30)
      {

        //--------------------Communication ID---------------------
        //AUTSENSIT3
        //41 55 54 53 45 4E 53 49 54 33

        //store_hex(10);

        //--------------------Revision---------------------
        //R 0 0
        //52 30 30

        //store_hex(3);

        // --------------------Level---------------------
        //L 0 6
        //4C 30 36

        //store_hex(3);

        // -------------------- DLE ETX --------------------- 
        //10h 03h
        //store_hex(2);

        // -------------------- CRC --------------------- 
        //Example "E4h 12h"
        //store_hex(2);

        //Send
        Serial2.write(0x10);

        //Send
        Serial2.write(0x31);
        
        if(Serial2.read() == 0x04)
        {
          Serial.println("Second Handshake passed");
        }
      }
    }
  }
}

void setup()
{

  Serial.begin(115200); //Computer Serial
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //Machine Vending Serial

  TaskSerialPoll.attach(5,first_handshake);

}

void loop()
{

}
