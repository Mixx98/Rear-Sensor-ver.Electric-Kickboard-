#include <Arduino.h>

#define VERY_NEAR 3
#define NEAR 2
#define FAR 1
#define VERY_FAR 0
#define SENSOR_COUNT 6

int lightPins[SENSOR_COUNT];
int sonarEchoPin[SENSOR_COUNT];
int sonarTrigPin[SENSOR_COUNT];

#define DIGITAL_PIN_MIN 2
#define DIGITAL_PIN_MAX 13

#define ANALOG_PIN_MIN A0
#define ANALOG_PIN_MAX A7

void getSonarDistance();
void getLightSensorValue();

int res[SENSOR_COUNT];

void getSonarDistance(){
  for(int i = 0; i<SENSOR_COUNT; ++i){
    digitalWrite(sonarTrigPin[i], LOW);
    digitalWrite(sonarEchoPin[i], LOW);
    delayMicroseconds(2);
    digitalWrite(sonarTrigPin[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(sonarTrigPin[i], LOW);

    unsigned long duration = pulseIn(sonarEchoPin[i], HIGH);
    int result = (int)(((double) (340 * duration) / 10000) / 2);
    #ifdef DEBUG
    res[i] = result;
    #endif

    #ifdef DEBUG
    if(result > 0 and result <=30) {
      res[i] = VERY_NEAR;
    } else if(result > 30 and result <=80) {
      res[i] = NEAR;
    } else if(result > 80 and result <=300) {
      res[i] = FAR;
    } else{
      res[i] = VERY_FAR;
    }
    #endif
    delay(10);
  }
  return;
}

void getLightSensorValue(){
  for(int i = 0; i < SENSOR_COUNT; ++i){
    if(!digitalRead(lightPins[i]))
      res[i] = VERY_NEAR;
  }
}


void setup() {
  // put your setup code here, to run once:
  int echoPin = DIGITAL_PIN_MAX;
  int sonarPinDiff = ((DIGITAL_PIN_MAX - DIGITAL_PIN_MIN + 1) / 2);

  for(int i = 0; i < SENSOR_COUNT && echoPin >= 8; ++i){
    sonarEchoPin[i] = echoPin;
    sonarTrigPin[i] = echoPin - sonarPinDiff;

    pinMode(sonarTrigPin[i], OUTPUT);
    pinMode(sonarEchoPin[i], INPUT);
    --echoPin;
  }

  int lightPin = ANALOG_PIN_MIN;
  for(int i = 0; i < SENSOR_COUNT && lightPin <= ANALOG_PIN_MAX; ++i){
    lightPins[i] = lightPin;
    PinMode(lightPins[i], INPUT);
    ++lightPin;
  }
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  getSonarDistance();
  getLightSensorValue();

  String str = "";

  for(int i = 0; i < SENSOR_COUNT; ++i){
    str += String(res[i]);
    str += ' ';
  }

  #ifdef DEBUG_APP
  int i = SENSOR_COUNT;
  while (i < 6){
    str += '1';
    str += ' ';
    ++i;
  }
  #endif

  str[str.length() - 1] = '\n';

  if(Serial.available()) Serial.print(str);
  delay(100 - 10 * SENSOR_COUNT);
}
