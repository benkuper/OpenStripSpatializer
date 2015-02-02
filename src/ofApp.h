#pragma once


#include "ofMain.h"
#include "SpoutManager.h"	// Spout SDK
#include "SpoutToPixelThread.h"
#include "LedManager.h"

#include "ofxXmlSettings.h"
#include "GUI.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		//UI
		GUI gui;
		void guiEvent(ofxUIEventArgs &e);

		//Settings
		//ofxXmlSettings settings;
		std::string fileName;
		void loadSettings(const std::string &file);
		void saveSettings();

		//Spout
		SpoutManager spout;
		bool showTexture;

		

		//Leds -> to transfer to LedManager
		LedManager ledManager;
		ofPixels pixels;

		unsigned char * directPix;

		

};

