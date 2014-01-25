const int firstLight = 8;
const int lightCount = 4;

unsigned long timeLast = 0;
int interval = 0;

void clearAllLights();
int getDelay();

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));

	for (int n = 0; n < lightCount; ++n)
		pinMode(n + firstLight, OUTPUT);
}

void loop() {
	unsigned long timeNow = millis();
	if (timeNow - timeLast > interval) {
		clearAllLights();
		
		int newLight = firstLight + random(0, 4);
		digitalWrite(newLight, HIGH);

		timeLast = timeNow;
		interval = getDelay();
	}
}

void clearAllLights() {
	PORTB &= ~0x0F;
}

int getDelay() {
	const int delayIncrement = 500;
	return random(3, 6) * delayIncrement;
}
