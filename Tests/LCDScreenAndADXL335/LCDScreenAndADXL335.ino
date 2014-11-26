#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

const int ap1 = A2; 
const int ap2 = A1;
const int ap3 = A0;

int sv1 = 0;        
int ov1 = 0;    
int sv2 = 0;      
int ov2= 0;      
int sv3 = 0;       
int ov3= 0;      

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();



void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.print("Accelerometer");
  lcd.setBacklight(WHITE);
}

void loop() {
  analogReference(EXTERNAL);    //connect 3.3v to AREF
  // read the analog in value:
  sv1 = analogRead(ap1);            
  // map it to the range of the analog out:
  ov1 = map(sv1, 0, 1023, 0, 255);  
  // change the analog out value:
  delay(2);                     
  //
  sv2 = analogRead(ap2);            
  
  ov2 = map(sv2, 0, 1023, 0, 255); 
 // 
  delay(2);                 
  //
  sv3 = analogRead(ap3);            
  
  ov3 = map(sv3, 0, 1023, 0, 255);  
  
  // print the results to the serial monitor:
  
  lcd.setCursor(0, 0);
  String s("X: ");
  s += sv1;
  s +=  " - Y: ";
  s += sv2;
  
  lcd.print(s);
  lcd.setCursor(0, 1);
  String t("Z: ");
  t += sv3;
  lcd.print(t);

  delay(500);                     
  
}
