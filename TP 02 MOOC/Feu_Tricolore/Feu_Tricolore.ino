/*
  TP02 MOOC Programmer un objet avec Arduino - Feu Tricolore
*/

const int redLight = 12;
const int orangeLight = 11;
const int greenLight = 10;
const int buttonPin = 2;
const int pedestrianRedLight = 9;
const int pedestrianGreenLight = 8;
int buttonState = 0;

// Initialisation des entrées-sorties
void setup() {
  
  pinMode(redLight, OUTPUT);
  pinMode(orangeLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(pedestrianRedLight, OUTPUT);
  pinMode(pedestrianGreenLight, OUTPUT);
  
}

// Fonction boucle qui tourne indéfiniment
void loop() {

      digitalWrite(pedestrianRedLight, HIGH);     // Rouge pour les piétons
      digitalWrite(pedestrianGreenLight, LOW);
      
      digitalWrite(redLight, LOW);                // Orange pour les voitures
      digitalWrite(orangeLight, HIGH);
      digitalWrite(greenLight, LOW);
      
      delay(1000);                               // Attente 1 seconde
      
      digitalWrite(redLight, HIGH);              // Rouge pour les voitures
      digitalWrite(orangeLight, LOW);
      delay(3000);                               // Attente 3 secondes

      digitalWrite(redLight, LOW);               // Vert pour les voitures
      digitalWrite(greenLight, HIGH);
      delay(3000);                               // Attente 3 secondes

      buttonState = digitalRead(buttonPin);      // Enregistrement de l'état du bouton

      if(buttonState  == HIGH) {                 // Si le bouon est pressé
          digitalWrite(greenLight, LOW);              // Orange pour les voitures
          digitalWrite(orangeLight, HIGH);
          delay(1000);                                // Attente une seconde
          digitalWrite(redLight, HIGH);               // Rouge pour les voitures
          digitalWrite(orangeLight, LOW);
          digitalWrite(pedestrianRedLight, LOW);      // Vert pour les piétons
          digitalWrite(pedestrianGreenLight, HIGH);
          delay(5000);                                // Attente 5 secondes

          digitalWrite(pedestrianRedLight, HIGH);     // Rouge pour les piétons
          digitalWrite(pedestrianGreenLight, LOW);
      
          digitalWrite(redLight, LOW);               // Vert pour les voitures
          digitalWrite(greenLight, HIGH);
          delay(3000);                               // Attente 3 secondes
      }
}
