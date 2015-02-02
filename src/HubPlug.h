#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "LedStrip.h"

class HubPlug
{
public:
	static const int maxLedPerPlug = 300; // Set in arduino code, current is 300 (see arduino Code)

	int plugIndex;
	int numStrips;
	int ledCount;


	vector<LedStrip * > strips;
	
	HubPlug(int index);

	void draw(int baseIndex, ofPixels * pixels, ofColor color);

	void addStrip();
	void removeStrip(int stripIndex = -1);

	bool mousePressed(bool force);
	void mouseReleased();
	
	void updatePositions();
	void updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels);

	void updateLedsSerial(unsigned char * buffer);


	void saveSettings(ofxXmlSettings settings);
	void loadSettings(ofxXmlSettings settings, int plugIndex);

	void clear();

	void updateLedCount();

};