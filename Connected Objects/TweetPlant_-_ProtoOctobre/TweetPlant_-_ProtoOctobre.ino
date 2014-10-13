/*
  TweetPlant is a project for the workshop at IIM school.
  This is the first prototype of the project.
*/

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h"
#include "TwitterAccount.h"

int sensorPin = A1;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);

  // for debugging, wait until a serial console is connected
  delay(4000);
  while(!Serial);

  Bridge.begin();
}

void loop()
{

    

    sensorValue = analogRead(sensorPin);
	delay(1000);
	if(sensorValue < 600)  
	{
		// Temboo Configuration
		TembooChoreo StatusesUpdateChoreo;
		StatusesUpdateChoreo.begin();
		StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
		StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
		StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);
		StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");

		// Define access to the twitter account, by Temboo
		StatusesUpdateChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
		StatusesUpdateChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);
		StatusesUpdateChoreo.addInput("ConsumerKey", TWITTER_API_KEY);    
		StatusesUpdateChoreo.addInput("ConsumerSecret", TWITTER_API_SECRET);

		String tweetText("J'ai soif!!");
		StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);
    	unsigned int returnCode = StatusesUpdateChoreo.run();

	    if (returnCode == 0) {
	        Serial.println("Success! Tweet sent!");
	    } else {
	      while (StatusesUpdateChoreo.available()) {
	        char c = StatusesUpdateChoreo.read();
	        Serial.print(c);
	      }
	    } 
	    StatusesUpdateChoreo.close();

	}
	else{
		Serial.println("Meme pas soif"); 
	}

	delay(250000); //wait 5 min
}
