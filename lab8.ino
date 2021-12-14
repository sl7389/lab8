bool buttonState[4] = {LOW, LOW, LOW, LOW};
bool lastButtonState[4] = {LOW, LOW, LOW, LOW};
bool buttonOn[4] = {false, false, false, false};
int buttonPins[4] = {35, 37, 39, 33};
int ledPins[4] = {7, 8, 9, 10};
unsigned long lastStepTime = 0;
int currentStep = 0;
int stepLength = 500;
//int directionPin = 14;
int midiNotes[4] = {48, 40, 41, 49};

bool buttonStatePrev = LOW;
bool buttonStateNext = LOW;
bool lastButtonStatePrev = LOW;
bool lastButtonStateNext = LOW;
int prevChannelButtonPin = 32;
int nextChannelButtonPin = 14;
int channelDisplayed = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
//  pinMode(directionPin, INPUT);
  pinMode(prevChannelButtonPin, INPUT);
  pinMode(nextChannelButtonPin, INPUT);
}

void loop() {

  checkButton();
  sequencing();
  checkLed();

}
void checkChannelButton() {
  lastButtonStatePrev = buttonStatePrev;
  buttonStatePrev = digitalRead(32);
  if (lastButtonStatePrev == LOW and buttonStatePrev == HIGH) {
    channelDisplayed=
  } else if (lastButtonStatePrev == HIGH and buttonStatePrev == LOW) {
    delay(5);
  }
}
}

void checkButton() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);

    if (lastButtonState[i] == LOW and buttonState[i] == HIGH) {
      buttonOn[i] = !buttonOn[i];
      delay(5);
    } else if (lastButtonState[i] == HIGH and buttonState[i] == LOW) {
      delay(5);
    }
  }
}

void sequencing() {
  if (millis() > lastStepTime + stepLength) {
    lastStepTime = millis();



    if (digitalRead(directionPin) == LOW) {
      currentStep++;
      if (currentStep > 3) {
        currentStep = 0;
      }

    } else {
      currentStep--;
      if (currentStep < 0) {
        currentStep = 3;
      }

    }
    Serial.println(buttonOn[currentStep]);
    if (buttonOn[currentStep] == HIGH) {

      usbMIDI.sendNoteOff(midiNotes[currentStep], 0, 1);
      delay(5);
      usbMIDI.sendNoteOn(midiNotes[currentStep], 90, 1);
      delay(5);
    }


  }

}


void checkLed() {
  for (int i = 0; i < 4; i++) {
    if (buttonOn[i] == true) {
      analogWrite(ledPins[i], 100);
    } if (i == currentStep) {
      //      Serial.println(currentStep);
      analogWrite(ledPins[i], 200);
    } else {
      analogWrite(ledPins[i], 0);
    }
  }
