#include "Timer.h" //include timer library
Timer t; // create a timer object

#define trigPin 8        //Trig Ultrasons (out)
#define echoPin 7         //Echo Ultrasons (in)
#define greenLED 3        //Green Led
#define orangeLED 2       //Orange Led
#define redLED 13          //Red Led
#define maxCount 100      // Idle duration

long counterIdle = 0;
long duration, distance, previousDistance;


int first_digit = 0;
int second_digit = 0;
int third_digit = 0;
int fourth_digit = 0;
int timer_event = 0;
int CA_1 = 12;
int CA_2 = 11;
int CA_3 = 10;
int CA_4 = 9;
int clk = 6;
int latch = 5;
int data = 4;
int count = 0;
int digits[4] ;
int CAS[4] = {12, 11, 10, 9};

//byte combinations for each number 0-9

byte numbers[10] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};


void setup() {
  Serial.begin(9600); //serial start and pin config

  // init distance sensor
  pinMode(trigPin, OUTPUT);  //Trig est une sortie
  pinMode(echoPin, INPUT);   //Echo est le retour, en entrée

  // init LEDs
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // Init 7 segment display
  pinMode(CA_1, OUTPUT);
  pinMode(CA_2, OUTPUT);
  pinMode(CA_3, OUTPUT);
  pinMode(CA_4, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
  cathode_high(); // blank the screen
}

void loop() {
  
 /* 
  t.update(); //timer update
  if (Serial.available()) { // read from serial
    t.stop(timer_event); //stop timer if anything to read
    cathode_high(); // blank the screen
    String s = Serial.readString(); //read the serail value
    number = (long)s.toInt(); //convert it to int
    if (number > 9999) { //check the number is 0-9999
      Serial.println("Please Enter Number Between 0 - 9999");
    } else {
      break_number(number);
      timer_event = t.every(1, display_number); // start timer again
    }
  }*/
  
  t.update(); //timer update
  
  // Set up counteridle and sensor
  //Serial.print("counterIdle = ");
  //Serial.println(counterIdle);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //Trig déclenché 10ms sur HIGH
  digitalWrite(trigPin, LOW);
  
// Calcul de l'écho et de la distance
  duration = pulseIn(echoPin, HIGH);
  // Distance proportionnelle à la durée de sortie
  distance = duration * 340 / (2 * 10000); //Vitesse du son théorique


  if (abs(distance-previousDistance)>5) {
    
    //t.stop(timer_event); //stop timer if anything to read
    //cathode_high(); // blank the screen
    break_number(distance);
//    timer_event = t.every(1, display_number);
    display_number();
    display_number();
    display_number();
    display_number();
  }


  if (abs(previousDistance - distance) <= 20) {
    counterIdle++;
  }
  else {
    counterIdle = 0;
  }

  previousDistance = distance;

 // Display LEDs
  if (distance < 10 && counterIdle < maxCount) {
    digitalWrite(redLED, HIGH);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  if ((distance >= 10 && distance < 50) && counterIdle < maxCount) {
    digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  if ((distance > 50 && distance < 100) && counterIdle < maxCount) {
    digitalWrite(redLED, LOW);
    digitalWrite(orangeLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  // Display serial port
   if ( distance <= 0) {
    Serial.print(duration);
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


  //delay(100);  //0,1 sec between 2 measures
/*  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(100);*/
}

void break_number(long num) { // seperate the input number into 4 single digits

  first_digit = num / 1000;
  digits[0] = first_digit;

  int first_left = num - (first_digit * 1000);
  second_digit = first_left / 100;
  digits[1] = second_digit;
  int second_left = first_left - (second_digit * 100);
  third_digit = second_left / 10;
  digits[2] = third_digit;
  fourth_digit = second_left - (third_digit * 10);
  digits[3] = fourth_digit;
}

void display_number() { //scanning

  cathode_high(); //black screen
  digitalWrite(latch, LOW); //put the shift register to read
  shiftOut(data, clk, LSBFIRST, numbers[digits[count]]); //send the data
  digitalWrite(CAS[count], LOW); //turn on the relevent digit
  digitalWrite(latch, HIGH); //put the shift register to write mode
  count++; //count up the digit
  if (count == 4) { // keep the count between 0-3
    count = 0;
  }
}

void cathode_high() { //turn off all 4 digits
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
}
