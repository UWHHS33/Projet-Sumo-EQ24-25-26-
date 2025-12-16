
const uint8_t ctr_pin[4] = {7, 8, 0, 0};
const uint8_t ultra_pin[4] = {2, 4, 3, 5};
const uint8_t laser_pin[4] = {A0, A1, 0, 0};

const uint8_t pins_array[3] = {
  7, 8, 0, 0, // ctr   : 0-3
  2, 4,       // ultra : 4-5
  A0, A1      // laser : 6-7
  }; 


const uint8_t trigger_array[8] = {
 0, 0, 0, 0,
 3, 5         // ultra trig: 4-5
 };

const uint8_t infra_pin = 0;

////////////// Processing variables
uint8_t i;
uint8_t imparity;
uint8_t last_imparity;
bool in;

// Debug
unsigned long mess_start = 0;



//------------------ACQ ----------



//////////// Ultrasonic

uint8_t pulse;
uint8_t trigger;
uint8_t pin;

bool enemy[2] = {false, false};
bool pulseStart_bool[2] = {false, false};
bool waitingForPulse[2] = {false, false};
unsigned long startTime[2] = {0, 0};
unsigned long pulseStart_val[2] = {0, 0};
unsigned long currentTime;


bool acq_ultra_pin() {
  echo = pins_array[i];
  trigger = trigger_array[i];
  currentTime = micros();



  if (waitingForPulse[imparity] == false) { // Send pulse

    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    waitingForPulse[imparity] = true; // wait for pulse 
    startTime[imparity] = currentTime; // Register send time

  } else {

    // Check for timeout
      if (currentTime - startTime[imparity] >= 5000) {
        waitingForPulse[imparity] = false;
        //enemy[imparity] = false;
      }

      // Check for pulse start (ECHO becomes HIGH)
      if ((pulseStart_bool[imparity] ==  false) && (digitalRead(echo) == HIGH)) {

        pulseStart_val[imparity] = currentTime; // Register pulse start
        pulseStart_bool[imparity] = true;

      }

      // Check pulse end (HIGH to LOW)
      if (pulseStart_bool[imparity] && digitalRead(echo) == LOW) {
        float distance = (currentTime - pulseStart_val[imparity]) / 29.1 / 2; // cm conversion

        waitingForPulse[imparity] = false; // 
        pulseStart_bool[imparity] = false; // Reset state variables
        enemy[imparity] = distance < 70;
      }
  }
  return enemy[imparity];
}

// end Ultrasonic





//////// Contrast

bool acq_ctr_pin() {
    return !digitalRead(pins_array[i]);
  }

//////// laser

bool acq_laser_pin() {
    uint16_t volt=map(analogRead(pins_array[i]), 0, 1023, 0, 5000);
    uint16_t cm=(21.61/(volt-0.1696))*1000;
    return cm >= 5 && cm <= 20;
  }

//////// Battery 
const uint8_t batt_thrsh = 0; // map(3.35, 0, 5, 0, 1024)
const uint8_t batt_pin = 0; 

bool acq_battery() {
  return true; //analogRead(batt_pin) >= map(3.35, 0, 5, 0, 1024);
}


/////// Infrared
bool start = false;
bool acq_infra() {
  if (!start) {
    start = true; // infra code here
  }

  return start; // if signal says ok
}



/////////// ALL



uint8_t len;
const bool (*acq_function[8])() = {
  acq_ctr_pin, acq_ctr_pin, acq_ctr_pin, acq_ctr_pin,
  acq_ultra_pin, acq_ultra_pin,
  acq_laser_pin, acq_laser_pin
  };



///////////////////// ACT

// pos = 0 : rotating forward
// neg = 1 : rotating backward

