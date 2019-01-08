//-----------------------------------------------------
//---------------- GET COLOR PIXELS -------------------
//-----------------------------------------------------
void setPixelRGBColorFromSerial(char buff_[20]) {
  // Get pixel and RGB color coordinate from serial message
  String msg_ = "";   // initialize string message variable

  // Get pixel index (3 digits)
  for (int i = 0; i < 3; i++) {
    msg_ += buff_[i];        // collect serial buffer characters into one string message
  }
  int pix_ = hexToDec(msg_); // convert hexadecimal pix index into decimal

  // Get red component (2 digits)
  msg_ = buff_[3];
  msg_ += buff_[4];
  int red_ = hexToDec(msg_);

  // Get green component (2 digits)
  msg_ = buff_[5];
  msg_ += buff_[6];
  int green_ = hexToDec(msg_);

  // Get blue component (2 digits)
  msg_ = buff_[7];
  msg_ += buff_[8];
  int blue_ = hexToDec(msg_);

  // Set pixel color
  setPixColor(pix_, (red_ << 16) | (green_ << 8) | blue_);

  // Reset save mode timer and turn off grid
  if(millis() - timerSaveScreen0 > delSavModeActiv){ 
    background(black);
  }
  timerSaveScreen0 = millis(); // reset save mode timer
  isGrid = false ;             // turn off grid
}

//-----------------------------------------------------
//----------------- GET COMMANDS ----------------------
//-----------------------------------------------------
void getSerialCommand(char buff_[20]) {
  // Get and use other incoming serial messages
  //!\\ number of char must be different from 10 (otherwise it s received as a led info)

  String msg_ = "";           // initialize string message variable
  for (int i = 0; i < bufIndex - 1; i++) {
    msg_ += (char)buff_[i];   // collect serial buffer characters into one string message
  }

  // Initialize command paramaters
  isGrid = false ;
  timerSaveScreen0 = millis(); // reset save mode timer

  // Get command and set new parameters
  if (msg_ == "connect") {
    isConnected = true;
    //Serial.println("connected");
  }
  if (msg_ == "disconnect") {
    background(black);
    isConnected = false;
    //Serial.println("disconnected");
  }
  if (msg_ == "turnoff") {
    background(black);
  }
  if (msg_ == "savemode") {
    timerSaveScreen0 = millis() - delSavModeActiv; // active save mode
  }
  if (msg_ == "dispgrid") {
    background(black);
    isGrid = true ;
  }
  if (msg_ == "endgame") {
    timerEndGame0 = millis();    // initialize end game animation timer
    switchToSaveMode = true ;
  }
  if(msg_ == "hit") {
    timerHit0 = millis();
    isHit = true ;
  }
}
