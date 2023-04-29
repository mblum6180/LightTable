#include <FastLED.h>

#define DATA_PIN 5
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 1024
#define WIDTH 32
#define HEIGHT 32
#define BRIGHTNESS 50

CRGB leds[NUM_LEDS];

uint8_t grid[WIDTH][HEIGHT];
uint8_t newGrid[WIDTH][HEIGHT];

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  randomSeed(analogRead(0));

  // Initialize the grid with random values
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      grid[x][y] = random(2);
    }
  }
}

void loop() {
  displayGrid();
  FastLED.show();
  FastLED.delay(100);

  updateGrid();
}

void displayGrid() {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int ledIndex = x + (y * WIDTH);
      if (grid[x][y] == 1) {
        leds[ledIndex] = CRGB::White;
      } else {
        leds[ledIndex] = CRGB::Black;
      }
    }
  }
}

void updateGrid() {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int neighbors = countNeighbors(x, y);

      if (grid[x][y] == 1) {
        newGrid[x][y] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
      } else {
        newGrid[x][y] = (neighbors == 3) ? 1 : 0;
      }
    }
  }

  memcpy(grid, newGrid, sizeof(grid));
}

int countNeighbors(int x, int y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0) continue;

      int nx = (x + i + WIDTH) % WIDTH;
      int ny = (y + j + HEIGHT) % HEIGHT;

      if (grid[nx][ny] == 1) {
        count++;
      }
    }
  }
  return count;
}

