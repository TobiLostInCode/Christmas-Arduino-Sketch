#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>


/* #region  Display */

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire, 4);
const int RIGHT = 0;
const int LEFT = 1;

int progressbarPosition = 0;

void printSnowflake(int x, int y)
{
  static const unsigned char PROGMEM snowflake_white[] =
  {
    0x01, 0x00, 0x02, 0x80, 0x21, 0x08, 0x13, 0x90, 0x0d, 0x60, 0x0b, 0xa0, 0x54, 0x54, 0xbd, 0x7a, 
    0x54, 0x54, 0x0b, 0xa0, 0x0d, 0x60, 0x13, 0x90, 0x21, 0x08, 0x02, 0x80, 0x01, 0x00
  };

  display.drawBitmap(x, y, snowflake_white, 15, 15, WHITE);
}

void clearPartOfDisplay(const int& xBegin, const int& yBegin, const int& width, const int& height)
{
  display.fillRect(xBegin, yBegin, width, height, BLACK);
}

/*!
Creates a progressbar and advances it by one recktangle every time this method is called
*/
void progressBar(bool progressbarBegin)
{
  int nextProgressbarPosition = progressbarPosition + 17;

  if (nextProgressbarPosition >= 121 || progressbarBegin)
  {
    nextProgressbarPosition = 0;
    printSnowflake(0, 17);
    clearPartOfDisplay(0, 17, 128, 16);
  }
  printSnowflake(nextProgressbarPosition, 17);
  display.display();
  delay(120);

  progressbarPosition = nextProgressbarPosition;
}

/*!
Initiates the OLED display and displays the booting screen
*/
void setupDisplay()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(25, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("Booting");
  display.display();
  
  progressBar(true);

  for (size_t i = 0; i < 7; i++)
  {
    progressBar(false);
  }
  
  delay(200);
  display.clearDisplay();
  display.stopscroll();
  display.display();
}

/*!
Set's some general stting for the display pointer
*/
void setCursorSettings(const int& cursorX, const int& cursorY, const int& size, const int& color = WHITE)
{
  display.setTextColor(color);
  display.setTextSize(size);
  display.setCursor(cursorX, cursorY);
}

/*!
Scrolls the content of the either left or right for x amount of millisecons
For a 128x32 OLED their are 4 sections 1-4
*/
void scrollDisplay(const int &startSection, const int &endSection, const int &duration, const int &direction)
{
  if (direction == RIGHT)
  {
    display.startscrollright(startSection, endSection);
  }
  else if (direction == LEFT)
  {
    display.startscrollleft(startSection, endSection);
  }
  delay(duration);
  display.stopscroll();
}

/*!
Prints a bitmap of ghosts and pumpkins that look to the right
*/
void printPixelart()
{
  static const unsigned char PROGMEM right_looking_white[] =
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x80, 
    0x03, 0x80, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x01, 0xc0, 
    0x07, 0xc0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x03, 0xe0, 
    0x0f, 0xe0, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x07, 0xf0, 
    0x07, 0xc0, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x03, 0xe0, 
    0x0f, 0xe0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x07, 0xf0, 
    0x1f, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x0f, 0xf8, 
    0x3f, 0xf8, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x1f, 0xfc, 
    0x0f, 0xe0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x07, 0xf0, 
    0x1f, 0xf0, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x0f, 0xf8, 
    0x3f, 0xf8, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x1f, 0xfc, 
    0x7f, 0xfc, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x3f, 0xfe, 
    0x03, 0x80, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x01, 0xc0 };

  display.drawBitmap(0, 16, right_looking_white, 128, 16, WHITE);
}

void printSnowflake2(int x, int y)
{
  static const unsigned char PROGMEM snowflake_white[] =
  {
    0x02, 0x00, 0x07, 0x00, 0x22, 0x20, 0x1a, 0xc0, 0x17, 0x40, 0x4a, 0x90, 0xfd, 0xf8, 0x4a, 0x90, 
    0x17, 0x40, 0x1a, 0xc0, 0x22, 0x20, 0x07, 0x00, 0x02, 0x00
  };

  display.drawBitmap(x, y, snowflake_white, 13, 13, WHITE);
}

/*!
Prints the halloween message that is displayed after the boot sequence is finished
*/
void printMessage()
{
  setCursorSettings(30, 0, 2);
  display.print("Merry");

  setCursorSettings(10, 17, 2);
  display.print("Christmas");
  display.display();

  int shake_spuky = 40;

  for (size_t i = 0; i < 5; i++)
  {    
    scrollDisplay(0, 1, shake_spuky, RIGHT);
    scrollDisplay(0, 1, shake_spuky, LEFT);

    scrollDisplay(0, 1, shake_spuky, LEFT);
    scrollDisplay(0, 1, shake_spuky, RIGHT);
  }
  
  int shake_halloween = 26;

  for (size_t i = 0; i < 5; i++)
  {
    scrollDisplay(2, 3, shake_halloween, RIGHT);
    scrollDisplay(2, 3, shake_halloween, LEFT);

    scrollDisplay(2, 3, shake_halloween, LEFT);
    scrollDisplay(2, 3, shake_halloween, RIGHT);
  }

  for (size_t i = 0; i < 3; i++)
  {
    display.clearDisplay();
    display.stopscroll();
    display.display();
    delay(250);

    setCursorSettings(30, 0, 2);
    display.print("Let it");
    display.display();
    delay(150);
    
    setCursorSettings(42, 17, 2);
    display.print("Snow");
    display.display();
    delay(320);
  }
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
#define NUMFLAKES 14

#define SNOWFLAKE_HEIGHT 15
#define SNOWFLAKE_WIDTH 15

void letItSnow() {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - SNOWFLAKE_WIDTH, display.width());
    icons[f][YPOS]   = -SNOWFLAKE_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer
    printPixelart();

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      //display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
      if (f%2 == 0)
      {
        printSnowflake(icons[f][XPOS], icons[f][YPOS]);
      }
      else
      {
        printSnowflake2(icons[f][XPOS], icons[f][YPOS]);
      }
    }

    display.display(); // Show the display buffer on the screen
    delay(80);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - SNOWFLAKE_WIDTH, display.width());
        icons[f][YPOS]   = -SNOWFLAKE_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}
/* #endregion */

void oledMain()
{
  setupDisplay();
  printMessage();
  
  delay(500);
  display.clearDisplay();
  display.stopscroll();

  letItSnow();
    
}

void setup()
{
  Serial.begin(9600);
  
  oledMain();
}

void loop()
{
  
}
