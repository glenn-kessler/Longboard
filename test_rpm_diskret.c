/* DREHZAHLERKENNUNG PER INTERNEM INTERRUP -> TIMER */

const int hall = 2;

volatile int rot=0, A = 0;
int f=0;

//Pindefinitionen Ausgänge
void setup() {
  Serial.begin(9600);

  pinMode(hall, INPUT);

  TimerConfig();
}

void loop() {
Serial.print(A);
Serial.println(rot);
rot = 0;
delay(1000);
}

void TimerConfig()
{
  TCCR5A = 0;
  TCCR5B = 0;
  TCCR5B |= (1<<WGM52) | (1<<CS50) | (1<<CS52);
  
  TIMSK5 = 0;
  TIMSK5 |= (1<<OCIE5A);

  OCR5A = 10;
  sei();
}

ISR(TIMER5_COMPA_vect){
  if(A==0 && f==1)
    rot++;
  A=f;
  f=!digitalRead(hall);
}
