
/*

  This sketch prints "temperature/humidity" and distance measure to the LCD HD44780
  using the ultrasound sensor HC-SR04.
  Leds are switched on according to the distance.

  Author: Ghislain BIDAUT
  Version 0.92
  
  The circuit for LCD:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K resistor:
    ends to +5V and ground
    wiper to LCD VO pin (pin 3)
   Display backend:
    A to 220 Ohms to digital pin 1
    K to ground
  VSS To ground
  VDD to +5V

  The circuit for ultrasound

    Trig -> pin 10
    Echo -> pin 9

  The circuit for LEDs
     Green LED + 470R  -> pin 8
     Orange LED + 470R  -> pin 7
     Red LED + 470R  -> pin 6

  The circuit for DHT11,
     VCC: 5V or 3V
     GND: GND
     DATA: 13

*/



#include <LiquidCrystal.h>
#include <SimpleDHT.h>
#include <Event.h>
#include <Timer.h>

#define LCDBacklight 1    // LCD back light pin

#define pinDHT11 13       // DHT11 measure PIN

#define trigPin 10        //Trig Ultrasons (out)
#define echoPin 9         //Echo Ultrasons (in)
#define greenLED 8        //Green Led
#define orangeLED 7       //Orange Led
#define redLED 6          //Red Led


#define maxCount 100      // Idle duration

#define closeDist 35      // Close distance in cm
#define intermediateDist 100  //Intermediate distance in cp
#define farDist 300       // Far distance in cm


long counterIdle = 0;
long duration = 0, distance = 0, previousDistance = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

SimpleDHT11 dht11;
Timer t;

void setup() {

  // Serial turned off in order to use pin 1 as LCD Backlight
  //Serial.begin (9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Version 0.92");
  lcd.setCursor(0, 1);
  lcd.print("Date 29/12/2017");
  
  // init distance sensor
  pinMode(trigPin, OUTPUT);  //Trig est une sortie
  pinMode(echoPin, INPUT);   //Echo est le retour, en entrée

  // init LEDs
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // turn on LCD backlight
  pinMode(LCDBacklight, OUTPUT);
  digitalWrite(LCDBacklight, HIGH);

  // set up timers for sampling measure
  int tempEvent = t.every(2000, takeTemperatureMesure);
  int distanceEvent = t.every(500, takeDistanceMesure);

  // Test LEDS
  digitalWrite(redLED, HIGH);
  digitalWrite(orangeLED, HIGH);
  digitalWrite(greenLED, HIGH);
  
  delay(3000);

  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  digitalWrite(greenLED, LOW);
}

// Take distance measure
void takeDistanceMesure() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Trig déclenché 10ms sur HIGH
  digitalWrite(trigPin, LOW);

  // Calcul de l'écho
  duration = pulseIn(echoPin, HIGH);
  // Distance proportionnelle à la durée de sortie
  distance = duration * 340 / (2 * 10000); //Vitesse du son théorique
}

// Take Temp and Humidity and display it
void takeTemperatureMesure() {

  // Display Temp/Humidity on LCD row 2
  lcd.setCursor(0, 1);
  // read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed");
    lcd.print("Read DHT11 failed");
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" °C, ");
  Serial.print((int)humidity); Serial.println(" %");

  lcd.print("T: ");
  lcd.print((int)temperature);
  lcd.print(" C (");
  lcd.print((int)humidity);
  lcd.print("%)   ");
}

// infinite loop
void loop() {
  t.update();
  Serial.print("counterIdle = ");
  Serial.println(counterIdle);

  if(abs(previousDistance - distance) <= 1 || distance >= farDist) {
    counterIdle++;
  }
  else {
    counterIdle = 0;
  }

  previousDistance = distance;

  // Display LEDs
  if ( distance <= 0) {
    digitalWrite(redLED, HIGH);
    digitalWrite(orangeLED, HIGH);
    digitalWrite(greenLED, HIGH);
  }
  if (distance < closeDist && counterIdle < maxCount) {
    digitalWrite(redLED, HIGH);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  if ((distance >= closeDist && distance < intermediateDist) && counterIdle < maxCount) {
    digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  if ((distance >= intermediateDist && distance < farDist) && counterIdle < maxCount) {
    digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  if (counterIdle >= maxCount) {
    lcd.noDisplay();
    digitalWrite(LCDBacklight, LOW);
  }
  else {
    lcd.display();
    digitalWrite(LCDBacklight, HIGH);
  }

  // Display distance on LCD Row 1
  lcd.setCursor(0, 0);
  if ( distance <= 0) {
    Serial.println("No sensing");
    lcd.println("No sensing");
  }
  else {
    lcd.print("D: ");
    lcd.print(distance);
    lcd.print("cm        ");

    Serial.print(distance);
    Serial.print(" cm ");
    Serial.print(previousDistance);
    Serial.print(" cm ");
    Serial.print(duration);
    Serial.println(" ms");
  }

  delay(100);  //0,2 sec between LED blinks
  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(100);

}

