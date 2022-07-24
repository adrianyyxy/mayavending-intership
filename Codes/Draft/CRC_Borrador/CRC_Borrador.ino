int i;

#include "Crc16.h"

//Crc16 crc(true, true, 0x8005, 0xffff, 0x0000, 0x8000, 0xffff);

//example ID
String comID = "6150210000RR01L01";
//String comID = "36313530323130303030525230314c3031";

//Método para calcular el CRC
uint16_t calcularCrc16C(String dato){

  int table[] = {
              0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
              0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
              0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
              0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
              0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
              0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
              0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
              0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
              0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
              0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
              0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
              0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
              0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
              0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
              0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
              0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
              0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
              0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
              0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
              0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
              0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
              0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
              0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
              0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
              0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
              0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
              0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
              0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
              0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
              0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
              0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
              0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
      };

      byte buf[dato.length()+1 ];
  
      //byte bytes[] = dato.getBytes();
      dato.getBytes(buf, dato.length()+1 );

      Serial.println("inicio");

      for(i=0; i<dato.length()+1 ;i++){
      Serial.write(buf[i]);
      }
      Serial.println(" ");
      Serial.println("salir");

      //byte buf[18] = {0x36,0x31,0x35,0x30,0x32,0x31,0x30,0x30,0x30,0x30,0x52,0x52,0x30,0x31,0x4c,0x30,0x31};
      
      uint16_t crc = 0x0000;
      
      for (byte b : buf) {
          crc = (crc >> 8) ^ table[(crc ^ b) & 0xff];

          /*crc = (crc >> 8) ^
              table_byte[(crc ^ *data++) & 0xff];*/
      }
      return crc;

///////////////////////
      /*crc.clearCrc();
  for(byte i=0;i<6;i++)
  {
     Serial.print("byte ");
     Serial.print(i);
     Serial.print(" = ");
     Serial.println(buf[i]);
     crc.updateCrc(buf[i]);
  }
  unsigned short value = crc.getCrc();
  Serial.print("crc = 0x");
  Serial.println(value, HEX);

  Serial.println("Calculating crc in a single call");

  //Modbus
  value = crc.Modbus(buf,0,6);
  Serial.print("Modbus crc = 0x");    
  Serial.println(value, HEX);*/
  ////////////////////////
  
}



//Calcular menos significativo
//calcularMenosSignificativo(crcRes);

//Calcular mas significativo
//calcularMasSignificativo(crcRess);

//Método para calcular valor mas significativo
//public static 
int calcularMasSignificativo(int crc16) {
    int msb = crc16>>8;
    return msb;
}

//Método para calcular el menos significativo
//public static 
int calcularMenosSignificativo(int crc16) {
    int lsb = crc16&0x00ff;
    return  lsb;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  Serial.println("hola mundo");

  //Calcula el CRC
  //uint16_t crcRes = calcularCrc16C(comID + 0x03);
  uint16_t crcRes = calcularCrc16C(comID);

  Serial.println(crcRes, HEX);

}

void loop() {
  // put your main code here, to run repeatedly:

  
  

}