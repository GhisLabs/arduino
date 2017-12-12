/*

  This sketch detects distance
  using the ultrasound sensor HC-SR04.

  The circuit for ultrasound

  Trig -> pin 10
  Echo -> pin 9

  The circuit for LEDs
   Green LED + 470R  -> pin 8
   Orange LED + 470R  -> pin 7
   Red LED + 470R  -> pin 6

  Author: Ghislain BIDAUT
  Version 0.01

*/

#define trigPin 10        //Trig Ultrasons (out)
#define echoPin 9         //Echo Ultrasons (in)
#define greenLED 8        //Green Led
#define orangeLED 7       //Orange Led
#define redLED 6          //Red Led
#define maxCount 100      // Idle duration

#define closeDist 20
#define intermediateDist 100
#define farDist 300


long counterIdle = 0;
long duration, distance, previousDistance;

void setup() {

  Serial.begin (9600);

  Serial.print("Booting Up");

  // init distance sensor
  pinMode(trigPin, OUTPUT);  //Trig est une sortie
  pinMode(echoPin, INPUT);   //Echo est le retour, en entrée

  // init LEDs
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {

  Serial.print("counterIdle = ");
  Serial.println(counterIdle);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Trig déclenché 10ms sur HIGH
  digitalWrite(trigPin, LOW);

  // Calcul de l'écho
  duration = pulseIn(echoPin, HIGH);
  // Distance proportionnelle à la durée de sortie
  distance = duration * 340 / (2 * 10000); //Vitesse du son théorique

  if (abs(previousDistance - distance) <= 10) {
    counterIdle++;
  }
  else {
    counterIdle = 0;
  }

  previousDistance = distance;

  // Display LEDs
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

  if ( distance <= 0) {
    //Serial.print(duration);
    Serial.println("No sensing");
  }
  else {
    Serial.print(distance);
    Serial.print(" cm ");
    Serial.print(previousDistance);
    Serial.print(" cm ");

    Serial.print(duration);
    Serial.println(" ms");

  }

  delay(100);  //0,1 sec between 2 measures
  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(100);
}

