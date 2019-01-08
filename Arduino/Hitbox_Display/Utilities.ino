//----------------------------------------------------
//------------------- UTILITIES ----------------------
//----------------------------------------------------
unsigned int hexToDec(String hexString) {
  // Convert a hexadecimal string number into a decimal integer
  // See : https://github.com/benrugg/Arduino-Hex-Decimal-Conversion/blob/master/hex_dec.ino

  unsigned int decValue = 0; // initialize return value
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}
