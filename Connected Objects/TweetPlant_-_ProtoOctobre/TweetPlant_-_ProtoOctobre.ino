/*
  TweetPlant is a project for the workshop at IIM school.
  This is the first prototype of the project.
*/

#include <Bridge.h>
#include <Temboo.h>
#include <Console.h>
#include "TembooAccount.h"
#include "TwitterAccount.h"

int sensorPin = A1;
int sensorValue = 0;

long lastTweetTime = 0;
long howManyTimeByDay = 3600;
long oneDay = 86400;

int tweetsNumber = 0;

void setup() {
	pinMode(13, OUTPUT);
	Bridge.begin();
	Console.begin();

	while (!Console);
	Console.println("You're connected to the Console!!!!");

	digitalWrite(13, HIGH);
	delay(1000);
	digitalWrite(13, LOW);
}

void loop()
{
	sensorValue = analogRead(sensorPin);
	delay(1000);
	if(sensorValue < 600 && timeIsChek())  
	{
                tweetsNumber++;
                
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

                lastTweetTime = getTimeStamp();
                String tweetText(makeTweet());

		StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);
		unsigned int returnCode = StatusesUpdateChoreo.run();
                
		if (returnCode == 0) {
			Console.println("Success! Tweet sent => " + tweetText);
			digitalWrite(13, HIGH);
                        
		} else {
			while (StatusesUpdateChoreo.available()) {
				char c = StatusesUpdateChoreo.read();
  				Console.print(c);
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
		Console.println("Meme pas soif"); 
                tweetsNumber = 0;
	}

        delay(1000);
        digitalWrite(13, LOW);
}

long getTimeStamp() {
        String result;
        Process time;
        time.begin("date");
        time.addParameter("+%s"); 
        time.run(); 
        
        while(time.available()>0) {
                char c = time.read();
                if(c != '\n')
                        result += c;
        }
        
        return result.toInt();
}

String getDate() {
        String result;
        Process time;
        time.begin("date");
        time.addParameter("+%T"); 
        time.run(); 
        
        while(time.available()>0) {
                char c = time.read();
                if(c != '\n')
                        result += c;
        }
        
        return result;
}

boolean timeIsChek(){
        int dif = getTimeStamp() - lastTweetTime;
        int maxTime = (int)(oneDay/howManyTimeByDay);
        return dif > maxTime;
}

String makeTweet(){
        String toReturn = "Il est "  + getDate();
        switch(tweetsNumber){
                case 1:
                       toReturn += " et j'ai soif!";
                       break; 
                case 2:
                       toReturn += " et j'ai encore soif!";
                       break;
                case 3:
                       toReturn += " et j'ai toujours soif!";
                       break;
                case 4:
                       toReturn += " et j'ai TOUJOURS soif!";
                       break; 
                default:
                       toReturn += " et j'ai TOUJOURS soif... Help me!!!";
                       break; 
        }
        return toReturn;
}
