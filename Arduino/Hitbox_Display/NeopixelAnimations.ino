void background(int c_) {
  // Set all pixels the same color
  for (int i = 0; i < nPix; i++) {
    setPixColor(i, c_);
  }
}

//-----------------------------------------------------
//------------------ ANIMATIONS -----------------------
//-----------------------------------------------------
void saveScreenAnimation(int col_) {
  background(black);
  int dStroke_ = 50;
  float angle_ = PI * cos(millis() / 2000.);

  drawThickLine(0, widthPannel / 2., (cos(millis() / 666.) + 1) * heightPannel / 2., 10, col_);
  drawThickLine(0, widthPannel / 2., (cos(millis() / 444.) + 1) * heightPannel / 2., 10, col_);
  drawThickLine(PI / 2, (cos(millis() / 666.) + 1) * widthPannel / 2., heightPannel / 2., 20, col_);
  drawThickLine(PI / 2, (cos(millis() / 555.) + 1) * widthPannel / 2., heightPannel / 2., 20, col_);
}

void endGameAnimation(int col_) {
  background(black);
  float coef_ = (millis() - timerEndGame0) / ((float)timeEndGame);
  coef_ = map(coef_, 0, 1, -0.1, 1.1);
  
  // Horizontal lines going up
  drawThickLine(0, widthPannel / 2., coef_ * heightPannel          , 10,   col_);
  drawThickLine(0, widthPannel / 2., coef_ * heightPannel - 2 * dhY  , 10, col_);
  drawThickLine(0, widthPannel / 2., coef_ * heightPannel - 4 * dhY  , 10, col_);

  // Horizontal lines going down
  drawThickLine(0, widthPannel / 2., heightPannel * (1 - coef_)          , 10, col_);
  drawThickLine(0, widthPannel / 2., heightPannel * (1 - coef_) + 2 * dhY  , 10, col_);
  drawThickLine(0, widthPannel / 2., heightPannel * (1 - coef_) + 4 * dhY  , 10, col_);
}

void hitDisplay(int col_) {
  if (millis() - timerHit0 < timeHitDisplay) {
    for (int i = COLS / 2 - 6; i < COLS / 2 + 6; i++) {
      int pix_ = getPixIndex(i * dwX, dhY);
      setPixColor(pix_, col_);
    }
    for (int i = COLS / 2 - 6 ; i < COLS / 2 + 6 ; i++) {
      int pix_ = getPixIndex(i * dwX, (ROWS - 2) * dhY);
      setPixColor(pix_, col_);
    }
  }
  else {
    if (isHit) {
      // turn off hit display only once after hit is over
      for (int i = COLS / 2 - 6; i < COLS / 2 + 6; i++) {
        int pix_ = getPixIndex(i * dwX, dhY); // sedonc led strip
        setPixColor(pix_, black);
      }
      for (int i = COLS / 2 - 6; i < COLS / 2 + 6; i++) {
        int pix_ = getPixIndex(i * dwX, (ROWS - 2) * dhY);
        setPixColor(pix_, black);
      }
      isHit = false ;
    }
  }
}

void grid(int c_) {
  background(black);
  drawThickLine(PI / 2, (cos(millis() / 666.) + 1) * widthPannel / 2., heightPannel / 2., 20, c_);
  drawThickLine(PI / 2, (cos(millis() / 555.) + 1) * widthPannel / 2., heightPannel / 2., 20, c_);

  for (int i = 0; i <= widthPannel; i += dwX) {
    for (int j = 0; j <= heightPannel; j += 2 * dhY) {
      setPixColorXY(i, j, c_);
    }
  }
}

//-----------------------------------------------------
//---------------- GEOMETRIC SHAPES -------------------
//-----------------------------------------------------
void drawFillCircle(int cx_, int cy_, int r_,  int c_ ) {
  // Draw a circle: center (cx_, cy_) radius (r_) color (c_)
  for (int i = dwX * int((cx_ - r_) / float(dwX)); i <= dwX * int((cx_ + r_) / float(dwX)) + 1; i += dwX) {
    for (int j = dhY * int((cy_ - r_) / float(dhY)); j <= dhY * int((cy_ + r_) / float(dhY)) + 1; j += dhY) {
      if (pow(i - cx_, 2) + pow(j - cy_, 2) <= pow(r_, 2)) {
        setPixColorXY(i, j, c_);
      }
    }
  }
}

