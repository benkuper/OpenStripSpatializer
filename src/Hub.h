#pragma once

#include "ofMain.h"
#include "HubPlug.h"
#include "ofxXmlSettings.h"


//Hub types
#define HUB_TYPE_SERIAL 1
#define HUB_TYPE_FIREFLY 2

class Hub{
public :
	int hubIndex;
	int hubType;

	static const int numPlugs = 8; //OctoWS2811 = 8 output pins
	int realNumPlugs;
	int plugLimit;

	vector<HubPlug *>plugs;

	int ledCount;

	//methods
	Hub::Hub();
	~Hub();

	virtual void init(int hubIndex, ofColor color = ofColor::peachPuff);
	virtual void connect();
	
	void draw(int baseIndex, ofPixels * pixels);

	void addStrip(int plugIndex);
	void removeStrip(int plugIndex, int stripIndex = -1);

	void clear();

	ofColor hubColor;

	bool mousePressed(bool force);
	void mouseReleased();
	
	void updatePositions();
	void updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels);

	void updateLeds();
	virtual void sendLeds(unsigned char * data, int length); //to be overriden by child classes

	void saveSettings(ofxXmlSettings settings);
	void loadSettings(ofxXmlSettings settings, int hubIndex);

	void updateLedCount();

	virtual void clean();

protected :
	virtual void Hub::loadSettingsInternal(ofxXmlSettings settings);
	virtual void Hub::saveSettingsInternal(ofxXmlSettings settings);
	

};