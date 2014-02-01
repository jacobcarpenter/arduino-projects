

const int anodes[] = { 3, 5, 9, 10 };
const int cathodes[] = { 7, 4, 2 };

unsigned long lastDelayChange = 0;
int delayChangeInterval = 1700;
int currentDelay = 2;
int maxDelay = 80;
int delayMutator = 1;

unsigned long lastRowChange = 0;
int rowChangeInterval = 200;
int currentCathode = 0;
int cathodeMutator = 1;

unsigned long lastColumnChange = 0;
int columnChangeInterval = 400;
int currentAnode = 0;
int anodeMutator = 1;

void setup() {

	for (int i = 0; i < sizeof(anodes) / sizeof(int); ++i) {
		pinMode(anodes[i], OUTPUT);
		digitalWrite(anodes[i], LOW);
	}

	for (int i = 0; i < sizeof(cathodes) / sizeof(int); ++i) {
		pinMode(cathodes[i], OUTPUT);
		digitalWrite(cathodes[i], HIGH);
	}
}

void loop() {

	for (int a = 0; a < sizeof(anodes) / sizeof(int); ++a) {
		digitalWrite(anodes[(a != 0 ? a : sizeof(anodes) / sizeof(int)) - 1 ], LOW);

		for (int c = 0; c < sizeof(cathodes) / sizeof(int); ++c) {
			if (a == currentAnode)
				digitalWrite(cathodes[c], c == currentCathode ? LOW : LOW);
			else
				digitalWrite(cathodes[c], c == currentCathode ? LOW : HIGH);
		}

		digitalWrite(anodes[a], HIGH);
		delay(currentDelay);
	}


	unsigned long now = millis();
	if (now - lastDelayChange > delayChangeInterval){
		if (currentDelay >= maxDelay)
			delayMutator = -delayMutator;

		if (delayMutator > 0)
			currentDelay = currentDelay * 3 / 2;
		else
			currentDelay = currentDelay * 2 / 3;

		if (currentDelay <= 2) {
			delayMutator = -delayMutator;
			currentDelay = 2;
		}

		lastDelayChange = now;
	}

	if (now - lastRowChange > rowChangeInterval) {
		if (currentCathode == ((sizeof(cathodes) / sizeof(int)) - 1))
			cathodeMutator = -cathodeMutator;

		currentCathode += cathodeMutator;

		if (currentCathode == 0)
			cathodeMutator = -cathodeMutator;
		
		lastRowChange = now;
	}

	if (now - lastColumnChange > columnChangeInterval) {
		if (currentAnode == ((sizeof(anodes) / sizeof(int)) - 1))
			anodeMutator = -anodeMutator;

		currentAnode += anodeMutator;

		if (currentAnode == 0)
			anodeMutator = -anodeMutator;

		lastColumnChange = now;
	}
}

