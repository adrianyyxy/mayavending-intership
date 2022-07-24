/* ----------------------- GLOBAL CONSTANTS -------------------------- */

#define SUCCESS 0 // function will return 0 if successful
#define FAIL  -1  // Function will return -1 if unsuccessful
#define MILLI_SECONDS 15

int i;

byte buffstore[2];

uint16_t   BCC, BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;
String CRC1 = "6150210000RR01L01";

uint16_t buff1;
uint16_t buff2;

/* ------------------------- DEX Commands --------------------------- */

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

/* ------------------------- ESP Pin --------------------------- */

#define RXD2 16
#define TXD2 17

/* ------------------------- Functions --------------------------- */

void alternate_answer(int j)
{
  delay(MILLI_SECONDS);
  Serial.write(DLE);
  delay(MILLI_SECONDS);
  Serial.write(0x30+(j));
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

void store_twohex()
{
  for(i=0;i<3;i++)
  {
    buffstore[i]=Serial.read();
    delay(MILLI_SECONDS);
  }
}

//agregar un return para confirmar
void first_handshake()
{
  Serial.write(0x05);
  if(Serial.available()>0)
  {
    store_twohex();
    
    if(buffstore[0] == 0x10 && buffstore[1] == 0x30)
    {
      //10h 01h
      Serial.write(0x10);
      Serial.write(0x01);

      //6150210000 
      Serial.write(0x36);
      Serial.write(0x31);
      Serial.write(0x35);
      Serial.write(0x30);
      Serial.write(0x32);
      Serial.write(0x31);
      Serial.write(0x30);
      Serial.write(0x30);
      Serial.write(0x30);
      Serial.write(0x30);

      //R 
      Serial.write(0x52);
      //R01 
      Serial.write(0x52);
      Serial.write(0x30);
      Serial.write(0x31);
      //L01
      Serial.write(0x4C);
      Serial.write(0x30);
      Serial.write(0x31);

      //10h 03h
      Serial.write(0x10);
      Serial.write(0x03);

      /* ------------------------- CRC --------------------------- */
      byte buf[CRC1.length()+1 ];
      CRC1.getBytes(buf, CRC1.length()+1 );

      BCC=0;
      for(i=0;i<=17;i++)
      {
        crc_16(buf[i]);
      }

      buff1 = calculateMSB(BCC);
      buff2 = calculateLSB(BCC);

      Serial.write(buff1);
      Serial.write(buff2);
      /* --------------------------------------------------------- */

      store_twohex();
    
      if(buffstore[0] == 0x10 && buffstore[1] == 0x31)
      {
        Serial.write(0x04);
      }

    }
  }
}

void setup()
{

  Serial.begin(115200); //Computer Serial
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //Machine Vending Serial

}

void loop()
{

}
