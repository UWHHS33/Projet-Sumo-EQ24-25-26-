

//////////////////////////Hardware

// Energie
const uint8_t batt_thrsh = 0;
int battery = 0;


// capteurs laser
const uint8_t laser_pin[2] = {0, 0}; // [0: gauche; 1: droit]
const uint8_t laser_prop = 0;

// Capteurs à ultrason
const uint8_t ultra_pin[2] = {0, 0};

// Capteurs de contraste
const uint8_t ctr_pin[4] = {0, 0, 0, 0};  // [0: front_left; 1: front_right; 2: back_left; 3: back_right] ctr: contrast
uint8_t ctr_thrsh = 0;


// Moteur
// ?????????????


//////////////////////////Software







/////////////////ACQ 




bool acq_ultra_pin(uint8_t ultra_pin) {
    float res; // codé par gab
    return res >= 2 && res <= 70;
  }

bool acq_ctr_pin() {
    float res; // pas codé
    return res > ctr_thrsh;
  }
bool acq_laser_pin(uint8_t laser_pin) {
    float res; // codé par gab
    return res >= 5 && res <= 70;
  }

bool acq_battery() {
    float res; // pas codé
    return res >= batt_thrsh;
}
bool acq_infra() {
    //pas codé
    return true; // if signal says ok
}

const uint8_t len_pins_array[3] = {4, 2, 2};
uint8_t len;

const uint8_t pins_array[3] = {ctr_pin, laser_pin, ultra_pin};

bool (*acq_function[])() = {acq_ctr_pin, acq_laser_pin, acq_ultra_pin};
uint8_t i;
uint8_t j;
uint8_t last_j;

bool in;




///////////// engine management

float engine_matrix[5] = {{0, 0, 0, 0},{0, 0},{0, 0}, {0, 0}, {0, 0, 0, 0}} // [0: ctr; 1: laser; 2: ultra; 3(ultra_mod): {0: same as 1: full steam ahead}, 4(ctr_mod):{0: same as 1: ?; 2: same as 3: full steam ahead}]


///////////////////// ACT


void engine(uint8_t pin_dir, uint8_t PWM) {
  continue;
};











// Pin config
void setup() {
  
    for (uint8_t iter = 0; iter < 4; iter++) {
      if (i < 2) {
        pinMode(laser_pin[i], INPUT);
        pinMode(ultra_pin[i], INPUT);
      }
      pinMode(ctr_pin[i], INPUT);
    }
}

// main loop
void loop() {

    in = true;
    for (i = 0; i < 3 && in; i++) {

      len = len_pins_array[i];
      for (j = 0; j < len; j++) {

        if (acq_function[i](acq_pin[i][j])) {
          in = false;
          break;
        }
      }
    }

    if (acq_battery() && acq_infra()) { // je sais pas si la telecommande emet en permanence

      if (in) {

        i = 1;
        j = last_j;

      } else {
        if (i == 0) { //? hmm

          if (acq_ctr_pin(ctr_pin[j ^ 1])) { // remember extremely devious reasoning
            i = 4;
          }

        } else {
          last_j = j;

          if(i == 2) {
            if (acq_ultra_pin(ultra_pin[j ^ 1]) {
              i++;
            }
          }

        }
      }

      engine(engine_matrix[i][j]);
    }

}