void drawThinCircle(int cx_, int cy_, int r_,  int strokeWeight_, int c_ ) {
  // Draw a circle: center (cx_, cy_) radius (r_) color (c_) stroke weight (strokeWeight_)
  for (int i = dwX * int((cx_ - r_) / float(dwX)); i <= dwX * int((cx_ + r_) / float(dwX)) + 1; i += dwX) {
    for (int j = dhY * int((cy_ - r_) / float(dhY)); j <= dhY * int((cy_ + r_) / float(dhY)) + 1; j += dhY) {
      if (pow(i - cx_, 2) + pow(j - cy_, 2) - pow(r_, 2) <= strokeWeight_) {
        setPixColorXY(i, j, c_);
      }
    }
  }
}

void drawSplitScreen(float angle_, int shiftX0_, int shiftY0_, int c_) {
  // Split the screen with a line (inclined) and fill part above the line (use angle+=PI to fill part under the line)
  if (abs(cos(angle_)) > 0.001) {
    // Get ax+b=y from angle
    float a_ = tan(angle_);
    for (int i = -shiftX0_; i < widthPannel - shiftX0_; i += dwX) {
      if (cos(angle_) > 0) {
        for (int j = a_ * i; j < heightPannel - shiftY0_; j += dhY) {
          setPixColorXY(i + shiftX0_, j + shiftY0_, c_);
        }
      }
      else {
        for (int j = a_ * i; j >= -shiftY0_; j -= dhY) {
          setPixColorXY(i + shiftX0_, j + shiftY0_, c_);
        }
      }
    }
  }
  else {
    if (sin(angle_) > 0) {
      for (int i = 0; i < shiftX0_; i += dwX) {
        for (int j = 0; j < heightPannel; j += dhY) {
          setPixColorXY(i, j, c_);
        }
      }
    }
    else {
      for (int i = shiftX0_; i < widthPannel; i += dwX) {
        for (int j = 0; j < heightPannel; j += dhY) {
          setPixColorXY(i, j, c_);
        }
      }
    }
  }
}

void drawThickLine(float angle_, int shiftX0_, int shiftY0_, int strokeWeight_, int c_) {
  // Draw an inclined line, center by shiftX0_ and shiftY0, with a thickness of strokeWeight_
  if (abs(cos(angle_)) > 0.001) {
    // Get ax+b=y from angle
    float a_ = tan(angle_);
    for (int i = -shiftX0_; i < widthPannel - shiftX0_; i += dwX) {
      for (int j = a_ * i - int(strokeWeight_ / (2.*cos(angle_))); j < a_ * i + int(strokeWeight_ / (2.*cos(angle_))); j++) {
        setPixColorXY(i + shiftX0_, j + shiftY0_, c_);
      }
    }
  }
  else {
    for (int i = shiftX0_ - int(strokeWeight_ / 2.); i < shiftX0_ + int(strokeWeight_ / 2.); i += dwX) {
      for (int j = 0; j < heightPannel; j += dhY) {
        setPixColorXY(i, j, c_);
      }
    }
  }
}

void drawThinLine(float angle_, int shiftX0_, int shiftY0_, int c_) {
  // Draw an inclined line, center by shiftX0_ and shiftY0, with a thickness of one pixel
  if (abs(cos(angle_)) > 0.001) {
    // Get ax+b=y from angle
    float a_ = tan(angle_);
    for (int i = -shiftX0_; i < widthPannel - shiftX0_; i += dwX) {
      setPixColorXY(i + shiftX0_, a_ * i + shiftY0_, c_);
    }
  }
  else {
    for (int j = 0; j < heightPannel; j += dhY) {
      setPixColorXY(shiftX0_, j, c_);
    }
  }
}
