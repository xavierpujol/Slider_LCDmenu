/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 *  I have 4 buttons (1,2,3,4) starting on the left:
 *   Button 1 pin 6
 *   Button 2 pin 7
 *   Button 3 pin 8
 *   Button 4 pin 9

 upScreenButton  6          //Button 1
 downScreenButton 7         //Button 2
 plusParametterButton 8     //Button 3
 minusParameterButton  9    //Button 4
*/



// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Input & Button Logic
const int numOfButtons = 4;
const int inputPins[numOfButtons] = {6,7,8,9};
int buttonState[numOfButtons];
int lastButtonSstate[numOfButtons] = {LOW,LOW,LOW,LOW};
bool buttonFlags[numOfButtons] = {LOW,LOW,LOW,LOW};
unsigned long lastDebounceTime[numOfButtons] = {0,0,0,0};
unsigned long debounceDelay = 5;

//LCD Menu Logic
const int numOfScreens = 3;
const int numberOfOptionsScreenOne = 2;
const int numberOfOptionsScreenTwo = 3;
int currentScreen = 0;
String screens[numOfScreens][2] = {{"Enable"," "},{"Mode"," "},{"Speed","RPM"}};
int parameters[numOfScreens];
String parametersStringsScreenOne[numberOfOptionsScreenOne] {"ON","OFF"};
String parametersStringsScreenTwo[numberOfOptionsScreenTwo] {"DIRECT", "REVERSE", "OSCILATE"};

//Function prototypes
void setInputFlags();
void resolveInputFlags();
void inputAction(int input);
void parameterChange(int key);
void printScreen();


void setup() {
  // Defining the buttons pins as inputs and seting them to HIGH
  for(int i = 0; i < numOfButtons; i++) {
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH); // pull-up 20k
  }

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  setInputFlags();
  resolveInputFlags();
}

void setInputFlags() {
  for(int i = 0; i < numOfButtons; i++) {
    int reading = digitalRead(inputPins[i]);
    if (reading != lastButtonSstate[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (buttonState[i] == HIGH) {
          buttonFlags[i] = HIGH;
        }
      }
    }
    lastButtonSstate[i] = reading;
  }
}

void resolveInputFlags() {
  for(int i = 0; i < numOfButtons; i++) {
    if(buttonFlags[i] == HIGH) {
      inputAction(i);
      buttonFlags[i] = LOW;
      printScreen();
    }
  }
}

  void inputAction(int input) {
  if(input == 1) {
    if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
    }else{
      currentScreen--;
    }
  }else if(input == 0) {
    if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
    }else{
      currentScreen++;
    }
  }else if(input == 2) {
    parameterChange(0);
  }else if(input == 3) {
    parameterChange(1);
  }
}

void parameterChange(int key) {
  if(key == 0){
    if (currentScreen == 2){
    parameters[currentScreen] = parameters[currentScreen] + 25;
  }else{
      parameters[currentScreen]++;
    }
    if (currentScreen == 0){  //pantalla del enable boto de anar cap amunt
      if (parameters[currentScreen] > 1)
      {
        parameters[currentScreen] = 0;
      }
    }
    else if (currentScreen == 1) //pantalla del mode boto de anar cap amunt
    {
      if (parameters[currentScreen] > 2)
      {
        parameters[currentScreen] = 0;
      }
    }
    else { //pantalla de RPM boto de anar cap amunt
      if (parameters[currentScreen] > 250){
        parameters[currentScreen] = 0;
      }
    }
      }
    else{
      if (currentScreen == 2){
      parameters[currentScreen] = parameters[currentScreen] - 25;
      }else{
        parameters[currentScreen]--;
      }

      if (currentScreen == 0){ //pantalla enable boto d'anar cap abaix.
        if (parameters[currentScreen] < 0){
          parameters[currentScreen] = 1;
        }
      }
      else if (currentScreen == 1){ //pantalla MODE boto d'anar cap abaix.
        if (parameters[currentScreen] < 0){
          parameters[currentScreen] = 2;
        }
      }
      else{
        if (parameters[currentScreen] < 0)
        {
          parameters[currentScreen] = 250;
        }
      }
    }
  }





void printScreen() {
  lcd.clear();
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0,1);

  if (currentScreen  == 2)
  {
  lcd.print(parameters[currentScreen]);
  }
  else if (currentScreen == 0) //Visualització pantalla enable
  {
    if (parameters[currentScreen] == 0)
    {
      lcd.print(parametersStringsScreenOne[0]);
    }
    else if  (parameters[currentScreen] == 1)
    {
      lcd.print(parametersStringsScreenOne[1]);
    }
  }
  else{
    if (parameters[currentScreen] == 0)
    {
      lcd.print(parametersStringsScreenTwo[0]);
    }
    else if  (parameters[currentScreen] == 1)
    {
      lcd.print(parametersStringsScreenTwo[1]);
    }
    else if (parameters[currentScreen] == 2){
      lcd.print(parametersStringsScreenTwo[2]);
    }
  }
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
}
