
const int trig = 28;
const int echo = 22;
const int m_forward = 7;
const int m_backward = 8;
const int m_spin = 9;



long t_echo; // s
int abstand; // m
double v_schall = 0.034; // cm/us

int spin = 0;

void setup()
{
	pinMode(m_forward,OUTPUT);
	pinMode(m_backward,OUTPUT);
	pinMode(m_spin,OUTPUT);
	 
	pinMode(13,OUTPUT);
	
	pinMode(trig,OUTPUT);
	pinMode(echo,INPUT);
	Serial.begin(9600);
}



void loop()
{
	digitalWrite(13, HIGH);
	digitalWrite(trig, LOW);
	delayMicroseconds(1000);
	
	digitalWrite(13, LOW);
	
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	
	t_echo = pulseIn(echo, HIGH);
	
	 if( t_echo < 16384 ) // Objekt nicht weiter weg als 3m
	 {
		 abstand = t_echo * v_schall / 2;
		 // s = t * v / 2
		
		/*
		 Serial.print("Abstand = ");
		 Serial.print(abstand);
		 Serial.println(" cm");
		*/
		
		 if( t_echo < 2048 )
		 {
			 spin = t_echo >>3;
			
			 if( spin < 128 )
			 {
				 digitalWrite(m_forward,LOW);
				 digitalWrite(m_backward,HIGH);
				
				 analogWrite(m_spin,spin);
			 }

			 else if( spin < 255 )
			 {
				 digitalWrite(m_forward,HIGH);
				 digitalWrite(m_backward,LOW);
			
				 analogWrite(m_spin,spin);
			 }
			
			 Serial.println(spin);
		 }
	 }
}
