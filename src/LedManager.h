#pragma once

#include "ofMain.h"
#include "Hub.h"
#include "ofxXmlSettings.h"
#include "ofxTimer.h"


class LedManager{
public :

	int numHubs;
	vector<Hub * >hubs;

	int ledCount;

	int sendRate; //fps
	ofxTimer timer;

	float lastUpdateTime;

	ofFbo fbo;
	ofFbo ledMapFbo;

	ofShader ledShader;
	
	ofTexture ledMapTex;
	ofFloatPixels ledMapPixels;
	ofFloatImage ledMapImage;
	
	ofPixels ledPixels;

	bool dragging;

	//static access
	//static LedManager * instance;

	//methods

	LedManager::LedManager();
	//void draw(ofPixels * pixels);
	void draw(ofTexture * sourceTexture);

	Hub * addHub(ofxXmlSettings settings, int hubIndex);
	Hub * addHub(int hubType, int hubIndex);
	void removeHub(int hubIndex);

	bool mousePressed(bool force);
	void mouseReleased();
	
	void updatePositions();
	void updateLedMap();

	void setSendRate(int sendRate);
	void onUpdateTimer(ofEventArgs &e);
	void updateLeds();

	void clear();

	void saveSettings(ofxXmlSettings settings);
	void loadSettings(ofxXmlSettings settings);


	void updateLedCount();


	//static
	static bool showLeds;
	static bool showLines;
	static bool showLinks;
	static bool showHandles;
	static bool showContour;
	static bool doUpdateLeds;
	static bool assignLeds;
};