const uint8_t engine_matrix[15][4] = {
  {255, 64, 1, 1}, {64, 255, 1, 1}, {255, 64, 0, 0}, {64, 255, 0, 0},      // ctr   ; i: 0-3 ; [0-1: {0: neg; 1: neg}; 2-3: {0: pos; 1: pos}]
  {128, 255, 0, 0}, {255, 128, 0, 0},                                      // ultra ; i: 4-5 ; [0-1: {0: pos; 1: pos}]  
  {255, 255, 1, 0}, {255, 255, 0, 1},                                      // laser ; i: 6-7 ; [0  : {0: neg; 1: pos}; 1: {0: pos; 1: neg}]

  //mod 
  {255, 255, 1, 1}, {255, 255, 1, 1}, {255, 255, 0, 0}, {255, 255, 0, 0},   // ctr_mod ; i: 8-11 ;  [0-1: {0: neg; 1: neg}; 2-3: {0: pos; 1: pos}]
  {255, 255, 0, 0}          // ultra mod  [0-1: {0: pos; 1: pos}]           // ultra_mod ; i: 12         

  };

const uint8_t engine_en_pin[2] = {0, 0};  // [0: left; 1: right]
const uint8_t engine_dir_pin[2] = {0, 0}; // [0: left; 1: right]

// i: [0: ctr; 1: laser; 2: ultra; 3: ultra_mod; 4: ctr_mod]:

// 3: [ultra_mod: {0: same as 1: full steam ahead}] 
// 4: [ctr_mod: {0: same as 1: ?; 2: same as 3: full steam ahead}]

// ctr reminder: [0: front_left; 1: front_right; 2: back_left; 3: back_right] ctr: contrast


void order() {

  for (uint8_t k = 0; k < 2; k++) {
    analogWrite(engine_dir_pin[k], engine_matrix[i][k ^ 1]); // retrieve dir 2 positions on the right of selected parameter
    analogWrite(engine_en_pin[k], engine_matrix[i][k]);
  }
}


// Pin config
void setup() {
  for (uint8_t k = 0; k < 2; k++) {
    pinMode(ultra_pin[k ^ 2], OUTPUT);
    pinMode(engine_dir_pin[k], OUTPUT);
    pinMode(engine_en_pin[k], OUTPUT);
  }

  Serial.begin(9600);

}

// main loop
void loop() {
  
    // Behavior start
  for (i = 0; i < 8 ; i++) {  // i increments before break
    imparity = i & 1;
    if (acq_function[i]()) {
      break;
    } // if one of the sensors returns true
  }
  // Behavior end

  ////////////// Special case start
  if (millis() - mess_start >= 1000) {
    mess_start = millis();
    //Serial.print("in: "); 
    //Serial.println(in);
    //Serial.print("i = "); 
    //Serial.println(i);
    //Serial.print("imparity = ");
    //Serial.println(imparity);
    imparity = 0;
    Serial.println(acq_ultra_pin());
  }


  if (i < 8) { // check for special cases
    
    if (i < 4 && !imparity) { // if contrast returned true and i is pair
      i = i | 1;
      if (acq_ctr_pin()) { // ctr_mod assignment = if both left and right ctr pins return true
        i = i | 8;    // if both pins returned true switch engine behavior to ctr_mod
      }
    } else { // if i > 0

      last_imparity = imparity; // update opponent's last direction 

      if(i == 6) { // if ultra 0 returned true
        i = 7; // if left check right
        if (acq_ultra_pin()) { // if true
          i = 14; //switch to i = 3 = full speed ahead
        } else {
          i = 6; // else revert i to previous state
        }
      }
    }

  } else { // if enemy absent look for enemy
    i = last_imparity | 4; // = include parity with laser behavior
  }
  ////////////// Special case end


  if (acq_battery() && acq_infra()) {
    order(); 
  } else {
    analogWrite(engine_en_pin[0], 0);
    analogWrite(engine_en_pin[1], 0);
  }

  ////////////////// DEBUG
  //if (millis() - mess_start >= 1000) {
  //  mess_start = millis();
  //  Serial.print("i = ");
  //  Serial.println(i);
  //  Serial.print("imparity = ");
  //  Serial.println(imparity);
  //}

}
















