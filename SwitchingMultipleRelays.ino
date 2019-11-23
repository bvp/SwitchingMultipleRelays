const byte BUTTONS[] = {2, 3, 4, 5};
const byte RELAYS[] = {8, 9, 10, 11};

byte state = 0b00001111; // the current state of the output pins
byte reading = 0b00000000; // the current reading from the input pins
byte previous = 0b00000000; // the previous reading from the input pins
int n = sizeof(RELAYS) / sizeof(RELAYS[0]);

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup() {
  for (int i = 0; i < n; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(RELAYS[i], OUTPUT);
  }
}

void readAndSwitch(int btn, int rly) {
  bitWrite(reading, btn, digitalRead(BUTTONS[btn]));

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (bitRead(reading, btn) == HIGH && bitRead(previous, btn) == LOW && millis() - time > debounce) {
    if (bitRead(state, rly) == HIGH) {
      bitWrite(state, btn, LOW);
    } else {
      bitWrite(state, btn, HIGH);
    }
    time = millis();
  }
  digitalWrite(RELAYS[rly], bitRead(state, rly));
  bitWrite(previous, btn, bitRead(reading, btn));
}

void loop() {
  for (int i = 0; i < n; i++) {
    readAndSwitch(i, i);
  }
}
