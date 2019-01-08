//-----------------------------------------------------
//---------------- PIXEL MANAGER ----------------------
//-----------------------------------------------------
void setNeoPixColor(int pix_, int col_) {
  leds.setPixel(pix_, col_);
}

void displayPix(int x_, int y_, int c_) {
  // Display one pixel based on XY space coordinate (in millimeters)
  int pix_ = getPixIndex(x_, y_);
  setNeoPixColor(pix_, c_);
}

int getPixIndex(int x_, int y_) {
  // Calculate the pixel index into the whole strip from its coordinates in millimeters
  x_ /= dwX; // get X index
  y_ /= dhY; // get Y index

  if (y_ % 2 != 0) {
    x_ = COLS - x_ - 1; // X direction varies from one row to another since the grid is made as a "snake"
  }
  y_ = ROWS - y_ - 1;
  
  int pix_ = y_ * COLS + x_;
  return pix_;
}

int getColorReference(int red_, int green_, int blue_){
  return (red_ << 16) | (green_ << 8) | blue_;
}
