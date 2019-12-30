/*
 Arduino code for a remote control clone of an RC-switch using TI CC1101
 https://www.grspy.com/cloning-the-remote-control-of-an-rc-switch-using-ti-cc1101
 Distributed under GPL v3.0
 grspy, 2019
*/

#include "cc1101.h"

#define LEDOUTPUT 7

CC1101 cc1101;

const int buttonPin1 = A5;
const int buttonPin2 = A4;

int button1State = 0;
int button2State = 0;

bool sending = false;

void blinker() {
  digitalWrite(LEDOUTPUT, HIGH);
  delay(100);
  digitalWrite(LEDOUTPUT, LOW);
  delay(100);
}

void setup()
{
  Serial.begin(9600);

  pinMode(LEDOUTPUT, OUTPUT);
  digitalWrite(LEDOUTPUT, LOW);

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // blink once
  blinker();

  Serial.println("Initializing CC1101.");
  cc1101.init();

  Serial.println("Setting up the PA_TABLE.");
  byte PA_TABLE[] = {0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00}; // 10dBm
  cc1101.writeBurstReg(CC1101_PATABLE, PA_TABLE, 8);

  Serial.println("Setting up the registers.");
  cc1101_SetupRegisters();

  delay(1000);
  Serial.println("CC1101 initialized.");
}

void sendoncode() {
  byte ondata[13] = { 0x88, 0x88, 0x8E, 0x8E, 0x8E, 0x8E, 0x8E, 0x8E, 0x88, 0xEE, 0x88, 0xEE, 0x80 };
  sending = true;
  cc1101.sendData(ondata, 13);
  sending = false;
}

void sendoffcode() {
  byte offdata[13] = { 0x88, 0x88, 0x8E, 0x8E, 0x8E, 0x8E, 0x8E, 0x8E, 0x88, 0xEE, 0xEE, 0x88, 0x80 };
  sending = true;
  cc1101.sendData(offdata, 13);
  sending = false;
}

void loop()
{
  if (!sending) {
    button1State = digitalRead(buttonPin1);
    button2State = digitalRead(buttonPin2);

    if (button1State == HIGH) {
      Serial.println("Sending the on code");
      digitalWrite(LEDOUTPUT, HIGH);
      sendoncode();
      digitalWrite(LEDOUTPUT, LOW);
    }

    if (button2State == HIGH) {
      Serial.println("Sending the off code");
      digitalWrite(LEDOUTPUT, HIGH);
      sendoffcode();
      digitalWrite(LEDOUTPUT, LOW);
    }
  }
}

void cc1101_SetupRegisters() {
  //
  // Rf settings for CC1101
  //
  cc1101.writeReg(CC1101_IOCFG0, 0x06);   // GDO0 Output Pin Configuration
  cc1101.writeReg(CC1101_FIFOTHR, 0x47);  // RX FIFO and TX FIFO Thresholds
  cc1101.writeReg(CC1101_PKTLEN, 0x0D);   // Packet Length
  cc1101.writeReg(CC1101_PKTCTRL0, 0x00); // Packet Automation Control
  cc1101.writeReg(CC1101_FSCTRL1, 0x06);  // Frequency Synthesizer Control
  cc1101.writeReg(CC1101_FREQ2, 0x10);    // Frequency Control Word, High Byte
  cc1101.writeReg(CC1101_FREQ1, 0xB0);    // Frequency Control Word, Middle Byte
  cc1101.writeReg(CC1101_FREQ0, 0xB3);    // Frequency Control Word, Low Byte
  cc1101.writeReg(CC1101_MDMCFG4, 0xF7);  // Modem Configuration
  cc1101.writeReg(CC1101_MDMCFG3, 0xB0);  // Modem Configuration
  cc1101.writeReg(CC1101_MDMCFG2, 0x30);  // Modem Configuration
  cc1101.writeReg(CC1101_MDMCFG1, 0x00);  // Modem Configuration
  cc1101.writeReg(CC1101_MDMCFG0, 0x2E);  // Modem Configuration
  cc1101.writeReg(CC1101_DEVIATN, 0x15);  // Modem Deviation Setting
  cc1101.writeReg(CC1101_MCSM0, 0x14);    // Main Radio Control State Machine Configuration
  cc1101.writeReg(CC1101_FOCCFG, 0x16);   // Frequency Offset Compensation Configuration
  cc1101.writeReg(CC1101_WORCTRL, 0xFB);  // Wake On Radio Control
  cc1101.writeReg(CC1101_FREND0, 0x11);   // Front End TX Configuration
  cc1101.writeReg(CC1101_FSCAL3, 0xE9);   // Frequency Synthesizer Calibration
  cc1101.writeReg(CC1101_FSCAL2, 0x2A);   // Frequency Synthesizer Calibration
  cc1101.writeReg(CC1101_FSCAL1, 0x00);   // Frequency Synthesizer Calibration
  cc1101.writeReg(CC1101_FSCAL0, 0x1F);   // Frequency Synthesizer Calibration
  cc1101.writeReg(CC1101_TEST2, 0x81);    // Various Test Settings
  cc1101.writeReg(CC1101_TEST1, 0x35);    // Various Test Settings
  cc1101.writeReg(CC1101_TEST0, 0x09);    // Various Test Settings
}
