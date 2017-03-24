const int DI_hall = 2;
const int DO_led = 13;

int hall_status = LOW;
int umin = 0;
int hall_zaehler = 0;
unsigned long loop_start = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(DI_hall, INPUT);
  pinMode(DO_led, OUTPUT);
}

void loop()
{
  hall_status = digitalRead(DI_hall);
  
  hall_zaehler = 0;
  loop_start = millis();
  
  while(millis() - loop_start < 10)
  {
    if(hall_status == LOW)
      hall_zaehler++;
    
    while(digitalRead(DI_hall) == LOW){}

    digitalWrite(DO_led, LOW);
    
    while(digitalRead(DI_hall) == HIGH){}  

    digitalWrite(DO_led, HIGH);
  }

  Serial.print("U/s ");
  Serial.println(hall_zaehler * 10);

}


