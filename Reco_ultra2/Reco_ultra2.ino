const uint8_t ultra_pin[4] = {A0, A1, A2, A3};

bool enemy[2] = {false, false};

// Variables pour la machine à état
unsigned long startTime[2], pulseStart[2];  // Temps de départ et début de l'impulsion
bool waitingForPulse[2] = {false, false};
bool pulseStarted[2] = {false, false};

uint8_t j;
uint8_t echo, trigger;
unsigned long currentTime = micros();

void setup() {
  for (int k = 0; k < 2; k++) {
    pinMode(ultra_pin[k], OUTPUT); // TRIG en sortie
    pinMode(ultra_pin[k ^ 2], INPUT);  // ECHO en entrée
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
      if (currentTime - startTime[j] >= 5000) {
        Serial.println("Aucune mesure (timeout)");
        waitingForPulse[j] = false;
        
        enemy[j] = false;
      }

      // Vérifier si l'impulsion commence (ECHO passe à HIGH)
      if (!pulseStarted[j] && digitalRead(echo) == HIGH) {
        pulseStart[j] = currentTime; // Enregistrer le début de l'impulsion
        pulseStarted[j] = true;
      }

      // Vérifier si l'impulsion se termine (ECHO passe à LOW)
      if (pulseStarted[j] && digitalRead(echo) == LOW) {
        unsigned long duration = currentTime - pulseStart[j]; // Calculer la durée
        float distance = duration / 29.1 / 2; // Convertir en distance (cm)

        // Afficher la distance
        Serial.print("Distance : ");
        Serial.print(distance);
        Serial.println(" cm");

        waitingForPulse[j] = false; // Réinitialiser
        pulseStarted[j] = false;
        enemy[j] = distance < 70;
      }
  }
  return enemy[j];
}

void loop() {
  acq_ultra();
  delay(1000);
  j = 0;
}


