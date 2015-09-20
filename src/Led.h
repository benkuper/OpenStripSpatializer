#pragma once

#include "ofMain.h"


class Led : public ofVec2f{
public :
	ofColor color;

	Led();
	int radius;

	void draw();
};