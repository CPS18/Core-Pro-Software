#include <LiquidCrystal.h>
#include <SD.h>
#include <DS3231.h>
#include <OneWire.h> 
#include <Wire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#define DHTPIN 10  
#define ONE_WIRE_BUS 41
#define DHTTYPE DHT22   
LiquidCrystal lcd(12, 11, 5, 4, 3, 8);
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
DS3231  rtc(SDA, SCL);
Time  t;
File file;
const byte buttonPin1 = 22;
const byte buttonPin2 = 24;   
const byte buttonPin3 = 26;   
const byte buttonPin4 = 28;
const byte LedR = 32;   
const byte LedG = 30;    
const byte screenWidth = 16;
const byte screenHeight = 2;
const byte trigPin = 36;
const byte echoPin = 34;
const byte LedPin20=38;
const byte LedPin50=40;
const byte LedPin75=42;
const byte LedPin100=44;
const byte swP=31;
const byte swF=33;
const byte swA=37;
const byte buzzer = 6;
const byte fan=7;
const byte ControlPin = 9;
const byte chipSelect = 53;
const byte sensorInterrupt = 0;
const byte sensorPin = 2;
int sensor_pin = A1; 
unsigned int percenti=0;
int analogInput = 0;
int photocellPin0 = 2; 
static unsigned int change1=1;
static unsigned int statusi=0;
float vout = 0.0;
int photocellReading0; 
float vin = 0.0;
boolean flag=false;
boolean stat=false;
boolean stat2=false;
int value = 0;
float Res0=10.0; 
float R1 = 10000.0; // resistance of R1 (10K) -see text!
float R2 = 4739.0; // resistance of R2 (4.7K) - see text!
float temp=0;
float Hum=0;
float temp2=0;
float curvolt=0;
int Moisure=0;
int lux0=0;
char degree=(char)223;
char degree2=(char)176;
String WaterErorr="Water tank is empty ";
String SensorErorr="Temp and Hum sensor didnt work correctly ";
String SDERR="Could not initialize SD card ";
String BateryErorr="Battery to low ";
String SDWritErr="";
String Erorr="";
String One="Temp:";
String T="SoilTem:";
String L="Bright:";
String prog="P:";
String H="Hum:";
String M="Mois:";
String W="Water:";
String per="% ";
String Cel="C ";
String lx=" lx";
String des="";
long last_millis = 0;
long last_milliss = 0;
long last_millisss=0;
long last_millis2=0;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
float calibrationFactor = 4.5;
unsigned long totalLiters=0;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;
String line1="";
String line2="";
int stringStart, stringStop = 0;
int scrollCursor = screenWidth;
void setup() {
  Serial.begin(9600);
  lcd.begin(screenWidth,screenHeight);
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
  dht.begin();
  sensors.begin();
  rtc.begin();
  pinMode(buzzer, OUTPUT); 
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LedPin20, OUTPUT);
  pinMode(LedPin50, OUTPUT);
  pinMode(LedPin75, OUTPUT);
  pinMode(LedPin100, OUTPUT);
  pinMode(chipSelect, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT); 
  pinMode(swP, OUTPUT); 
  pinMode(swF, OUTPUT); 
  pinMode(swA, OUTPUT); 
  pinMode(ControlPin, OUTPUT);
  //rtc.setTime(17, 13, 30);    
  //rtc.setDate(19, 5, 2018);
  SD_Init();
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  delay(4000);
}

