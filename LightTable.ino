#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define DATA_PIN 5
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 32
#define BRIGHTNESS 200
#define RESET_ITERATIONS 10
#define FADE_STEPS 5
#define MIN_BRIGHTNESS 66
#define OFF_PIXEL_COLOR matrix.Color(2, 9, 2)
#define RANDOM_LIFE_PROBABILITY 5 // Probability (in percentage) of a new life pixel being added on each update


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    MATRIX_WIDTH, MATRIX_HEIGHT, DATA_PIN,
    NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
    NEO_GRB + NEO_KHZ800);

// Game of Life
byte world[MATRIX_WIDTH][MATRIX_HEIGHT];
byte newWorld[MATRIX_WIDTH][MATRIX_HEIGHT];
byte fadeBuffer[MATRIX_WIDTH][MATRIX_HEIGHT];
uint8_t stableCount = 0;

void setup() {
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  randomSeed(analogRead(0));
  resetWorld();
}

void resetWorld() {
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      world[x][y] = random(0, 3);
      fadeBuffer[x][y] = 0;
    }
  }
}

byte getNeighborCount(byte x, byte y) {
  byte count = 0;
  for (int8_t i = -1; i <= 1; i++) {
    for (int8_t j = -1; j <= 1; j++) {
      if (!(i == 0 && j == 0)) {
        count += world[(x + i + MATRIX_WIDTH) % MATRIX_WIDTH][(y + j + MATRIX_HEIGHT) % MATRIX_HEIGHT] > 0;
      }
    }
  }
  return count;
}

bool updateWorld() {
  bool changed = false;
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      byte neighbors = getNeighborCount(x, y);
      
      if (world[x][y] > 0 && (neighbors < 2 || neighbors > 3)) {
        newWorld[x][y] = 0;
        changed = true;
      } else if (world[x][y] == 0 && neighbors == 3) {
        newWorld[x][y] = random(1, 3);
        changed = true;
      } else {
        newWorld[x][y] = world[x][y];
      }
    }
  }
  
  // Randomly add a new life pixel
  if (random(100) < RANDOM_LIFE_PROBABILITY) {
    uint8_t x = random(MATRIX_WIDTH);
    uint8_t y = random(MATRIX_HEIGHT);
    newWorld[x][y] = random(1, 3);
    changed = true;
  }

  memcpy(world, newWorld, sizeof(world));
  return changed;
}

void updateFadeBuffer() {
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      if (world[x][y] > 0 && fadeBuffer[x][y] < 255) {
        fadeBuffer[x][y] = min(fadeBuffer[x][y] + FADE_STEPS, 255);
      } else if (world[x][y] == 0 && fadeBuffer[x][y] > MIN_BRIGHTNESS) {
        fadeBuffer[x][y] = max(fadeBuffer[x][y] - FADE_STEPS, MIN_BRIGHTNESS);
      }
    }
  }
}

void drawWorld() {
  updateFadeBuffer();
  
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      uint8_t intensity = fadeBuffer[x][y];
      if (world[x][y] == 1) {
        matrix.drawPixel(x, y, matrix.Color(intensity, 0, 0)); // Red
      } else if (world[x][y] == 2) {
        matrix.drawPixel(x, y, matrix.Color(0, 0, intensity)); // Blue
      } else {
        matrix.drawPixel(x, y, OFF_PIXEL_COLOR); // Dim green
      }
    }
  }
}

void loop() {
  matrix.fillScreen(0);

  drawWorld();

  // Update the world and check if it has changed
  bool changed = updateWorld();

  if (!changed) {
    stableCount++;
  } else {
    stableCount = 0;
  }

  // If the world has been stable for RESET_ITERATIONS, reset it
  if (stableCount >= RESET_ITERATIONS) {
    resetWorld();
    stableCount = 0;
  }

  matrix.show();
  delay(100); // Adjust this value to change the speed of the animation
}
