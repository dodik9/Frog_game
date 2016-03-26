#ifndef CONTROLLER_OBSERVER
#define CONTROLLER_OBSERVER

class ControllerObserver{
public:
  ControllerObserver(){};
  virtual void onOn(){};
  virtual void onA(){};
  virtual void onB(){};
  virtual void onC(){};
  virtual void onUp(){};
  virtual void onDown(){};
  virtual void onLeft (){};
  virtual void onRight (){};
  virtual void onStart (){};
};

#endif
