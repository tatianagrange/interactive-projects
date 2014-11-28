/*
 *  Humidity is a project's part for the workshop at IIM school.
 */
#include <Bridge.h>
#include <Console.h>

/* ************************************************************************ */
/*                                Variables                                 */
/* ************************************************************************ */

//Define all the pin we want to use
int sensorPin = A1;             // The value of the pin where is the sensor
int sensorValue = 0;            // The sensor's value at the begining
int ledPin = 13;                // The led's pin

//And this value is to define by you!
int minimumHumidity = 450;


/* ******************************************************************* */
/*                             Main Code                               */
/* ******************************************************************* */


// The setup loop is calling only one time, when the arduino start
void setup() {
        Bridge.begin();
        // We want to use the console to check what append
	Console.begin();

        //Wait the connection of the Console
	while (!Console);

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
	if(sensorValue < minimumHumidity)  
	{
                //This plant need water!
                //Do whaterver you want               
	}
	else{
                //This plant is fine
	}

        delay(1000);
}
