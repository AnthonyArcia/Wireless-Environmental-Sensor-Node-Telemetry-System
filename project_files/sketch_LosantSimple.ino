#include <ESP8266WiFi.h>
#include <Losant.h>

// WiFi credentials.
const char* WIFI_SSID = "Anthony25";
const char* WIFI_PASS = "0522edian";

// Losant credentials. - from accces key download from simulation
const char* LOSANT_DEVICE_ID = "6aee3475ef6843a29859d398";
const char* LOSANT_ACCESS_KEY = "83468bf6-3b3b-47c1-b243-39479b2cb9fc";
const char* LOSANT_ACCESS_SECRET = "3bbaaba0771a3eee06e2a9ab0b2568ab7dd5c1667dd6d8f1f9a3a8f91b48136a";
// Button is inverted - out=on
const int BUTTON_PIN = D3;
const int LED_PIN = D0;

bool ledState = false;
unsigned char dotcnt=0;

// For an unsecure connection to Losant.
 WiFiClient wifiClient;

LosantDevice device(LOSANT_DEVICE_ID);

void toggle() {
  Serial.println("Toggling LED.");
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

// Called whenever the device receives a command from the Losant platform.
void handleCommand(LosantCommand *command) {
  Serial.print("Command received: ");
  Serial.println(command->name);

  if(strcmp(command->name, "toggle") == 0) {
    toggle();
  }
}

void connect() {

  // Connect to Wifi.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(dotcnt++<35)
    Serial.print(".");
    else
    {
      dotcnt=0;
      Serial.println(".");
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to Losant.
  Serial.println();
  Serial.print("Connecting to Losant...");

  //device.connectSecure(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

  // For an unsecure connection.
   device.connect(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

  while(!device.connected()) {
    delay(500);
    if(dotcnt++<35)
    Serial.print(".");
    else
    {
      dotcnt=0;
      Serial.println(".");
    }

  }

  Serial.println("Connected!");
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { }
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Register the command handler to be called when a command is received
  // from the Losant platform.
  device.onCommand(&handleCommand);

  connect();
}

void buttonPressed() {
  Serial.println("Button Pressed!");

  // Losant uses a JSON protocol. Construct the simple state object.
  // { "button" : true }
  StaticJsonDocument<200> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["button"] = true;

  // Send the state to Losant.
  device.sendState(root);
}

int buttonState = 0;

void loop() {

  bool toReconnect = false;

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    toReconnect = true;
  }

  if(!device.connected()) {
    Serial.println("Disconnected from Losant");

    toReconnect = true;
  }

  if(toReconnect) {
    connect();
  }

  device.loop();

  int currentRead = digitalRead(BUTTON_PIN);

  if(currentRead != buttonState) {
    buttonState = currentRead;
    if(buttonState) {
      buttonPressed();
    }
  }

  delay(100);
}
