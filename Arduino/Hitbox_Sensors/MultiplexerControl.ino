//--------------------------------
//----------- DIGITAL ------------
//--------------------------------
// DIGITAL MULTIPLEXER CONTROL FUNCTIONS
void clearRegisters(){
  //set all register pins to LOW
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

void setRegisterPin(int index, int value){
  //set an individual pin HIGH or LOW
  registers[index] = value;
}
 
void writeRegisters(){
  //Set and display registers
  //Only call AFTER all values are set how you would like (slow otherwise)
  digitalWrite(REGPIN, LOW); 
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(CLOCKPIN, LOW); 
    int val = registers[i]; 
    digitalWrite(DATAPIN, val);
    digitalWrite(CLOCKPIN, HIGH); 
  }
  digitalWrite(REGPIN, HIGH);
}

//--------------------------------
//------------ ANALOG ------------
//--------------------------------
// ANALOG MULTIPLEXER CONTROL FUNCTIONS
int readMux(int sigPin_, int channel_) {
  //read the value at the SIG pin
  for (int i = 0; i < 4; i ++) {
    digitalWrite(controlPin[i], muxChannel[channel_][i]); // set control pins configuration to match the channel you want
  }
  int val = analogRead(sigPin_); //return the value
  return val;
}
