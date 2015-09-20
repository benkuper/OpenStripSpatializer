#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "GUI.h"
#include "LedManager.h"
#include "SpoutToPixelThread.h"

#ifdef _WIN32
#include "SpoutManager.h"	// Spout SDK
#elif MAC_OS_X_VERSION_10_6
#include "ofxSyphon.h"
#endif

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
        #ifdef _WIN32
		SpoutManager spout;
        #elif MAC_OS_X_VERSION_10_6
        ofxSyphonClient syphonClient;
        #endif
    
		bool showTexture;

		//Leds -> to transfer to LedManager
		LedManager ledManager;
		ofPixels pixels;

		unsigned char * directPix;
};

