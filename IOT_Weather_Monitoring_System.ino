#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6dTSfyOyG"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define LDR D4
#define TH D3
#define Rain A0

double T, P;
char status;

LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(TH, DHT11);
BlynkTimer timer;

char auth[] = "8MJ8xiGWHFZ6zKFi-6V8ZyDgMu3CFHsx";
char ssid[] = "Juswaaaaaa";
char pass[] = "Lopez1208250223";

void setup() {
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(4, 1);
  lcd.print("Loading..");
  delay(4000);
  lcd.clear();
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
}

void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Blynk.virtualWrite(V2, Rvalue);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(Rvalue);
  lcd.print(" ");
  Serial.println(Rvalue);
}

void LDRsensor() {
  int l = digitalRead(LDR);

  if (l == 0) {
    Blynk.virtualWrite(V4, 255);  // Turn on virtual LED (L) in Blynk app
    lcd.setCursor(9, 1);
    lcd.print("L :");
    lcd.print("High");
    lcd.print(" ");
  } else if (l == 1) {
    Blynk.virtualWrite(V4, 0);  // Turn off virtual LED (L) in Blynk app
    lcd.setCursor(9, 1);
    lcd.print("L :");
    lcd.print("Low");
    lcd.print(" ");
  }
}

void loop() {
  Blynk.run();
  DHT11sensor();
  rainSensor();
  LDRsensor();
}
