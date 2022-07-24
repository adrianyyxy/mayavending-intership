/* ----------------------- GLOBAL CONSTANTS -------------------------- */

#define SUCCESS 0 // function will return 0 if successful
#define FAIL  -1  // Function will return -1 if unsuccessful
#define MILLI_SECONDS 15

#include <Ticker.h>
#include <string.h>

int i;
int flag=0;

char DEXresponse;

byte ESP_ID[23] = {0x10, 0x01, 0x36, 0x31, 0x35, 0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x52, 0x52, 0x30, 0x31, 0x4C, 0x30, 0x31, 0x10, 0x03, 0x1A, 0x6D};

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

void alternate_answer(int j){
  Serial2.write(DLE);
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

void SH_store_calculateCRC(){
  char buffstore[20];
  int counter=0;
  int buff_flag=0;

  while(buff_flag == 0){
    while(Serial2.available() == 0){
  }
    DEXresponse = Serial2.read();
    if(DEXresponse != '' ){
      buffstore[counter] = DEXresponse;
      counter=counter+1;
      
      if(DEXresponse == '' ){buff_flag=1;}
      }
    }
    
  if(buff_flag == 1){
    buff_flag=0;

    BCC=0;
    for(i=0;i<=counter-1;i++){
      crc_16(buffstore[i]);
    }

    buff1 = calculateMSB(BCC);
    buff2 = calculateLSB(BCC);
    //Serial2.write(buff2);
    //Serial2.write(buff1);

  }
  //counter=0;
}

void DATA_store_calculateCRC(){
  //global char datastore[10000];
  char buffstore[1000];
  int counter=0;
  int savecounter = 0;
  int control_flag=0;
  int finalstate=0;
  
  int block=0;
  
  int buff_flag=0;

  while(control_flag == 0){
    
    if(block == 0){
      alternate_answer(0);
      /////
      if(finalstate==1){
        flag=2;
        control_flag = 1;
      }
      /////
      while(flag == 0){
        while(Serial2.available() == 0){
        }
        DEXresponse = Serial2.read();
        if(DEXresponse == '' ){flag=1;}
      }
      if(flag=1){
        flag = 0;
        ///////////////////////////////////
        ///////////////////////////////////
        while(buff_flag == 0){
          while(Serial2.available() == 0){
          }
          DEXresponse = Serial2.read();
          if(DEXresponse != '' ){
            buffstore[counter] = DEXresponse;
            counter=counter+1;
      
            if(DEXresponse == '' ){buff_flag=1;}
            else if(DEXresponse == '' ){buff_flag=1; finalstate==1;}
          }
        }
    
        if(buff_flag == 1){
          buff_flag=0;

          BCC=0;
          for(i=0;i<=counter-1;i++){
            crc_16(buffstore[i]);
          }

          buff1 = calculateMSB(BCC);
          buff2 = calculateLSB(BCC);

          counter = 0;

          while(flag == 0){
            while(Serial2.available() == 0) {
            }
            DEXresponse = Serial2.read();
            if(DEXresponse == buff1 ){flag=1;}
          }

          if(flag=1)
          {
            flag = 0;
            block = 1;
            //block = 0;
          }
        ///////////////////////////////////
        ///////////////////////////////////
        }
      }
    }

    if(block == 1){
      alternate_answer(1);
      //////
      /////
      if(finalstate==1){
        flag=2;
        control_flag = 1;
      }
      /////
      while(flag == 0){
        while(Serial2.available() == 0){
        }
        DEXresponse = Serial2.read();
        if(DEXresponse == '' ){flag=1;}
      }
      if(flag=1){
        flag = 0;
        ///////////////////////////////////
        ///////////////////////////////////
        while(buff_flag == 0){
          while(Serial2.available() == 0){
          }
          DEXresponse = Serial2.read();
          if(DEXresponse != '' ){
            buffstore[counter] = DEXresponse;
            counter=counter+1;
      
            if(DEXresponse == '' ){buff_flag=1;}
            else if(DEXresponse == '' ){buff_flag=1; finalstate==1;}
          }
        }
    
        if(buff_flag == 1){
          buff_flag=0;

          BCC=0;
          for(i=0;i<=counter-1;i++){
            crc_16(buffstore[i]);
          }

          buff1 = calculateMSB(BCC);
          buff2 = calculateLSB(BCC);

          counter = 0;

          while(flag == 0){
            while(Serial2.available() == 0) {
            }
            DEXresponse = Serial2.read();
            if(DEXresponse == buff1 ){flag=1;}
          }

          if(flag=1)
          {
            flag = 0;
            block = 0;
          }
        ///////////////////////////////////
        ///////////////////////////////////
        }
      }
      /////
      /////
    }
  }
  flag=0;
  //control_flag=0;
}

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
      SH_store_calculateCRC();

      while(flag == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == buff1 ){flag=1;}
      }

      if(flag=1)
      {
        flag = 0;
        alternate_answer(1);

        ///////////////
        while(flag == 0){
          while(Serial2.available() == 0) {
          }
          DEXresponse = Serial2.read();
          if(DEXresponse == ''){flag=1;}
          }
    
        if(flag=1){
          flag = 0;
          data_handshake();
        }
        //////////////
      }
      
    }
  }
}

void data_handshake(){
  while(flag == 0){
    while(Serial2.available() == 0) {
  }
  DEXresponse = Serial2.read();
  if(DEXresponse == '' ){flag=1;}
  }

  if(flag=1){
    flag = 0;
    DATA_store_calculateCRC();

    //////////
    while(flag == 0){
      while(Serial2.available() == 0) {
      }
      DEXresponse = Serial2.read();
      if(DEXresponse == ''){flag=1;}
      }
    
      if(flag=1){
        flag = 0;
        //Complete
      }
    //////////
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
  //disparar si no hay respuesta cada cierto tiempo y si hay 04, esperar 5 min para volver a disparar 
}
