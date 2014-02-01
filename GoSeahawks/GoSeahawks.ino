const int anodes[] = { 3, 5, 9, 10 };
const int cathodes[] = { 7, 4, 2 };

const int scrollTextLength = 58;
int data[][scrollTextLength] = {
	{0,0,0,0,0,0, 0,1,0, 0, 0,1,0, 0,0,0,0, 0,1,1, 0, 1,1,1, 0, 0,1,0, 0, 1,0,1, 0, 0,1,0, 0, 1,0,0,1, 0, 1,0,1, 0, 0,1,1, 0,0, 1, 0,0,0,0,0,0}, 
	{0,0,0,0,0,0, 1,0,0, 0, 1,0,1, 0,0,0,0, 0,1,0, 0, 1,0,0, 0, 1,0,1, 0, 1,0,1, 0, 1,0,1, 0, 1,0,0,1, 0, 1,1,0, 0, 0,1,0, 0,0, 1, 0,0,0,0,0,0},
	{0,0,0,0,0,0, 1,0,1, 0, 1,0,1, 0,0,0,0, 0,0,1, 0, 1,1,0, 0, 1,1,1, 0, 1,1,1, 0, 1,1,1, 0, 1,1,1,1, 0, 1,1,0, 0, 0,0,1, 0,0, 0, 0,0,0,0,0,0},
	{0,0,0,0,0,0, 0,1,0, 0, 0,1,0, 0,0,0,0, 1,1,0, 0, 1,1,1, 0, 1,0,1, 0, 1,0,1, 0, 1,0,1, 0, 0,1,1,0, 0, 1,0,1, 0, 1,1,0, 0,0, 1, 0,0,0,0,0,0}
};

int pauseAt[] = {6, 10, 17, 21, 25, 29, 33, 37, 38, 42, 46, 50};

const int lightDelay = 3;

int currentColumn = 0;
int columnAdvanceInterval = 240;

const int pauseCycles = 2;
int nextPause = 0;
int pauseCounter = 0;

unsigned long lastColumnAdvance = 0;

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
		// turn off last row
		digitalWrite(anodes[(a != 0 ? a : sizeof(anodes) / sizeof(int)) - 1 ], LOW);

		for (int c = 0; c < sizeof(cathodes) / sizeof(int); ++c) {
			digitalWrite(cathodes[c], getPixel(a, c));
		}

		// turn on current row
		digitalWrite(anodes[a], HIGH);
		delay(lightDelay);
	}

	unsigned long now = millis();
	if (now - lastColumnAdvance > columnAdvanceInterval) {

		if (currentColumn == pauseAt[nextPause]) {
			pauseCounter++;
			if (pauseCounter == pauseCycles){
				pauseCounter = 0;
				nextPause = (nextPause + 1) % (sizeof(pauseAt) / sizeof(int));
			}
		}
		else {
			currentColumn = currentColumn + 3 == scrollTextLength ? 0 : currentColumn + 1;
		}
	
		lastColumnAdvance = now;
	}
}

int getPixel(int a, int c) {
	return data[a][currentColumn + c] ? LOW : HIGH;
}

