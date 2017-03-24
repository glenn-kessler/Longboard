const int DI_hall_CLK = 2;
const int DI_hall_A = 3;

volatile int A=0, CLK=0, rotation=0;
int Data_0=0, Q=0;


void setup()
{
	Serial.begin(9600);

	pinMode(DI_hall_CLK, INPUT);
	pinMode(DI_hall_A, INPUT);
	
	// initialize timer1 
	noInterrupts();		// disable all interrupts
	TCCR5A = 0;
	TCCR5B = 0;
	TCNT5	= 0;

	OCR5A = 108;		// compare match register 16MHz/256/8Hz
	TCCR5B |= (1 << WGM52);	// CTC Modus: Zählregister sollen beim Erreichen des Vergleichswertes von Kanal A wieder auf 0 gesetzt werden 
	TCCR5B |= (1 << CS52);	// 256 prescaler 
	TIMSK5 |= (1 << OCIE5A);// Interrupt an Kanal A aktiviere
	interrupts();		// enable all interrupts
}

ISR(TIMER5_COMPA_vect)		// Interrupt des Timer1 Kanal A
{
	if( Data_0 == 0 && A == 1 )
		rotation++;

	Data_0 = A;
	A = !digitalRead(DI_hall_A);
	CLK = !digitalRead(DI_hall_CLK);
}

void loop()
{
	get_richtung();
	printshit();
}

string get_richtung()
	{
	// D_Flipflop
	     if(CLK==0 && A==0)	Q = Q; // speichern
	else if(CLK==0 && A==1)	Q = 0; // rücksetzen
	else if(CLK==1 && A==0)	Q = Q; // speichern
	else if(CLK==1 && A==1)	Q = A; // setzen

	if(Q==0)
			return "Vorwaerts";
	else
			return "Rueckwaerts";
	}

void printshit()
	{	
	Serial.print(A);
	Serial.print("\t");
	Serial.print(B);
	Serial.print("\t");
	Serial.print(Q);
	Serial.print("\t");
	Serial.print(richtung);
	Serial.print("\t");
	Serial.print(rotation);
	Serial.println();
	rotation=0;
	delay(100);	
	}
