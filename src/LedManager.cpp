#include "LedManager.h"

#include "SerialHub.h"
#include "FireflyHub.h"

bool LedManager::showLeds = true;
bool LedManager::showLinks = true;
bool LedManager::showLines = true;
bool LedManager::showHandles = true;
bool LedManager::showContour = false;
bool LedManager::doUpdateLeds = true;
bool LedManager::assignLeds = true;

LedManager::LedManager()
{
	
	numHubs = 0;

	dragging = false;

	//string comPorts[3] = {"COM17","COM18","COM13"}; //must initialized with same length as numHubs
	ofColor colors[3] = {ofColor::orangeRed,ofColor::aliceBlue,ofColor::forestGreen};
	
	//for(int i=0;i<1;i++) addHub(comPorts[i],460800,colors[i]);
	
	fbo.allocate(256,256);
	ledShader.load("LedShader/shader");
	
	ledMapImage.allocate(256,256,OF_IMAGE_COLOR);

	ledMapPixels.allocate(256,256,OF_IMAGE_COLOR);
	ledPixels.allocate(256,256,OF_IMAGE_COLOR_ALPHA);

	int imgSize = ledMapImage.width*ledMapImage.height;

	updateLedMap();

	setSendRate(50);
	updatePositions();
	ofAddListener(timer.TIMER_REACHED,this,&LedManager::onUpdateTimer);

}

Hub * LedManager::addHub(ofxXmlSettings settings, int hubIndex)
{

	printf("Add hub with settings , new hub size %i\n",numHubs);

	settings.pushTag("hub",hubIndex);
	int hubType = settings.getValue("type",HUB_TYPE_SERIAL);
	settings.popTag();

	
	Hub * h = addHub(hubType,hubIndex);
	if(h == NULL) return NULL;

	h->loadSettings(settings,hubIndex);
	h->connect();

	return h;
}


Hub * LedManager::addHub(int hubType, int hubIndex)
{
	Hub * h = NULL;
	switch(hubType)
	{
	case HUB_TYPE_SERIAL:
		h = new SerialHub();
		break;
	case HUB_TYPE_FIREFLY:
		h = new FireflyHub();
		break;
	}

	if(h == NULL) return NULL;

	h->init(hubIndex,ofColor::lightGreen);

	hubs.push_back(h);
	numHubs = hubs.size();
	
	updateLedCount();

	return h;
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
    ofPushStyle();
    
	if(dragging) updateLedMap();

	fbo.begin();
	
    ofSetColor(0);
    ofRect(0, 0, fbo.getWidth(),fbo.getHeight());
    
	ledShader.begin();
    
	ledShader.setUniform2f("fboSize", fbo.getWidth(),fbo.getHeight());  // SET A UNIFORM
    ledShader.setUniform2f("sourceSize",sourceTexture->getWidth(),sourceTexture->getHeight());
	ledShader.setUniformTexture("sourceTex",*sourceTexture,1);
	ledShader.setUniformTexture("ledMap",ledMapImage.getTextureReference(),2);

    // OSX sucks. Need to inverse Y coords when using GL on OSX to get the right pixels from sourceTex
    
    #ifdef _WIN32
    ledShader.setUniform1i("inverse", 0);
    #elif MAC_OS_X_VERSION_10_6
    ledShader.setUniform1i("inverse", 1);
    #endif
    
    
    ofSetColor(255);
	ofRect(0, 0, fbo.getWidth(),fbo.getHeight());

	ledShader.end();
	sourceTexture->unbind();
	fbo.end();

    // Debug
    //fbo.draw(0,ofGetHeight()- fbo.getHeight(),fbo.getWidth(),fbo.getHeight());
    
	fbo.readToPixels(ledPixels);
	int baseIndex = 0;
	for (int i=0;i<numHubs;i++) 
	{
		hubs[i]->draw(baseIndex,&ledPixels);
		baseIndex+= hubs[i]->ledCount;
	}
    
    ofPopStyle();
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
	for (int i=0;i<numHubs;i++) hubs[i]->updatePositions();
}

void LedManager::updateLedMap()
{
	//printf("UPDATE LED MAP \n");

	int index = 0;
	for (int i=0;i<numHubs;i++) 
	{
		hubs[i]->updateLedMap(index,&ledMapPixels);
		index+=hubs[i]->ledCount;
	}

	ledMapImage.setFromPixels(ledMapPixels);
}

void LedManager::updateLeds()
{
	//printf("Update leds serial\n");
	for (int i=0;i<numHubs;i++) hubs[i]->updateLeds();
}


void LedManager::clear()
{
	while(numHubs > 0) removeHub(0);
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
	
	clear();

	settings.pushTag("leds");
	
	int loadedHubs = settings.getNumTags("hub");

	//printf("Loaded hubs %i\n",loadedHubs);

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
	//printf("ms delay = %f\n",(1000.00f/sendRate));
	this->sendRate = sendRate;
	timer.reset();
	timer.setup((1000.00f/sendRate),true);
	timer.startTimer();
	printf("Set send rate %i\n",sendRate);
}

void LedManager::onUpdateTimer(ofEventArgs &e)
{
	float curTime = ofGetElapsedTimef();
	//printf("timer diff : %f\n",curTime-lastUpdateTime);
	
	updateLeds();
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