#include <Arduino.h>

// Pins definieren

//Motor
#define motorPin1   15  //Motor Pin 1
#define motorPin2   16  //Motor Pin 2
#define motorPin3   17  //Motor Pin 3
#define motorPin4   18  //Motor Pin 4

//Sensoren
#define sensorUnten 2   //Unterer Sensor an der Hühnerklappe
#define sensorOben  3   //Oberer Sensor an der Hühnerklappe
#define lichtSensor 14  // Photoressistor zur Erkennung, ob Tag oder Nacht ist

//Taster
#define switchPos   4   //Taster zum Öffnen und Schließen der Hühnerklappe
#define switchMod   5   //Taster zum Wechseln zwischen Hand- und Automodus

//LED
#define ledPosUnten 9   //LED signalisiert Position unten an der Hühnerklappe
#define ledPosOben  10  //LED signalisiert Position oben an der Hühnerklappe
#define ledModeHand 12  //LED signalisiert Handmodus aktiv
#define ledModeAuto 13  //LED signalisiert Automodus aktiv
#define ledError    11  //LED signalisiert Fehler

// Konstanten
const int lichtschwelle = 500;
const int motordelay = 2;
const unsigned long motortimeout = 10000; // Zeitlimit Motor in Millisekunden

// Globale Variablen
bool handModus = false;
bool autoModus = true;
bool licht = false;
int stepNumber = 0;
unsigned long motorStartTime = 0;
bool motorTimedOut = false;

// Motorzustände
enum MotorState { STOPPED, CLOCKWISE, COUNTERCLOCKWISE };
MotorState motorStatus = STOPPED;

// Funktionen deklarieren
void updateModus();
void updateLeds();
void controlMotor();
void statusMotor(bool dir);
void zustandLichtSensor();
void checkMotorTimeout();

//---------------------------------------------------------------------------------------------------------------------

void setup() {
    // Pins konfigurieren

    //Motor
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    //Sensoren
    pinMode(sensorUnten, INPUT_PULLUP);
    pinMode(sensorOben, INPUT_PULLUP);

    //Taster
    pinMode(switchPos, INPUT_PULLUP);
    pinMode(switchMod, INPUT_PULLUP);

    //LED
    pinMode(ledPosUnten, OUTPUT);
    pinMode(ledPosOben, OUTPUT);
    pinMode(ledModeHand, OUTPUT);
    pinMode(ledModeAuto, OUTPUT);
    pinMode(ledError, OUTPUT);
}

void loop() {
    updateModus();
    updateLeds();
    zustandLichtSensor();
    checkMotorTimeout();

    //Klappe öffnen/Schliessen

    //Handmodus
    if (handModus) {
        if (digitalRead(switchPos) == HIGH          //Bedingungen öffnen (Motor CW)
            && digitalRead(sensorUnten) == HIGH
            && digitalRead(sensorOben) == LOW) {
            motorStatus = CLOCKWISE;
            motorStartTime = millis();
            motorTimedOut = false;
        } else if (digitalRead(switchPos) == HIGH   //Bedingungen schliessen (Motor CCW)
            && digitalRead(sensorOben) == HIGH
            && digitalRead(sensorUnten) == LOW) {
            motorStatus = COUNTERCLOCKWISE;
            motorStartTime = millis();
            motorTimedOut = false;
        }
        //Automodus
    } else if (autoModus) {
        if (licht                                       //Bedingungen öffnen (Motor CW)
            && digitalRead(sensorUnten) == HIGH
            && digitalRead(sensorOben) == LOW) {
            motorStatus = CLOCKWISE;
            motorStartTime = millis();
            motorTimedOut = false;
        } else if (!licht                               //Bedingungen schliessen (Motor CCW)
            && digitalRead(sensorOben) == HIGH
            && digitalRead(sensorUnten) == LOW) {
            motorStatus = COUNTERCLOCKWISE;
            motorStartTime = millis();
            motorTimedOut = false;
        }
    }

    if ((motorStatus == CLOCKWISE                       //Bedingungen Stopp (Motor Stopp)
        && digitalRead(sensorOben) == HIGH)
        || (motorStatus == COUNTERCLOCKWISE
        && digitalRead(sensorUnten) == HIGH)) {
        motorStatus = STOPPED;
    }

    controlMotor();
}

