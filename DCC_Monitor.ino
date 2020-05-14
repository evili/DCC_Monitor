#include "NmraDcc.h"

#define DEBUG_PRINT
#define DCC_IN_PIN 3

const unsigned long IDLE_WAIT = 1000;
volatile unsigned long num_packets = 0;

volatile unsigned long last_print = 0;
volatile unsigned long now = 0;

volatile unsigned long last_msg = 0;
volatile unsigned long msg_now = 0;


NmraDcc  Dcc ;
DCC_MSG  Packet ;

void setup() {
  pinMode(DCC_IN_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("DCC Monitor:");
  Serial.flush();
  Dcc.pin(DCC_IN_PIN, 0);
  Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
}

void loop() {
  Dcc.process();
  now = millis();
  if((now-last_print) >= IDLE_WAIT) {
    Serial.print("Num IDLEs: ");
    Serial.println(num_packets);
    last_print = now;
  }
}


void    notifyDccIdle(void) {
  num_packets +=1;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void notifyDccMsg( DCC_MSG * Msg ) {
  msg_now = millis();
  if((msg_now-last_msg) >= IDLE_WAIT) {
    last_msg = msg_now;
    Serial.print("DCC Message: Size=");
    Serial.print(Msg->Size);
    Serial.print(", Preamble=");
    Serial.print(Msg->PreambleBits);
    Serial.print(", Address=");
    Serial.println(Msg->Data[0]);
    Serial.println(Msg->Crc);
  }
}
