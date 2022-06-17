#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h"
#include<math.h>

#define USE_ARDUINO_INTERRUPTS true
#define UpperThreshold 750
#define LowerThreshold 575

#include <PulseSensorPlayground.h>
PulseSensorPlayground pulseSensor;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
WiFiEspClient  client;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number1 = 0;
int number2 = 0;
int number3 = 0;

float temperature = 0;
int myBPM = 0;
String myStatus = "";

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  // initialize serial for ESP module
  setEspBaudRate(ESP_BAUDRATE);

  while (!Serial) {
    ;
  }

  Serial.print("Searching for ESP8266...");
  // initialize ESP module
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");

  ThingSpeak.begin(client);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  if (number1 >= 0)
  {
    number1 = analogRead(A0);
    Serial.print("\n");
    Serial.println(number1);
    if ((number1 < UpperThreshold) && (number1 > LowerThreshold)) {
      Serial.println("ECG Normal");
    }
    else {
      Serial.println("ECG Abnormal");
    }
  }
  if (number2 >= 0)
  {
    number2 = analogRead(A3);
    Serial.print("BPM: ");
    Serial.println(number2);
  }
  if (number3 >= 0)
  {
    number3 = analogRead(A2);
    float mv = ( number3 / 1024.0) * 5000;
    temperature = mv / 10;
    float farh = (temperature * 9) / 5 + 32;
    Serial.print("TEMPRATURE = ");
    Serial.print(temperature);
    Serial.print("*C");
    Serial.println();
    delay(1000);

  }
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, temperature);

  ThingSpeak.setStatus(myStatus);
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000);
}

void setEspBaudRate(unsigned long baudrate) {
  long rates[6] = {115200, 74880, 57600, 38400, 19200, 9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for (int i = 0; i < 6; i++) {
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);
  }
  Serial1.begin(baudrate);
}
