

int columns[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int rows[] = { A3, A2, A1, A0, 8, 9, 10, 11 };

unsigned long lastDropTime = 0;
int nextDropInterval = 0;
word grid[8] = {};

void setup() {
	randomSeed(analogRead(5));

	for (int i = 0; i < 8; ++i) {
		pinMode(columns[i], OUTPUT);
		pinMode(rows[i], OUTPUT);
	}
}

void loop() {
	unsigned long now = millis();

	// TODO: add trails... will might need to precompute the grid

	for (int r = 0; r < 8; ++r) {
		for (int c = 0; c < 8; ++c) {
			byte falling = getFallingSpeed(r, c);
			if (falling != 0) {
				digitalWrite(columns[c], HIGH);

				if (random(101) < (2 << (falling - 1)))
				{
					grid[r] &= ~(3 << (c * 2));
					if (r < 7)
						dropOntoGrid(r + 1, c, falling);
				}
			}
		}

		digitalWrite(rows[r], HIGH);

		delay(2);

		digitalWrite(rows[r], LOW);

		for (int c = 0; c < 8; ++c)
			digitalWrite(columns[c], LOW);
	}

	if (now - lastDropTime > nextDropInterval) {
		nextDropInterval = random(768);
		dropOntoGrid(random(8), random(8), random(4));		
		lastDropTime = now;
	}
}

byte getFallingSpeed(int row, int column)
{
	return (grid[row] >> (column * 2)) & 3;
}

void dropOntoGrid(int row, int column, byte fallingSpeed)
{
	grid[row] |= max(fallingSpeed, getFallingSpeed(row, column)) << (column * 2);
}

