/* ---------------------------------------------
 *                OLIVER STOCKS
 *  MENG MECHANICAL WITH BIOMEDICAL ENGINEERING
 *      UNIVERSITY OF SOUTHAMPTON 2018/19
 * ---------------------------------------------
 * 
 * This script is written for GDP 47 -
 * fracture fixation device. Upload to a Teensy 
 * 3.2 microcontroller. Direct code queries to 
 * ojs1g14@soton.ac.uk
 * 
 * Overview
 * --------
 * 
 *  Motor Control:
 *    PI control of 6 DC motors (https://www.pololu.com/product/2291)
 *    is enabled using the functions extendStrutXX() 
 *    and contractStrutXX() [XX refers to the 
 *    board and motor number - for example, extending
 *    the strut connected to board 1 / motor 0 
 *    uses extendStrut10(<arg>).
 *    The control loop can be adjusted by 
 *    altering the "Control parameters" declared 
 *    below.
 *    
 *  Vibration Measurement:
 *    Three axis measurement of an accelerometer
 *  
 */


#include <i2c_t3.h>
#include <PololuQik.h>
#include <Encoder.h>


PololuQik2s9v1 qik(9,10,4);      // Declare qik object

// DECLARE ENCODER PINS
  Encoder MOTOR_01(14,15);       // Motor encoder feedback
  Encoder MOTOR_00(16,17);  
  Encoder MOTOR_11(11,12);  
  Encoder MOTOR_10(7,8);  
  Encoder MOTOR_21(2,3);
  Encoder MOTOR_20(5,6); 
//----------------------

// ENCODER READ VARIABLES
  long count;
  const long SETPOINT = 20000;    // User defined
//-----------------------

// CONTROL PARAMETERS
  const float Kp = 0.01;
  const float Ki = 0.1;
  long error;
  long SPEED;
  long sum_SPEED;
  long val;
//----------------------

// X Y AND Z BUFFERS
  int16_t xa;                     
  int16_t ya;
  int16_t za;
//--------------------------

// ARRAYS FOR CALILBRATION
  int16_t x_array[24];
  int16_t y_array[24];
  int16_t z_array[24];
// -------------------------------

// TIMER VARIABLES
  unsigned long t0;
  unsigned long t1;
// ---------------

// BUFFERS FOR READING ADXL 377 - DEPRECATED
  int scale = 200;
  float scaledX, scaledY, scaledZ;
  int analogX, analogY, analogZ;
// ----------------------------

// ADXL375 OFFSETS
  int8_t OFSX=2;
  int8_t OFSY=-1;
  int8_t OFSZ=-2;
// ----------------

String receivedText = "";       // Useful string for bluetooth
int i;                         // Useful counter

int8_t fiforeg;
int8_t zero = 0;
int8_t FIFO = 64;


