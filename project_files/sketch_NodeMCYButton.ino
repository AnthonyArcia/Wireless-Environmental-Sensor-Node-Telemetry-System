#define LED D0            // Led in NodeMCU at pin GPIO16 (D0) pin 15
#define BUTTON_PIN D3    // On boad Flash button GPIO0 (D3) pin 12

void blink(int);

void setup() {
pinMode(LED, OUTPUT);    // LED pin as output.
pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(BUTTON_PIN))
  {// off state
        blink(500);
  }
    else
   { // on state
        blink(100);
   }
}

void blink(int duration)
{
      digitalWrite(LED, LOW); // turn the LED on.
      delay(duration); // wait for 0.1 second.
      digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually 
                        //the LED is on; this is because it is acive low on the ESP8266.
      delay(duration);            // wait for 0.1 second.
}
