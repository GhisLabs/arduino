//**********************************************************
//  Capteur de distance à ultrasons HC-SR04 avec Arduino Uno
//  TipTopBoards.com  06 01 2014  Rolland
//  Càabler +5V et GND du détecteur sur l'arduino
//
// Trig -> pin 13
// Echo -> pin 12
// Led verte + 470R  -> pin 10
// Led rouge + 470R  -> pin 11
// Les leds s'allument selon la distance trop près / trop loin
//**********************************************************
#define trigPin 13  //Trig Ultrasons (sortie)
#define echoPin 12  //Echo Ultrasons (entrée)
#define ledVerte 11      //Led verte
#define ledRouge 10     //Led rouge

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);  //Trig est une sortie
  pinMode(echoPin, INPUT);   //Echo est le retour, en entrée
  pinMode(ledVerte, OUTPUT);
  pinMode(ledRouge, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Trig déclenché 10ms sur HIGH
  digitalWrite(trigPin, LOW);

  // Calcul de l'écho
  duration = pulseIn(echoPin, HIGH);
  // Distance proportionnelle à la durée de sortie
  distance = duration * 340 / (2 * 10000); //Vitesse du son théorique

  if (distance < 4) {  // Distance trop près
    digitalWrite(ledVerte, HIGH); //Vert éclairé seulement
    digitalWrite(ledRouge, LOW);
  }
  else {   //Trop loin
    digitalWrite(ledVerte, LOW);
    digitalWrite(ledRouge, HIGH);
  }
  //Hors de portée (duration)
  if ( distance <= 0) {
    //Serial.print(duration);
    Serial.println("Hors de portee");
  }
  else {
    Serial.print(distance);
    Serial.print(" cm ");
    Serial.print(duration);
    Serial.println(" ms");
  }
  delay(100);  //0.1 sec entre deux mesures
}


