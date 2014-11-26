/*
 *  TweetPlant is a project for the workshop at IIM school.
 */

#include <Bridge.h>
#include <Temboo.h>
#include <Console.h>
#include "TembooAccount.h"
#include "TwitterAccount.h"

/* ************************************************************************ */
/*                                Variables                                 */
/* ************************************************************************ */

//Define all the pin we want to use
int sensorPin = A1;             // The value of the pin where is the sensor
int sensorValue = 0;            // The sensor's value at the begining
int ledPin = 13;                // The led's pin

//Define all variables for check the number of tweet
long lastTweetTime = 0;        // The time since the last tweet
long howManyTimeByDay = 24;    // This plant will tweet one time by hour. 24 times by day.
long oneDay = 86400;           // How many of seconds in one day?
int tweetsNumber = 0;          // Remember the number of tweet


/* ******************************************************************* */
/*                             Main Code                               */
/* ******************************************************************* */


// The setup loop is calling only one time, when the arduino start
void setup() {
  
        // Define the led like an output
	pinMode(ledPin, OUTPUT);

        // We want to use Bridge, to access to internet.
        // And we want to use the console to check what append
	Bridge.begin();
	Console.begin();

        //Wait the connection of the Console
	//while (!Console);

        //And Print it
	Console.println("You're connected to the Console!!!!");

        //All is good. Just turn on and off the led
	digitalWrite(ledPin, HIGH);
	delay(1000);
	digitalWrite(ledPin, LOW);
}



// The loop function is calling in loop. 
// When the last line is executed, the first line is executed next
void loop()
{
	sensorValue = analogRead(sensorPin);
	delay(1000);

        // 600 is the value when the captor is in the water. 
        // Mabey you have to test to choose what you want to do...
	if(sensorValue < 600 && timeIsChek())  
	{
                // Even if we can't tweet, the plant have to say something.
                // So we have to count that time
                tweetsNumber++;
                
                /* ************************ */
                /*          Temboo          */
                /* ************************ */
		TembooChoreo StatusesUpdateChoreo;
		StatusesUpdateChoreo.begin();
		StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
		StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
		StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);
		StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");
		StatusesUpdateChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
		StatusesUpdateChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);
		StatusesUpdateChoreo.addInput("ConsumerKey", TWITTER_API_KEY);    
		StatusesUpdateChoreo.addInput("ConsumerSecret", TWITTER_API_SECRET);
                /* **************************** */
                /*          End Temboo          */
                /* **************************** */

                // The last time that the plant tweet is now.
                lastTweetTime = getTimeStamp();
                
                // Define a string name tweetText with the function makeTweet()
                String tweetText(makeTweet());

                // And send the tweet
		StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);
		unsigned int returnCode = StatusesUpdateChoreo.run();
                
                
                // If the return code is 0, all is OK! Say It!
		if (returnCode == 0) {
			Console.println("Success! Tweet sent => " + tweetText);
			digitalWrite(13, HIGH);
                        
		} else {
                        // Print the error
			while (StatusesUpdateChoreo.available()) {
				char c = StatusesUpdateChoreo.read();
  				Console.print(c);
			}

                        //And make the led craaaaazy
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

                // All is finnish with temboo. Close the connection
		StatusesUpdateChoreo.close();

	}
	else if(sensorValue >= 600){
                // If I Can tweet, because the last tweet was a lang time ago
                // But the plant don't need water, juste reset the number of tweet
		Console.println("Meme pas soif"); 
                tweetsNumber = 0;
	}

        delay(1000);
        digitalWrite(13, LOW);
}

/* ******************************************************************* */
/*                             Functions                               */
/* ******************************************************************* */

/**
 *  return the time in int
 */
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


/**
 *  Return the date, and actualy the hour, in a string
 */
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

/**
 *  This function return true if the time between now and the last time we tweet
 *  is over the time necessary to tweet X time in day
 */
boolean timeIsChek(){
        long dif = getTimeStamp() - lastTweetTime;
        long maxTime = (int)(oneDay/howManyTimeByDay);
        return dif > maxTime;
}

/**
 *  Make a tweet with the time and with some "joke". Ok... It's not funny...
 *  But it is your job to make the tweet funny.
 */
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
