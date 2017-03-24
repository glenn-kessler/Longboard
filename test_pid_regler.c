// PID-Regler
// ohne Ansteuerung des A/D- D/A-Umsetzers
// ohne Bildung der Regeldifferenz
//
float PID(float e)
{
	const float kP = 1;	// P-Anteil	
	const float kI = 1;	// I-Anteil	kI = kP * T/Tn
	const float kD = 0;	// D-Anteil	kD = Tv/T

	float u;			//

	static float uI = 0;	
	static float ek_1 = 0;	// Regeldifferenz d. vorigen Abtastung

	uI += kI*e;		// I-Anteil

	u = kP*e + uI + kD*(e - ek_1);

	ek_1 = e;		// zuw. d. aktuellen Wertes von e f. nexten Durchlauf

	return u;
}
