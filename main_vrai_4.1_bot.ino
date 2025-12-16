
const uint8_t ctr_pin[4] = {7, 8, 0, 0};
const uint8_t ultra_pin[4] = {2, 4, 3, 5};
const uint8_t laser_pin[4] = {A0, A1, 0, 0};
const uint8_t infra_pin = 0;

////////////// Processing variables
uint8_t i;
uint8_t j;
uint8_t last_j;
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
  uint8_t echo = ultra_pin[j];
  trigger = ultra_pin[j ^ 2];
  currentTime = micros();

  if (waitingForPulse[j] == false) { // Send pulse
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    waitingForPulse[j] = true; // wait for pulse 
    startTime[j] = currentTime; // Register send time

  } else {

    // Check for timeout (5 ms)
      if (currentTime - startTime[j] >= 5000) { 
        waitingForPulse[j] = false;
        //enemy[j] = false;
      }

      // Check for pulse start (ECHO becomes HIGH)
      if ((pulseStart_bool[j] ==  false) && (digitalRead(echo) == HIGH)) {

        pulseStart_val[j] = currentTime; // Register pulse start
        pulseStart_bool[j] = true;

      }

      // Check pulse end (HIGH to LOW)
      if (pulseStart_bool[j] && digitalRead(echo) == LOW) {
        float distance = (currentTime - pulseStart_val[j]) / 29.1 / 2; // cm conversion

        waitingForPulse[j] = false; // 
        pulseStart_bool[j] = false; // Reset state variables
        enemy[j] = distance < 70;
      }
  }
  return enemy[j];
}

// end Ultrasonic





//////// Contrast

bool acq_ctr_pin() {
    return !digitalRead(ctr_pin[j]);
  }

//////// laser

bool acq_laser_pin() {
    uint16_t volt=map(analogRead(laser_pin[j]), 0, 1023, 0, 5000);
    uint16_t cm=(21.61/(volt-0.1696))*1000;
    return cm >= 5 && cm <= 20;
  }

//////// Battery 
const uint8_t batt_thrsh = 0; // map(3.35, 0, 5, 0, 1024)
const uint8_t batt_pin = 0; 

bool acq_battery() {
  return analogRead(batt_pin) >= map(3.35, 0, 5, 0, 1024);
}


/////// Infrared
bool start = false; //change
bool acq_infra() {
  if (!start) {
    start = true; // infra code here
  }
  
  return start; // if signal says ok
}



/////////// ALL

const uint8_t pins_array[3] = {ctr_pin, laser_pin, ultra_pin}; 
// [0: ctr_array; 1: laser_array; 2: ultra_array]:  

const uint8_t len_pins_array[3] = {4, 2, 2}; ///////////////////////////////////////////////////////////////////////////////////////////////////DEBUG: ctr -> 4
uint8_t len;
bool (*acq_function[3])() = {acq_ctr_pin, acq_laser_pin, acq_ultra_pin};



///////////////////// ACT

// pos = 0 : rotating forward
// neg = 1 : rotating backward

const uint8_t en_engine_matrix[5][4][2] = {
  {{255, 64 }, {64, 255 }, {255, 64 }, {64, 255}},                  // ctr        [0-1: {0: neg; 1: neg}; 2-3: {0: pos; 1: pos}]
  {{255, 255}, {255, 255}, {0 , 0}, {0 , 0}},                       // laser      [0  : {0: neg; 1: pos}; 1: {0: pos; 1: neg}]   (2-3 unused)
  {{128, 255 }, {255, 128 }, {0 , 0}, {0 , 0}},                     // ultra      [0-1: {0: pos; 1: pos}]                        (2-3 unused)

  {{255, 255 }, {255, 255 }, {0 , 0}, {0 , 0}},                     // ultra mod　　［０－1： ｛０： pos； 1： pos｝］　　　　　　　　　　　　　
  {{255, 255}, {255, 255}, {255, 255}, {255, 255}}                  // ctr        [0-1: {0: neg; 1: neg}; 2-3: {0: pos; 1: pos}]
};

const uint8_t engine_en_pin[2] = {0, 0};  // [0: left; 1: right]
const uint8_t engine_dir_pin[2] = {0, 0}; // [0: left; 1: right]

// i: [0: ctr; 1: laser; 2: ultra; 3: ultra_mod; 4: ctr_mod]:

// 3: [ultra_mod: {0: same as 1: full steam ahead}] 
// 4: [ctr_mod: {0: same as 1: ?; 2: same as 3: full steam ahead}]

// ctr reminder: [0: front_left; 1: front_right; 2: back_left; 3: back_right] ctr: contrast
const uint8_t dir_engine_matrix[5][4][2] = {
  {{1, 1},  {1, 1}, {0, 0}, {0, 0}},                                  // ctr        [0-1: {0: neg; 1: neg}; 2-3: {0: pos; 1: pos}]
  {{ 1, 0}, {0, 1}, {0, 0}, {0, 0}},                                  // laser      [0  : {0: neg; 1: pos}; 1: {0: pos; 1: neg}]   (2-3 unused)
  {{ 0, 0}, {0, 0}, {0, 0}, {0, 0}},                                  // ultra      [0-1: {0: pos; 1: pos}]                        (2-3 unused)

  {{0, 0}, {0, 0}, {0, 0}, {0, 0}},                                   // ultra mod  [0-1: {0: pos; 1: pos}]                        (2-3 unused)
  {{1, 1}, {1, 1}, {0, 0}, {0, 0}}                                    // ctr        [0-1: {0: neg; 1: neg}; 2-3: {0: pos; 1: pos}]
};



void order() {

  for (uint8_t k = 0; k < 2; k++) {
    analogWrite(engine_dir_pin[k], dir_engine_matrix[i][j][k]); // retrieve dir 2 positions on the right of selected parameter
    analogWrite(engine_en_pin[k], en_engine_matrix[i][j][k]);
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
  in = true;
  for (i = 0; i < 2 && in; i++) {  // i increments before break
    len = len_pins_array[i];
    for (j = 0; j < len; j++) {

      if (acq_function[i]()) { // if one of the sensors returns true
        in = false;
        break;
      }
    }
  }
  // Behavior end

  ////////////// Special case start
  if (millis() - mess_start >= 1000) {
    mess_start = millis();
    //Serial.print("in: "); 
    //Serial.println(in);
    //Serial.print("i = "); 
    //Serial.println(i);
    //Serial.print("j = ");
    //Serial.println(j);
    j = 0;
    Serial.println(acq_ultra_pin());
  }
  if (in) { // = if all acq functions returned false

    i = 1; // = same as laser
    j = last_j; // assign direction to last direction in which enemy was detected
  } else { // else check for special cases

    i--;


    if ((i == 0) && !(j & 1)) { // if contrast returned true and j is pair
      j = j | 1; // if left check right
      if (acq_ctr_pin()) { // ctr_mod assignment = if both left and right ctr pins return true
        i = 4;    // if both pins returned true switch to special case
      } else {
          j = j ^ 1; // else revert j to previous state
        }
    } else { // if i > 0
      last_j = j; // update opponent's last position 

      if((i == 2) && (j == 0)) { // if ultra 0 returned true
        j = 1; // if right check left if left check right
        if (acq_ultra_pin()) { // if true
          i = 3; //switch to i = 3 = full speed ahead
        } else {
          j = 0; // else revert j to previous state
        }
      }
    }
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
  //  Serial.print("j = ");
  //  Serial.println(j);
  //}

}
















