#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define DATA_PIN 5
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 32
#define BRIGHTNESS 50

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    MATRIX_WIDTH, MATRIX_HEIGHT, DATA_PIN,
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
    NEO_GRB + NEO_KHZ800);

// Game of Life
byte world[MATRIX_WIDTH][MATRIX_HEIGHT];
byte newWorld[MATRIX_WIDTH][MATRIX_HEIGHT];

void setup() {
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  randomSeed(analogRead(0));

  // Initialize the world with random values
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      world[x][y] = random(0, 2);
    }
  }
}

byte getNeighborCount(byte x, byte y) {
  byte count = 0;
  for (int8_t i = -1; i <= 1; i++) {
    for (int8_t j = -1; j <= 1; j++) {
      if (!(i == 0 && j == 0)) {
        count += world[(x + i + MATRIX_WIDTH) % MATRIX_WIDTH][(y + j + MATRIX_HEIGHT) % MATRIX_HEIGHT];
      }
    }
  }
  return count;
}

void updateWorld() {
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      byte neighbors = getNeighborCount(x, y);
      
      if (world[x][y] == 1 && (neighbors < 2 || neighbors > 3)) {
        newWorld[x][y] = 0;
      } else if (world[x][y] == 0 && neighbors == 3) {
        newWorld[x][y] = random(1, 3);
      } else {
        newWorld[x][y] = world[x][y];
      }
    }
  }
  
  memcpy(world, newWorld, sizeof(world));
}

void drawWorld() {
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
      if (world[x][y] == 1) {
        matrix.drawPixel(x, y, matrix.Color(255, 0, 0)); // Red
      } else if (world[x][y] == 2) {
        matrix.drawPixel(x, y, matrix.Color(0, 0, 255)); // Blue
      } else {
        matrix.drawPixel(x, y, matrix.Color(0, 0, 0)); // Black
      }
    }
  }
}

void loop() {
  matrix.fillScreen(0);

  drawWorld();
  updateWorld();

  matrix.show();
  delay(100);
}
