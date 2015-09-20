#pragma once
#include "ofMain.h"
#include "ofxUI.h"

//#include "LedManager.h"

class GUI
{
public:

	bool visible;



	//UI
	ofxUISuperCanvas *panel;
	ofxUISuperCanvas *ledPanel;

	ofxUILabel * numLedsLabel;

	//Elements
	ofxUILabel * frameRateLabel;
	
	ofxUIRadio * hubRadio;
	ofxUIRadio * stripsRadio;

	void setup();

	void updateHubList();
	void updateStripList();

	void windowResized(ofResizeEventArgs & args);
	void onDraw(ofEventArgs & args);

	//tmp
	ofxUISlider * sendRateSlider;
	void setSendRate(float sendRate);

};