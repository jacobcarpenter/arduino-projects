const int button1 = 2;

const int TL = 9;
const int ML = 8;
const int BL = 7;
const int MC = 6;
const int TR = 12;
const int MR = 11;
const int BR = 10;

int lastButtonState = LOW;
int lastRoll = 0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(button1, INPUT);
  pinMode(TL, OUTPUT);
  pinMode(ML, OUTPUT);  
  pinMode(BL, OUTPUT);
  pinMode(MC, OUTPUT);
  pinMode(TR, OUTPUT);
  pinMode(MR, OUTPUT);
  pinMode(BR, OUTPUT);
}

void clearDice() {
  digitalWrite(TL, LOW);
  digitalWrite(ML, LOW);
  digitalWrite(BL, LOW);
  digitalWrite(MC, LOW);
  digitalWrite(TR, LOW);
  digitalWrite(MR, LOW);
  digitalWrite(BR, LOW);
}

void showValue(int diceValue) {
  clearDice();

  switch(diceValue) {
  case 1:
    digitalWrite(MC, HIGH);
    break;
  case 2:
    digitalWrite(TL, HIGH);
    digitalWrite(BR, HIGH);
    break;
  case 3:
    digitalWrite(TL, HIGH);
    digitalWrite(MC, HIGH);
    digitalWrite(BR, HIGH);
    break;
  case 4:
    digitalWrite(TL, HIGH);
    digitalWrite(TR, HIGH);
    digitalWrite(BL, HIGH);
    digitalWrite(BR, HIGH);
    break;
  case 5:
    digitalWrite(TL, HIGH);
    digitalWrite(TR, HIGH);
    digitalWrite(MC, HIGH);
    digitalWrite(BL, HIGH);
    digitalWrite(BR, HIGH);
    break;
  case 6:
    digitalWrite(TL, HIGH);
    digitalWrite(ML, HIGH);
    digitalWrite(BL, HIGH);
    digitalWrite(TR, HIGH);
    digitalWrite(MR, HIGH);
    digitalWrite(BR, HIGH);
    break;
  }
}

int getNext() {
  return random(1, 7);
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
