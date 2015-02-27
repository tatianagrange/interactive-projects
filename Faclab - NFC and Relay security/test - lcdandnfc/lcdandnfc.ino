#include <LiquidCrystal.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

////////////////////////////////////////////////
//                  Define                    //
////////////////////////////////////////////////
#define TIME_MAX 70 //1200 //20 min


////////////////////////////////////////////////
//                Declarations                //
////////////////////////////////////////////////

/*Objects*/
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
LiquidCrystal lcd(9, 8, 6, 5, 3, 2);

/*Variables*/
NfcTag tag;
boolean hasTag = false;
int timeLeft = -1;
int buzzerPin = A0;


////////////////////////////////////////////////
//                 Base Code                  //
////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	pinMode(buzzerPin, OUTPUT);
	nfc.begin();
	lcd.begin(16, 2);
	
	printOnLCD("Veuillez passer","votre badge");
}

void loop() {

	boolean isTagPresent = nfc.tagPresent(40);
	if(isTagPresent && !hasTag){
		hasTag = true;
		tag = nfc.read();
		timeLeft = millis()/1000;
	}else if(!isTagPresent){
		hasTag = false;
	}

	if(timeLeft > 0){
		printOnLCD(getUID(),getTimeLeft());
	}else if(timeLeft == 0){
		digitalWrite(buzzerPin, HIGH);
		delay(1000);
		digitalWrite(buzzerPin, LOW);
		timeLeft = -1;
		printOnLCD("Veuillez passer","votre badge");
	}
}

////////////////////////////////////////////////
//             		  Fuctions 	              //
////////////////////////////////////////////////
String getUID(){
	return tag.getUidString();
}

void printOnLCD(String textFirstLine, String textSecondLine){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(textFirstLine);
	lcd.setCursor(0,1);
	lcd.print(textSecondLine);
}

String getTimeLeft(){
	int actualTime = millis()/1000;
	int maxTime = timeLeft + TIME_MAX;
	int toReturn = maxTime - actualTime;
	int minutes = toReturn/60;
	int secondes = toReturn - minutes*60;

	if(toReturn <= 0)
		timeLeft = 0;

	String myTime(minutes);
	myTime += "m";
	myTime += secondes;
	myTime += "s";

	return myTime;
}