void setup() {

  // SETUP COMMUNICATION LINES
      Wire.begin(I2C_MASTER,0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
      Serial.begin(38400);
      Serial1.begin(9600);        // Begin serial communication with HM17
      Serial2.begin(38400);       // Begin serial communication with driver boards
  //---------------------------
  
  // SET ENCODERS TO ZERO
      MOTOR_00.write(0);        
      MOTOR_01.write(0);
      MOTOR_10.write(0);
      MOTOR_11.write(0);
      MOTOR_20.write(0);
      MOTOR_21.write(0);
  //-----------------------
  
  // INITIALISE QIKS
      qik.init(38400);
  //----------------

  // HM 17 SETTINGS
      pinMode(23, INPUT);         // Set HM17 ENABLE pin to high impedance input
  //---------------

  // SEE IF TEENSY IS ALIVE
    pinMode(13, OUTPUT);          // Switch on Teensy LED
    digitalWrite(13, HIGH);
  //-----------------------

  // SETUP AND ENABLE ADXL
    setBW(13);                        // set bandwidth to 400 Hz
    clearBuffer();                    // set FIFO register to Bypass mode (clears buffer)
    setFIFOReg();                     // set FIFO register to fifo mode
    setOFSX();
    setOFSY();
    setOFSZ();
    enableADXL();                     // enable the measure bit
  // ----------------------
    
    delay(50);
}


void loop(){
  
//  t0 = millis();
//  
//  for (i=0;i<800;i++){
//    
  
  readXYZADXL();

//  }
//  
//    t1 = millis()-t0;           
//    Serial.println("Time elapsed: ");
//    Serial.println(t1);
//    delay(5000);
}


void setOFSX(){
  /*
   * Sets the X offset of the ADXL 375 prior to measurement.
   * For experimental work, the offset can be removed during 
   * postprocessing.
  */
  
  Wire.beginTransmission(0x53);
  Wire.write(0x1E);                       // Access PWR_CTL Register
  Wire.write(OFSX);
  Wire.endTransmission();
  delay(10);
}


void setOFSY(){
  /*
   * Sets the Y offset of the ADXL 375 prior to measurement.
   * For experimental work, the offset can be removed during 
   * postprocessing.
  */
  Wire.beginTransmission(0x53);
  Wire.write(0x1F);                       // Access PWR_CTL Register
  Wire.write(OFSY);
  Wire.endTransmission();
  delay(10);
}


void setOFSZ(){
  /*
   * Sets the Z offset of the ADXL 375 prior to measurement.
   * For experimental work, the offset can be removed during 
   * postprocessing.
  */
  Wire.beginTransmission(0x53);
  Wire.write(0x20);                       // Access PWR_CTL Register
  Wire.write(OFSZ);
  Wire.endTransmission();
  delay(10);
}


void measureVibration(unsigned long TIME){
  /* Main function for measuring vibration during test
   *  
   *  TIME (Unsigned long):  
   *    The length of time that the ADXL will transmit data
  */
  
    setBW(13);                        // set bandwidth to 400 Hz
    clearBuffer();                    // set FIFO register to Bypass mode (clears buffer)
    setFIFOReg();                     // set FIFO register to fifo mode
    enableADXL();                     // enable the measure bit
    
    t0 = millis();

    while (millis()<(t0+TIME)){
      readXYZADXL();
    }

    disableADXL();
    clearBuffer();
}


void clearBuffer(){
  
  Wire.beginTransmission(0x53);
  Wire.write(0x38);                     // Access FIFO_CTL Register
  Wire.write(zero);                     // Set to bypass mode => clears buffer
  Wire.endTransmission();
  delay(40);
  
}


void setFIFOReg(){
  
  Wire.beginTransmission(0x53);
  Wire.write(0x38);                       // Access FIFO_CTL Register
  Wire.write(FIFO);                         // 64 = 1000000 FIFO mode
  Wire.endTransmission();
  delay(40);
}


void calibrateOffsetADXL(int16_t*x_ar, int16_t*y_ar, int16_t*z_ar){
  
  setBW(13);                        // set bandwidth to 400 Hz
  clearBuffer();                    // set FIFO register to Bypass mode (clears buffer)
  //setFIFOReg();                     // set FIFO register to fifo mode
  enableADXL();                     // enable the measure bit

  for (i=0;i<5;i++){                  // read 36 bytes to ensure buffer is clear
    
    Wire.beginTransmission(0x53);     //start transmission to device
    Wire.write(0x32);                 //sends address to read from (WRITE appends 0)
    Wire.endTransmission();
    Wire.requestFrom(0x53, 6);        // Read 6 bytes

    if (Wire.available()==6){
      xa = ( Wire.read()| (Wire.read() << 8)); // X-axis value
      ya = ( Wire.read()| (Wire.read() << 8)); // Y-axis value
      za = ( Wire.read()| (Wire.read() << 8)); // Z-axis value
      
      x_ar[i] = xa;
      y_ar[i] = ya;
      z_ar[i] = za;
    }
  }

  t0 = millis();

  for (i=0;i<80;i++){                 // record actual values
    
    Wire.beginTransmission(0x53);     //start transmission to device
    Wire.write(0x32);                 //sends address to read from (WRITE appends 0)
    Wire.endTransmission();
    Wire.requestFrom(0x53, 6);  // Read 6 bytes

    while (Wire.available()==6){
      xa = ( Wire.read()| (Wire.read() << 8)); // X-axis value
      ya = ( Wire.read()| (Wire.read() << 8)); // Y-axis value
      za = ( Wire.read()| (Wire.read() << 8)); // Z-axis value
      
      x_ar[i] = xa;
      y_ar[i] = ya;
      z_ar[i] = za;
    }
  }
  
    t1 = millis()-t0;           

  for (i=0;i<80;i++){
    
    Serial.print(i);
    Serial.print("\t");
    Serial.print(x_ar[i]);
    Serial.print("\t");
    Serial.print(y_ar[i]);
    Serial.print("\t");
    Serial.println(z_ar[i]);
    
  }
  
  Serial.println("-----------------");
  Serial.println("Time elapsed: ");
  Serial.println(t1);

  delay(5000);
}


void selfTestOff(){
  
  Wire.beginTransmission(0x53);
  Wire.write(0x31);                       // Access PWR_CTL Register
  Wire.write(0xB);
  Wire.endTransmission();
  delay(40);
  
}


void readXYZADXL(){
  
  Wire.beginTransmission(0x53);     //start transmission to device
  Wire.write(0x32);                 //sends address to read from (WRITE appends 0)
  Wire.endTransmission();
  Wire.requestFrom(0x53, 6);        // Read 6 bytes

  if (Wire.available()==6){
    
    xa = ( Wire.read()| (Wire.read() << 8)); // X-axis value
    ya = ( Wire.read()| (Wire.read() << 8)); // Y-axis value
    za = ( Wire.read()| (Wire.read() << 8)); // Z-axis value
  
    Serial.print(xa);
    Serial.print(" ");
    Serial.print(ya);
    Serial.print(" ");
    Serial.println(za);
  }
  
  else{
    
    delay(1);
    
  }
  
  delayMicroseconds(980);
  
}


void readFIFOReg(){

  Wire.beginTransmission(0x53);     //start transmission to device
  Wire.write(0x38);                 // address the FIFO register
  Wire.endTransmission();
  Wire.requestFrom(0x53,1);         // Read 1 bytes
  
  if(Wire.available()){
      fiforeg = Wire.read();
      Serial.println(fiforeg, BIN);
  }
  
  else{
    Serial.println("no data");
  }
delay(40);
}


void readDataFormat(){
  // Useful for checking if self-test is on
  
  Wire.beginTransmission(0x53);     //start transmission to device
  Wire.write(0x31);                 // address the BW_RATE register
  Wire.endTransmission();
  Wire.requestFrom(0x53,1, true);   // Read 1 bytes
  Serial.println(Wire.read(), BIN);
  
}


void setBW(int8_t RATE){
  // set the bandwidth of the ADXL
  // a value 13 gives a bandwidth of 400 Hz
  
  Wire.beginTransmission(0x53);
  Wire.write(0x2C);                       // Access PWR_CTL Register
  Wire.write(RATE);
  Wire.endTransmission();
  delay(10);

}


void readBW(){
  
  // Returns the value of the BW_RATE register
  // 1010 = 50 Hz BW
  // 1101 = 400 Hz BW
  // (Values can be found on page 10 of the spec sheet)
  
  Wire.beginTransmission(0X53);     //start transmission to device
  Wire.write(0x2C);                 // address the BW_RATE register
  Wire.endTransmission();
  Wire.requestFrom(0x53,1);   // Read 1 bytes
  Serial.println(Wire.read(), BIN);
  delay(10);
  
}


void selfTest(){
  
  write_To(0x53, 0x31, 0);
  write_To(0x53, 0x31, 0xD7);
  write_To(0x53, 0x31, 0);

}


void printADXLID(){
  
  // Returns the ADXL ID of 0xE5 (11100101). Useful for debugging.
  
  Wire.beginTransmission(0x53);     //start transmission to device
  Wire.write(0x00);                 //sends address to read from
  Wire.endTransmission();           //end transmission
  
  Wire.beginTransmission(0x53);     //restart transmission
  Wire.requestFrom(0x53, 1);        // request 1 byte

  while(Wire.available())           //device may send less than requested
  { 
    
    Serial.println(Wire.read(), HEX);    // receive a byte
  }
  
  Wire.endTransmission();           //end transmission

}


void disableADXL(){
  // Clears the measure bit in the PWR_CTL register.
  // Worth calling this function after measurement

  Wire.beginTransmission(0x53);
  Wire.write(0x2D);                       // Access PWR_CTL Register
  Wire.write(0x04);                       // 00000100 - Bit D3 Low 
  Wire.endTransmission();
  delay(10);
  
}


void enableADXL() {
  // Enables the measure bit in the PWR_CTL register.
  // This function must be called prior to measurement (only needs to be
  // called once after power up).

  Wire.beginTransmission(0x53);
  Wire.write(0x2D);                       // Access PWR_CTL Register
  Wire.write(8);                          // 0000 1000 - Bit D3 High 
  Wire.endTransmission();
  delay(10);
  
}


void write_To(int device, byte address, byte val) {
  
  Wire.beginTransmission(device);     //start transmission to device 
  Wire.write(address);                // send register address
  Wire.write(val);                    // send value to write
  Wire.endTransmission();             //end transmission

}


void basicBluetooth(){
  
   if (Serial.available()) 
   Serial1.write(Serial.read());
  while(Serial1.available() > 0) { // While there is more to be read, keep reading.
    receivedText += (char)Serial1.read();
    delay(10);  
  }

  if(receivedText != "") Serial.println(receivedText);
  receivedText = "";

}


void contractStrut01(long CONTRACTION){
  
  MOTOR_01.write(0);
  count = abs(MOTOR_01.read()); 
  
  sum_SPEED = 0;
  
  while(count<CONTRACTION){

    error = CONTRACTION - count;              // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM1SpeedReverse(val, 0x00);             // Drive Motor   
  
    delay(50);
    count = abs(MOTOR_01.read());
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM1SpeedReverse(0x00, 0x00);              // Stop Motor
  
}


void contractStrut00(long CONTRACTION){
  
  MOTOR_00.write(0);
  count = abs(MOTOR_00.read()); 
  
  sum_SPEED = 0;
  
  while(count<CONTRACTION){

    error = CONTRACTION - count;              // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM0SpeedReverse(val, 0x00);             // Drive Motor   
  
    delay(50);
    count = abs(MOTOR_00.read());
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM0SpeedReverse(0x00, 0x00);              // Stop Motor
  
}


void contractStrut10(long CONTRACTION){
  
  MOTOR_10.write(0);
  count = abs(MOTOR_10.read()); 
  
  sum_SPEED = 0;
  
  while(count<CONTRACTION){

    error = CONTRACTION - count;              // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM0SpeedReverse(val, 0x01);             // Drive Motor   
  
    delay(50);
    count = abs(MOTOR_10.read());
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM0SpeedReverse(0x00, 0x01);              // Stop Motor
  
}


void contractStrut11(long CONTRACTION){
  
  MOTOR_11.write(0);
  count = abs(MOTOR_11.read()); 
  
  sum_SPEED = 0;
  
  while(count<CONTRACTION){

    error = CONTRACTION - count;              // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM1SpeedReverse(val, 0x01);             // Drive Motor   
  
    delay(50);
    count = abs(MOTOR_11.read());
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM1SpeedReverse(0x00, 0x01);              // Stop Motor
  
}


void contractStrut21(long CONTRACTION){
  
  MOTOR_21.write(0);
  count = abs(MOTOR_21.read()); 
  
  sum_SPEED = 0;
  
  while(count<CONTRACTION){

    error = CONTRACTION - count;              // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM1SpeedReverse(val, 0x02);             // Drive Motor   
  
    delay(50);
    count = abs(MOTOR_21.read());
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM1SpeedReverse(0x00, 0x02);              // Stop Motor
  
}


void contractStrut20(long CONTRACTION){
  
  MOTOR_20.write(0);
  count = abs(MOTOR_20.read()); 
  
  sum_SPEED = 0;
  
  while(count<CONTRACTION){

    error = CONTRACTION - count;              // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM0SpeedReverse(val, 0x02);             // Drive Motor   
  
    delay(50);
    count = abs(MOTOR_20.read());
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM0SpeedReverse(0x00, 0x02);              // Stop Motor
  
}


void extendStrut20(long EXTENSION){
  
  MOTOR_20.write(0);
  count = MOTOR_20.read(); 
  
  sum_SPEED = 0;
  
  while(count<EXTENSION){

    error = EXTENSION - count;                 // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM0SpeedForward(val, 0x02);             // Drive Motor   
  
    delay(50);
    count = MOTOR_20.read();
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM0SpeedForward(0x00, 0x02);              // Stop Motor
  
}


void extendStrut21(long EXTENSION){
  
  MOTOR_21.write(0);   
  count = MOTOR_21.read(); 
  
  sum_SPEED = 0;
  
  while(count<EXTENSION){

    error = SETPOINT - count;                 // Calculate error
        
    SPEED = Kp*error;                         // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
      
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM1SpeedForward(val, 0x02);             // Drive Motor   
  
    delay(50);
    count = MOTOR_21.read();
    Serial.println(val);
    Serial.println(error);
  }
  
  SetM1SpeedForward(0x00, 0x02);             // Stop Motor
  
}


void extendStrut00(long EXTENSION){
  
  MOTOR_00.write(0);    
  count = MOTOR_00.read(); 
  
  sum_SPEED = 0;
  
  while(count<EXTENSION){

    error = SETPOINT - count;       // Calculate error
        
    SPEED = Kp*error;          // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM0SpeedForward(val, 0x00);  // Drive Motor   
  
    delay(50);
    count = MOTOR_00.read(); 
    
  }
  
  SetM0SpeedForward(0x00, 0x00);    // Stop Motor
  
}


void extendStrut01(long EXTENSION){
  
  MOTOR_01.write(0);      //
  count = MOTOR_01.read();  //
  
  sum_SPEED = 0;
  
  while(count<EXTENSION){

    error = SETPOINT - count;       // Calculate error
        
    SPEED = Kp*error;          // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM1SpeedForward(val, 0x00);  // Drive Motor   
  
    delay(50);
    count = MOTOR_01.read();  //
  }
  
  SetM1SpeedForward(0x00, 0x00);    // Stop Motor
  
}


void extendStrut11(long EXTENSION){
  
  MOTOR_11.write(0);
  count = MOTOR_11.read();
  
  sum_SPEED = 0;
  
  while(count<EXTENSION){

    error = SETPOINT - count;       // Calculate error
        
    SPEED = Kp*error;          // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM1SpeedForward(val, 0x01);  // Drive Motor
  
    delay(50);
    count = MOTOR_11.read();
  }
  
  SetM1SpeedForward(0x00, 0x01);    // Stop Motor
  
}


void extendStrut10(long EXTENSION){
  
  MOTOR_10.write(0);
  count = MOTOR_10.read();
  
  sum_SPEED = 0;
  
  while(count<EXTENSION){

    error = SETPOINT - count;       // Calculate error
        
    SPEED = Kp*error;          // Proportional control

    sum_SPEED = (sum_SPEED+SPEED)*Ki + SPEED; // Integral Control


    if (sum_SPEED > 70){
      val = 70;
    }
    
    else if (sum_SPEED < 10){
      val = 10;
    }

    else {
      val = sum_SPEED;
    }
    
    SetM0SpeedForward(val, 0x01);  // Drive Motor
  
    delay(50);
    count = MOTOR_10.read();
  }
  
  SetM0SpeedForward(0x00, 0x01);    // Stop Motor
  
}


void SetM0SpeedReverse(byte SPEED, byte ADDRESS){
  
  // Use the pololu protocol to set the motor 0 speed.
  // Args:
  //  - SPEED: Depends on mode (check 'get PWM config')
  //  - ADDRESS:  0x00, 0x01 or 0x02
  //  - MOTOR: 0x08 (M0) or 0x0C (M1)
  
  Serial2.write(0xAA);
  Serial2.write(ADDRESS);    // Specify board address
  Serial2.write(0x0A);       // Select M0
  Serial2.write(SPEED);
}


void SetM1SpeedReverse(byte SPEED, byte ADDRESS){
  
  // Use the pololu protocol to set the motor 0 speed.
  // Args:
  //  - SPEED: Depends on mode (check 'get PWM config')
  //  - ADDRESS:  0x00, 0x01 or 0x02
  //  - MOTOR: 0x08 (M0) or 0x0C (M1)
  
  Serial2.write(0xAA);
  Serial2.write(ADDRESS);    // Specify board address
  Serial2.write(0x0E);       // Select M0
  Serial2.write(SPEED);
}


void SetM0SpeedForward(byte SPEED, byte ADDRESS){
  
  // Use the pololu protocol to set the motor 0 speed.
  // Args:
  //  - SPEED: Depends on mode (check 'get PWM config')
  //  - ADDRESS:  0x00, 0x01 or 0x02
  //  - MOTOR: 0x08 (M0) or 0x0C (M1)
  
  Serial2.write(0xAA);
  Serial2.write(ADDRESS);    // Specify board address
  Serial2.write(0x08);      // Select M0
  Serial2.write(SPEED);
}


void SetM1SpeedForward(byte SPEED, byte ADDRESS){
  
  // Use the pololu protocol to set the motor 1 speed.
  // Args:
  //  - SPEED: Depends on mode (check 'get PWM config')
  //  - ADDRESS:  0x00, 0x01 or 0x02
  
  Serial2.write(0xAA);
  Serial2.write(ADDRESS);    // Specify board address
  Serial2.write(0x0C);      // Select M0
  Serial2.write(SPEED);
}

byte GetAddress(byte ADDRESS){
  
  // Use the pololu protocol to get the board address.
  // This function is used only for debugging as the address
  // must be specified in order for the function to work.
  // Expected behaviour:
  //  - Returns board address if correctly specified
  //  - Returns 255 if incorrectly specified
  
  Serial2.write(0xAA);
  Serial2.write(ADDRESS);    // Specify board address
  Serial2.write(0x03);
  Serial2.write(0x00);
  
  byte vers = Serial2.read(); 
    
  return vers; 
}

