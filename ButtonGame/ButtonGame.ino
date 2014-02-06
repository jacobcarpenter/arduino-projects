
const int playerButtons[] = { 2, 3 };
const int playerLights[] = { 8, 12 };
const int playerCount = 2;

const int colorChannels[] = { 9, 10 }; // 11 = BLUE

enum GameState {
	AllowReset,
	SelectingColor,
	InputLoses,
	InputWins,
	DoWinnerAnimation
};

enum GameState currentState = AllowReset;

int buttonStates[] = { HIGH, HIGH };
unsigned long lastButtonStateChanges[] = { 0, 0 };
const int buttonPressDebounceInterval = 20;
const int winnerBlinkInterval = 180;
const int winnerBlinks = 5;

unsigned long now = 0;
unsigned long timeLastChange = 0;
int selectionInterval = 0;

int lastWinner = 0;
int lastWinnerBlinkCount = 0;

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));

	// buttons
	for (int i = 0; i < playerCount; ++i) {
		pinMode(playerButtons[i], INPUT_PULLUP);
	}

	// player lights
	for (int i = 0; i < playerCount; ++i) {
		pinMode(playerLights[i], OUTPUT);
		digitalWrite(playerLights[i], LOW);
	}

	// tri-color LED
	for (int i = 0; i < sizeof(colorChannels) / sizeof(int); ++i) {
		pinMode(colorChannels[i], OUTPUT);
		digitalWrite(colorChannels[i], LOW);
	}
}

void loop() {

	now = millis();

	// check button states
	bool pressedButtons[] = { false, false };
	for (int i = 0; i < playerCount; ++i) {
		if (readButtonPress(now, playerButtons[i], buttonStates[i], lastButtonStateChanges[i]))
			pressedButtons[i] = true;
	}

	if (now - timeLastChange > selectionInterval) {
		switch (currentState) {
			case SelectingColor:
				selectColor();
			break;
			case DoWinnerAnimation:
				doWinnerAnimation(lastWinner, lastWinnerBlinkCount);
			break;
			case InputLoses:
			case InputWins:
				colorLightOff();
				startColorSelection();
			break;
		}
	}

	for (int i = 0; i < playerCount; ++i) {
		if (pressedButtons[i]) {
			if (currentState == InputLoses || currentState == InputWins) {
				colorLightOff();
				int winner = currentState == InputLoses ?
					(i + 1) % 2 :
					i;
				doWinnerAnimation(winner, 0);
				break;
			}
			else if (currentState == AllowReset) {
				digitalWrite(playerLights[0], LOW);
				digitalWrite(playerLights[1], LOW);
				startColorSelection();
				break;
			}
		}
	}
}

void doWinnerAnimation(int winner, int winnerBlinkCount) {
	timeLastChange = now;
	lastWinner = winner;
	lastWinnerBlinkCount = winnerBlinkCount;
	selectionInterval = winnerBlinkInterval;

	bool isLit = digitalRead(playerLights[winner]) == HIGH;
	if (isLit)
		++lastWinnerBlinkCount;

	digitalWrite(playerLights[winner], isLit ? LOW : HIGH);

	if (lastWinnerBlinkCount == winnerBlinks)
		startColorSelection();
	else
		currentState = DoWinnerAnimation;
}


void colorLightOff() {
	digitalWrite(colorChannels[0], LOW);
	digitalWrite(colorChannels[1], LOW);
}

void startNewTimeInterval() {
	timeLastChange = now;
	selectionInterval = getRandomDelay();
}

void startColorSelection() {
	currentState = SelectingColor;
	startNewTimeInterval();
}

void selectColor() {
	int color = random(2);
	digitalWrite(colorChannels[(color + 1) % 2], LOW);
	digitalWrite(colorChannels[color], HIGH);

	currentState = color == 0 ? InputLoses : InputWins;

	startNewTimeInterval();
}

int getRandomDelay() {
	const int delayIncrement = 500;
	return random(4, 8) * delayIncrement;
}

bool readButtonPress(unsigned long now, int pin, int & lastButtonState, unsigned long & lastButtonStateChange) {
	bool wasPressed = false;
	int buttonState = digitalRead(pin);
	if (lastButtonStateChange == 0 && buttonState != lastButtonState) {
		// set state change flag; check it again in a little bit
		lastButtonStateChange = now;
	}
	else if (lastButtonStateChange != 0 && now - lastButtonStateChange > buttonPressDebounceInterval) {
		if (buttonState != lastButtonState) {
			wasPressed = buttonState == LOW;
			lastButtonState = buttonState;
		}

		// time elapsed; clear state change flag
		lastButtonStateChange = 0;
	}
	return wasPressed;
}

