// Broches du capteur à ultrasons
const int trigPin = 12;  // Broche TRIG
const int echoPin = 11; // Broche ECHO

// Variables pour la machine à état
unsigned long startTime;  // Temps de départ
unsigned long pulseStart; // Début de l'impulsion
bool waitingForPulse = false; // État d'attente de l'impulsion
bool pulseStarted = false;    // État de début d'impulsion

void setup() {
  pinMode(trigPin, OUTPUT); // TRIG en sortie
  pinMode(echoPin, INPUT);  // ECHO en entrée
  Serial.begin(9600);       // Initialisation du port série
}

int cpt_sonor() {
  static unsigned long lastMeasure = 0;
  unsigned long currentTime = micros();

  if (currentTime - lastMeasure >= 10000) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    waitingForPulse = true; // Commencer à attendre l'impulsion
    startTime = currentTime; // Enregistrer le temps de départ
    lastMeasure = currentTime;
  }

  // Gérer l'impulsion sur ECHO
  if (waitingForPulse) {
    // Vérifier si le timeout (1s) est dépassé
    if (currentTime - startTime >= 1000000) {
      Serial.println("Aucune mesure (timeout)");
      waitingForPulse = false;
      return;
    }

    // Vérifier si l'impulsion commence (ECHO passe à HIGH)
    if (!pulseStarted && digitalRead(echoPin) == HIGH) {
      pulseStart = currentTime; // Enregistrer le début de l'impulsion
      pulseStarted = true;
    }

    // Vérifier si l'impulsion se termine (ECHO passe à LOW)
    if (pulseStarted && digitalRead(echoPin) == LOW) {
      unsigned long duration = currentTime - pulseStart; // Calculer la durée
      float distance = duration / 29.1 / 2; // Convertir en distance (cm)

      // Afficher la distance
      Serial.print("Distance : ");
      Serial.print(distance);
      Serial.println(" cm");

      waitingForPulse = false; // Réinitialiser
      pulseStarted = false;
    }
  }
}

void loop() {
  cpt_sonor();
}