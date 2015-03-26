#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ400);
Adafruit_LSM303 lsm;

// mess with this number to adjust TWINklitude :)
// lower number = more sensitive
#define MOVE_THRESHOLD 75

void setup() 
{
	Serial.begin(9600);
	
	if (!lsm.begin())
	{
		Serial.println("Impossible de démarrer l'acceleromètre");
		while (1);
	}
	strip.begin();
	strip.show();
}

void loop() 
{
	//Première valeur
	lsm.read();
	double storedVector = lsm.accelData.x*lsm.accelData.x;
	storedVector += lsm.accelData.y*lsm.accelData.y;
	storedVector += lsm.accelData.z*lsm.accelData.z;
	storedVector = sqrt(storedVector);
	delay(100);
	
	//Seconde valeur
	lsm.read();
	double newVector = lsm.accelData.x*lsm.accelData.x;
	newVector += lsm.accelData.y*lsm.accelData.y;
	newVector += lsm.accelData.z*lsm.accelData.z;
	newVector = sqrt(newVector);
	

	if (abs(newVector - storedVector) > MOVE_THRESHOLD) {
		//Etape 1: J'allume la première
		strip.setPixelColor(0, strip.Color(255, 255, 255));
		strip.show();
		delay(100);

		//Etape 2: J'allume la seconde et j'éteind la première
		strip.setPixelColor(0, strip.Color(0, 0, 0));
		strip.setPixelColor(1, strip.Color(255, 0, 0));
		strip.show();
		delay(100);

		//Etape 3: J'éteind la seconde
		strip.setPixelColor(1, strip.Color(0, 0, 0));
		strip.show();
	}
}
