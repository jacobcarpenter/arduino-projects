
const int firstLight = 8;
const int lightCount = 4;

enum ProgramState {
	programStopped,
	programStarting,
	programRunning
};
enum ProgramState state;

const int button = 12;
int buttonStateLast = HIGH;

unsigned long timeLast = 0;
int interval = 0;

const int blinksToStart = 5;
int blinkCount = 0;
int blinkStateLast = LOW;

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));

	pinMode(button, INPUT_PULLUP);

	for (int n = 0; n < lightCount; ++n)
		pinMode(n + firstLight, OUTPUT);
}

void loop() {
	if (readButtonPress(button, &buttonStateLast)) {
		switch (state) {
		case programStopped:
			blinkCount = 0;
			blinkStateLast = LOW;
			state = programStarting;
			break;

		case programStarting:
		case programRunning:
			clearAllLights();
			interval = 0;
			state = programStopped;
			break;
		}
	}

	unsigned long timeNow = millis();
	if (timeNow - timeLast > interval) {
		switch (state) {
		case programStarting:
			tickStartingAnimation();
			break;

		case programRunning:
			clearAllLights();
			
			int newLight = firstLight + random(0, 4);
			digitalWrite(newLight, HIGH);

			interval = getRandomDelay();
			break;
		}

		timeLast = timeNow;
	}
}

void tickStartingAnimation() {
	bool isAnimationComplete = blinkCount == blinksToStart;
	if (!isAnimationComplete) {

		if (blinkStateLast == HIGH) {
			clearAllLights();
			blinkStateLast = LOW;
		}
		else {
			setRedLights();
			blinkStateLast = HIGH;
			++blinkCount;
		}

		interval = 250;
	}
	else {
		clearAllLights();
		interval = 2000 + getRandomDelay();
		state = programRunning;
	}
}

void setRedLights() {
	PORTB |= 0x0A;
}
void clearAllLights() {
	PORTB &= ~0x0F;
}

int getRandomDelay() {
	const int delayIncrement = 500;
	return random(3, 6) * delayIncrement;
}

bool readButtonPress(int pin, int * lastState) {
	int buttonState = digitalRead(pin);
	if (buttonState != *lastState) {
		delay(50);
		if (buttonState == digitalRead(pin)) {
			*lastState = buttonState;
			bool isButtonPressed = buttonState == LOW;
			return isButtonPressed;
		}
	}

	return false;
}
