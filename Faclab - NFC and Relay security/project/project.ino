#include <LiquidCrystal.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <TinkerKit.h>
#include <SD.h>
#include <SPI.h>

////////////////////////////////////////////////
//                  Define                    //
////////////////////////////////////////////////
#define LOG_FILE "log.csv"
#define ACCESS_FILE "acces.csv"
#define ADMIN_FILE "admin.csv"
#define TIME_MAX 70 //1200 //20 min

////////////////////////////////////////////////
//                Declarations                //
////////////////////////////////////////////////

/*Objects*/
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
boolean hasTag = false;
LiquidCrystal lcd(9, 8, 6, 5, 3, 2);
TKRelay relay1(7);

/*Variables*/
const int sdChip = 4;
boolean isCardPresent = true;
NfcTag tag;
int timeLeft = -1;
int buzzerPin = A0;
boolean adminMode = false;



////////////////////////////////////////////////
//                 Base Code                  //
////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	pinMode(buzzerPin, OUTPUT);
	nfc.begin();
	lcd.begin(16, 2);
	
	printOnLCD("Initialisation","Carte SD");
	delay(1000);
	pinMode(10, OUTPUT);
	if (!SD.begin(sdChip)) {
		printOnLCD("PB Carte SD","06.06.06.06.06");
		isCardPresent = false;
		return;
	}
	printOnLCD("Carte SD OK","");
	delay(1000);
	printOnLCD("Veuillez passer","votre badge");

	File dataFile;
	if (!SD.exists(LOG_FILE)) {
		dataFile = SD.open(LOG_FILE, FILE_WRITE);
		dataFile.println("uid,hour,acces");
		dataFile.close();
	}
}

void loop() {
	// If sd card is not present, you can't start 
	if(!isCardPresent)
		return;

	boolean isTagPresent = nfc.tagPresent(40);
	if(isTagPresent && !hasTag){
		hasTag = true;
		tag = nfc.read();
		isAdmin();

		if(adminMode){

		}
		else{
			boolean isAuth = isAuthorised();
			logAccess(isAuth);
			if(isAuth){
				timeLeft = millis()/1000;
				//Relais On
			}
		}
	}else if(!isTagPresent){
		hasTag = false;
	}

	if(timeLeft > 0){
		printOnLCD("Accès autorisé",getTimeLeft());
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

void logAccess(boolean authorizedAccess){
	File dataFile = SD.open(LOG_FILE, FILE_WRITE);
	String text = getUID();
	text += ", 14h42, ";
	text += authorizedAccess ? "accès autorisé" : "accès refusé";
	dataFile.println(text);
	dataFile.close();

	lcd.clear();
	printOnLCD("Bonjour",getUID());
	delay(1000);
}

void isAdmin(){

}

boolean isAuthorised(){
	return true;
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

