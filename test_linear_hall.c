int linhall = A1; 

void setup ()
	{
	Serial.begin (9600); 
	}
void loop ()
	{
	Serial.print( analogRead (linhall) );
	Serial.println();
	delay (500);
	}
