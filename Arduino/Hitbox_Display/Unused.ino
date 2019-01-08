float getPixCooX(int pix_) {
  // Return pixel X coordinate in millimeters
  int x_ = pix_ % COLS;
  x_ *= dwX;
  return x_;
}

float getPixCooY(int pix_) {
  // Return pixel Y coordinate in millimeters
  int y_ = ROWS - 1 - pix_ / COLS;
  y_ *= dhY;
  return y_;
}

//-----------------------------------------------------
//------------------ COLOR SHADES ---------------------
//-----------------------------------------------------
// See : https://learn.adafruit.com/multi-tasking-the-arduino-part-3/utility-functions

uint32_t getLerpColor(uint32_t color1, uint32_t color2, uint16_t totalSteps, uint16_t index) {
  uint8_t red_ = ((Red(color1) * (totalSteps - index)) + (Red(color2) * index)) / totalSteps;
  uint8_t green_ = ((Green(color1) * (totalSteps - index)) + (Green(color2) * index)) / totalSteps;
  uint8_t blue_ = ((Blue(color1) * (totalSteps - index)) + (Blue(color2) * index)) / totalSteps;

  uint32_t lerpCol_ = (red_ << 16) | (green_ << 8) | blue_ ;
  
  return lerpCol_ ;
}

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
  return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
  return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
  return color & 0xFF;
}
