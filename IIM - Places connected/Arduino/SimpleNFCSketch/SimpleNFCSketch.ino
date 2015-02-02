#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>


////////////////////////////////////////////////
//                Declarations                //
////////////////////////////////////////////////
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
boolean hasTag = false;

////////////////////////////////////////////////
//                 Base Code                  //
////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	nfc.begin();
}

void loop() {

	if(nfc.tagPresent(40)){
			write("Machin truc");
                        //read();
	}else{
		hasTag = false;
	}
}


////////////////////////////////////////////////
//               Main Fuctions                //
////////////////////////////////////////////////
void write(String text)
{
	if (!hasTag) {
		hasTag = true;

		format();

		if(nfc.tagPresent(40))
			erase();

		if(nfc.tagPresent(40))
			save(text);
	}
}

void read(){
	if (!hasTag) {
		hasTag = true;

		NfcTag tag = nfc.read();
		NdefMessage mess = tag.getNdefMessage();
		NdefRecord record = mess.getRecord(0);
		byte payloadCheck[record.getPayloadLength()];
		record.getPayload(payloadCheck);

		String toSend = "";
		for(int i = 3 ; i < record.getPayloadLength() ; i++){  // Start at 3. The 3 frists char are for the encoding.
			toSend += (char)payloadCheck[i];
		}
		
		Serial.println(toSend);
	}
}


////////////////////////////////////////////////
//              Other Fuctions                //
////////////////////////////////////////////////
void save(String text){
	NdefMessage message = NdefMessage();
	message.addTextRecord(text);
	bool success3 = nfc.write(message);
	if (success3) {
	  Serial.println("Success.");        
	} else {
	  Serial.println("Write failed.");
	}
}

void format(){
	bool success = nfc.format();
    if (success) {
        Serial.println("Success, tag is formatted.");        
    } else {
        Serial.println("Unable to format.");
    }
}

void erase(){
	bool success = nfc.erase();
    if (success) {
        Serial.println("Success, tag contains an empty record.");        
    } else {
        Serial.println("Unable to erase tag.");
    }
}

