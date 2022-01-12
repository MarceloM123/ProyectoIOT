// SENSORES DE LUZ Y TEMPERATURA AGREGADOS
// AGREGAR BOTON Y SERVO DE CLIMA

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "92ZcEX4J8U-uP5Dy56GmtyE5KphmnMYp";

// Your WiFi credentials.
char ssid[] = "INFINITUM1DC6_2.4";
char pass[] = "Mp83485845";

#include <Servo.h>

Servo servo1; // LUZ 1
Servo servo2; // LUZ 2
Servo servo3; // CLIMA

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;

#define button1 D1 
#define button2 D2
#define button3 D6

#define LED D4

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Button State 1
int buttonState1 = 0;
int directionState1 = 0;

// Button State 2
int buttonState2 = 0; 
int directionState2 = 0;

// Button State 3
int buttonState3 = 0;

int pinValue1; // Virtual Button 1 // SERVO 1
int pinValue2; // Virtual Button 2 // SERVO 2

int pinValue3; // Virtual Button 3 // CONTROL FOTORESISTOR

BLYNK_WRITE(V1) // V1 is the number of Virtual Pin SERVO 1
{
  pinValue1 = param.asInt();
}

BLYNK_WRITE(V2) // V2 is the number of Virtual Pin SERVO 2
{
  pinValue2 = param.asInt();
}

BLYNK_WRITE(V6) // Boton de Servo 3
{
  pinValue3 = param.asInt();
}

WidgetLED ledBlynk1(V3); // LED 1
WidgetLED ledBlynk2(V4); // LED 2

float sliderLuz; // Valor de Slider de luz
BLYNK_WRITE(V7) // Slider para luces
{
  sliderLuz = param.asFloat();
}


int lightON = 180;
int lightOFF = 90;

BlynkTimer timer;

int photoresistor;
void lightSensor() {
  photoresistor = analogRead(A0);
  Blynk.virtualWrite(V5, photoresistor);
}

float t;
void tempSensor() {
  t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V9, t);
}

bool queServo;

void setup() { // *********SETUP*********
  // Debug Console
  Serial.begin(9600);

  // TEMPERATURE SENSOR
  dht.begin();

  // LED FOR LIGHTS
  pinMode(LED, OUTPUT);

  // SETUP FIRST BUTTON
  pinMode(button1, INPUT);
  servo1.attach(D7);
 
  // SETUP SECOND BUTTON
  pinMode(button2, INPUT);
  servo2.attach(D8);

  // SETUP THIRD BUTTON
  pinMode(button3, INPUT);
  servo3.attach(D3);

  // TIMER BLYNK
  timer.setInterval(500L, lightSensor);
  timer.setInterval(500L, tempSensor);

  Blynk.begin(auth, ssid, pass);
}

void loop() { // *********LOOP*********

  Blynk.run();
  timer.run();

  // SERVO 1 LUZ 1
  buttonState1 = digitalRead(button1);
  if (directionState1 == 0) {
    if (buttonState1 == HIGH || pinValue1 == true) {
      directionState1 = 1;
      digitalWrite(LED, HIGH);
      servo1.attach(D7);

      for (pos1 = lightOFF; pos1 <= lightON; pos1 += 1) {
        servo1.write(pos1);
        delay(15);
      }

      servo1.detach();
      digitalWrite(LED, LOW);
    }

  } else if (directionState1 == 1) {
    if (buttonState1 == HIGH || pinValue1 == true) {
      directionState1 = 0;
      digitalWrite(LED, HIGH);
      servo1.attach(D7);

      for (pos1 = lightON; pos1 >= lightOFF; pos1 -= 1) {
        servo1.write(pos1);
        delay(15);
      }

      servo1.detach();
      digitalWrite(LED, LOW);
    }
  }

  if (servo1.read() == lightON) { // ESTADO LED 1 EN BLYNK
    ledBlynk1.on();
  } else {
    ledBlynk1.off();
  }



  // SERVO 2 LUZ 2
  buttonState2 = digitalRead(button2);
  if (directionState2 == 0) {
    if (buttonState2 == HIGH || pinValue2 == true) {
      directionState2 = 1;
      digitalWrite(LED, HIGH);
      servo2.attach(D8);

      for (pos2 = lightOFF; pos2 <= lightON; pos2 += 1) {
        servo2.write(pos2);
        delay(15);
      }

      servo2.detach();
      digitalWrite(LED, LOW);
    }

  } else if (directionState2 == 1) {
    if (buttonState2 == HIGH || pinValue2 == true) {
      directionState2 = 0;
      digitalWrite(LED, HIGH);
      servo2.attach(D8);

      for (pos2 = lightON; pos2 >= lightOFF; pos2 -= 1) {
        servo2.write(pos2);
        delay(15);
      }
      
      servo2.detach();
      digitalWrite(LED, LOW);
    }
  }

  if (servo2.read() == lightON) { // ESTADO LED 2 EN BLYNK
    ledBlynk2.on();
  } else {
    ledBlynk2.off();
  }


  // SERVO 3 CLIMA
  int buttonState3 = digitalRead(button3);
  if (buttonState3 == HIGH) {
    Serial.println(buttonState3);

    servo3.attach(D3);

    for (pos3 = lightOFF; pos3 <= lightON; pos3 += 1) {
      servo3.write(pos3);
      delay(15);
    }

    for (pos3 = lightON; pos3 >= lightOFF; pos3 -= 1) {
      servo3.write(pos3);
      delay(15);
    }

    servo3.detach();
  
   
  }
  
  if (pinValue3 == true) {
    //Serial.print("Valor Slider: ");
    //Serial.println(sliderLuz);

    //Serial.print("Valor Fotoresistor: ");
    //Serial.println(photoresistor);

    if (sliderLuz < photoresistor) {

      if (servo1.read() == lightOFF) {
        Serial.println("Luz 1 apagado");
        
        directionState1 = 1;
        digitalWrite(LED, HIGH);
        servo1.attach(D7);

        for (pos1 = lightOFF; pos1 <= lightON; pos1 += 1) {
          servo1.write(pos1);
          delay(15);
        }

        servo1.detach();
        digitalWrite(LED, LOW);

        Serial.println("Luz 1 prendido");

      }

      if (servo2.read() == lightOFF) {
        Serial.println("Luz 2 apagado");

        directionState2 = 1;
        digitalWrite(LED, HIGH);
        servo2.attach(D8);

        for (pos2 = lightOFF; pos2 <= lightON; pos2 += 1) {
          servo2.write(pos2);
          delay(15);
        }

        servo2.detach();
        digitalWrite(LED, LOW);

        Serial.println("Luz 2 prendido");

      }

    }

  }

  if (buttonState1 == HIGH || pinValue1 == true) {
      Blynk.virtualWrite(V6, LOW);
  } else if (buttonState2 == HIGH || pinValue2 == true) {
      Blynk.virtualWrite(V6, LOW);
  }
  
} // EN OF VOID LOOP

/*void movServo(int pos, int light, Servo servo, bool oneOrTwo) { **** PARA DESPUES **** 

  if (oneOrTwo == true) {

    for (pos = light; pos <= light; pos += 1) {
      servo.write(pos);
      delay(15);
    }

  } else if (oneOrTwo == false) {

    for (pos = light; pos >= light; pos -= 1) {
      servo.write(pos);
      delay(15);
    }

  }

}*/ 





