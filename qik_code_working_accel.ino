#include <PololuQik.h>
#include <Encoder.h>
#include <Wire.h>




// DECLARE ENCODER PINS
  Encoder MOTOR_01(14,15);
  Encoder MOTOR_00(16,17);  
  Encoder MOTOR_11(11,12);  
  Encoder MOTOR_10(7,8);  
  Encoder MOTOR_21(2,3);
  Encoder MOTOR_20(5,6); 
//----------------------

// DECLARE ENCODER READ VARIABLES
  long count;
  const long SETPOINT = 20000;    // User defined
//-------------------------------

  PololuQik2s9v1 qik(9,10,4);      // Declare qik object

// DECLARE PI PARAMETERS
  const float Kp = 0.01;
  const float Ki = 0.1;
  long error;
  long SPEED;
  long sum_SPEED;
  long val;
//----------------------

String receivedText = "";       // String for bluetooth
long num;
byte xdat;

// Declare arrays to store accelerometer data
  byte X0DATA[8000];               
  byte Y0DATA[8000];
  byte Z0DATA[8000];
//-------------------------------------------

byte id;
int i;                         // Useful counter


void setup() {

  // SETUP COMMUNICATION LINES
      Wire.begin();               // Begin i2c for ADXL375
      Wire.setSDA(18);
      Wire.setSCL(19);
      
      Serial.begin(9600);
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

  qik.init(38400);            // Initialise qik object

  pinMode(23, INPUT);         // Set HM17 ENABLE input to high impedance input
  
  pinMode(13, OUTPUT);        // Switch on Teensy LED
  digitalWrite(13, HIGH);
  
  delay(50);
}


void loop(){

  byte databit;
  printVibrationData(1, databit, 1);

}


void printVibrationData(int num, byte dat, int i){
  
  setSampleRate();
  enableADXL();
  
  // X SAMPLE --------------------------------------------------
    Wire.beginTransmission(0x53);     //start transmission to device 
    Wire.write(0x32);                 //sends address to read from
    Wire.endTransmission();           //end transmission
  
    Wire.beginTransmission(0x53);     //restart transmission
    Wire.requestFrom(0x53, num);      // request num bytes
  
    
    if(Wire.available())           //device may send less than requested
    { 
      dat = Wire.read();              // receive a byte
      Serial.print(dat, DEC);
      Serial.print("\t");
    }
    else{
      Serial.print("NAN");
      Serial.print("\t");
    }
    
    Wire.endTransmission();           //end transmission
  //---------------------------------------------------------------

  //Y SAMPLE ------------------------------------------------------
    Wire.beginTransmission(0x53);     //start transmission to device 
    Wire.write(0x34);                 //sends address to read from
    Wire.endTransmission();           //end transmission
  
    Wire.beginTransmission(0x53);     //restart transmission
    Wire.requestFrom(0x53, num);      // request num bytes
  
    
    if(Wire.available())           //device may send less than requested
    { 
      dat = Wire.read();              // receive a byte
      Serial.print(dat, DEC);
      Serial.print("\t");
    }
    else{
      Serial.print("NAN");
      Serial.print("\t");
    }
    
    Wire.endTransmission(); //end transmission
  //----------------------------------------------------------------

  // Z SAMPLE ------------------------------------------------------
    Wire.beginTransmission(0x53);     //start transmission to device 
    Wire.write(0x36);                 //sends address to read from
    Wire.endTransmission();           //end transmission
  
    Wire.beginTransmission(0x53);     //restart transmission
    Wire.requestFrom(0x53, num);      // request num bytes
  
    
    if(Wire.available())              //device may send less than requested
    { 
      dat = Wire.read();              // receive a byte
      Serial.println(dat, DEC);
    }
    else{
      Serial.println("NAN");
    }
    
    Wire.endTransmission();           //end transmission
  //----------------------------------------------------------------
}



void logVibration(){

  setSampleRate();
  enableADXL();
  
  for (i=0;i<8000;i++){
    readFromADXL(0x32, 1, X0DATA, i);       // Read X0 least significant byte
    readFromADXL(0x34, 1, Y0DATA, i);       // Read Y0 least significant byte
    readFromADXL(0x36, 1, Z0DATA, i);       // Read Z0 least significant byte
  }
  
}


void printADXLID(){
  
  readFromADXL(0x00, 1, &id, 1);
  Serial.println(id, DEC);

}


void enableADXL() {
  
  write_To(0x53, 0x2D, 0);      
  write_To(0x53, 0x2D, 16);             // 16 refers to D3 (measure) bit
  write_To(0x53, 0x2D, 8);              // does this set the bit?

}


void setSampleRate(){
  
  write_To(0x53,0x2C,0x0F);            // 3200Hz

}


void write_To(int device, byte address, byte val) {
  
  Wire.beginTransmission(device);     //start transmission to device 
  Wire.write(address);                // send register address
  Wire.write(val);                    // send value to write
  Wire.endTransmission();             //end transmission

}


void readFromADXL(byte address, int num, byte buff[], int i) {
  // If not used in a loop, set i to 1
  
  Wire.beginTransmission(0x53);     //start transmission to device 
  Wire.write(address);              //sends address to read from
  Wire.endTransmission();           //end transmission

  Wire.beginTransmission(0x53);     //restart transmission
  Wire.requestFrom(0x53, num);      // request num bytes

  
  while(Wire.available())           //device may send less than requested
  { 
    
    buff[i] = Wire.read();             // receive a byte
  }
  Wire.endTransmission(); //end transmission
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

