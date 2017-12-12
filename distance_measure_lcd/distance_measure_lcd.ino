/*
 
 This sketch prints "Hello World!" and distance measure to the LCD HD44780 
 using the ultrasound sensor HC-SR04.
 
 The circuit for LCD:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)
 * Display backend: 
  * A to 200 Ohms to +5V
  * K to ground
* VSS To ground
* VDD to +5V

The circuit for ultrasound

* Trig -> pin 10
* Echo -> pin 9

The circuit for LEDs
 * Green LED + 470R  -> pin 8
 * Orange LED + 470R  -> pin 7
 * Red LED + 470R  -> pin 6

* Author: Ghislain BIDAUT
* Version 0.01

 */

// include the library code:
#include <LiquidCrystal.h>
#define trigPin 10        //Trig Ultrasons (out)
#define echoPin 9         //Echo Ultrasons (in)
#define greenLED 8        //Green Led
#define orangeLED 7       //Orange Led
#define redLED 6          //Red Led

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  
  Serial.begin (9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Booting Up");

  // init distance sensor
  pinMode(trigPin, OUTPUT);  //Trig est une sortie
  pinMode(echoPin, INPUT);   //Echo est le retour, en entrée

  // init LEDs
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  
  long duration, distance;
  lcd.begin(16, 2);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Trig déclenché 10ms sur HIGH
  digitalWrite(trigPin, LOW);

  // Calcul de l'écho
  duration = pulseIn(echoPin, HIGH);
  // Distance proportionnelle à la durée de sortie
  distance = duration * 340 / (2 * 10000); //Vitesse du son théorique

// Display LEDs
  if (distance < 10) {
    digitalWrite(redLED, HIGH);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  if(distance >=10 && distance < 50) {
    digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  if(distance <= 0 || (distance >50 && distance < 100)) { 
    digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
   
   if ( distance <= 0) {
  
    //Serial.print(duration);
    Serial.println("No sensing");
    lcd.print("No sensing");
  }
  else {
    Serial.print(distance);
    Serial.print(" cm ");
    Serial.print(duration);
    Serial.println(" ms");

    lcd.print(distance);
    lcd.print(" cm ");
    lcd.print(duration);
    lcd.print(" ms");
    
  }
  
  delay(100);  //0,1 sec between 2 measures
  digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, LOW);
    delay(100);
}

