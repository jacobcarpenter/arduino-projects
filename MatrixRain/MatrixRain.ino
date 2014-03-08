

int columns[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int rows[] = { A3, A2, A1, A0, 8, 9, 10, 11 };

unsigned long lastDropTime = 0;
int nextDropInterval = 0;
word trails[8] = {};
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

	for (int r = 0; r < 8; ++r)
	{
		digitalWrite(rows[r], HIGH);

		for (int c = 0; c < 8; ++c)
		{
			byte falling = getGridValue(grid, r, c);
			if (falling)
			{
				digitalWrite(columns[c], HIGH);

				if (random(1024) < (2 << falling))
				{
					grid[r] &= ~(3 << (c * 2));
					setGridValue(trails, r, c, 3);
					if (r < 7)
						dropOntoGrid(r + 1, c, falling);
				}
			}
		}

		delayMicroseconds(4);

		for (int c = 0; c < 8; ++c)
		{
			byte trail = getGridValue(trails, r, c);
			if (trail)
			{
				if (now % trail != 0)
					digitalWrite(columns[c], HIGH);

				if (now % 45 == 0)
					setGridValue(trails, r, c, trail - 1);
			}
		}

		delayMicroseconds(2);

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

byte getGridValue(const word * grid, int row, int column)
{
	return (grid[row] >> (column * 2)) & 3;
}

void setGridValue(word * grid, int row, int column, byte value)
{
	grid[row] &= ~(3 << (column * 2));
	grid[row] |= value << (column * 2);
}

void dropOntoGrid(int row, int column, byte fallingSpeed)
{
	setGridValue(grid, row, column, max(fallingSpeed, getGridValue(grid, row, column)));
}

