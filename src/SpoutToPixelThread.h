#pragma once
#include "ofMain.h"
#include "SpoutManager.h"

class SpoutToPixelThread : public ofThread
{

	public :
	ofTexture tex;
	ofImage img;
	ofShortPixels pixels;

	SpoutToPixelThread::SpoutToPixelThread(ofTexture srcTex)
	{
		this->tex = srcTex;
	}

	// the thread function
	void SpoutToPixelThread::threadedFunction()
	{
		printf("Start thread !\n");
		
		// start
		while(isThreadRunning()) {
			
			printf("tex : %f, %f\n",tex.getWidth(),tex.getHeight());

			lock();
			tex.readToPixels(pixels);
			unlock();

			printf("pix 0 : %i\n",pixels.getColor(0,0).r);
			//ofSleepMillis(100);
		}

		printf("Stop thread\n");
	 }

	
};