//---------------------------------------------------------------------------------------------------------------------

//Wechsel zwischen Automodus und Handmodus
void updateModus() {
    static bool letztmod = LOW;                         //Wechsel zwischen Automodus und Handmodus
    bool akturllmod = digitalRead(switchMod);

    if (akturllmod == HIGH && letztmod == LOW) {
        handModus = !handModus;
        autoModus = !autoModus;
    }

    letztmod = akturllmod;
}

//---------------------------------------------------------------------------------------------------------------------

//Steuerung LEDs
void updateLeds() {
    digitalWrite(ledModeHand, handModus);
    digitalWrite(ledModeAuto, autoModus);
    digitalWrite(ledPosUnten, digitalRead(sensorUnten));
    digitalWrite(ledPosOben, digitalRead(sensorOben));
    digitalWrite(ledError, motorTimedOut);
}

//---------------------------------------------------------------------------------------------------------------------

//Abfrage Lichsensor und erkennung ob Tag oder Nacht
void zustandLichtSensor() {
    int lichtWert = analogRead(lichtSensor);
    licht = lichtWert > lichtschwelle;
}
//https://www.instructables.com/Light-Sensor-Photoresistor-Arduino-Tinkercad/
//---------------------------------------------------------------------------------------------------------------------

//Prüfen, ob der Motor die Zeit überschritten hat
void checkMotorTimeout() {
    if ((motorStatus == CLOCKWISE || motorStatus == COUNTERCLOCKWISE) && !motorTimedOut) {
        if (millis() - motorStartTime > motortimeout) {
            motorStatus = STOPPED;
            motorTimedOut = true;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

//Steuerung Motor
void controlMotor() {
    switch (motorStatus) {
        case CLOCKWISE:
            statusMotor(true);
            break;
        case COUNTERCLOCKWISE:
            statusMotor(false);
            break;
        case STOPPED:
            digitalWrite(motorPin1, LOW);
            digitalWrite(motorPin2, LOW);
            digitalWrite(motorPin3, LOW);
            digitalWrite(motorPin4, LOW);
            break;
    }
}

void statusMotor(bool dir) {
    if (dir) {                      // Uhrzeigersinn
        switch (stepNumber) {
            case 0:
                digitalWrite(motorPin1, HIGH);
                digitalWrite(motorPin2, LOW);
                digitalWrite(motorPin3, LOW);
                digitalWrite(motorPin4, LOW);
                break;
            case 1:
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, HIGH);
                digitalWrite(motorPin3, LOW);
                digitalWrite(motorPin4, LOW);
                break;
            case 2:
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, LOW);
                digitalWrite(motorPin3, HIGH);
                digitalWrite(motorPin4, LOW);
                break;
            case 3:
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, LOW);
                digitalWrite(motorPin3, LOW);
                digitalWrite(motorPin4, HIGH);
                break;
        }
    } else {                    // Gegenuhrzeigersinn
        switch (stepNumber) {
            case 0:
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, LOW);
                digitalWrite(motorPin3, LOW);
                digitalWrite(motorPin4, HIGH);
                break;
            case 1:
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, LOW);
                digitalWrite(motorPin3, HIGH);
                digitalWrite(motorPin4, LOW);
                break;
            case 2:
                digitalWrite(motorPin1, LOW);
                digitalWrite(motorPin2, HIGH);
                digitalWrite(motorPin3, LOW);
                digitalWrite(motorPin4, LOW);
                break;
            case 3:
                digitalWrite(motorPin1, HIGH);
                digitalWrite(motorPin2, LOW);
                digitalWrite(motorPin3, LOW);
                digitalWrite(motorPin4, LOW);
                break;
        }
    }
//https://elektro.turanis.de/html/prj143/index.html
    stepNumber = (stepNumber + 1) % 4;
    delay(motordelay);
}
