// :%s#Serial.#/*&*/f#
#include <stdio.h>

int kP=0;
int kI=1;
int kD=0;
int w=0;
int n=0;
int e=0;
int u=0;
int Zykluszeit=0;

void schreibe_log()
	{
		static char erster_Aufruf = 'Y';
		if( erster_Aufruf == 'Y' )
			{
			
			erster_Aufruf = 'N';

			/*Serial.*/printf( " Drehzahl_Soll," );
			/*Serial.*/printf( " Drehzahl_Ist," );
			/*Serial.*/printf( " Drehzahl_Fehler," );
			/*Serial.*/printf( " PWM_Vorgabe_an_ESC," );
			/*Serial.*/printf( " kP = " );
			/*Serial.*/printf( kP );
			/*Serial.*/printf( " ," );
			/*Serial.*/printf( " kI = " );
			/*Serial.*/printf( kI );
			/*Serial.*/printf( " ," );
			/*Serial.*/printf( " kD = " );
			/*Serial.*/printf( kD );
			/*Serial.*/printf( " ," );
			/*Serial.*/printf( " tH = " );
			/*Serial.*/printf( Zykluszeit );
			/*Serial.*/printf( " tH = " );
			/*Serial.*/printf( Zykluszeit/n );
			/*Serial.*/printf("\n");
			}

			/*Serial.*/printf("%d",w );
			/*Serial.*/printf(",");
			/*Serial.*/printf("%d",n );
			/*Serial.*/printf(",");
			/*Serial.*/printf("%d",e );
			/*Serial.*/printf(",");
			/*Serial.*/printf("%d",u );
			/*Serial.*/printf(",");
			/*Serial.*/printf( "0" ); /* 0-Werte damit die .csv valide ist und der Graf ohne Fehler erstellt wird */
			/*Serial.*/printf(",");
			/*Serial.*/printf( "0" );
			/*Serial.*/printf(",");
			/*Serial.*/printf( "0" );
			/*Serial.*/printf(",");
			/*Serial.*/printf( "0" );
			/*Serial.*/printf(",");
			/*Serial.*/printf( "0" );
			/*Serial.*/printf("\n");
	}

int main(void)
{ 

	while(kP>=0)
	{
		schreibe_log();

		if(kP<255)
			kI=-1;
		kP+=kI;
	}
	return 0;
}
