#pragma once

#include "ofMain.h"
#include "HubPlug.h"
#include "ofxXmlSettings.h"
//#include "ofxUDPManager.h"
class Hub{
public :
	int hubIndex;

	static const int numPlugs = 8; //OctoWS2811 = 8 output pins
	vector<HubPlug *>plugs;

	int ledCount;

	string port;
	int baudRate;
	
	ofSerial serial;
	bool serialOpened;

	

	//methods

	Hub::Hub(ofxXmlSettings settings, int hubIndex, ofColor color);
	Hub::Hub(int hubIndex, string port, int baudRate, ofColor color);
	~Hub();

	void draw(int baseIndex, ofPixels * pixels);

	void addStrip(int plugIndex);
	void removeStrip(int plugIndex, int stripIndex = -1);

	void clear();

	ofColor hubColor;

	bool mousePressed(bool force);
	void mouseReleased();
	
	void updatePositions();
	void updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels);

	void updateLedsSerial();
	void updateLedsArtNet();

	void saveSettings(ofxXmlSettings settings);
	void loadSettings(ofxXmlSettings settings, int hubIndex);

	void updateLedCount();

	void clean();

private:
	void init();
};