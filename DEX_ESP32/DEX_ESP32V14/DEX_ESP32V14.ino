/* ----------------------- GLOBAL CONSTANTS -------------------------- */

//#include <Ticker.h>
#include <string.h> //Library to use String Functions

int i;  //i is a counter
int flag=0; //global flag for main functions

char DEXresponse; //Store each hex of the DEX Machine

byte ESP_ID[23] = {0x10, 0x01, 0x36, 0x31, 0x35, 0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x52, 0x52, 0x30, 0x31, 0x4C, 0x30, 0x31, 0x10, 0x03, 0x1A, 0x6D}; //ESP ID to send to VEM

uint16_t   BCC, BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;
String CRC1 = "6150210000RR01L01";

//------------------------- CRC Variables --------------------------- 
uint16_t buff1;
uint16_t buff2;

//------------------------- Save Data Variables --------------------------- 
char datastore[10000];
int savecounter = 0;

//------------------------- Send Data Variables ---------------------------
String ConvertData;
//String FinalData;

//Ticker TaskSerialPoll;

//------------------------- WIFI and Server SETUP --------------------------- 
#include <WiFiClientSecure.h>

const char* ssid     = "INFINITUM5AA4_2.4";     // your network SSID (name of wifi network)
const char* password = "usHt356AZq"; // your network password
String ESP_identifier = "BraulioChi123456789";

const char*  server = "www.app-vending.com";  // Server URL


const char* test_root_ca= \
      "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
      "-----END CERTIFICATE-----";


WiFiClientSecure client;

//------------------------- DEX Commands --------------------------- 
#define DLE 0x10

//------------------------- ESP Pin --------------------------- 
#define RXD2 16
#define TXD2 17

// ------------------------- Functions --------------------------- 

void alternate_answer(int j){ //Send 10 30, 10 31 or 10 32
  Serial2.write(DLE);
  Serial2.write(0x30+(j));
}

void ESP_SEND_ID(){ //Send the ESP ID to the VEM machine
  for(i=0;i<23;i++){
    Serial2.write(ESP_ID[i]);
  }
}

void crc_16(unsigned char data){  //CRC Calculator
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

int calculateMSB(int CRCmsb) {  //Calculate MSB of the CRC
    int msb = CRCmsb>>8;
    return msb;
}

int calculateLSB(int CRClsb) {  //Calculate LSB of the CRC
    int lsb = CRClsb&0x00ff;
    return  lsb;
}

void SH_store_calculateCRC(){ //Function to store Data of the SH in the CRC Buffer
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

  }
}

