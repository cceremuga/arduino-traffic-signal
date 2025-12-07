const int SEGMENT_PINS[] = { 12, 2, 3, 4, 5, 6, 7 };
const int DIGIT_PINS[] = { 14, 16, 15 };
const byte SEGMENT_MAP[] = {
  0b0000001,
  0b1001111,
  0b0010010,
  0b0000110,
  0b1001100,
  0b0100100,
  0b0100000,
  0b0001111,
  0b0000000,
  0b0000100
};
const long DISPLAY_INTERVAL_MS = 3;

const int RED_PIN = 11;
const int YELLOW_PIN = 10;
const int GREEN_PIN = 9;
const long RED_LENGTH_MS = 35000;
const long YELLOW_LENGTH_MS = 3000;
const long GREEN_LENGTH_MS = 30000;

int displayValue = 0;
int currentDigit = 0;
unsigned long lastDigitTime = 0;

void setup() {
  // set up segments for all 3 digits
  for (int pin : SEGMENT_PINS) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  for (int pin : DIGIT_PINS) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // set up relay output pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  runPhase(RED_PIN, RED_LENGTH_MS);
  runPhase(GREEN_PIN, GREEN_LENGTH_MS);
  runPhase(YELLOW_PIN, YELLOW_LENGTH_MS);
}

void runPhase(int lightPin, long durationMs) {
  // deactivate all lights, then activate just the
  // one for this phase.
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(lightPin, HIGH);

  unsigned long startTime = millis();
  unsigned long endTime = startTime + durationMs;
  int lastSecond = -1;

  while (millis() < endTime) {
    long remainingMs = endTime - millis();
    int currentSecond = (remainingMs + 999) / 1000;

    if (currentSecond != lastSecond) {
      displayValue = currentSecond;
      lastSecond = currentSecond;
    }

    updateDisplayNonBlocking();
  }
}

// flashes digits on/off thousands of times per
// second in the runPhase loop, appearing solid to
// the eye
void updateDisplayNonBlocking() {
  unsigned long currentTime = millis();

  if (currentTime - lastDigitTime >= DISPLAY_INTERVAL_MS) {
    lastDigitTime = currentTime;

    int ones = displayValue % 10;
    int tens = (displayValue / 10) % 10;
    int hund = (displayValue / 100) % 10;

    currentDigit = (currentDigit + 1) % 3;

    if (currentDigit == 0) {
      if (hund == 0) {
        clearSegments();
      } else {
        driveDigit(0, hund);
      }
    } else if (currentDigit == 1) {
      if (hund == 0 && tens == 0) {
        clearSegments();
      } else {
        driveDigit(1, tens);
      }
    } else if (currentDigit == 2) {
      driveDigit(2, ones);
    }
  }
}

void clearSegments() {
  for (int pin : SEGMENT_PINS) {
    digitalWrite(pin, HIGH);
  }
}

void driveDigit(int digitIndex, int number) {
  for (int pin : DIGIT_PINS) {
    digitalWrite(pin, LOW);
  }

  clearSegments();

  if (number < 0 || number > 9) {
    return;
  }

  byte segPattern = SEGMENT_MAP[number];
  for (int i = 0; i < 7; i++) {
    if (!(segPattern & (1 << (6 - i)))) {
      digitalWrite(SEGMENT_PINS[i], LOW);
    }
  }

  digitalWrite(DIGIT_PINS[digitIndex], HIGH);
}