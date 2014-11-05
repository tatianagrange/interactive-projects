/* Setup shield-specific #include statements */
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

//Plant qui tweet definition
int sensorPin = A1;
int sensorValue = 0;

long lastTweetTime = 0;
long howManyTimeByDay = 3600;
long oneDay = 86400;

int tweetsNumber = 0;



//Ethernet definitions
byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;
EthernetClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 2;   // Maximum number of times the Choreo should be executed




// Code
void setup() {
  Serial.begin(9600);
  
  while(!Serial);

  //Stetup Ethernet
  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(5000);

  Serial.println("Setup complete.\n");
  
  //Setup PlantQUiTweet
  pinMode(13, OUTPUT);
  
  lastTweetTime = millis();
}

void loop()
{
	sensorValue = analogRead(sensorPin);
	delay(1000);
	if(sensorValue < 600 && timeIsChek())  
	{
                tweetsNumber++;
                
		// Temboo Configuration                
                TembooChoreo StatusesUpdateChoreo(client);
                StatusesUpdateChoreo.begin();
                StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
                StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
                StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);
                StatusesUpdateChoreo.setProfile("PlantQuiTweet");
                
                // Make Input for the tweet
                lastTweetTime = millis();
                String tweetText(makeTweet());
                StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);
            
                // Identify the Choreo to run
                StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");
            
                // Run the Choreo; when results are available, print them to serial
                unsigned int returnCode = StatusesUpdateChoreo.run();
            
                if (returnCode == 0) {
			Serial.println("Success! Tweet sent => " + tweetText);
			digitalWrite(13, HIGH);
                        
		} else {
			while(StatusesUpdateChoreo.available()) {
                          char c = StatusesUpdateChoreo.read();
                          Serial.print(c);
                        }

			digitalWrite(13, HIGH);
			delay(500);
			digitalWrite(13, LOW);
			delay(500);
			digitalWrite(13, HIGH);
			delay(500);
			digitalWrite(13, LOW);
			delay(500);
			digitalWrite(13, HIGH);
			delay(500);
			digitalWrite(13, LOW);
			delay(500);
			digitalWrite(13, HIGH);
			delay(500);
			digitalWrite(13, LOW);
			delay(500);
		}

                StatusesUpdateChoreo.close();

	}
	else if(sensorValue >= 600){
		Serial.println("Meme pas soif"); 
                tweetsNumber = 0;
	}

        delay(1000);
        digitalWrite(13, LOW);
}

boolean timeIsChek(){
        long maxTime = (int)(oneDay/howManyTimeByDay)*1000;
        Serial.println(maxTime);
        Serial.println(lastTweetTime+maxTime);
        Serial.println(millis());
        
        return (lastTweetTime+maxTime < millis());
}

String makeTweet(){
        String toReturn =  "Tweet at " + String(millis(), DEC) + " - ";
        switch(tweetsNumber){
                case 1:
                       toReturn += " J'ai soif!";
                       break; 
                case 2:
                       toReturn += " J'ai encore soif!";
                       break;
                case 3:
                       toReturn += " J'ai toujours soif!";
                       break;
                case 4:
                       toReturn += " J'ai TOUJOURS soif!";
                       break; 
                default:
                       toReturn += " J'ai TOUJOURS soif... Help me!!!";
                       break; 
        }
        
        Serial.println(toReturn);
        return toReturn;
}
