#include "SegaController.h"

SegaController::SegaController(PinConfig pins, Box & box)
: inputMap{
  { pins.UpPin,     UP,    UP,     Z}, // P0 DB9 Pin 1
  { pins.DownPin,   DOWN,  DOWN,   Y}, // P0 DB9 Pin 2
  { pins.LeftPin,   ON,    LEFT,   X}, // P0 DB9 Pin 3
  { pins.RightPin,  ON,    RIGHT,  0}, // P0 DB9 Pin 4
  { pins.ABPin,     A,     B,      0}, // P0 DB9 Pin 6
  { pins.StartCPin, START, C,      0}, // P0 DB9 Pin 9
},
selectPin(pins.SelectPin),
lastState(-1),
sixButtonMode(0),
currentState(0),
box(box)
{

}

void SegaController::begin(){
    // Setup input pins
  for (int i = 0; i < sizeof(inputMap) / sizeof(Input); i++)
  {
    pinMode(inputMap[i].pin, INPUT);
    digitalWrite(inputMap[i].pin, HIGH);
  }

  pinMode(selectPin, OUTPUT);
  digitalWrite(selectPin, HIGH);
}


bool SegaController::loop() {
  int state = readButtons();

  int now = millis();
  Serial.println(state, HEX);
  // if (isJustPressed(state, prevState, SegaController::ON)   )   observer->onOn();
  if (isJustPressed(state, prevState, SegaController::UP)   )   box.move(0,1);
  if (isJustPressed(state, prevState, SegaController::DOWN) )   box.move(0,-1);
  if (isJustPressed(state, prevState, SegaController::LEFT) )   box.move(-1,0);
  if (isJustPressed(state, prevState, SegaController::RIGHT))   box.move(1,0);
  // if (isJustPressed(state, prevState, SegaController::START))   observer->onStart();
  // if (isJustPressed(state, prevState, SegaController::A)    )   observer->onA();
  // if (isJustPressed(state, prevState, SegaController::B)    )   observer->onB();
  // if (isJustPressed(state, prevState, SegaController::C)    )   observer->onC();

  // if (isPressedLong(state, now, SegaController::UP)   )   observer->onUp();
  // if (isPressedLong(state, now, SegaController::DOWN) )   observer->onDown();
  // if (isPressedLong(state, now, SegaController::LEFT) )   observer->onLeft();
  // if (isPressedLong(state, now, SegaController::RIGHT))   observer->onRight();
  // if (isPressedLong(state, now, SegaController::START))   observer->onStart();
  // if (isPressedLong(state, now, SegaController::A)    )   observer->onA();
  // if (isPressedLong(state, now, SegaController::B)    )   observer->onB();
  // if (isPressedLong(state, now, SegaController::C)    )   observer->onC();

  bool result = prevState != state;
  prevState = state;

  return result;
}


int SegaController::readButtons()
{
  resetState();
  if (sixButtonMode)
  {
    read6buttons();
  }
  else
  {
    read3buttons();
  }
  return currentState;
}

bool SegaController::isPressedLong(int state, unsigned long now, ButtonHex btnHex) {
  bool pressedNow = ((state & btnHex) == btnHex);

  if( pressedNow && whenPressed(now, btnHex) ){
    rememberWhenPressed(btnHex);
    return true;
  }

  return false;
}

bool SegaController::isJustPressed(int state, int prevState, ButtonHex btnHex) {
  if(((state & btnHex) == btnHex) && !((prevState & btnHex) == btnHex)){
    rememberWhenPressed(btnHex);
    return true;
  }
  return false;
}

void SegaController::rememberWhenPressed(ButtonHex butn){
  if (isArrowButton(butn)){
    arrowsBtnPresedTime = millis();
    arrowsBtnPresed = butn;
  }else{
    actionsBtnPresedTime = millis();
    actionsBtnPresed = butn;
  }
}

bool SegaController::whenPressed(unsigned long now, ButtonHex butn){
  if (isArrowButton(butn)){
    if(arrowsBtnPresed == butn){
      bool rr = (now - arrowsBtnPresedTime) > 80;
      return rr;
    }
  }else if(actionsBtnPresed == butn){
    return (now - actionsBtnPresedTime) > 150;
  }

  return false;
}

bool SegaController::isArrowButton(ButtonHex butn){
  return butn & (SegaController::UP | SegaController::DOWN | SegaController::LEFT | SegaController::RIGHT);
}

int  SegaController::readChangedButtons()
{
  readButtons();

  if (currentState != lastState)
  {
    lastState = currentState;
    return currentState;
  }
  return 0;
}

void SegaController::resetState()
{
  currentState = 0;
}

void SegaController::read3buttons()
{
  // Set SELECT LOW and read lowFlag
  digitalWrite(selectPin, LOW);

  delayMicroseconds(20);

  for (int i = 0; i < sizeof(inputMap) / sizeof(Input); i++)
  {
    if (digitalRead(inputMap[i].pin) == LOW)
    {
      currentState |= inputMap[i].lowFlag;
    }
  }

  // Set SELECT HIGH and read highFlag
  digitalWrite(selectPin, HIGH);

  delayMicroseconds(20);

  for (int i = 0; i < sizeof(inputMap) / sizeof(Input); i++)
  {
    if (digitalRead(inputMap[i].pin) == LOW)
    {
      currentState |= inputMap[i].highFlag;
    }
  }

  // When a six-button first connects, it'll spam UP and DOWN,
  // which signals the game to switch to 6-button polling
  if (currentState == (ON | UP | DOWN))
  {
    sixButtonMode = true;
  }
  // When a controller disconnects, revert to three-button polling
  else if ((currentState & ON) == 0)
  {
    sixButtonMode = false;
  }

  delayMicroseconds(20);
}

void SegaController::read6buttons()
{
  // Poll for three-button states twice
  read3buttons();
  read3buttons();

  // After two three-button polls, pulse the SELECT line
  // so the six-button reports the higher button states
  digitalWrite(selectPin, LOW);
  delayMicroseconds(20);
  digitalWrite(selectPin, HIGH);

  for (int i = 0; i < sizeof(inputMap) / sizeof(Input); i++)
  {
    if ( digitalRead(inputMap[i].pin) == LOW)
    {
      currentState |= inputMap[i].pulse3Flag;
    }
  }

  delayMicroseconds(1000);
}






