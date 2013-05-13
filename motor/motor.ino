// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
 
// Define port pins used
const int kLed = 13;
const int kAuxPower = A2;
const int kPotInput = 1;
const int kBridgeA = 10;
const int kBridgeB = 9;

// define constants
const int kPotMid = 443;      // mid value

const int kDeadZone = 50;     // pot range that is off
const int kPWMMax = 255;      // max pwm value
const int kForwardMax = 255;  // max forward duty cycle (out of 255)
const int kReverseMax = 127;  // max reverse duty cycle

const int kMaxDelta = 1000;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(kLed, OUTPUT);  
  digitalWrite(kLed, LOW);
  
  digitalWrite(kAuxPower, HIGH);  
  pinMode(kAuxPower, OUTPUT);  
 
  digitalWrite(kBridgeA, HIGH);  
  pinMode(kBridgeA, OUTPUT);  

  digitalWrite(kBridgeB, HIGH);  
  pinMode(kBridgeB, OUTPUT);  

  // speed up the pwm frequency to 2 KHz from the default value
  TCCR1B &= 0b11111000;
  TCCR1B |= _BV(CS11);
  
  Serial.begin(9600);
  Serial.println("Full Bridge Motor Control");
}

// the loop routine runs over and over again forever:
void loop() {
  
  static int filterPWM = 0;
  
  
  //static long filterValue = (long)analogRead(kPotInput) << 2;
  
  //filterValue = filterValue - (filterValue >> 2) + analogRead(kPotInput);
  //int potValue = (filterValue >> 2);
  
  // get pot value
  int potValue =  kPotMid - analogRead(kPotInput);
  
  // range check
  if(potValue < -kPotMid) potValue = -kPotMid;
  if(potValue > kPotMid) potValue = kPotMid;
  
  // map pot value to demanded pwm value (reverse is negative)
  int demandPWM;
  if(potValue < -kDeadZone){
    demandPWM = map(potValue, -kPotMid, -kDeadZone, -kReverseMax, 0);
  } else if(potValue > kDeadZone){
    demandPWM = map(potValue, kDeadZone, kPotMid, 0, kForwardMax);
  } else {
    demandPWM = 0;
  }
  
  // update motor demand with slew rate limit
  int deltaPWM = demandPWM - filterPWM;
  if(deltaPWM > kMaxDelta) deltaPWM = kMaxDelta;
  if(deltaPWM < -kMaxDelta) deltaPWM = -kMaxDelta;
  filterPWM += deltaPWM;
  
  Serial.println(filterPWM);
  
  if(filterPWM > 0){
    digitalWrite(kBridgeA, HIGH);
    analogWrite(kBridgeB, kPWMMax - filterPWM);    
  } else if(filterPWM < 0){
    digitalWrite(kBridgeB, HIGH);
    analogWrite(kBridgeA, kPWMMax + filterPWM);
  } else {
    digitalWrite(kBridgeA, HIGH);
    digitalWrite(kBridgeB, HIGH);
  }
    
  // *** flash the light to show that we are alive... And slow down the loop
  digitalWrite(kLed, LOW);    // turn the LED off by making the voltage LOW
  delay(50);                 // wait for a 50
  digitalWrite(kLed, HIGH);   // turn the LED off by making the voltage LOW
  delay(50);                 // wait for a second
  
}
