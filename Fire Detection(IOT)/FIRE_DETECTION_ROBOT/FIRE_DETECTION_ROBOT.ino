#define BLYNK_TEMPLATE_ID "TMPLl2PCO9yq"
#define BLYNK_TEMPLATE_NAME "FIRE DETECTION ROBOT"
#define BLYNK_AUTH_TOKEN "wxPp31AFasTmtTS10KnkOpswZkbrqXTp"
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
int buzzer = D2;
#define Buzzer D7
#define flame D0
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "wxPp31AFasTmtTS10KnkOpswZkbrqXTp"; //Enter the Auth code which was send by Blink
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "temp"; //Enter your WIFI Name
char pass[] = "temp12345"; //Enter your WIFI Password
#define DHTPIN 2 // Digital pin 4
int mq2 = A0; // smoke sensor is connected with the analog pin A0
int data = 0;
// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21 // DHT 21, AM2301
int relay3 = 5; // D1 pin
int relay4 = 12; // D6 pin
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, h); //V1 is for Humidity
  Blynk.virtualWrite(V0, t); //V0 is for Temperature
}
void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
  pinMode(Buzzer, OUTPUT);
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);
  dht.begin();
  pinMode(relay3,OUTPUT);
  digitalWrite(relay3, HIGH);
  pinMode(relay4,OUTPUT);
  digitalWrite(relay4, HIGH);
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, getSendData);
  timer.setInterval(1000L, flamesensor);
  }
void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
void flamesensor() {
  bool value = digitalRead(flame );
  if (value == 1) {
    digitalWrite(Buzzer, LOW);
  } else if (value == 0) {
    Blynk.logEvent("fire", "Fire was detected");
    digitalWrite(Buzzer, HIGH);
  }
}
void getSendData()
{
data = analogRead(mq2);
  Blynk.virtualWrite(V2, data);
  if (data > 800 )
  {
     digitalWrite(buzzer, HIGH);
    Blynk.logEvent("smoke", "Smoke Detected!");  
  }
}