void DATA_store_calculateCRC(){ //Function to collect and store Data in the CRC Buffer

  char buffstore[1000];
  int counter=0;
  
  int control_flag=0;
  int finalstate=0;
  
  int block=0;
  
  int buff_flag=0;

  Serial.println("Data Blocks Start");

  while(control_flag == 0){
    
    if(block == 0){ //If the data block is unpair
      alternate_answer(0);
      /////
      if(finalstate==1){
        Serial.println("0x10 0x30");
        Serial.println("Data Blocks End");
        flag=2;
        control_flag = 1;
        block = 2;
        break;
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

            //////////////////////////////////////////////
            if( (DEXresponse != '') || (DEXresponse != '') ){
              datastore[savecounter] = DEXresponse;
              savecounter=savecounter+1;
            } 
            //////////////////////////////////////////////
      
            if(DEXresponse == '' ){buff_flag=1;}
            else if(DEXresponse == '' ){buff_flag=1; finalstate=1;Serial.println("The end confirmation is:");}
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
          }
        ///////////////////////////////////
        ///////////////////////////////////
        }
      }
    }

    if(block == 1){ //If the data block is pair
      alternate_answer(1);
      //////
      /////
      if(finalstate==1){
        Serial.println("0x10 0x31");
        Serial.println("Data Blocks End");
        flag=2;
        control_flag = 1;
        block = 2;
        break;
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

            //////////////////////////////////////////////
            if( (DEXresponse != '') || (DEXresponse != '') ){
              datastore[savecounter] = DEXresponse;
              savecounter=savecounter+1;
            } 
            //////////////////////////////////////////////
      
            if(DEXresponse == '' ){buff_flag=1;}
            else if(DEXresponse == '' ){buff_flag=1; finalstate=1;Serial.println("The end confirmation is:");}
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
  control_flag=1;
  Serial.println("All data Send");
}

//////////////////////////////////////////////
String convertToString(char* a, int size) //Convert data char array to String to use String Functions
{
    int x;
    String s = "";
    for (x = 0; x < size; x++) {
        s = s + a[x];
    }
    return s;
}
//////////////////////////////////////////////

void comprobacion(){  //Check if the data collect was correct
  int DX = 0;
  int found;

  //////////////////////////////////////////
  ConvertData = convertToString(datastore, savecounter);
  Serial.println("The String is");
  Serial.println(ConvertData);
  Serial.println("End of String");
  //////////////////////////////////////////

  Serial.println("Checking Data...");
  if(DX==0){
    //////////////////////////////////////////
    found=ConvertData.indexOf("DXS");
    if (found!=-1){
      DX=DX+1;
      Serial.println("DXS was found");  
    }
    //////////////////////////////////////////

    //////////////////////////////////////////
    found=ConvertData.indexOf("DXE");
    if (found!=-1){
      DX=DX+1;
      Serial.println("DXE was found");  
    }
    //////////////////////////////////////////
    Serial.println("Verified Data");
    if(DX==2){
      flag=1;
    }
  }
}

void addcomma(){  //Regex Function to clean the data
  ConvertData.replace(" ", "");
  ConvertData.replace("", "");
  ConvertData.replace("\n", "");
  ConvertData.replace("\n", ",");
  ConvertData.replace("\r", "");
  Serial.println(ConvertData);
  ///////////////////////////
  //FinalData = ConvertData;
  //Serial.println(FinalData);
  ///////////////////////////
}

void send_to_web(){ //Function to open ports and Send data to the server
  Serial.println("SEND TO SERVER");
  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 807))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    Serial.println("Sending");
    // Make a HTTP request:
    client.println("GET /WebServiceTestAppVending.asmx/SendTestDataDEX?identifier="+ESP_identifier+"&dataDex="+ConvertData+" HTTP/1.1");
    client.println("Host: www.app-vending.com");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }
}

void first_handshake(){ //FH Function
  
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
      Serial.println("First Handshake Start");
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

void second_handshake(){  //SH Function
  while(flag == 0){
      while(Serial2.available() == 0){
    }
      DEXresponse = Serial2.read();
      if(DEXresponse == '' ){flag=1;}
  }

  if(flag == 1){
    flag = 0;
    Serial.println("Second Handshake Start");
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
          Serial.println("Second Handshake passed");
          data_handshake();
        }
        //////////////
      }
      
    }
  }
}

void data_handshake(){  //DH Function
  while(flag == 0){
    while(Serial2.available() == 0) {
  }
  DEXresponse = Serial2.read();
  if(DEXresponse == '' ){flag=1;}
  }

  if(flag=1){
    flag = 0;
    Serial.println("Data Handshake Start");
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
        Serial.println("Data Handshake End");
        data_CheckAndReplace();
        Serial.println("Data Handshake End");
        
      }
    //////////
  }
}

void data_CheckAndReplace(){  //Prepare data to send
  comprobacion(); 
  if(flag==1){
    Serial.println("DXS and DXE are in the String");
    Serial.println("Making the String to Send");
    Serial.println("The String with commas is: ");
    addcomma();
    
  }
}

void setup()
{

  Serial.begin(115200); //Computer Serial
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //GSM Serial

//////////////////////////////////////////////////
//////////////////////////////////////////////////
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  client.setCACert(test_root_ca);
//////////////////////////////////////////////////
//////////////////////////////////////////////////

  //TaskSerialPoll.attach(5,first_handshake);

}

void loop()
{
  //disparar si no hay respuesta cada cierto tiempo y si hay 04, esperar 5 min para volver a disparar 
  delay(1000);
  first_handshake();
  delay(1000);
  send_to_web();
  delay(300000);

}
