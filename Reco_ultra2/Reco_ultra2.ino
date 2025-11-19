const uint8_t ultra_pin[4] = {8, 9, 10, 11};

bool enemy[2] = {false, false};

// Variables pour la machine à état
unsigned long startTime[2], pulseStart_val[2];  // Temps de départ et début de l'impulsion
bool waitingForPulse[2] = {false, false};
bool pulseStart_bool[2] = {false, false};


uint8_t j;
uint8_t echo, trigger;
unsigned long currentTime = micros();

//TestBench variables
unsigned long mess_start;

void setup() {
  for (int k = 0; k < 2; k++) {
    pinMode(ultra_pin[k], INPUT); // ECHO en entrée
    pinMode(ultra_pin[k ^ 2], OUTPUT);  // TRIG en sortie
  }
  Serial.begin(9600);       // Initialisation du port série
}

bool acq_ultra() {
  echo = ultra_pin[j];
  trigger = ultra_pin[j ^ 2];
  currentTime = micros();

  if (waitingForPulse[j] == false) { // envoyer impulsion
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    
    waitingForPulse[j] = true; // Commencer à attendre l'impulsion
    startTime[j] = currentTime; // Enregistrer le temps de départ

  } else {

    // Vérifier si le timeout (5ms) est dépassé
      if (currentTime - startTime[j] >= 10000) {
        waitingForPulse[j] = false;

      }

      // Vérifier si l'impulsion commence (ECHO passe à HIGH)
      if ((pulseStart_bool[j] ==  false) && (digitalRead(echo) == HIGH)) {
        
        pulseStart_val[j] = currentTime; // Enregistrer le début de l'impulsion
        pulseStart_bool[j] = true;
        
      }

      // Vérifier si l'impulsion se termine (ECHO passe à LOW)
      if (pulseStart_bool[j] && digitalRead(echo) == LOW) {
        unsigned long duration = currentTime - pulseStart_val[j]; // Calculer la durée
        float distance = duration / 29.1 / 2; // Convertir en distance (cm)

        // Afficher la distance

        waitingForPulse[j] = false; // Réinitialiser
        pulseStart_bool[j] = false;
        enemy[j] = distance < 30;
      }
  }
  return enemy[j];
}


void loop() {
  j = (j + 1) % 2;
  if (millis() - mess_start  > 500) {
    mess_start = millis();
    Serial.println(acq_ultra());
  } else {
    acq_ultra();
  }
}


