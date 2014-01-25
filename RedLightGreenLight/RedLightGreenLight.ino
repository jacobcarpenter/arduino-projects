const int firstLight = 8;
const int lightCount = 4;

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));

	for (int n = 0; n < lightCount; ++n)
		pinMode(n + firstLight, OUTPUT);
}

void loop() {
	int next = firstLight + random(0, 4);

	digitalWrite(next, HIGH);

	int delayMultiplier = random(3, 6);
	delay(500 * delayMultiplier);

	digitalWrite(next, LOW);
}

