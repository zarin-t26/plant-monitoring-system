#define BLYNK_TEMPLATE_ID "TMPL64XQsnI9K"
#define BLYNK_TEMPLATE_NAME "Smart Plant"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "aR6rqS9I_2zu8bliktastOUJjuekpoTh";
char ssid[] = "CUET--Students";
char pass[] = "1020304050";

DHT dht(D4, DHT11);
BlynkTimer timer;

#define soil A0
#define PIR D5
int PIR_ToggleValue;

#define RELAY_PIN_1       D3
#define VPIN_BUTTON_1    V12 

//Create three variables for pressure
double T, P;
char status;
bool motorState = false;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(PIR, INPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(0, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(11, 1);
  lcd.print("W:OFF");

  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(500L, checkPIRToggle);
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

void soilMoistureSensor() {
  int value = analogRead(soil);
  if (value < 1024) {
    if (!motorState) {
      digitalWrite(RELAY_PIN_1, HIGH); // Turn the motor pump on
      lcd.setCursor(11, 1);
      lcd.print("W:ON ");
      motorState = true;
    }
  } else {
    if (motorState) {
      digitalWrite(RELAY_PIN_1, LOW); // Turn the motor pump off
      lcd.setCursor(11, 1);
      lcd.print("W:OFF");
      motorState = false;
    }
  }
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  Blynk.virtualWrite(V3, value);
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(value);
  lcd.print(" ");
}

void checkPIRToggle() {
  if (PIR_ToggleValue == 1) {
    lcd.setCursor(5, 1);
    lcd.print("M:ON ");
    PIRsensor();
  } else {
    lcd.setCursor(5, 1);
    lcd.print("M:OFF");
    WidgetLED LED(V5);
    LED.off();
  }
}

void PIRsensor() {
  bool value = digitalRead(PIR);
  if (value) {
    Blynk.logEvent("pirmotion", "WARNING! Motion Detected!");
    WidgetLED LED(V5);
    LED.on();
  } else {
    WidgetLED LED(V5);
    LED.off();
  }
}

BLYNK_WRITE(V6) {
  PIR_ToggleValue = param.asInt();
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V6);
}

void loop() {
  Blynk.run();
  timer.run();
}


