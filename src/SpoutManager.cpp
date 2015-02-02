#include "SpoutManager.h"

SpoutManager::SpoutManager()
{
	
}

void SpoutManager::setup()
{
	
	bMemoryShare	= false;
	bInitialized	= false;				// Spout receiver initialization
	g_Width			= ofGetWindowWidth();					// set global width and height to something
	g_Height		= ofGetWindowHeight();					// they need to be reset when the receiver connects to a sender
	SenderName[0]	= NULL;					// the name will be filled when the receiver connects to a sender

	spoutreceiver	= new SpoutReceiver;	// Create a new Spout receiver

	// Allocate a texture for shared texture transfers
	// An openFrameWorks texture is used so that it can be drawn.
	myTexture.allocate(g_Width, g_Height, GL_RGBA);	
}

bool SpoutManager::receiveTexture(bool imageMode, unsigned char *pixels)
{
	bool result = false;

	char str[256];
	ofSetColor(255);
	unsigned int width, height;
	char tempname[256]; // temp name
	

	if(!bInitialized) {
		
		if(spoutreceiver->CreateReceiver(SenderName, width, height)) {
			// Is the size of the detected sender different ?
			if(width != g_Width || height != g_Height ) {
				// The sender dimensions have changed so update the global width and height
				g_Width  = width;
				g_Height = height;
				// Update the local texture to receive the new dimensions
				myTexture.allocate(g_Width, g_Height, GL_RGBA);
				// reset render window
				// ofSetWindowShape(g_Width, g_Height);
			}
			bInitialized = true;
			return false; // quit for next round
		} // receiver was initialized
		else {
			sprintf_s(str, "No sender detected");
			//ofDrawBitmapString(str, 20, 20);
		}
	} // already initialized

	// The receiver has initialized so OK to draw
	if(bInitialized) {
		
		// Save current global width and height - they will be changed
		// by receivetexture if the sender changes dimensions
		width  = g_Width;
		height = g_Height;
		
		if(imageMode)
		{
			//printf("Before receiveImage");
			if(spoutreceiver->ReceiveImage(SenderName,width,height,pixels))
			{
				if(width != g_Width || height != g_Height ) {
					// Update the global width and height
					g_Width  = width;
					g_Height = height;
					
					return false; // quit for next round
				}

				result = true;

				if(!bMemoryShare) {
				}else {
					sprintf_s(str, "Memoryshare receiver");
					ofDrawBitmapString(str, 20, 20);
				}
			}else {
				spoutreceiver->ReleaseReceiver();
				bInitialized = false;
				SenderName[0] = 0; 
				return false;
			}
		}else
		{
			if(spoutreceiver->ReceiveTexture(SenderName, width, height, myTexture.getTextureData().textureID, myTexture.getTextureData().textureTarget)) {

				//	width and height are changed. The local texture then has to be resized.
				if(width != g_Width || height != g_Height ) {
					// Update the global width and height
					g_Width  = width;
					g_Height = height;
					// Update the local texture to receive the new dimensions
					myTexture.allocate(g_Width, g_Height, GL_RGBA);

					return false; // quit for next round
				}

			
				result = true;

				if(!bMemoryShare) {
				}
				else {
					sprintf_s(str, "Memoryshare receiver");
					ofDrawBitmapString(str, 20, 20);
				}
			}
			else {
				spoutreceiver->ReleaseReceiver();
				bInitialized = false;
				SenderName[0] = 0; 
				return false;
			}
		}
	}

	return result;
}

void SpoutManager::exit()
{
	// can be used for repeated Create and Release of a receiver within the program
	spoutreceiver->ReleaseReceiver(); 
	// Program termination so clean up everything
	delete spoutreceiver;
	spoutreceiver = NULL;
}