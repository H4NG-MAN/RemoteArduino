const int ACTION_SETMODE = 1;
const int ACTION_WRITE = 0;

int readCount = 0;
int pinData = 1 << 15;

int chk[14];
int readPins[14];

void setRead(int pin);
void setWrite(int pin);
void readInput();

void setup() {
  Serial.begin(9600);
}

void loop() {
  readInput();
  int i, p, data = 0;
  for (i = 0; i < readCount; ++i) {
    p = readPins[i];
    data |= digitalRead(p) << p;
  }
  if (data != pinData) {
    Serial.write(data >> 8);
    Serial.write(data);
    pinData = data;
  }
}

void readInput() {
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();

    int action = (incomingByte & (1l << 7)) != 0;
    int argument = ((incomingByte >> 5) & 3);
    int p = incomingByte & ((1l << 5) - 1);

    if (action == ACTION_SETMODE) {
      if (argument == 1) {
        setWrite(p);
      } else {
        setRead(p, argument);
      }
      pinData = 1 << 15;
    } else if (action == ACTION_WRITE) {
      digitalWrite(p, argument);
    }
  }
}

void setRead(int pin, int mode) {
  if (chk[pin]) return;
  pinMode(pin, mode ? INPUT_PULLUP : INPUT);
  chk[pin] = 1;
  readPins[readCount++] = pin;
}

void setWrite(int pin) {
  pinMode(pin, OUTPUT);
}

