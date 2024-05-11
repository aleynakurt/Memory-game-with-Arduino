#include <IRremote.h>
#include <LiquidCrystal.h>

#define DECODE_NEC
#define IR_RECEIVE_PIN 2

LiquidCrystal lcd(13, 12, 3, 4, 5, 6);
IRrecv irReceiver(IR_RECEIVE_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("Welcome!");
  delay(2000);
  lcd.clear();
  lcd.print("Game is Starting");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press 1 for");
  lcd.setCursor(0, 1);
  lcd.print("Green");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press 2 for");
  lcd.setCursor(0, 1);
  lcd.print("Yellow");
  delay(2000);
  lcd.clear();
  lcd.print("Press 3 for");
  lcd.setCursor(0, 1);
  lcd.print("Red");
  delay(2000);
  lcd.clear();
  lcd.print("Press 4 for");
  lcd.setCursor(0, 1);
  lcd.print("Blue");
  delay(2000);
  lcd.clear();
  irReceiver.enableIRIn();
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  randomSeed(analogRead(A0));
}

int generatedSequence[10];
int currentLength = 0;
int lengths[] = {3,3,3};
int sequenceCount = 0; 

void generateSequence() {
  if (sequenceCount >= sizeof(lengths) / sizeof(lengths[0])) {
    return;
  }

  currentLength = lengths[sequenceCount];
  for (int j = 0; j < currentLength; j++) {
    int pin = random(8, 12);
    generatedSequence[j] = pin;
    digitalWrite(pin, HIGH);
    delay(350);
    digitalWrite(pin, LOW);
    delay(350);
  }
  delay(2000);
  sequenceCount++;
}

int playerInput[10];
int inputCount = 0;

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Memorize");
  lcd.setCursor(0, 1);
  lcd.print("sequence!");
  delay(1000);
  lcd.clear();
  lcd.print("3...");
  delay(1000);
  lcd.clear();
  lcd.print("2...");
  delay(1000);
  lcd.clear();
  lcd.print("1!");
  delay(1000);
  lcd.clear();
  lcd.print("GO!!!");
  delay(500);
  lcd.clear();

  generateSequence();
  delay(1000);

  lcd.clear();
  lcd.print("Now, your turn!");

  while (inputCount < currentLength) {
    if (irReceiver.decode()) {
      int buttonPressed = ButtonToPin(irReceiver.decodedIRData.command);
      if (buttonPressed != -1) {
        digitalWrite(buttonPressed, HIGH);
        delay(500);
        digitalWrite(buttonPressed, LOW);
        playerInput[inputCount++] = buttonPressed;
      }
      irReceiver.resume();
    }
  }

  bool correct = true;
  for (int i = 0; i < currentLength; i++) {
    if (playerInput[i] != generatedSequence[i]) {
      correct = false;
      break;
    }
  }

  lcd.clear();
  if (correct) {
    lcd.print("Correct!");
    delay(2000);
  } else {
    lcd.print("Wrong!");
    lcd.setCursor(0, 1);
    lcd.print("Try again!");
    delay(2000);
    lcd.clear();
    sequenceCount = 0; 
    inputCount = 0;   
    for (int i = 0; i < currentLength; i++) {
      digitalWrite(generatedSequence[i], LOW); 
    }
    return; 
  }

  inputCount = 0;

  if (sequenceCount >= sizeof(lengths) / sizeof(lengths[0])) {
    lcd.clear();
    lcd.print("Well done!");
    delay(2000);
    lcd.clear();
    lcd.print("Game is");
    lcd.setCursor(0, 1);
    lcd.print("Finished!!");
    delay(10000);
  }
}

int ButtonToPin(int c) {
  switch (c) {
    case 16: return 8;
    case 17: return 9;
    case 18: return 10;
    case 20: return 11;
    default: return -1;
  }
}