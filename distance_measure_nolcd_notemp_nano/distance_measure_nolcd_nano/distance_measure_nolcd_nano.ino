
/*
  Distance measurement with arduino nano.
  Leds are switched on according to the distance.

  Author: Ghislain BIDAUT
  Version 0.93

  The circuit for ultrasound

    Trig -> pin 10
    Echo -> pin 9

  The circuit for LEDs
     Green LED + 470R  -> pin 8
     Orange LED + 470R  -> pin 7
     Red LED + 470R  -> pin 6

*/

#include <Event.h>
#include <Timer.h>

#define trigPin 10        //Trig Ultrasons (out)
#define echoPin 9         //Echo Ultrasons (in)
#define greenLED 8        //Green Led
#define orangeLED 7       //Orange Led
#define redLED 6          //Red Led


#define maxCount 100      // Idle duration

#define veryCloseDist 15      // Close distance in cm
#define closeDist 25      // Close distance in cm
#define intermediateDist 100  //Intermediate distance in cp
#define farDist 300       // Far distance in cm


long counterIdle = 0;
long duration = 0, distance = 0, previousDistance = 0;

Timer t;

void setup() {

  Serial.begin (9600);
  
  // init distance sensor
  pinMode(trigPin, OUTPUT);  //Trig est une sortie
  pinMode(echoPin, INPUT);   //Echo est le retour, en entrée

  // init LEDs
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // set up timers for sampling measure
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
  
  if (distance < veryCloseDist && counterIdle < maxCount) {
    digitalWrite(redLED, HIGH);
    digitalWrite(orangeLED, HIGH);
    digitalWrite(greenLED, HIGH);
  }

  if (distance >= veryCloseDist && distance < closeDist && counterIdle < maxCount) {
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

  delay(100);  //0,2 sec between LED blinks
  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(100);

}
