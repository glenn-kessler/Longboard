// Henry's Bench
// Module KY023
//
// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-ps2-joystick-tutorial-keyes-ky-023-deek-robot/

int X= A0; // X Input Pin
int Y = A1; // Y Input Pin
int KEYin = 3; // Push Button + 1k Resistor between button an VCC

void setup ()
	{
	pinMode (KEYin, INPUT);
	Serial.begin (9600); 
	}
void loop ()
	{
	Serial.print( analogRead (X) );
	Serial.print("\t");
	Serial.print( analogRead (Y) );
	Serial.print("\t");
	Serial.print( KEYin );
	Serial.println();
	delay (500);
	}
