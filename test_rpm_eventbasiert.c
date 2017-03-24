/* DREHZAHLERKENNUNG PER EXTERNEM INTERRUPT */

/* rpm */
const unsigned short D2_clk = 2;
const unsigned short D3_data = 3;
const unsigned short IRQ0 = 0;	// mappt auf Digital Pin 2
const unsigned short rpm_update_zeit = 1000/* ms */;
volatile unsigned int rotationen = 0;
volatile bool v_neu = true;
volatile bool v_1 = true;
volatile bool v_2 = true;
String richtung = "";

void setup()
	{
	Serial.begin(9600);
	pinMode(D2_clk, INPUT);
        pinMode(D3_data, INPUT);        
        
	/** Aufruf von isr_rpm() bei jeder durch den Hall-Switch erkannten Umdrehung */
	attachInterrupt(IRQ0, isr_rpm, RISING);
	}

void loop()
	{
		if( v_2 == true && v_1 == true && v_neu == true)
			richtung = "Vorwärts ";
		else if( v_2 == false && v_1 == false && v_neu == false)
			richtung = "Rückwärts ";
  
		Serial.print( richtung );
		Serial.print("\t");
		Serial.println( rotationen );
		rotationen = 0;
          delay(10);

	}

/* RPM Funktionen */

/** ext interrupt zählt jede Rotation */
void isr_rpm()
	{
	rotationen++;
        v_2 = v_1;
	v_1 = v_neu;

	if( !digitalRead(D3_data) == 1 )
		v_neu = true;
	else
                v_neu = false;
	}

