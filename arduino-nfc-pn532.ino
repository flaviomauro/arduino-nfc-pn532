/*
*******************************************************************************
Description:
Sample code to show how to read NFC tags with Arduino and PN532.
GitHub: https://github.com/flaviomauro/arduino-nfc-pn532
*******************************************************************************
Item list:
  1 x Adafruit Feather
  1 x Micro USB cable
  1 x Protoboard
  1 x NFC PN532
  4 x Jumper cables M/F

External Library list:
  PN532.h - https://github.com/elechouse/PN532
  PN532_I2C.h - https://github.com/elechouse/PN532

Additional Actions:
  Please setup your PN532 switches to I2C.

*******************************************************************************
@author: Flavio H. C. Mauro
@date: 10-Jan-2017
LinkedIn: https://br.linkedin.com/in/flaviomauro
*******************************************************************************
*/
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void setup(void) {
  Serial.begin(9600);
  Serial.println("NFC TAG READER");
  nfc.begin();
}

void loop(void) {
  Serial.println("\nScan your NFC tag\n");

  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString());

    if (tag.hasNdefMessage())
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("\nThis Message in this Tag is ");
      Serial.print(message.getRecordCount());
      Serial.print(" NFC Tag Record");
      if (message.getRecordCount() != 1) { 
        Serial.print("s");
      }
      Serial.println(".");

  
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nNDEF Record ");Serial.println(i+1);
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);


        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Information (as String): ");
        Serial.println(payloadAsString);


        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");Serial.println(uid);
        }
      }
    }
  }
  delay(500);
}
