int sensorPin = A1;    // sélection de la pin de mesure
int sensorValue = 0;    // initialisation de la valeur
 
void setup() {
   Serial.begin(9600);  
}
 
void loop() {
 
  sensorValue = analogRead(sensorPin);    // lecture directe de la valeur
  delay(1000);    // patienter 1 s
  if(sensorValue < 600)  
  {
      Serial.print("sensor = " );    // impression du titre
      Serial.println(sensorValue);    // impression de la valeur mesurée
  }
  else{
    Serial.println("Meme pas soif"); 
  }
}
