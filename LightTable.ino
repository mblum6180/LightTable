#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define LED_PIN 5
#define LED_COUNT 1024

CRGB leds[LED_COUNT];
#define BRIGHTNESS 16
#define MATRIX_TILE_WIDTH 32
#define MATRIX_TILE_HEIGHT 32
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 32
#define MATRIX_TYPE

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK 0

#define LED_RED_VERYLOW (3 << 11)
#define LED_RED_LOW (7 << 11)
#define LED_RED_MEDIUM (15 << 11)
#define LED_RED_HIGH (31 << 11)

#define LED_GREEN_VERYLOW (1 << 5)
#define LED_GREEN_LOW (15 << 5)
#define LED_GREEN_MEDIUM (31 << 5)
#define LED_GREEN_HIGH (63 << 5)

#define LED_BLUE_VERYLOW 3
#define LED_BLUE_LOW 7
#define LED_BLUE_MEDIUM 15
#define LED_BLUE_HIGH 31

#define LED_ORANGE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW (LED_RED_LOW + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM (LED_RED_MEDIUM + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH (LED_RED_HIGH + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW (LED_RED_LOW + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM (LED_RED_MEDIUM + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH (LED_RED_HIGH + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW (LED_GREEN_LOW + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM (LED_GREEN_MEDIUM + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH (LED_GREEN_HIGH + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW (LED_RED_LOW + LED_GREEN_LOW + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM (LED_RED_MEDIUM + LED_GREEN_MEDIUM + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH (LED_RED_HIGH + LED_GREEN_HIGH + LED_BLUE_HIGH)

// FastLED_NeoMatrix matrix(leds, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE);

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, MATRIX_TILE_HEIGHT, MATRIX_TILE_HEIGHT, 1, 1,
                                                  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
                                                      NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG +
                                                      NEO_TILE_TOP + NEO_TILE_RIGHT + NEO_TILE_PROGRESSIVE);

int grid[32][32];

void setup()
{
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
  matrix->fillScreen(CRGB::Black);
  randomSeed(analogRead(0));
  generateRandomGrid();
}

void loop()
{
  updateGrid();
  displayGrid();
  delay(1000);
}

void generateRandomGrid()
{
  for (int i = 0; i < MATRIX_WIDTH; i++)
  {
    for (int j = 0; j < MATRIX_HEIGHT; j++)
    {
      if (random(0, 8) == 1)
      {
        grid[i][j] = 1;
        matrix->drawPixel(i, j, (uint16_t)LED_WHITE_MEDIUM);
      }
      else
      {
        grid[i][j] = 0;
      }
    }
  }
}

/* Need to add fading of LEDS
Re-add where new pixels are "born" when count is too low.
  Should be next to a current pixel
*/

void updateGrid()
{
  int newGrid[32][32];
  int lifeCount = 0;
  for (int i = 0; i < MATRIX_WIDTH; i++)
  {
    for (int j = 0; j < MATRIX_HEIGHT; j++)
    {
      int neighbors = countNeighbors(i, j);
      if (grid[i][j] == 1)
      {
        if (neighbors < 2 || neighbors > 3)
        {
          newGrid[i][j] = 0;
          lifeCount++;
          matrix->drawPixel(i, j, (uint16_t)LED_RED_MEDIUM);
        }
        else
        {
          newGrid[i][j] = 1;
          lifeCount++;
          matrix->drawPixel(i, j, (uint16_t)LED_BLUE_MEDIUM);
        }
      }
      else
      {
        if (neighbors == 3)
        {
          newGrid[i][j] = 1;
          matrix->drawPixel(i, j, (uint16_t)LED_BLUE_MEDIUM);
        }
        else
        {
          newGrid[i][j] = 0;
        }
      }
    }
  }
  memcpy(grid, newGrid, sizeof(newGrid));
}

int countNeighbors(int x, int y)
{
  int count = 0;
  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      int neighbor_x = (x + i + MATRIX_WIDTH) % MATRIX_WIDTH;
      int neighbor_y = (y + j + MATRIX_HEIGHT) % MATRIX_HEIGHT;
      count += grid[neighbor_x][neighbor_y];
    }
  }
  count -= grid[x][y];
  return count;
}

void displayGrid()
{
  matrix->show();
}
