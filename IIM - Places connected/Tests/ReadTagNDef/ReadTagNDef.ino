
#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);
#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif

void setup(void) {
    Serial.begin(9600);
    Serial.println("NDEF Reader");
    nfc.begin();
}

void loop(void) {
    if (nfc.tagPresent())
    {
      Serial.println("ok");
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
    delay(5000);
}
