#pragma once

#include "ofMain.h"
#include "Led.h"

#include "ofxXmlSettings.h"

#define SHAPE_LINE 0
#define SHAPE_CIRCLE 1
#define SHAPE_SQUARE 2

class LedStrip{
public :

	int numLeds;
	vector<Led *> leds;

	ofVec2f start;
	ofVec2f end;

	ofVec2f * dragHandle;

	int plugIndex;
	int stripIndex;

	int shape;

	string customLabel;

	//methods

	LedStrip::LedStrip(int plugIndex, int stripIndex, int shape = SHAPE_LINE);
	void draw(int baseIndex, ofPixels * pixels, ofColor color);
	bool mousePressed(bool force);
	void mouseReleased();

	void setNumLeds(int numLeds);
	
	void updatePositions();
	void updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels);


	void updateLeds(unsigned char * buffer, int offset);

	void saveSettings(ofxXmlSettings settings);
	void loadSettings(ofxXmlSettings settings, int stripIndex);
};

