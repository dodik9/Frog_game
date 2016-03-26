#include <Adafruit_NeoPixel.h>
#include "SegaController.h"
#include "Box.h"

#define DATA_PIN 13
#define WIDTH 10
#define HEIGHT 20
#define LED_LENGTH WIDTH * HEIGHT

Adafruit_NeoPixel leds(LED_LENGTH, DATA_PIN, NEO_RGB + NEO_KHZ800);

//Box Start Settings: start position x and y, size x and y.
Box box(4,0, 2, 2);   //Frog = Player
Box box2(8,12, 2, 2); 
Box box3(0,6, 2, 2);


SegaController controller({40, 42, 44, 46, 48, 50, 52}, box);
// Ports for SEGA Controller.

void setup() {
  leds.begin();
  leds.setBrightness(250);
  Serial.begin(9600);
  controller.begin();
}

int
getPixelIndex (int x, int y) 
//LED coordinate system, mathematical algorithm for LED Matrix
{
  if(x<0 || y < 0 || x >= WIDTH || y >=HEIGHT) return -1;

  if (x % 2)
  {
    return (x * HEIGHT + HEIGHT - y - 1);
  }
  else
  {
    return (x * HEIGHT + y);
  }
}


void 
setPixel(uint32_t color, int x, int y)
{
  int i = getPixelIndex(x, y);
  if(i<0) return;

  leds.setPixelColor (i, color); // get color
} // for one LED tourn on.


void clearLed() {
  for (int i = 0; i < WIDTH * HEIGHT; i++)
  {
    leds.setPixelColor (i, 0x0);
  }
} //clean everything on Board = reset LEDs

void
drawBox(uint32_t color, int x, int y, int sideX, int sideY) {
  for (int i = x; i < x + sideX; i++) {
    for (int j = y; j < y + sideY; j++) {
      setPixel(color, i, j);
    }
  }
}
//NOTE: Class uinty32_t from Library Adafruit_NeoPixel

// Richtungen der Bewegung einstellung
int dx = 1;
int dy = 0;
int dx2 = 0; 
int dy2 = 0; 



void loop() {
  clearLed(); //clean borad
  controller.loop();  // controller on

  uint32_t color = 0xFF0000; // Box 1 color
  uint32_t color2 = 0xFF00FF; // Box 2 color
  uint32_t color3 = 0x00FFFF; // Box 3 color
  
  drawBox(color, box.x, box.y, box.width, box.height); //Frog, Player
  drawBox(color2, box2.x, box2.y, box2.width, box2.height);
  drawBox(color3, box3.x, box3.y, box3.width, box3.height);
 // draw Box 1 to 3: color, start position x, start position y, Box width (2), Box height (2)
  leds.show(); 
  delay(100); //Geschwindigkeit

  
  box2.move(dx2,dy2); //move Box 2 

   if(box2.x == 0 && box2.y == 12 ){
     dx2 = 1; // now move right
     dy2 = 0; //  
  // }else if(box2.y == 18 && box2.x == 8){
  //   dx2 = -1;
  //   dy2 = 0;
  
   }else if(box2.x == 8 && box2.y == 12){
     dx2 = -1; // now move left
     dy2 = 0;
//   }else if(box2.y == 0 && box2.x == 0){
//     dx2 = 1;
//     dy2 = 0;
   }
   
  box3.move(dx,dy);
    
   if(box3.x == 8 && box3.y == 6 ){
    //Box 3 start position: x=8 and y=6
     dx = -1; //now move left
     dy = 0;
// }else if(box3.y == 18 && box3.x == 8){
//     dx = -1;
//     dy = 0;
   }else if(box3.x == 0 && box3.y == 6){
     dx = 1; //now move right
     dy = 0;
//   }else if(box3.y == 0 && box3.x == 0){
//     dx = 1;
//     dy = 0;
   }
}
