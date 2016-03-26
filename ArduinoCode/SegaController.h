#ifndef SEGA_CONTROLLER
#define SEGA_CONTROLLER

#include <Arduino.h>
#include "Box.h"

/*
  1 - 52 
  2 - 50 
  3 - 48 
  4 - 46 
  5 - Vin
  6 - 44 
  7 - 40 
  8 - Gnd 
  9 - 42 
*/

typedef int ButtonHex;

class SegaController {
public:

	struct PinConfig {
		int SelectPin;
		int StartCPin;
		int ABPin;
		int RightPin;
		int LeftPin;
		int DownPin;
		int UpPin;
	};

	// Controller Button Flags
	static const ButtonHex ON = 1;
	static const ButtonHex UP = 2;
	static const ButtonHex DOWN = 4;
	static const ButtonHex LEFT = 8;
	static const ButtonHex RIGHT = 16;
	static const ButtonHex START = 32;
	static const ButtonHex A = 64;
	static const ButtonHex B = 128;
	static const ButtonHex C = 256;
	static const ButtonHex X = 512;
	static const ButtonHex Y = 1024;
	static const ButtonHex Z = 2048;

	SegaController(PinConfig pins, Box & box);

	void begin();
	bool loop();

private:

	Box & box;

	int readButtons();
	int readChangedButtons();

	typedef struct
	{
		int pin;
		int lowFlag;
		int highFlag;
		int pulse3Flag;
	} Input;

	// Controller DB9 Pin 7 Mappings
	int selectPin;
	// Controller DB9 Pin to Button Flag Mappings
	// First column is the controller index, second column
	// is the Arduino pin that the controller's DB9 pin is
	// attached to
	Input inputMap[6];

	// Controller State
	int currentState;
	int lastState;
	// Default to three-button mode until six-button connects
	boolean sixButtonMode;

	void read3buttons();
	void read6buttons();
	void resetState();
	void sendStates();

	bool isJustPressed(int state, int oldState, ButtonHex btnHex);
	bool isPressedLong(int state, unsigned long now, ButtonHex btnHex);

	void rememberWhenPressed( ButtonHex btnHex);
	bool whenPressed(unsigned long now, ButtonHex btnHex);
	bool isArrowButton(ButtonHex butn);

	unsigned int prevState;
	unsigned long arrowsBtnPresedTime;
	ButtonHex arrowsBtnPresed;

	unsigned long actionsBtnPresedTime;
	ButtonHex actionsBtnPresed;
};

#endif
