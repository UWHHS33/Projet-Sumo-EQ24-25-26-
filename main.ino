

// ACQ

// capteurs laser
left_laser_pin 0; // broche 0 
right_laser_pin = 0;

laser_prop = 0;

// Capteurs à ultrason
left_ultra_pin = 0;
right_ultra_pin = 0;

// Capteurs de contraste
ctr_pin[4] = [0, 0, 0, 0];

ctr_thrsh = 0;



// TRT

uint8_t last_dir = 0;


trt_



// ACQ functions
float acq_laser(laser_pin) {
    return analogRead(laser_pin) * laser_prop;
}

float acq_ultra(ultra_pin) {
    // fait
}

bool acq_ctr(ctr_pin) {
    if (analogRead(ctr_pin) > ctr_thrsh) {
      return true;
    } else {
      return false;
    }
}

// TRT functions

trt_ctr_threat(ctr_pin)

// ACT functions

void act_engine(left_pow, right_pow) {
    left_engine(left_pow);
    right_engine
}


void setup() {
    pinMode(left_laser_pin, INPUT);
    pinMode(right_laser_pin, INPUT);
    pinMode(left_ultra_pin, INPUT);
    pinMode(right_ultra_pin, INPUT);

    pinMode(front_left_ctr_pin, INPUT);
    pinMode(front_left_ctr_pin, INPUT);
    pinMode(back_left_ctr_pin, INPUT);
    pinMode(back_right_ctr_pin, INPUT);


}

void loop() {
  // ACQ
    left_laser_dis = acq_laser(left_laser_pin);
    right_laser_dis = acq_laser(right_laser_pin);

    left_ultra_dis = acq_ultra(left_ultra_pin);
    right_ultra_dis = acq_ultra(right_ultra_pin);
    

    front_left_isOut = acq_ctr(ctr_pin)
    front_right_isOut = acq_ctr(ctr_pin)
    back_left_isOut = acq_ctr(ctr_pin)
    back_right_isOut= acq_ctr(ctr_pin)

}
















