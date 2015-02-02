#pragma once
#include "ofMain.h"
#include "Spout.h"

class SpoutManager
{
public:
	
	SpoutReceiver *spoutreceiver;	// A receiver object
	bool bInitialized;				// Initialization result
	bool bMemoryShare;				// Texture share compatibility
	ofTexture myTexture;			// Texture used for texture share transfers
	unsigned int g_Width, g_Height;	// Global width and height
	char SenderName[256];			// Sender name used by a receiver
		

	SpoutManager::SpoutManager();
	void setup();
	bool receiveTexture(bool imageMode = false, unsigned char *pixels = NULL);
	void exit();
};