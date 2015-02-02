#pragma once

#include "LedManager.h"

bool LedManager::showLeds = true;
bool LedManager::showLinks = true;
bool LedManager::showLines = true;
bool LedManager::showHandles = true;
bool LedManager::showContour = false;
bool LedManager::updateLeds = true;
bool LedManager::assignLeds = true;
bool LedManager::useSerial = false;
bool LedManager::useArtNet = true;

LedManager::LedManager()
{
	
	numHubs = 0;

	dragging = false;

	string comPorts[3] = {"COM17","COM18","COM13"}; //must initialized with same length as numHubs
	ofColor colors[3] = {ofColor::orangeRed,ofColor::aliceBlue,ofColor::forestGreen};
	
	//for(int i=0;i<1;i++) addHub(comPorts[i],460800,colors[i]);
	
	fbo.allocate(256,256);
	ledShader.load("LedShader/shader");
	
	ledMapImage.allocate(256,256,ofImageType::OF_IMAGE_COLOR);

	ledMapPixels.allocate(256,256,ofImageType::OF_IMAGE_COLOR);
	ledPixels.allocate(256,256,ofImageType::OF_IMAGE_COLOR_ALPHA);

	int imgSize = ledMapImage.width*ledMapImage.height;

	updateLedMap();

	setSendRate(50);
	updatePositions();
	ofAddListener(timer.TIMER_REACHED,this,&LedManager::onUpdateTimer);
}

void LedManager::addHub(ofxXmlSettings settings, int hubIndex)
{
	printf("Add Hub %i\n",hubIndex);
		
	//tmp
	ofColor colors[3] = {ofColor::orangeRed,ofColor::aliceBlue,ofColor::forestGreen};
	
	hubs.push_back(new Hub(settings,hubIndex,colors[hubIndex]));
	numHubs = hubs.size();	
	printf("Add hub, new hub size %i\n",numHubs);

	updateLedCount();
}

void LedManager::addHub(string port,int baudRate,ofColor color)
{
	hubs.push_back(new Hub(numHubs,port,baudRate,color));
	numHubs = hubs.size();	

	printf("Add hub, new hub size %i\n",numHubs);

	updateLedCount();
}

void LedManager::removeHub(int hubIndex)
{
	if(hubIndex == -1) hubIndex = numHubs-1;
	if(hubIndex < numHubs && numHubs > 0) hubs.erase(hubs.begin()+hubIndex);

	
	numHubs = hubs.size();

	for(int i=0;i<numHubs;i++) hubs[i]->hubIndex = i;

	updateLedCount();
}

/*
void LedManager::draw(ofPixels * pixels)
{
	//if(!showHandles && !showLines && !showLeds && !showLinks) return;

	for (int i=0;i<numHubs;i++) hubs[i]->draw(pixels);

	
}
*/

void LedManager::draw(ofTexture * sourceTexture)
{
	//pass from source Texture to FBO with ledShader and then map the leds from the smaller generated texture

	if(dragging) updateLedMap();

	fbo.begin();
	ofSetColor(255);
	ledShader.begin();
	ledShader.setUniform2f("fboSize", fbo.getWidth(),fbo.getHeight());  // SET A UNIFORM
	ledShader.setUniformTexture("sourceTex",*sourceTexture,1);
	ledShader.setUniformTexture("ledMap",ledMapImage.getTextureReference(),2);

	ofRect(0, 0, fbo.getWidth(),fbo.getHeight());

	ledShader.end();
	sourceTexture->unbind();
	fbo.end();
	
	//fbo.draw(10,ofGetHeight()-266,256,256);
	//ledMapImage.draw(10,10,200,200);

	
	fbo.readToPixels(ledPixels);
	int baseIndex = 0;
	for (int i=0;i<numHubs;i++) 
	{
		hubs[i]->draw(baseIndex,&ledPixels);
		baseIndex+= hubs[i]->ledCount;
	}

}


bool LedManager::mousePressed(bool force)
{
	bool result = false;
	for (int i=0;i<numHubs;i++) 
	{
		if(hubs[i]->mousePressed(force)) result = true;
		if(!force && result) 
		{

			result = true;
			break;
		}
	}

	dragging = result;
	printf("Dragging ? %i\n",dragging);

	return result;
}

void LedManager::mouseReleased()
{
	for (int i=0;i<numHubs;i++) hubs[i]->mouseReleased();
	dragging = false;
}

void LedManager::updatePositions()
{
	printf("Updat epositions");
	for (int i=0;i<numHubs;i++) hubs[i]->updatePositions();
}

void LedManager::updateLedMap()
{
	printf("UPDATE LED MAP \n");

	int index = 0;
	for (int i=0;i<numHubs;i++) 
	{
		hubs[i]->updateLedMap(index,&ledMapPixels);
		index+=hubs[i]->ledCount;
	}

	ledMapImage.setFromPixels(ledMapPixels);
}

void LedManager::updateLedsSerial()
{
	for (int i=0;i<numHubs;i++) hubs[i]->updateLedsSerial();
}

void LedManager::updateLedsArtNet()
{
	for (int i=0;i<numHubs;i++) hubs[i]->updateLedsArtNet();
}

void LedManager::saveSettings(ofxXmlSettings settings)
{
	settings.addTag("leds");
	settings.pushTag("leds");
	
	for (int i=0;i<numHubs;i++) hubs[i]->saveSettings(settings);

	settings.popTag();

	
}

void LedManager::loadSettings(ofxXmlSettings settings)
{
	settings.pushTag("leds");

	int loadedHubs = settings.getNumTags("hub");

	printf("Loaded hubs %i\n",loadedHubs);

	for(int i=0;i<loadedHubs;i++)
	{
		addHub(settings,i);
		//hubs[i]->loadSettings(settings,i);
	}
		
 
	settings.popTag();

	updateLedCount();

	updatePositions();
	updateLedMap();
}

void LedManager::setSendRate(int sendRate)
{
	printf("ms delay = %f\n",(1000.00f/sendRate));
	timer.setup((1000.00f/sendRate),true);
	timer.startTimer();
}

void LedManager::onUpdateTimer(ofEventArgs &e)
{
	float curTime = ofGetElapsedTimef();
	//printf("timer diff : %f\n",curTime-lastUpdateTime);
	
	if(useSerial) updateLedsSerial();
	if(useArtNet) updateLedsArtNet();
	lastUpdateTime = curTime;
}

void LedManager::updateLedCount()
{
	ledCount = 0;
	for(int i=0;i<numHubs;i++) 
	{
		hubs[i]->updateLedCount();
		ledCount += hubs[i]->ledCount;
	}
}