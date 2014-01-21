
const int R_PIN = 9;
const int G_PIN = 10;
const int B_PIN = 11;

int colors[] = { 0xFFFFFF, 0xb5b5b5, 0x002144, 0x75c530 };

void setup() {
  // put your setup code here, to run once:
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
}

void loop() {
  for (int colorIndex = 0; colorIndex < (sizeof(colors) / sizeof(int)); colorIndex++){
        int currentColor = colors[colorIndex];
        
        analogWrite(R_PIN, (currentColor >> 8 * 2) & 0xFF);
        analogWrite(G_PIN, (currentColor >> 8 * 1) & 0xFF);
        analogWrite(B_PIN, (currentColor >> 8 * 0) & 0xFF);
        
        delay(400);
  }
  
  analogWrite(R_PIN, 0);
  analogWrite(G_PIN, 0);
  analogWrite(B_PIN, 0);
  
  delay(3000);

  analogWrite(R_PIN, 255);
  analogWrite(G_PIN, 255);
  analogWrite(B_PIN, 255);
  
  delay(3000);

}

