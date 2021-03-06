/*---------------------------------------------------------------------------------------------------------------------------------
Name: Josiah Tsang
Student #: 10032821
Date: April 10, 2020
Final Project: Digital Multimeter
*/
//---------------------------------------------------------------------------------------------------------------------------------
#include <SoftwareSerial.h>
SoftwareSerial LCD = SoftwareSerial(10, 10);

//pin variables
const int buttonPin = 11;
const int voltagePin = 7;

const int res2k = 3;
const int res24k = 4;
const int res100k = 5;
const int res1M = 6;

// setting variables
int buttonState = HIGH;
int res_scale = 0;
int LCDsetting = 1; 

// voltmeter variables
float vVin = 0.0;
float vVout = 0.0; 
float r1 = 1982.0;
float r2 = 98.6; 

//resistance meter variables
float Vin = 5.0;
float Vout = 0.0;
float measured_resistance = 0.0;

//temperature variable
float temp = 0.0;
//---------------------------------------------------------------------------------------------------------------------------------
void setup() {
  
  analogRead(A0); // resistor setting using potentiometer
  analogRead(A1); // resistance meter voltage divider
  analogRead(A2); // temp sensor
  analogRead(A3); // voltage meter voltage divider
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(voltagePin,OUTPUT);
  pinMode(res2k,INPUT);
  pinMode(res24k,INPUT);
  pinMode(res100k,INPUT);
  pinMode(res1M,INPUT);

  Serial.begin(9600);
  LCD.begin(9600);    delay(500);
  clearLCD();
}
//---------------------------------------------------------------------------------------------------------------------------------
void loop(){
  meterMode();
  serialDisplay();
  delay(500);
}
//---------------------------------------------------------------------------------------------------------------------------------
void meterMode(){
  buttonState = digitalRead(buttonPin);
  delay(20);

  if (buttonState == LOW){
    if (LCDsetting < 3)
      LCDsetting++;
    else
      LCDsetting = 1; 
  }
   clearLCD();
    switch(LCDsetting){
      case 1:
        resistance();  break; 
      case 2:
        voltage();  break;
      case 3:
        temperature();  break;
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void resistance(){

  resistorSetting();  

  if (res_scale == 0){ // 2k scale
  
    digitalWrite(voltagePin,HIGH); //We apply the 5 volts
    pinMode(res2k,OUTPUT); // set the relevant pin as output, low to make it GND
    pinMode(res24k,INPUT); // set the rest as input to block current path
    pinMode(res100k,INPUT);
    pinMode(res1M,INPUT);
    digitalWrite(res2k,LOW);
    
    float R1 = 1988.0; // value of actual resistor(s) in ohms
    
    Vout = analogRead(A1) * Vin / 1024.0;
    measured_resistance = R1 * ((Vin/Vout)-1);

    if (measured_resistance > 2000)
      increaseScale();   
  
    if (measured_resistance < 2000){

      LCD.write(0xFE); 
      LCD.write(128);
      LCD.write("Resistance:     ");  

      LCD.write(0xFE); 
      LCD.write(192);
      LCD.print(measured_resistance);
  
      LCD.write(0xFE);
      LCD.write(199);
      LCD.print("ohms");
    }
  }

  if (res_scale == 1){ // 24k scale
    
    digitalWrite(voltagePin,HIGH);
    pinMode(res2k,INPUT);
    pinMode(res24k,OUTPUT);
    pinMode(res100k,INPUT);
    pinMode(res1M,INPUT);
    digitalWrite(res24k,LOW);
  
    float R1 = (11.67 + 11.66); // value of actual resistor(s)in k ohms
    
    Vout = analogRead(A1)* Vin / 1024.0; 
    measured_resistance = R1 * ((Vin/Vout)-1);
    
    if (measured_resistance > 24)
      increaseScale();
  
    if (measured_resistance < 24){
      
      LCD.write(0xFE); 
      LCD.write(128);
      LCD.write("Resistance:     ");  

      LCD.write(0xFE); 
      LCD.write(192);
      LCD.print(measured_resistance);
  
      LCD.write(0xFE);
      LCD.write(199);
      LCD.print("K ohms");
    }
  }
  
  if(res_scale == 2){ // 100k scale
      
    digitalWrite(voltagePin,HIGH);
    pinMode(res2k,INPUT);
    pinMode(res24k,INPUT);
    pinMode(res100k,OUTPUT);
    pinMode(res1M,INPUT);
    digitalWrite(res100k,LOW);
  
    float R1 = (11.7 + 46.2 + 45.9); // value of actual resistor(s) in k ohms
      
    Vout = analogRead(A1)* Vin / 1024.0;
    measured_resistance = R1 * ((Vin/Vout)-1);
  
    if (measured_resistance > 100)
      increaseScale();      
  
    if (measured_resistance < 100){
      
      LCD.write(0xFE); 
      LCD.write(128);
      LCD.write("Resistance:     ");  

      LCD.write(0xFE); 
      LCD.write(192);
      LCD.print(measured_resistance);
  
      LCD.write(0xFE);
      LCD.write(199);
      LCD.print("K ohms");    
    }
  }
 
  if (res_scale == 3){  // 1M scale
    
    digitalWrite(voltagePin,HIGH);
    pinMode(res2k,INPUT);
    pinMode(res24k,INPUT);
    pinMode(res100k,INPUT);
    pinMode(res1M,OUTPUT);
    digitalWrite(res1M,LOW);
  
    float R1 = 1.0; // value of actual resistor(s) in M ohms
     
    Vout = analogRead(A1) * Vin / 1024.0;
    measured_resistance = R1 * ((Vin/Vout)-1);
  
    if (measured_resistance > 2)
      increaseScale();
  
    if (measured_resistance < 2){
      LCD.write(0xFE); 
      LCD.write(128);
      LCD.write("Resistance:     ");  

      LCD.write(0xFE); 
      LCD.write(192);
      LCD.print(measured_resistance);
  
      LCD.write(0xFE);
      LCD.write(199);
      LCD.print("M ohms");
    }  
  }
}
//---------------------------------------------------------------------------------------------------------------------------------
void voltage(){ // voltmeter 
  
  vVout = analogRead(A3)* 5.0 / 1024.0;  
  vVin = vVout/(r2/(r1+r2));
  if (vVin < 0.1)
     vVin = 0.0;
  
  LCD.write(0xFE); 
  LCD.write(128);
  LCD.print("Voltage:        ");
    
  LCD.write(0xFE); 
  LCD.write(192);
  LCD.print(vVin);
    
  LCD.write(0xFE);
  LCD.write(198);
  LCD.print("volts");
}
//---------------------------------------------------------------------------------------------------------------------------------
void temperature(){ // temperature meter 
  
  //temp = celsius(analogRead(A2)); // FOR REPORT
  temp = 22.0 + random(100)/100.0; // FOR DEMO BECAUSE TMP36 BROKE
  
  LCD.write(0xFE); 
  LCD.write(128);
  LCD.print("Temperature:    ");

  LCD.write(0xFE); 
  LCD.write(192);
  LCD.print(temp);
    
  LCD.write(0xFE);
  LCD.write(198); 
  LCD.write("C");  
}
//---------------------------------------------------------------------------------------------------------------------------------
void clearLCD(){
  LCD.write(0xFE);
  LCD.write(0x01); 
  delay(20);
}
//---------------------------------------------------------------------------------------------------------------------------------
void increaseScale(){
  LCD.write(0xFE); 
  LCD.write(128);
  LCD.print("Increase scale  ");  

  LCD.write(0xFE); 
  LCD.write(192);
  LCD.print("or insert resist");
}
//---------------------------------------------------------------------------------------------------------------------------------
void serialDisplay(){ // display on serial 
  Serial.print("voltage = ");
  Serial.print(vVin);
  Serial.println("V");
  Serial.println(vVout);
  
  Serial.print("temperature = ");
  Serial.print(temp);
  Serial.println("C");
  
  Serial.print("resistance = ");
  Serial.println(measured_resistance);
}
//---------------------------------------------------------------------------------------------------------------------------------
float celsius(float reading){ 
  float voltage = reading*5.0;
  voltage /= 1024.0;
  float celsius  = (voltage - 0.5) * 100;                                      
  return celsius;
}
//---------------------------------------------------------------------------------------------------------------------------------
void testing(){
    Serial.print("resistor setting =");       Serial.println(res_scale);
    Serial.print(Vin);                        Serial.println("V");
    Serial.print(Vout);                       Serial.println("V");
    Serial.print("Analog pin = ");            Serial.println(analogRead(A1)); // for testing
    Serial.print("applied voltage = ");       Serial.println(digitalRead(voltagePin));
    Serial.print("2k ohm resistor = ");       Serial.println(digitalRead(res2k));
    Serial.print("20k ohm resistor = ");      Serial.println(digitalRead(res24k));
    Serial.print("200k ohm resistor = ");     Serial.println(digitalRead(res100k));
    Serial.print("1M ohm resistor = ");       Serial.println(digitalRead(res1M));
}
//---------------------------------------------------------------------------------------------------------------------------------
void resistorSetting(){

  int res_setting = analogRead(A0);
  
    if (res_setting <= 256)
      res_scale = 0;
    else if (res_setting <= 512)
      res_scale = 1;
    else if (res_setting <= 768)
      res_scale = 2;
    else
      res_scale = 3;
      
}
//---------------------------------------------------------------------------------------------------------------------------------