void loop() {
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  Sensors();
  printout();
  Timer();
  Errors();
  Water_flow();
  Water_level();
  batterylevel(15,1);
  change1=Change();
  
  switch (change1) {
    case 1:
    des=" Pepper Sow";
    
    if (buttonState3 == HIGH) {
      statusi++;  
    }
    Peppersow_controll();
      break;
      
    case 2:
      des=" Pepper";
    if (buttonState3 == HIGH) {
       statusi++;
      
    }
      Peppergrow_controll();
      break;
      
    case 3:
      des=" Tomato Sow";
    if (buttonState3 == HIGH) {
       statusi++;
    }
      Tomatosow_controll();
      break;
      
      case 4:
      des=" Tomato";
    if (buttonState3 == HIGH) {
       statusi++;
    }
      Tomatogrow_controll();
      break;
      default:
      des="";
      break;
  }
   if (buttonState4 == HIGH) {
       statusi=0;
      
    }    

}
void Peppersow_controll(){
  const unsigned int Tempmax=34;
  const unsigned int Tempmin=30;
  const unsigned int Moismax=80;
  const unsigned int Moismin=65;
  const unsigned int Hummax=80;
  const unsigned int Hummin=70;
   analogWrite(fan,0);
   digitalWrite(swP,LOW);
   digitalWrite(swA,LOW);
   analogWrite(ControlPin,0); 
  
  if(statusi>0){
    digitalWrite(LedR,LOW);
    digitalWrite(LedG,HIGH);

    if(Tempmin<=temp){
   if((temp>Tempmax && Tempmin<temp) || Hum > 90){
      analogWrite(fan,255);
    }
    else{
      analogWrite(fan,0);
    }
 }
 else{
     analogWrite(fan,0);
  }
    if(Hum>Hummax || Moisure>Moismax){
      digitalWrite(swP,LOW);
      analogWrite(ControlPin,0); 
    }
    else if(Hummin>Hum){
      unsigned long now_milliss = millis();
       digitalWrite(swP,HIGH); 
       if(now_milliss - last_milliss > 500)
        {
          last_milliss = now_milliss;
          analogWrite(ControlPin,255);
        }
    }
    if(Moismin>Moisure && Moisure<=Moismax){
       digitalWrite(swA,HIGH); 
      unsigned long now_millisss = millis();
        if(now_millisss - last_millisss > 500)
        {
          last_millisss = now_millisss;
          analogWrite(ControlPin,255);
        }
    }
    else{
      digitalWrite(swA,LOW);
      analogWrite(ControlPin,0); 
    }
    
  }
}
void Peppergrow_controll(){
  const unsigned int Tempmax=27;
  const unsigned int Tempmin=24;
  const unsigned int Moismax=80;
  const unsigned int Moismin=65;
  const unsigned int Hummax=80;
  const unsigned int Hummin=70;
  analogWrite(fan,0);
  digitalWrite(swP,LOW);
  digitalWrite(swF,LOW);
  analogWrite(ControlPin,0);
  if(statusi>0){
    digitalWrite(LedR,LOW);
    digitalWrite(LedG,HIGH);
   if((temp>Tempmax && Tempmin<temp) || Hum > Hummax+5){
      analogWrite(fan,255);
    }
    else{
      analogWrite(fan,0);
    }
    if(Hum>Hummax || Moisure>Moismax){
      digitalWrite(swP,LOW);
      analogWrite(ControlPin,0); 
    }
    else if(Hummin>Hum){
      unsigned long now_milliss = millis();
       digitalWrite(swP,HIGH); 
       if(now_milliss - last_milliss > 500)
        {
          last_milliss = now_milliss;
          analogWrite(ControlPin,255);
        }
    }
    if(Moismin>Moisure && Moisure<=Moismax){
       digitalWrite(swF,HIGH); 
      unsigned long now_millisss = millis();
        if(now_millisss - last_millisss > 500)
        {
          last_millisss = now_millisss;
          analogWrite(ControlPin,255);
        }
    }
    else{
      digitalWrite(swF,LOW);
      analogWrite(ControlPin,0); 
    }
  }
}
void Tomatosow_controll(){
  const unsigned int Tempmax=27;
  const unsigned int Tempmin=25;
  const unsigned int Moismax=70;
  const unsigned int Moismin=40;
  const unsigned int Hummax=70;
  const unsigned int Hummin=60;
  analogWrite(fan,0);
  digitalWrite(swP,LOW);
  digitalWrite(swA,LOW);
   analogWrite(ControlPin,0); 
  if(statusi>0){
    digitalWrite(LedR,LOW);
    digitalWrite(LedG,HIGH);
 if(Tempmin<=temp){
   if((temp>Tempmax && Tempmin<temp) || Hum > 90){
      analogWrite(fan,255);
    }
    else{
      analogWrite(fan,0);
    }
 }
 else{
     analogWrite(fan,0);
  }
   if(Hum>Hummax || Moisure>Moismax){
      digitalWrite(swP,LOW);
      analogWrite(ControlPin,0); 
    }
     else if(Hummin>Hum){
      unsigned long now_milliss = millis();
       digitalWrite(swP,HIGH); 
       if(now_milliss - last_milliss > 500)
        {
          last_milliss = now_milliss;
          analogWrite(ControlPin,255);
        }
    }
     if(Moismin>Moisure && Moisure<=Moismax){
       digitalWrite(swA,HIGH); 
      unsigned long now_millisss = millis();
        if(now_millisss - last_millisss > 500)
        {
          last_millisss = now_millisss;
          analogWrite(ControlPin,255);
        }
    }
    else{
      digitalWrite(swA,LOW);
      analogWrite(ControlPin,0); 
    }
  }
}
void Tomatogrow_controll(){
  const unsigned int Tempmax=24;
  const unsigned int Tempmin=21;
  const unsigned int Moismax=70;
  const unsigned int Moismin=40;
  const unsigned int Hummax=70;
  const unsigned int Hummin=60;
  analogWrite(fan,0);
  digitalWrite(swP,LOW);
  digitalWrite(swF,LOW);
  analogWrite(ControlPin,0);
  if(statusi>0){
    digitalWrite(LedR,LOW);
    digitalWrite(LedG,HIGH);
    if((temp>Tempmax && Tempmin<temp) || Hum > Hummax+5){
      analogWrite(fan,255);
    }
    else{
      analogWrite(fan,0);
    }
    if(Hum>Hummax || Moisure>Moismax){
      digitalWrite(swP,LOW);
      analogWrite(ControlPin,0); 
    }
    else if(Hummin>Hum){
      unsigned long now_milliss = millis();
       digitalWrite(swP,HIGH); 
       if(now_milliss - last_milliss > 500)
        {
          last_milliss = now_milliss;
          analogWrite(ControlPin,255);
        }
    }
    if(Moismin>Moisure && Moisure<=Moismax){
       digitalWrite(swF,HIGH); 
      unsigned long now_millisss = millis();
        if(now_millisss - last_millisss > 500)
        {
          last_millisss = now_millisss;
          analogWrite(ControlPin,255);
        }
    }
    else{
      digitalWrite(swF,LOW);
      analogWrite(ControlPin,0); 
    }
  }
}
unsigned int Change(){
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  static unsigned int change=1;
  if(statusi==0){
   digitalWrite(LedR,HIGH);
   digitalWrite(LedG,LOW);
  if (buttonState1 == HIGH) {
    change--;
    
  }
   if (buttonState2 == HIGH) {
    change++;
    
  }
  if(change<=65535 && change>4 || change==0)
  {
    change=1;
  }
}
  return change;
}
void printout(){

 if(stat2==true){
  //line1="";
  String at=prog + change1;
  line1=Erorr;
  line2=at + des;
  lcd.setCursor(scrollCursor, 0);
  lcd.print(line1.substring(stringStart,stringStop));
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(500);
  lcd.clear();
  if(stringStart == 0 && scrollCursor > 0){
    scrollCursor--;
    stringStop++;
  } else if (stringStart == stringStop){
    stringStart = stringStop = 0;
    scrollCursor = screenWidth;
  } else if (stringStop == line1.length() && scrollCursor == 0) {
    stringStart++;
  } else {
    stringStart++;
    stringStop++;
  }
}
else{
//line1="";
String Two=One+ temp;
String Three=Two + degree + Cel + H;
String Fourth=Three + Hum;
String Fifthy=Fourth + per + M;
String Six=Fifthy + Moisure;
String Seven=Six + per + T;
String Up=Seven + temp2;
String Up2=Up + degree + Cel + W;
String Eig=Up2 + percenti; 
String Ten=Eig + per + L + lux0;
String at=prog + change1;
  line1=Ten + lx;
  line2=at + des;
  lcd.setCursor(scrollCursor, 0);
  lcd.print(line1.substring(stringStart,stringStop));
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(500);
  lcd.clear();
  if(stringStart == 0 && scrollCursor > 0){
    scrollCursor--;
    stringStop++;
  } else if (stringStart == stringStop){
    stringStart = stringStop = 0;
    scrollCursor = screenWidth;
  } else if (stringStop == line1.length() && scrollCursor == 0) {
    stringStart++;
  } else {
    stringStart++;
    stringStop++;
  }
}
  
}
void batterylevel(int xpos,int ypos)
{
  if(curvolt >= 13.5)
  {
    byte batlevel[8] = {
    B01110,
    B11111,
    B10101,
    B10001,
    B11011,
    B11011,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 13.4 && curvolt > 12.0)
  {
    byte batlevel[8] = {
    B01110,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 12.0 && curvolt > 11.5)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 11.5 && curvolt > 11.0)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 11.0 && curvolt > 10.7)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 10.7 && curvolt > 10.2)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 10.2 && curvolt > 9.7)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt < 9.7)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
}
float ReadVcc(){
  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0;
  vin = vout / (R2/(R1+R2));
  return vin; 
}
unsigned int Water_level(){
  const float percentmid=50.00;
  const float cmmid=10.00;
  float duration,correct,conv,distance;
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  //delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  correct=distance-1.00;
  conv=(correct*percentmid)/cmmid;
  percenti=100.00-conv;
  if(percenti < 0)
  {
    percenti=0;
  }

  if(percenti < 20){
    digitalWrite(LedPin20, HIGH);
    digitalWrite(LedPin50, LOW);
    digitalWrite(LedPin75, LOW);
    digitalWrite(LedPin100, LOW);  
  }
  else if(percenti >20 && percenti <= 50){
    digitalWrite(LedPin20, LOW);
    digitalWrite(LedPin50, HIGH);
    digitalWrite(LedPin75, LOW);
    digitalWrite(LedPin100, LOW);
  }
 else if(percenti >50 && percenti <= 75){
    digitalWrite(LedPin20, LOW);
    digitalWrite(LedPin50, HIGH);
    digitalWrite(LedPin75, HIGH);
    digitalWrite(LedPin100, LOW);
  }
  else{
    digitalWrite(LedPin20, LOW);
    digitalWrite(LedPin50, HIGH);
    digitalWrite(LedPin75, HIGH);
    digitalWrite(LedPin100, HIGH);
  }
  return percenti;
  
}
float Sensors(){
  unsigned long now_millis = millis();
if (flag==false){
 if(now_millis - last_millis > 3000)
  {
  last_millis = now_millis;
  curvolt=ReadVcc();
  Moisure= analogRead(sensor_pin);
  Serial.println(Moisure);
  //Moisure = map(Moisure,120,270,0,100);
 Moisure = map(Moisure,10,1023,0,100);
  Serial.println(Moisure);
  sensors.requestTemperatures(); 
  Hum = dht.readHumidity();
  temp = dht.readTemperature();
  temp2=sensors.getTempCByIndex(0);
  fotoresistor();
  flag=true;
  }
}
if(flag==true){
 if(now_millis - last_millis > 60000)
  {
   last_millis = now_millis;
   if(percenti<=11 || percenti>=100 || stat2==true)
   {
     stringStart=0;
     stringStop = 0;
   }
  Moisure= analogRead(sensor_pin);
  Moisure = map(Moisure,10,1023,0,100);
  //Moisure = map(Moisure,120,270,0,100);
  curvolt=ReadVcc();  
  sensors.requestTemperatures(); 
  Hum = dht.readHumidity();
  temp = dht.readTemperature();
  temp2=sensors.getTempCByIndex(0);
    fotoresistor();
  }
 }
}

