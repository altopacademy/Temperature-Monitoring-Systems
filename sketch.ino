#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

unsigned long myChannelNumber = 1;
const char *myWriteAPIKey = "P3IPVWCSRD332ESU";
WiFiClient  client;

const int DHT_PIN = 15;

DHTesp dhtSensor;
LiquidCrystal_I2C lcd(0x27, 20, 4);

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  ThingSpeak.begin(client);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  lcd.init();   
  lcd.backlight();
}

void loop() {
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  delay(1000);

  DateTime now = rtc.now();

  Serial.print("Current time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  String Jam = String(now.hour(), DEC) + ":" +String(now.minute(), DEC) + ":" + String(now.second(), DEC);


  Serial.println();
  delay(3000);

  lcd.setCursor(1,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()] + Jam);
  lcd.setCursor(2,1);
  lcd.print("Suhu: " + String(data.temperature, 2));

  

  // int x = ThingSpeak.writeField(myChannelNumber, 1, random(1,100), myWriteAPIKey);
  // if(x == 200){
  // Serial.println("Channel update successful.");
  // }
  // else{
  // Serial.println("Problem updating channel. HTTP error code " + String(x));
  // }
  // delay(30000); // TODO: Build something amazing!
}
