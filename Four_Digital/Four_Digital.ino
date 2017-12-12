//www.elegoo.com
//2016.12.12

int latch = 4; //74HC595  pin 9 STCP
int clock = 5; //74HC595  pin 10 SHCP
int data = 3; //74HC595  pin 8 DS

unsigned char table[] =
{ 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c
  , 0x39, 0x5e, 0x79, 0x71, 0xFFFF
};

void setup() {
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
}
void Display(unsigned char num)
{

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, table[num]);
  digitalWrite(latch, HIGH);

}
void loop() {

  for(int num = 0; num < 18; num++) {
    Display(num);
    delay(500);
  }

}
