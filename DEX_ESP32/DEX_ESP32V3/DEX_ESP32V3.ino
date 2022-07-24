/* ----------------------- GLOBAL CONSTANTS -------------------------- */

#define SUCCESS 0 // function will return 0 if successful
#define FAIL  -1  // Function will return -1 if unsuccessful
#define MILLI_SECONDS 15

#include <Ticker.h>
#include <string.h>

int i;
int flag=0;
int buff_flag=0;

char DEXresponse;

byte ESP_ID[23] = {0x10, 0x01, 0x36, 0x31, 0x35, 0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x52, 0x52, 0x30, 0x31, 0x4C, 0x30, 0x31, 0x10, 0x03, 0x1A, 0x6D};

uint16_t   BCC, BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;
String CRC1 = "6150210000RR01L01";

uint16_t buff1;
uint16_t buff2;

char buffstore[20]; //byte buffstore[20];
int counter=0;



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

void alternate_answer(int j){
  //delay(MILLI_SECONDS);
  Serial2.write(DLE);
  //delay(MILLI_SECONDS);
  Serial2.write(0x30+(j));
}

void ESP_SEND_ID(){
  for(i=0;i<23;i++){
    Serial2.write(ESP_ID[i]);
  }
}

void crc_16(unsigned char data){
  uint8_t j;
  for (j=0;j<8;j++){
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

void calculateCRC(){
        //------------------------- CRC ---------------------------
      /*byte buf[CRC1.length()+1 ];
      CRC1.getBytes(buf, CRC1.length()+1 );

      BCC=0;
      for(i=0;i<=17;i++)
      {
        crc_16(buf[i]);
      }

      buff1 = calculateMSB(BCC);
      buff2 = calculateLSB(BCC);

      Serial2.write(buff2);
      Serial2.write(buff1);*/
      //---------------------------------------------------------
}

int calculateMSB(int CRCmsb) {
    int msb = CRCmsb>>8;
    return msb;
}

int calculateLSB(int CRClsb) {
    int lsb = CRClsb&0x00ff;
    return  lsb;
}

void storeandCRC_SecHand()
{
  
  String juan;
  String juan2;
  char second_buffstore[20]; //byte buffstore[20];
  while(buff_flag == 0){
    while(Serial2.available() == 0){
  }
    DEXresponse = Serial2.read();
    juan2 = DEXresponse;
    if(DEXresponse != '' ){
      second_buffstore[counter] = DEXresponse;

      //memcpy(&juan, &juan2, sizeof(juan2));
      //strncat (&juan, &juan2, sizeof(juan2));
      
      counter=counter+1;
      
      if(DEXresponse == '' ){buff_flag=1;}
      }
    }
    
  if(buff_flag == 1){
    buff_flag=0;
    Serial.println(second_buffstore[counter]);

    byte buf[counter+1];
    //second_buffstore.getBytes(buf, counter+1);
    //juan.getBytes(buf, counter+1);
    
    //byte buf[CRC1.length()+1 ];
    //CRC1.getBytes(buf, CRC1.length()+1 );

    BCC=0;
    //counter indefinido - 1 
    for(i=0;i<=counter-1;i++){
      crc_16(second_buffstore[i]);
    }

    buff1 = calculateMSB(BCC);
    buff2 = calculateLSB(BCC);

    //Serial2.write(buff2);
    //Serial2.write(buff1);

  }
  counter=0;
}

void store_hex(int n_bytes)
{
  for(i=0;i<n_bytes;i++)
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

    while(flag == 0){
        while(Serial2.available() == 0){
      }
        DEXresponse = Serial2.read();
        if(DEXresponse == '0' ){flag=1;}
    }
    
    if(flag == 1){
      flag=0;
      ESP_SEND_ID();

      while(flag == 0){
        while(Serial2.available() == 0){
      }
        DEXresponse = Serial2.read();
        if(DEXresponse == '1' ){flag=1;}
      }
    
      if(flag == 1){
        flag=0;
        Serial2.write(0x04);
        Serial.println("First Handshake passed");
        second_handshake();
      }
  }
}

void second_handshake(){
  while(flag == 0){
      while(Serial2.available() == 0){
    }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){flag=1;}
  }

  if(flag == 1){
    flag = 0;
    alternate_answer(0);

    while(flag == 0){
      while(Serial2.available() == 0){
    }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){flag=1;}
    }

    if(flag == 1){
      flag = 0;
      //CRC
      storeandCRC_SecHand();

      while(flag == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == buff1 ){flag=1;}
      }

      if(flag=1)
      {
        alternate_answer(1);
      }
      
    }
  }
}

/*
 * 
      flag=0;

      while(flag == 0){
      while(Serial2.available() == 0){
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){flag=1;}
      }

      if(flag=1){
        //04h
        flag=0;
        Serial2.write(0x10);
        Serial2.write(0x30);
      }
      /////
      

      while(flag == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){flag=1;}
      }

      if(flag=1){
        //04h
        Serial2.write(0x10);
        Serial2.write(0x31);
      }

      flag=0;

      while(flag == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){flag=1;}
      }

      if(flag=1)
      {
        //04h
        Serial2.write(0x10);
        Serial2.write(0x30);
      }

    //}
 */

void secondt_handshake()
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
