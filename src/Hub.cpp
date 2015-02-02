#pragma once
#include "Hub.h"


Hub::Hub(ofxXmlSettings settings, int hubIndex, ofColor color)
{
	init();
	hubColor = color;
	this->hubIndex = hubIndex;

	loadSettings(settings,hubIndex);
	updatePositions();
}

Hub::Hub(int hubIndex, string port = "COM1", int baudRate = 9600,ofColor color = ofColor::peachPuff)
{
	init();

	this->port = port;
	this->baudRate = baudRate;
	hubColor = color;

	updatePositions();
	
}

void Hub::init()
{
	for(int i=0;i<numPlugs;i++) plugs.push_back(new HubPlug(i));
	updatePositions();
}


void Hub::draw(int baseIndex, ofPixels * pixels)
{
	for(int i=0;i<numPlugs;i++) 
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
	for(int i=0;i<numPlugs;i++) 
	{
		plugs[i]->updateLedMap(baseIndex,ledMapPixels);
		baseIndex += plugs[i]->ledCount;
	}
}

void Hub::updateLedsSerial()
{
	float m0 = ofGetElapsedTimef();

	const int numBytes = (numPlugs*HubPlug::maxLedPerPlug)*3+1; //3bytes per led + 255 final byte

	unsigned char buffer[numBytes];// = new unsigned char[totalLeds*3];
	memset(buffer,0,numBytes);

	//printf("total numBytes:%i\n",numBytes);
	for(int i=0;i<numPlugs;i++) plugs[i]->updateLedsSerial(buffer);


	buffer[numBytes-1] = 255;

	float elapsed0 = ofGetElapsedTimef() - m0;
	//printf("Buffer fill : %f micros\n",elapsed0);

	if(!serial.isInitialized())
	{
		//printf("Hub on port : %s	is not initialized, not sending.\n",port.c_str());
		return;
	}
	unsigned long long m1 = ofGetElapsedTimeMicros();
	serial.writeBytes(buffer,numBytes);
	unsigned long long elapsed = ofGetElapsedTimeMicros() - m1;
	//printf("Serial write : %llu micros\n",elapsed);
	
}

void Hub::saveSettings(ofxXmlSettings settings)
{
	settings.addTag("hub");

	settings.pushTag("hub",settings.getNumTags("hub")-1);
	settings.addValue("port",port);
	for(int i=0;i<numPlugs;i++) plugs[i]->saveSettings(settings);
	settings.popTag();

}

void Hub::loadSettings(ofxXmlSettings settings, int hubIndex)
{
	printf("Load settings\n");
	clear();

	settings.pushTag("hub",hubIndex);

	int loadedStrips = settings.getNumTags("strip");
	for(int i=0;i<numPlugs;i++)  plugs[i]->loadSettings(settings,i);

	port = settings.getValue("port","COM1");
	baudRate = settings.getValue("baudRate",9600);
	
	settings.popTag();

	//serialOpened = serial.setup(port,baudRate);
	printf("Serial opened (%s) ? %i\n",port.c_str(),serialOpened?1:0);

}

void Hub::updateLedCount()
{
	ledCount = 0;
	for(int i=0;i<numPlugs;i++)
	{
		plugs[i]->updateLedCount();
		ledCount += plugs[i]->ledCount;
	}
}

void Hub::clean()
{
	printf("Clean hub (%s)\n",port.c_str());
	serial.close();
}

Hub::~Hub()
{
	clear();
	clean();
}
