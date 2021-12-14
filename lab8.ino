bool buttonState[4] = {LOW, LOW, LOW, LOW};
bool lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonOn[3][4] = {
  { LOW, HIGH, LOW, HIGH },
  { LOW, HIGH, HIGH, HIGH },
  { LOW, LOW, LOW, HIGH }
};
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
  checkChannelButton();
  sequencing();
  checkLed();
  Serial.println(digitalRead(32));

}
void checkChannelButton() {
  lastButtonStatePrev = buttonStatePrev;
  buttonStatePrev = digitalRead(32);
  lastButtonStateNext = buttonStateNext;
  buttonStateNext = digitalRead(14);
  if (lastButtonStatePrev == LOW and buttonStatePrev == HIGH) {
    channelDisplayed--;
    if (channelDisplayed < 0) {
      channelDisplayed = 2;
    }else{
      channelDisplayed = channelDisplayed;
      }

  }
  if (lastButtonStateNext == LOW and buttonStateNext == HIGH) {
    channelDisplayed++;
    if (channelDisplayed > 2) {
      channelDisplayed = 0;
    }
  }
}


void checkButton() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);

    if (lastButtonState[i] == LOW and buttonState[i] == HIGH) {
      buttonOn[channelDisplayed][i] = !buttonOn[channelDisplayed][i];
      delay(5);
    } else if (lastButtonState[i] == HIGH and buttonState[i] == LOW) {
      delay(5);
    }
  }
}

void sequencing() {
  if (millis() > lastStepTime + stepLength) {
    lastStepTime = millis();



//    if (digitalRead(directionPin) == LOW) {
      currentStep++;
      if (currentStep > 3) {
        currentStep = 0;
      }

//    } else {
//      currentStep--;
//      if (currentStep < 0) {
//        currentStep = 3;
//      }
//
//    }
//    Serial.println(buttonOn[currentStep]);
    if (buttonOn[0][currentStep] == HIGH) {

      usbMIDI.sendNoteOff(midiNotes[currentStep], 0, 1);
      delay(5);
      usbMIDI.sendNoteOn(midiNotes[currentStep], 90, 1);
      delay(5);
    }


  }

}


void checkLed() {
  for (int i = 0; i < 4; i++) {
    if (buttonOn[channelDisplayed][i] == true) {
      analogWrite(ledPins[i], 60);
    } if (i == currentStep) {
      //      Serial.println(currentStep);
      analogWrite(ledPins[i], 100);
    } else {
      analogWrite(ledPins[i], 0);
    }
  }
}
