#pragma once

#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>

class Sprite{
private:

protected:
	float x, y;
	double zoom;
	int curFrame;

public:
	Sprite() {curFrame = x = y = 0; zoom = 1;}
	~Sprite() {}
	void virtual setCoordinates(int newX, int newY, int newZoom) {x = newX; y = newY; zoom = newZoom;}
	void virtual incrementCoordinates(int dx, int dy, int dZoom) {x += dx; y += dy; zoom += dZoom;}
	void virtual draw(ALLEGRO_BITMAP* image) {al_draw_bitmap(image, x, y, 0);}
};