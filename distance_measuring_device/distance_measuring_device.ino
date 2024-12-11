
/* author- sadman alvee 
 * Owner/Founder @ impedancePlus
 */
#include<LiquidCrystal.h>

// LCD PINS
/* Connect the LCD to pins as shown */
int RS = 7 ;
int EN = 8;
int D4 = 9;
int D5 = 10;
int D6 = 11;
int D7 = 12;

// ULTRASONIC SENSOR PINS
int trigPin = 3;
int echoPin = 2;
double signalTravelTime;
double numberOfMeasurements = 20;
double distance; // core mathematical calculations
double sumDistance = 0; // summation for all numberOfMeasurements
double distanceSummation;
double averageDistance;
double finalDistance;

// INFRARED SENSOR PINS
int IRpin = A1;
int IRval;
int operatingMode;

//LED-BUZZER PINS
int greenLedPin = 6;
int yellowLedPin = 5;
int redLedPin = A2;
int buzzerPin = 13;

//Creating object type - LiquidCrystal
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

int _configureIRsensor() {
  IRval = digitalRead(IRpin);
  if (IRval == 1 )
    return IRval;
  else if (IRval == 0)
    return IRval;
}

double _distanceCalculation(double signalTravelTime) {
  ///Error in Sensor : in INCHES: 0.25 amd in CM: 0.4; find out yourself for your sensor!
  if (operatingMode == 1 )
    distance =  ((0.01350391 * signalTravelTime) / 2) + 0.25 ; // Outputs in INCHES
  else if (operatingMode == 0 )
    distance =  (((0.01350391 * signalTravelTime) / 2) * 2.54) + 0.4; // Outputs in CENTIMETERS
  else
    Serial.print("INVALID");

  sumDistance = sumDistance + distance  ;
  return sumDistance;
}

double _configureUltrasonicSensor() {
  ///for accurate data and eliminating noise e use for loop upto numberOfMeasurements
  for ( int i = 0; i < numberOfMeasurements; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    signalTravelTime = pulseIn(echoPin, HIGH); // returns in microseconds
    delay(20);
    distanceSummation =  _distanceCalculation(signalTravelTime);
  }
  // 0.40 is the faulty value of the US sensor i've used
  averageDistance = ( distanceSummation / numberOfMeasurements)  ;
  /* debugging purposes
    Serial.print ("AVERAGE DISTANCE : ");
    Serial.print(averageDistance);
    Serial.println("");
  */
  sumDistance = 0; // clears the previous data . orelse it will keep adding
  return averageDistance;
}

void _configureLCD() {
  if (operatingMode == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DISTANCE: INCHES");
    lcd.setCursor(0, 1);
    lcd.print(finalDistance);
    lcd.print(" in.(aprx.)  ");
  }
  else if (operatingMode == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DISTANCE: CM");
    lcd.setCursor(0, 1);
    lcd.print(finalDistance);
    lcd.print(" cm.(aprx.)  ");
  }
}

void _configureSignalSystem() {
  if (operatingMode == 1) {
    if ( finalDistance <= 5) {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(30);
      digitalWrite(redLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(30);

    } else if ( finalDistance > 5 && finalDistance <= 8) {
      digitalWrite(yellowLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(80);
      digitalWrite(yellowLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(80);

    } else if ( finalDistance > 8) {
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(300);
      digitalWrite(greenLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(300);
    }
  }
  else if (operatingMode == 0) {
    if ( finalDistance <= 12.7) {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(30);
      digitalWrite(redLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(30);
      
    } else if ( finalDistance > 12.7 && finalDistance <= 20.32) {
      digitalWrite(yellowLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(80);
      digitalWrite(yellowLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(80);
      
    } else if ( finalDistance > 20.32) {
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(300);
      digitalWrite(greenLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(300);
    }
  }
}

void _printData() {
  if (operatingMode == 1) {
    Serial.print("INCHES:  ");
    Serial.print(finalDistance);
    Serial.println("");
    delay(100);
  } else if (operatingMode == 0) {
    Serial.print("CENTIMETERS:  ");
    Serial.print(finalDistance);
    Serial.println("");
    delay(100);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // set-up serial monitor
  lcd.begin(16, 2);   // set-up LCD display
  //Set-up the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Set-up LED-BUZZER
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(IRpin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  operatingMode = _configureIRsensor();
  finalDistance = _configureUltrasonicSensor();
  _printData();       // to serial Monitor
  _configureLCD();    // to LCD
  _configureSignalSystem();
}
 
 
