#include "Hub.h"


Hub::Hub()
{
	printf("Hub constructor\n");
}

void Hub::init(int hubIndex, ofColor color)
{
	printf("Hub init\n");
	for(int i=0;i<numPlugs;i++) plugs.push_back(new HubPlug(i));
	printf("--> Num plugs in hub : %lu\n", plugs.size());
	hubColor = color;

	realNumPlugs = 0;
	hubColor = color;
	this->hubIndex = hubIndex;
	//loadSettings(settings,hubIndex);
	//connect();	

	updatePositions();
}


void Hub::connect()
{
	printf("Hub connect \n");
	
}


void Hub::draw(int baseIndex, ofPixels * pixels)
{
	for(int i=0;i<realNumPlugs;i++) 
	{
		plugs[i]->draw(baseIndex, pixels,hubColor);	
		baseIndex += plugs[i]->ledCount;
	}
}

void Hub::addStrip(int plugIndex)
{
	plugs[plugIndex]->addStrip();
}

void Hub::removeStrip(int plugIndex, int stripIndex)
{
	plugs[plugIndex]->removeStrip(stripIndex);
}

void Hub::clear()
{
	for(int i=0;i<numPlugs;i++) plugs[i]->clear();
}

bool Hub::mousePressed(bool force)
{
	bool result = false;
	for(int i=0;i<numPlugs;i++)
	{
		if(plugs[i]->mousePressed(force)) result = true;
		if(!force && result) return true;
	}

	return result;
}

void Hub::mouseReleased()
{
	for(int i=0;i<numPlugs;i++) plugs[i]->mouseReleased();
}

void Hub::updatePositions()
{
	for(int i=0;i<numPlugs;i++) plugs[i]->updatePositions();
}

void Hub::updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels)
{
	for(int i=0;i<realNumPlugs;i++) 
	{
		plugs[i]->updateLedMap(baseIndex,ledMapPixels);
		baseIndex += plugs[i]->ledCount;
	}
}

void Hub::updateLeds()
{
	//printf("Hub updateLedsSerial\n");
	float m0 = ofGetElapsedTimef();

	const int numBytes = (numPlugs*HubPlug::maxLedPerPlug)*3+1; //3bytes per led + 255 final byte
	const int realNumBytes = (realNumPlugs*HubPlug::maxLedPerPlug)*3+1;

	unsigned char buffer[numBytes];// = new unsigned char[totalLeds*3];
	memset(buffer,0,numBytes);

	//printf("total plugs /bytes : %i %i\n",realNumPlugs,realNumBytes);
	for(int i=0;i<realNumPlugs;i++) plugs[i]->updateLeds(buffer);


	buffer[realNumBytes-1] = 255;

	float elapsed0 = ofGetElapsedTimef() - m0;
	//printf("Buffer fill : %f micros\n",elapsed0);

	//printf("Update leds %i / %i / %i / %i\n",realNumPlugs,realNumBytes,numPlugs,numBytes);
	sendLeds(buffer, realNumBytes);
	
}

//
void Hub::sendLeds(unsigned char * data, int length)
{
	//To be overriden by child classes (serial, firefly, etc.)
}

void Hub::saveSettings(ofxXmlSettings settings)
{
	settings.addTag("hub");

	settings.pushTag("hub",settings.getNumTags("hub")-1);
	settings.addValue("type",hubType);

	saveSettingsInternal(settings);
	for(int i=0;i<realNumPlugs;i++) plugs[i]->saveSettings(settings);
	settings.popTag();
}

void Hub::saveSettingsInternal(ofxXmlSettings settings)
{
	//to be filled by child classes
}

void Hub::loadSettings(ofxXmlSettings settings, int hubIndex)
{
	//printf("Load settings\n");
	clear();

	settings.pushTag("hub",hubIndex);

	plugLimit = settings.getValue("plugLimit",numPlugs);
	realNumPlugs = settings.getNumTags("plug");
	realNumPlugs = min(realNumPlugs,plugLimit);

	for(int i=0;i<realNumPlugs && i < plugLimit;i++)  
	{
		bool plugLoaded = plugs[i]->loadSettings(settings,i);
	}

	loadSettingsInternal(settings);

	settings.popTag();
}

void Hub::loadSettingsInternal(ofxXmlSettings settings)
{
	//to be filled with child
}

void Hub::updateLedCount()
{
	ledCount = 0;
	for(int i=0;i<realNumPlugs;i++)
	{
		plugs[i]->updateLedCount();
		ledCount += plugs[i]->ledCount;
	}
}

void Hub::clean()
{
	
}

Hub::~Hub()
{
	clear();
	clean();
}
