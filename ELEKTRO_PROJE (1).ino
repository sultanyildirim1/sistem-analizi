
#include <SoftwareSerial.h>
SoftwareSerial sim808(2, 3); 

unsigned long donguzaman = 0;
unsigned long serialzaman = 0;


String phone_no = "5422659439"; 
String msggelen;
String msggiden;
String latitude, longitude, tarih, enlem, boylam, yil, gun, ay, saat, dakika, rakim, hiz, zaman;

#define bir 1
#define onn 10
#define yuz 100
#define ikiyuz 200


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) 
  {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void gpskonum()
{
  Serial.println("GPS Kontrol Ediliyor");
  sim808.print("AT+CGNSINF\r\n");                                           delay(bir);
  readsimPort();

  if (msggelen != "")
  {
    Serial.println("Konum sim808 GELEN:  " );
    Serial.println(msggelen);
  }

  if ((msggelen.indexOf("+CGNSINF: 1,1,") > -1))
  {
    Serial.println("GPS Aktif Konum Bulundu :");                            delay(onn);
    tarih = getValue(msggelen, ',', 2);                                     delay(onn);
    yil = tarih.substring(0, 4);                                            delay(onn);
    //Serial.print("Tarih :" + yil);                                          delay(onn);
    ay = tarih.substring(4, 6);                                             delay(onn);
    //Serial.print("/" + ay);                                                 delay(onn);
    gun = tarih.substring(6, 8);                                            delay(onn);
    //Serial.println("/" + gun);                                              delay(onn);
    saat = tarih.substring(8, 10);                                          delay(onn);
    //Serial.print("Saat :" + saat);                                          delay(onn);
    dakika = tarih.substring(10, 12);                                       delay(onn);
    //Serial.println(":" + dakika);                                           delay(onn);

    enlem = getValue(msggelen, ',', 3);                                     delay(onn);
    latitude = enlem.substring(0, 10);                                      delay(onn);
    //Serial.println("Enlem :" + latitude);                                   delay(onn);

    boylam = getValue(msggelen, ',', 4);                                    delay(onn);
    longitude = boylam.substring(0, 11);                                    delay(onn);
    Serial.println("Boylam :" + longitude);                                 delay(onn);

    Serial.println("Konum Linki:");                                               delay(onn);
    Serial.print("https://www.google.com/maps/search/?api=1&query=" + latitude);  delay(onn);
    Serial.println("," + longitude);                                              delay(onn);

    rakim = getValue(msggelen, ',', 5);                                     delay(onn);
    rakim = rakim.substring(0, 6);                                          delay(onn);
    //Serial.println("Rakım :" + rakim + " metre");                           delay(onn);

    hiz = getValue(msggelen, ',', 6);                                       delay(onn);
    hiz = hiz.substring(0, 5);                                              delay(onn);
    //Serial.println("Hiz :" + hiz + " km/h");                                delay(onn);
    msggelen = "";                                                          delay(onn);
    readsimPort();                                                          delay(onn);

    //mesaj atiliyor
    sim808.print("AT+CMGS=\"+90");
    sim808.print(phone_no);
    sim808.print("\"\r\n");                                                                           delay(50);
    sim808.println("Konum Linki:");                                                                   delay(onn);
    sim808.println("https://www.google.com/maps/search/?api=1&query=" + latitude + "," + longitude);  delay(onn);
    sim808.println("Tarih :" + gun + "/" + ay + "/" + yil);                                           delay(onn);
    sim808.println("Saat :" + saat + ":" + dakika);                                                   delay(onn);
    sim808.println("Hiz :" + hiz + " km/h");                                                          delay(onn);
    sim808.println("Rakim :" + rakim + " metre");                                                     delay(onn);    
    sim808.println((char)26);                                                                         delay(yuz);
    sim808.flush();                                                                                   delay(onn);
  }

  else if ((msggelen.indexOf("+CGNSINF: 1,0,") > -1))
  {
    Serial.println("GPS Aktif Konum Bulunamadi lutfen bekleyiniz:");          delay(onn);

  }
  else if ((msggelen.indexOf("+CGNSINF: 0,") > -1))
  {
    Serial.println("GPS Kapali Acilacak :");                                  delay(onn);
  }
  else
  {
    msggelen = "";                                                            delay(onn);
  }
}

void verikontrol()
{
  if (((msggelen.indexOf("konum") > -1) || (msggelen.indexOf("Konum") > -1) || (msggelen.indexOf("KONUM") > -1)))
  {
    Serial.println("Konum atilacak");                                       delay(yuz);
    msggelen = "";                                                          delay(onn);
    return gpskonum();
  }
}
void setup()
{
  
  Serial.begin(9600);               delay(bir);
  sim808.begin(9600);               delay(bir);


  
  setsim808();
}

void loop()
{
  donguzaman = millis();
  readsimPort();
  readSerialPort();


  if (msggelen != "")
  {
    Serial.println("sim808 GELEN:  " );
    Serial.println(msggelen);
    verikontrol();
    msggelen = "";
  }
  if (msggiden != "")
  {
    Serial.print("bizden giden : " );
    Serial.println(msggiden);
    sim808.print(msggiden);
    msggiden = "";
  }
}


void setsim808()
{
  Serial.println("at komutlar manuel gonderildi"); delay(onn);

  sim808.print("AT+CGPSPWR=1\r\n");               delay(ikiyuz); 
  sim808.print("AT+CGNSSEQ=RMC\r\n");             delay(ikiyuz); 
  sim808.print("AT+CGPSRST=0\r\n");             delay(yuz); 

  sim808.print("AT+CMGF=1\r\n");                delay(ikiyuz);
  sim808.print("AT+CNMi=2,2,0,0,0\r\n");        delay(ikiyuz); 
  sim808.print("AT+CLIP=1\r\n");                delay(ikiyuz);
  sim808.print("AT+DDET=1\r\n");                delay(ikiyuz);
  sim808.print("AT+CNUM\r\n");                  delay(ikiyuz);
  sim808.print("AT+CLCC=1\r\n");                delay(ikiyuz);
  sim808.print("ATS0=1\r\n");                   delay(ikiyuz);
  sim808.print("AT+CMGDA=DEL ALL\r\n");         delay(yuz); 
  sim808.print("AT&W\r\n");                     delay(ikiyuz);

}
void readsimPort()
{
  serialzaman = donguzaman;    delay(bir);
  while ((donguzaman - serialzaman) < 200)
  {
    donguzaman = millis();
    while (sim808.available())
    {
      donguzaman = millis();
      serialzaman = donguzaman;
      char c = sim808.read();  
      msggelen += c; 
    }
  }
  sim808.flush();
}

void readSerialPort() {
  while (Serial.available())
  {
    delay(onn);
    if (Serial.available() > 0)
    {
      char d = Serial.read();  
      msggiden += d; 
    }
  }
  Serial.flush();
}
