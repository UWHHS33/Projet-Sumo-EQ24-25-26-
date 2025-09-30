

//////////////////////////Hardware

// Energie
const uint8_t batt_thrsh = 0;
int battery = 0;


// capteurs laser
uint8_t laser_pin[2] = [0, 0]; // [0: gauche; 1: droit]
bool laser[2] = [false, false];

const uint8_t laser_prop = 0;

// Capteurs à ultrason
uint8_t ultra_pin[2] = [0, 0];
bool ultra[2] = [false, false];

// Capteurs de contraste
uint8_t ctr_pin[4] = [0, 0, 0, 0];  // [0: front_left; 1: front_right; 2: back_left; 3: back_right] ctr: contrast

uint8_t ctr_thrsh = 0;

// Moteur

int engine_pin = [0, 0];  // [0: left; 1: right]



//////////////////////////Software






/////////////////ACQ 

bool acq_laser(laser_pin) {
    float res =  0;
    return res >= 5 && res <= 70;
  }


bool acq_ultra(ultra_pin) {
    float res; // codé par gab
    return res >= 2 && res <= 70;
  }

bool acq_ctr(ctr_pin) {
    return analogRead(ctr_pin) > ctr_thrsh;
  } 

float acq(battery)
    return analogRead(battery) < battery_thrsh;

/////////////////// TRT 

uint8_t last_dir = 0;

bool pass = true;

const int engine_vel[4] = [0, 0, 0, 0]; // [0: none 1: slow; 2: medium; 3: max] vel: velocity

int engine_vel_i[2] = [0, 0]; // [0: left; 1: right]

int engine_pow[2]= [0, 0];  // [0: left; 1: right]

int engine_var_i[2] = [0, 0]; // [0: left engine; 1: right engine]

int engine_var[2] = [[0, 127],  [127, 0]]; // [0: left; 1: right]

///////////////////// ACT


engine_vel_i[0] = 3;
engine_vel_i[1] = 3;
engine_pow[0] = 1;


void engine(pin_dir, PWM) {};

// Pin config
void setup() {
  
    for (uint8_t i = 0; i < 4; i++) {
      if (i < 2) {
        pinMode(laser_pin[i], INPUT);
        pinMode(ultra_pin[i], INPUT);
      }
      pinMode(ctr_pin[i], INPUT);
    }
}


// main loop
void loop() {
  // sensor acquisition
    if (!(tension < batt_thrsh)) {
      for (uint8_t i = 0; i < 4; i++) {
        if acq_ctr(ctr_pin[i]) {
          // choose i parameters
          pass = false;
          break
        }
      if  (!pass) {

      }
      if (acq_laser[0]) {
        
      }
    else {
      
    }

    



}
















