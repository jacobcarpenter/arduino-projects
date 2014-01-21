const int button1 = 2;         // push button wired to ground on one side and a 10K pull up resistor to 5v + line to pin on other

const int group1 = 1 << 0;     // single LED with single 330 ohm resistor (orange, orange, brown)
const int group2 = 1 << 1;     // two LEDs in series with single 100 ohm resistor (brown, black x3, brown)
const int group3 = 1 << 2;     // same as group 2
const int group4 = 1 << 3;     // same as group 2

const int ledPatterns[] = { 0, // clear all
  group1,                      // 1
  group2,                      // 2
  group1 | group2,             // 3
  group2 | group3,             // 4
  group1 | group2 | group3,    // 5
  group2 | group3 | group4     // 6
};

int lastRoll = 0;
int lastButtonState = LOW;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(button1, INPUT);
  DDRB = group1 | group2 | group3 | group4; // set pins as outputs
}

int getNext() {
  return random(1, 7);
}

void showValue(int diceValue) {
  PORTB = ledPatterns[diceValue];
}

const int animationLength = 4;
int animateNext() {
  int nextValue = getNext();
  int animation[animationLength + 2];

  animation[0] = lastRoll;
  animation[animationLength + 1] = nextValue;

  for (int i = 1; i < animationLength + 1; i++) {
    int animationValue;
    do {
      animationValue = getNext();
    } while (animationValue == animation[i - 1] || animationValue == animation[i + 1]);
    animation[i] = animationValue;
  }

  for (int i = 0; i < sizeof(animation) / sizeof(int); i++) {
    showValue(animation[i]);
    delay(300);
  }
  return nextValue;
}

void loop() {
  int button1State = digitalRead(button1);
  if (button1State != lastButtonState) {
    lastButtonState = button1State;
    if (button1State == HIGH) {
      lastRoll = animateNext();
    }
  }
}
