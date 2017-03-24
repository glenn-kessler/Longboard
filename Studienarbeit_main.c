/* MOTOR STEUERUNG MIT MESSUNG DER DREHZAHL */

/* Version	0.5	
 *
 * Datum	2017-03-07
 *
 * Author	Glenn Keßler
 * 
 * Aufgabe	Steuern eines Gleichstrommotors anhand Benutzervorgabe und Drehzahlmessung
 * 
 * Funktionen	- Motor ansteuern, Vorwaerst, Rückwärts mit variabler Drehzahl
 * 		- Motordrehzahl erfassen
 * 		- Benutzereingabe erfassen
 * 		- Regelung der Motordrehzahl gemäß Benutzereingabe anhand erfasster Drehzahl
 * 		- Fahrt freigeben anhand Erkennung ob Passagier auf dem Board steht
 * 		- Fahrt freigeben anhand Erkennung ob Board auf dem Boden steht
 * 		- Fahrt freigeben anhand Erkennung ob Fernbedienung verbindung zum Arduino hat
 *
 */

	
/* Input Signale */
	const int D2_Hall_CLK = 2;
	const int D3_Hall_DATA = 3;
	int A1_Joystick_Y = A1;
/* Output Signale */
	const int D4_L298N_vor = 4;
	const int D5_L298N_zurueck = 5;
	const int D6_L298N_PWM = 6;
/* Externer Interrupt */
	const unsigned short IRQ0 = 0;  // mappt auf Digital Pin 2 bzw. D2_Hall_CLK 
/* Speichervariablen */
	const unsigned int Anzahl_Magnete = 1;
	bool drehsinn = true;	// true = vorwaerts
	volatile unsigned int erkannte_Flanken = 0;	// inkrement durch externe ISR auf D2_Hall_CLK, Reset in Hautptprogramm
	volatile bool vorw_neu = true;
	volatile bool vorw_1 = true;
	volatile bool vorw_2 = true;	
	unsigned long letzter_Aufruf = 0 /* ms */;
	unsigned short Zykluszeit = 300 /* ms */;
	bool vz = true;
	int n=0, w=0, e=0, u=0;	// Reglerparameter

void setup()
	{
	Serial.begin( 9600 );

	/* externer Interrupt um Hall-Switch zu lesen */
	attachInterrupt( IRQ0, isr_rpm, RISING );

	/* Input Signale */
	pinMode( D2_Hall_CLK, INPUT );
	pinMode( D3_Hall_DATA, INPUT );

	/* Output Signale */
	pinMode( D4_L298N_vor,OUTPUT );
	pinMode( D5_L298N_zurueck,OUTPUT );
	pinMode( D6_L298N_PWM,OUTPUT );
	}

void loop()
	{
	if( ( millis() - letzter_Aufruf ) > Zykluszeit )
		{
			letzter_Aufruf = millis();

			vz = aktueller_drehsinn();
			n = gemessene_Drehzahl_mit_VZ( vz, erkannte_Flanken );
			w = Sollwert_aus_Fernbedienung_mit_Anpassung_an_Motortreiber(); /* für Tests wird w auf einen festen Wert gesetzt */
			e = Regeldifferenz( w, n );
			u = PID_Regler( e );

			L298N_Ansteuerung( u );

			// Schreibe_log();

			erkannte_Flanken = 0;
		}
	}

