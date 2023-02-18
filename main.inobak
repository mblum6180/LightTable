#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>

const int rows = 32;
const int cols = 32;
#define mh 32
#define mw 32
const int matrixRows = 32;
const int matrixCols = 32;
const int LED_PIN = 5;
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS (matrixRows * matrixCols)
#define BRIGHTNESS 160
const int lifeMin = 69;
const int lifeReset = 6;

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK		0

#define LED_RED_VERYLOW 	(3 <<  11)
#define LED_RED_LOW 		(7 <<  11)
#define LED_RED_MEDIUM 		(15 << 11)
#define LED_RED_HIGH 		(31 << 11)

#define LED_GREEN_VERYLOW	(1 <<  5)   
#define LED_GREEN_LOW 		(15 << 5)  
#define LED_GREEN_MEDIUM 	(31 << 5)  
#define LED_GREEN_HIGH 		(63 << 5)  

#define LED_BLUE_VERYLOW	3
#define LED_BLUE_LOW 		7
#define LED_BLUE_MEDIUM 	15
#define LED_BLUE_HIGH 		31

#define LED_ORANGE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW		(LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW	(LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW		(LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM	(LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH		(LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW	(LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW		(LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM		(LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH		(LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW		(LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)


const int ledColor = 3;

int grid[rows][cols];
CRGB leds[NUM_LEDS];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, matrixRows, matrixCols, 1, 1, 
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);


unsigned long previousMillis = 0;
const long interval = 4000; // Change state every 4 secs

void setup() {
Serial.begin(115200);

setupGrid();

FastLED.addLeds<NEOPIXEL,LED_PIN>(  leds, NUM_LEDS  ).setCorrection(TypicalLEDStrip);  
matrix->begin();
matrix->setBrightness(BRIGHTNESS);
matrix->clear();

previousMillis = millis();

 // Calculate the values of the Mandelbrot set
  /*for (int x = 0; x < rows; x++) {
    for (int y = 0; y < cols; y++) {
      int real = map(x, 0, rows, -2, 2);
      int imag = map(y, 0, cols, -2, 2);
      int iterations = mandelbrot(real, imag);
      grid[x][y] = iterations;
    }
  }*/



}

void loop() {
    delay(100);
    life();
    //displayGrid(grid);
    displayLedGrid(grid);

    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      //ESP.restart();
      int x = random(2, rows - 4);
      int y = random(2, cols - 4);
      grid[x][y] = 1;
      grid[x + 1][y] = 1;
    }
}

void setupGrid() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[i][j] = random(2);
    }
  }
}


int mandelbrot(int real, int imag) {
  int iterations = 0;
  float realPart = 0;
  float imagPart = 0;
  float realSquared = 0;
  float imagSquared = 0;
  while (iterations < 255 && realSquared + imagSquared <= 4) {
    imagPart = 2 * realPart * imagPart + imag;
    realPart = realSquared - imagSquared + real;
    realSquared = realPart * realPart;
    imagSquared = imagPart * imagPart;
    iterations++;
  }
  return iterations;
}


void life() {
  int newGrid[rows][cols];
  int count = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int neighbors = countNeighbors(i, j);
      if (grid[i][j] == 1 && (neighbors == 2 || neighbors == 3)) {
        newGrid[i][j] = 1;
        count++;
      } else if (grid[i][j] == 0 && neighbors == 3) {
        newGrid[i][j] = 1;
        count++;
      } else {
        newGrid[i][j] = 0;
      }
    }
  }
  if (count <= lifeMin){
      int x = random(2, rows - 4);
      int y = random(2, cols - 4);
      newGrid[x][y] = 1;
      newGrid[x + 1][y] = 1;
  }
  copyGrid(newGrid, grid);
  if (count <= lifeReset){
      setupGrid();
  }

  //Serial.println(count);
}

int countNeighbors(int x, int y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int row = (x + i + rows) % rows;
      int col = (y + j + cols) % cols;
      count += grid[row][col];
    }
  }
  count -= grid[x][y];
  yield();
  return count;
}

void copyGrid(int source[][cols], int target[][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      target[i][j] = source[i][j];
    }
  }
}

void displayGrid(int grid[][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Serial.print(grid[i][j]);
    }
    Serial.println();
  }

}
void displayLedGrid(int grid[][cols]) {
  matrix->clear();
  for (uint16_t i=0; i<mh; i++) {
	  for (uint16_t j=0; j<mw; j++) {
      if (grid[i][j] >= 1) {
        matrix->drawPixel(j, i, (uint16_t)LED_WHITE_MEDIUM);
        } else {
        matrix->drawPixel(j, i, (uint16_t)LED_PURPLE_VERYLOW);
      }
	    //matrix->drawPixel(j, i, i%3==0?(uint16_t)LED_BLUE_HIGH:i%3==1?(uint16_t)LED_RED_HIGH:(uint16_t)LED_GREEN_HIGH);
	    // depending on the matrix size, it's too slow to display each pixel, so
	    // make the scan init faster. This will however be too fast on a small matrix.
	    /*#ifdef ESP8266
	    if (!(j%3)) matrix->show();
	    yield(); // reset watchdog timer
	    #elif ESP32
	    delay(1);
	    matrix->show();
	    #else 
	    matrix->show();
	    #endif*/
    }
  }
  matrix->show();
}