int fotoresistor(){
 photocellReading0 = analogRead(photocellPin0);  
 float Vout0=photocellReading0*0.0048828125;      
 lux0=500/(Res0*((5-Vout0)/Vout0))*10;           
 return lux0;        
}

void SD_Write(){
  
   file = SD.open("Datalog.csv", FILE_WRITE); // open "file.txt" to write data
  if (file) {
    file.print(rtc.getDateStr());
    file.print(" -- ");
    file.print(rtc.getTimeStr());
    file.print(",");
    file.print(temp);
    file.print(",");
    file.print(Hum);
    file.print(",");
    file.print(percenti);
    file.print(",");
    file.print(temp2);
    file.print(",");
    file.print(Moisure);
    file.print(",");
    file.print(lux0);
    file.print(",");
    file.println(totalLiters);
    file.close(); // close file
  } else {
    SDWritErr="Could not open file (writing). ";
    Erorr+=SDWritErr;
  }


  
}
void SD_Init(){
  if (!SD.begin(chipSelect)) { // Initialize SD card
    Serial.println("Could not initialize SD card."); // if return value is false, something went wrong.
  }
    if (SD.exists("Datalog.csv")) { // if "file.txt" exists, fill will be deleted
    Serial.println("Files exists.");
    if (SD.remove("Datalog.csv") == true) {
      Serial.println("Successfully removed file.");
    } else {
      Serial.println("Could not removed file.");
    }
  }

  file = SD.open("Datalog.csv", FILE_WRITE);
  if(file)
  {
    file.print("Date & Time");    
    file.print(",");
    file.print("Temperature (");
    file.print(degree2);
    file.print("C)");
    file.print(",");
    file.print("Humidity (%)");
    file.print(",");
    file.print("Water Level (%)");
    file.print(",");
    file.print("Temperature Soil (");
    file.print(degree2);
    file.print("C)");
    file.print(",");
    file.print("Soil Moisure (%)");
    file.print(",");
    file.print("Brightness (lx)");
    file.print(",");
    file.println("Water Consumption (L)");
    file.close();
    
  }
  
}
void Timer(){
   t = rtc.getTime();
   
 if(stat==false){
  if (t.hour == 4  && t.min==0 && t.sec==0) 
   {
     SD_Write();
     stat=true;
     
   }else if(t.hour == 8  && t.min==0 && t.sec==0.000){
     SD_Write();
     stat=true;
   }else if(t.hour == 12  && t.min==0 && t.sec==0){
     SD_Write();
     stat=true;
   }else if(t.hour == 16  && t.min==0 && t.sec==0){
     SD_Write();
     stat=true;
   }else if(t.hour == 20  && t.min==0 && t.sec==0){
     SD_Write();
     stat=true;
   }else if(t.hour == 23  && t.min==59 && t.sec==0){
     SD_Write();
     stat=true;
   }

 }
 if(t.min==01 && t.sec==0.00){
    stat=false;
   }
}
void Water_flow(){
   if((millis() - oldTime) > 1000) 
  { 
    detachInterrupt(sensorInterrupt);  
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60)*1000;
    totalMilliLitres += flowMilliLitres;
    totalLiters=totalMilliLitres/1000;  
    unsigned int frac;
    frac = (flowRate - int(flowRate)) * 10;
    pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
  
}

void pulseCounter()
{
  pulseCount++;
}
void Errors(){
unsigned long now_millis2 = millis();
if(now_millis2 - last_millis2 > 50000)
{
  last_millis2 = now_millis2;
   if(percenti < 11){
   tone(buzzer, 1500);
   analogWrite(ControlPin,0);
   Erorr+=WaterErorr;
   digitalWrite(swF,LOW);
   digitalWrite(swA,LOW);
   digitalWrite(swP,LOW);
   statusi=0;
   stat2=true;
       
 }
 else if (isnan(Hum) || isnan(temp)) {
    Erorr+=SensorErorr;
    tone(buzzer, 1500);
    statusi=0;
    stat2=true;
  }
 else if (!SD.begin(chipSelect)) {
    Erorr+=SDERR;
    tone(buzzer, 1500);
    statusi=0;
    stat2=true;
  }
  else if(curvolt<=9.5){
    Erorr+=BateryErorr;
    tone(buzzer,1500);
    statusi=0;
    stat2=true; 
  }
 else{
  noTone(buzzer);
  Erorr="";
  stat2=false;
 }        
}
  
   
}

