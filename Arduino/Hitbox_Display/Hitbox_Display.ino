#include <OctoWS2811.h> // reference: http://www.pjrc.com/teensy/td_libs_OctoWS2811.html

//--------------------------------
//---------- LED PANNEL ----------
//--------------------------------
// Leds configuration
#define COLS 60                // total number of pixel columns
#define ROWS 30                // total number of pixel rows
#define nStrip 5               // number of led strips ensembles (one ensemble per teensy pin, see library examples for pin connections)
#define nPix COLS*ROWS         // total number of leds
#define nPixStrip nPix/nStrip  // number of pixel per strip
int colorStrip[COLS * ROWS];   // array to store current pixel color values

// Pannel dimensions
#define dwX 16                 // width between pixels in millimeters
#define dhY 35                 // height between pixels in millimeters
#define widthPannel COLS*dwX   // width of the led pannel in millimeters
#define heightPannel ROWS*dhY  // height of the led pannel in millimeters

//--------------------------------
//--------- OCTOWS2811 -----------
//--------------------------------
// Define led strips to the led control library, to see pin connection check intro from File/Examples/OctoWS2811/BasicTest
DMAMEM int displayMemory[nPixStrip * 6];
int drawingMemory[nPixStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(nPixStrip, displayMemory, drawingMemory, config);

//--------------------------------
//--------- UTILITIES ------------
//--------------------------------
// Led display variables
boolean isConnected = false;    // if false led pannel is not refreshed (avoid serial connection issues)
long timerLedShow0;             // time reference to refresh led pannel display
int timeScreenRefresh = 10;           // time in millisecond to refresh led pannel

// Serial buffer
char buff[20];                  // buffer to store incoming messages
int bufIndex = 0;               // index of the last received character

//--------------------------------
//------ COLOR N ANIMATION -------
//--------------------------------
// Led colors
#define BRIGHTNESS 255
#define black   ((0 << 16) | (0 << 8) | 0)        // ((RED << 16) | (GREEN << 8) | BLUE)
#define white   ((255 << 16) | (255 << 8) | 255)
#define green   ((0 << 16) | (200 << 8) | 30)
#define yellow  ((200 << 16) | (175 << 8) | 0)
#define magenta ((216 << 16) | (0 << 8) | 216)
int myColor = green;

// Save screen animation mode
long timerSaveScreen0;         // time reference to launch save screen
int delSavModeActiv = 10000;   // delay in millis before to launch save screen

// Grid animation mode
boolean isGrid = false ;

// Hit animation
int timeHitDisplay = 100;      // duration of the hit animation
long timerHit0;
boolean isHit = false;

// End game animation
int timeEndGame = 1200;        // duration of the animation in millisecond
long timerEndGame0;
boolean switchToSaveMode = false ; // if true, switch to save screen after end game animation

void setup() {
  // Initialize serial communication
  Serial.begin(38400);
  timerSaveScreen0 = millis();

  // Initialize led strips
  leds.begin();
  leds.show();
  timerLedShow0 = millis();    // initialize display timer
}

void loop() {
  //--------------------------------
  //----- GET SERIAL MESSAGES ------
  //--------------------------------
  // Get serial messages
  if (Serial.available() > 0) {
    // RECEIVE SERIAL MESSAGE
    bufIndex = 0;
    do
    {
      buff[bufIndex] = (char)Serial.read(); // Collect incoming serial character into char array
      if (buff[bufIndex] != -1) {
        bufIndex++;
      }
    } while (buff[bufIndex - 1] != '_');    // Each message ends with '_' character

    // USE SERIAL MESSAGE
    if (bufIndex == 10) {
      setPixelRGBColorFromSerial(buff);     // Get RGB color for a specific pixel
    }
    else {
      getSerialCommand(buff);               // Get other messages for extra command
    }
  }

  //--------------------------------
  //--------- ANIMATIONS -----------
  //--------------------------------
  // COLOR ANIMATION
  float speed_ = millis() / 2000;
  myColor = getColorReference(cos(speed_) * 255, sin(speed_) * 255, (1 - cos(speed_)) * 255);

  // SAVE SCREEN ANIMATION
  if (millis() - timerSaveScreen0 > delSavModeActiv) {
    isGrid = false ; // turn off grid
    saveScreenAnimation(myColor);
  }

  // GRID ANIMATION
  if (isGrid) {
    grid(myColor);
  }

  // END GAME ANIMATION
  if (millis() - timerEndGame0 < timeEndGame) {
    endGameAnimation(myColor);
  }
  else {
    // Switch to save mode after end game
    if (switchToSaveMode) {
      background(black);
      switchToSaveMode = false ;
    }
  }

  // HIT DISPLAY
  hitDisplay(magenta);

  //--------------------------------
  //----------- DISPLAY ------------
  //--------------------------------
  // Display new led pattern
  if (millis() - timerLedShow0 > timeScreenRefresh) {
    leds.show();
    timerLedShow0 = millis();
  }
}
