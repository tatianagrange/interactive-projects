#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

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
LiquidCrystal_I2C lcd(0x27,16,2);

/*Variables*/
const int sdChip = 4;
boolean isCardPresent = true;
NfcTag tag;
int timeLeft = -1;
int buzzerPin = A0;



////////////////////////////////////////////////
//                 Base Code                  //
////////////////////////////////////////////////
void setup() {
	/************** Init Pin **************/
	Serial.begin(9600);
	pinMode(buzzerPin, OUTPUT);
	nfc.begin();
	lcd.init();
	lcd.backlight();
	
	/************** Check SD Card **************/
	printOnLCD("Initialisation","Carte SD");
	delay(1000);
	pinMode(10, OUTPUT);
	if (!SD.begin(sdChip)) {
		printOnLCD("PB Carte SD","06.06.06.06.06");
		isCardPresent = false;
		return;
	}

	/************** Check Files on SD Card **************/
	printOnLCD("Vérification","des fichiers");
	delay(1000);
	generateFiles();


	/************** Check Admin on SD Card **************/
	File dataFile = SD.open(ACCESS_FILE,FILE_READ);
	if (!dataFile.available()) {
		printOnLCD("Pas d'access","Ajouter cartes");
		dataFile.close();
		return;
	}
	dataFile.close();

	/************** All is ok **************/
	askForCard();
}

void loop() {
	// If sd card is not present, you can't start 
	if(!isCardPresent)
		return;

	boolean isTagPresent = nfc.tagPresent(40);
	if(isTagPresent && !hasTag){
		hasTag = true;
		tag = nfc.read();

		//Is the tag authorised to action relais ?
		boolean isAuth = isAuthorised(getUID());

		//Log it
		logAccess(isAuth);


		if(isAuth){
			timeLeft = millis()/1000;
			//Relais On
		}
	}else if(!isTagPresent){
		hasTag = false;
	}

	showOnLCD();
}

////////////////////////////////////////////////
//             		  Fuctions 	              //
////////////////////////////////////////////////
String getUID(){
	return tag.getUidString();
}

void showOnLCD(){
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

void printOnLCD(String textFirstLine, String textSecondLine){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(textFirstLine);
	lcd.setCursor(0,1);
	lcd.print(textSecondLine);
	Serial.println(textFirstLine);
	Serial.println(textSecondLine);
}

void logAccess(boolean authorizedAccess){
	File dataFile = SD.open(LOG_FILE, FILE_WRITE);
	if(dataFile){
		String text = getUID();
		text += ", 14h42, ";
		text += authorizedAccess ? "accès autorisé" : "accès refusé";
		dataFile.println(text);
		dataFile.close();

		printOnLCD("Bonjour",getUID());
		delay(1000);
	}
	dataFile.close();
}

boolean isAuthorised(String uid){
	File dataFile = SD.open(ACCESS_FILE,FILE_READ);
	String content("");
	while (dataFile.available()) {
		char c = dataFile.read();
		if(c == 10){
			if(uid.equals(content)){
				digitalWrite(buzzerPin, HIGH);
				delay(200);
				digitalWrite(buzzerPin, LOW);
				delay(200);
				return true;
			}

			content = "";
		}
		else{
			content += c;
		}
	}
	dataFile.close();
	return false;
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

void generateFiles(){
	File dataFile;
	if (!SD.exists(LOG_FILE)) {
		printOnLCD("Création du","fichier de log");
		delay(2000);
		dataFile = SD.open(LOG_FILE, FILE_WRITE);
		dataFile.println("uid,hour,acces");
		dataFile.close();
	}

	if (!SD.exists(ACCESS_FILE)) {
		printOnLCD("Création du","fichier acces");
		delay(2000);
		dataFile = SD.open(ACCESS_FILE, FILE_WRITE);
		dataFile.close();
	}
}

void askForCard(){
	printOnLCD("Veuillez passer","votre badge");
}
