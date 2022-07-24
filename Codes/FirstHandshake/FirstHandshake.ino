uint16_t   BCC, BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;

uint16_t hola1;
uint16_t hola2;

//byte comID[18] = {'6','1','5','0','2','1','0','0','0','0','R','R','0','1','L','0','1',0x03};

String comID = "6150210000RR01L01";

////////////////////////////////////////////////////////////////////////
  /* CRC check routine  */
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
  ////////////////////////////////////////////////////////////////////////

int calcularMasSignificativo(int CRCmsb) {
    int msb = CRCmsb>>8;
    return msb;
}

int calcularMenosSignificativo(int CRClsb) {
    int lsb = CRClsb&0x00ff;
    return  lsb;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

    int i;

  

  byte buf[comID.length()+1 ];
  
      comID.getBytes(buf, comID.length()+1 );

  BCC=0;
  for(i=0;i<=17;i++){
    crc_16(buf[i]);
  }

  /*
    for(i=0;i<=17;i++){
    crc_16(comid[i]);
  }
  */

  //Calcular menos significativo
hola1 = calcularMenosSignificativo(BCC);

//Calcular mas significativo
hola2 = calcularMasSignificativo(BCC);

  delay(5000);
  /*Serial.print(BCC,HEX);
  Serial.print('\n');

  Serial.print(hola1,HEX);
  Serial.print('\n');

  Serial.print(hola2,HEX);
  Serial.print('\n');*/

  Serial.write(hola1);

  Serial.write(hola2);

}
