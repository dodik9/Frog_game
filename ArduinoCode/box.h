#ifndef _BOX_
#define _BOX_

class Box{
public:
	int x;
	int y;
	int width;
	int height;

	Box(int x, int y, int width, int height):x(x), y(y), width(width), height(height){}
	
 void move(int dx, int dy){
 	x+=dx;
 	y+=dy;
 }
};

#endif