/* erzeugt Ausgabe als Komma separierte Liste -> Format .csv */
void Schreibe_Log()
	{
		static bool erster_Aufruf = bool;
		if( erster_Aufruf )
			{
			
			erster_Aufruf = false;

			Serial.print( " Drehzahl_Soll," );
			Serial.print( " Drehzahl_Ist," );
			Serial.print( " Drehzahl_Fehler," );
			Serial.print( " PWM_Vorgabe_an_ESC," );
			Serial.print( " kP = " );
			Serial.print( kP );
			Serial.print( " ," );
			Serial.print( " kI = " );
			Serial.print( kI );
			Serial.print( " ," );
			Serial.print( " kD = " );
			Serial.print( kD );
			Serial.print( " ," );
			Serial.print( " tH = " );
			Serial.print( Zykluszeit );
			Serial.println();
			}

		Serial.print( w );
		Serial.print( "," );
		Serial.print( n );
		Serial.print( "," );
		Serial.print( e );
		Serial.print( "," );
		Serial.print( u );
		Serial.print( "," );
		/* ab hier 0-Werte da die Reglerparameter zwar in der Legende auftauchen sollen, im Graf aber nicht benötigt werden */
		Serial.print( 0 ); 
		Serial.print( "," );
		Serial.print( 0 );
		Serial.print( "," );
		Serial.print( 0 );
		Serial.print( "," );
		Serial.print( 0 );
		Serial.println();
	}

bool aktueller_drehsinn()
	{
	if( vorw_2 == true && vorw_1 == true && vorw_neu == true )
		drehsinn = true;
	else if( vorw_2 == false && vorw_1 == false && vorw_neu == false )                                                                                          
		drehsinn = false;

		return drehsinn;
	}

int gemessene_Drehzahl_mit_VZ( bool vz, int erkannte_Flanken )
	{
	if( vz == false )
		erkannte_Flanken = ~r+1;

	return erkannte_Flanken;
	}

int Sollwert_aus_Fernbedienung_mit_Anpassung_an_Motortreiber()
	{
		/* gemessene maximale Drehzahl = 72
		 * 
		 * diese ist in beiden Fahrtrichtungen möglich
		 * d.h.
		 * der Wertebereich des 10Bit Wertes aus dem analogen Eingang 
		 * muss auf einen Wertebereich zwischen -72 und +72 aufgeteilt werden
		 *
		 * ( 72+1 ) * 2 * x = 1024 ---> x = 7,01369
		 *
		 * also so ungefähr 7
		 *
		 * Da die Werte des Analogeinganges alle im Positiven Bereich liegen
		 * muss dieser noch verschoben werden, so dass die Mitte des Wertebereichs
		 * auch wirklich eine Vorgabe von 0 liefert
		 * 
		 */
		static int x = 7;
		static int Verschiebung = 73;
	 	return ( analogRead( A1_Joystick_Y ) / 7 ) - Verschiebung;
	}

/* belässt den vorigen Drehsinn unverändert wenn nicht die letzten drei Messungen das selbe Ergebnis hatten */
int Regeldifferenz( int w, int n )
	{
	return w - n;
	}

int PID_Regler( int e )
	{
	const double kP = 5;	// P-Anteil	
	const double kI = 0.04;	// I-Anteil	kI = kP * T/Tn
	const double kD = 0;	// D-Anteil	kD = Tv/T

	double u;

	static double uI = 0;	
	static double ek_1 = 0;	// Regeldifferenz d. vorigen Abtastung

	uI += kI*e;		// I-Anteil

	u = kP*e + uI + kD*( e - ek_1 );

	ek_1 = e;		// zuw. d. aktuellen Wertes von e f. nexten Durchlauf

	/* verhindert dass u den Wertebereich des L298N über/unterschreitet */
	if( u > 255 )
		u = 255;
	else if( u < -255 )
		u = -255;

	return u;
	}

void L298N_Ansteuerung( int Stellwert )
	{
	if( Stellwert < 0 )
		{	
		digitalWrite( D5_L298N_zurueck, HIGH );
		digitalWrite( D4_L298N_vor, LOW );

		analogWrite( D6_L298N_PWM, ( ~Stellwert + 1 ) );
		}
	else
		{
		digitalWrite( D5_L298N_zurueck, LOW );
		digitalWrite( D4_L298N_vor, HIGH );

		analogWrite( D6_L298N_PWM, Stellwert );
		}
	}

void isr_rpm()
	{
	erkannte_Flanken++;

	vorw_2 = vorw_1;
	vorw_1 = vorw_neu;

	if( !digitalRead( D3_Hall_DATA ) == 1 )
		vorw_neu = true;
	else
		vorw_neu = false;
	}
