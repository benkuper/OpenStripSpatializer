#include "HubPlug.h"
#include "LedManager.h"

HubPlug::HubPlug(int index)
{
	numStrips = 0;
	plugIndex = index;
}

void HubPlug::draw(int baseIndex, ofPixels * pixels,ofColor hubColor)
{
	
	int w = ofGetWidth();
	int h = ofGetHeight();

	for(int i=0;i<numStrips;i++) 
	{
		strips[i]->draw(baseIndex, pixels,hubColor);	
		baseIndex += strips[i]->numLeds;
		
		if(LedManager::showLinks)
		{
			if(i >0) 
			{
				ofPushStyle();
				ofSetColor(255,0,255,70);
				LedStrip * s = strips[i-1];
				ofLine(s->end.x*w,s->end.y*h,strips[i]->start.x*w,strips[i]->start.y*h);
				ofPopStyle();
			}  
		}
	}
}

void HubPlug::addStrip()
{
	strips.push_back(new LedStrip(plugIndex,numStrips));
	numStrips = strips.size();
}

void HubPlug::removeStrip(int stripIndex)
{
	if(stripIndex == -1) stripIndex = numStrips-1;
	if(stripIndex < numStrips && numStrips > 0) strips.erase(strips.begin()+stripIndex);

	
	numStrips = strips.size();

	for(int i=0;i<numStrips;i++) strips[i]->stripIndex = i;
}



void HubPlug::saveSettings(ofxXmlSettings settings)
{
	settings.addTag("plug");
	settings.pushTag("plug",settings.getNumTags("plug")-1);
	for(int i=0;i<numStrips;i++) strips[i]->saveSettings(settings);
	settings.popTag();
}

bool HubPlug::loadSettings(ofxXmlSettings settings, int plugIndex)
{
	clear();


	bool result = settings.pushTag("plug",plugIndex);

	if(!result) return false;

	int loadedStrips = settings.getNumTags("strip");
	for(int i=0;i<loadedStrips;i++)
	{
		addStrip();
		strips[i]->loadSettings(settings,i);
	}
		
 
	settings.popTag();

	return true;
}


bool HubPlug::mousePressed(bool force)
{
	bool result = false;
	for(int i=0;i<numStrips;i++)
	{
		if( strips[i]->mousePressed(force)) result = true;
		if(!force && result) return true;
	}

	return result;
}

void HubPlug::mouseReleased()
{
	for(int i=0;i<numStrips;i++) strips[i]->mouseReleased();
}

void HubPlug::updatePositions()
{
	for(int i=0;i<numStrips;i++) strips[i]->updatePositions();
}

void HubPlug::updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels)
{
	for(int i=0;i<numStrips;i++) 
	{
		strips[i]->updateLedMap(baseIndex,ledMapPixels);
		baseIndex += strips[i]->numLeds;
	}
}


void HubPlug::updateLeds(unsigned char * buffer)
{
	int offset = plugIndex*(maxLedPerPlug*3); //(3 bytes per led
	//printf("Offset for plug %i is %i, numStrips :%i\n",plugIndex,offset,numStrips);
	
	int curStripLedsOffset = 0;

	for(int i=0;i<numStrips;i++) 
	{
		strips[i]->updateLeds(buffer,offset+curStripLedsOffset);
		curStripLedsOffset += strips[i]->numLeds*3;
	}

	
	//printf("Led count for fill in is %i // diff = %i\n",ledCount,maxLedPerPlug-ledCount);
	for(int i= ledCount;i<maxLedPerPlug;i++) //fill in for the rest of the plug (i.e., if only 20 leds are used, fill with 280 byte so the next plug is addressing right)	
	{
		int ledOffset = offset + (i*3);

		buffer[ledOffset] = 0; 
		buffer[ledOffset+1] = 0; 
		buffer[ledOffset+2] = 0;
	}
}

void HubPlug::updateLedCount()
{
	ledCount = 0;
	for(int i=0;i<numStrips;i++) ledCount += strips[i]->numLeds;
}

void HubPlug::clear()
{
	while(numStrips > 0) removeStrip();
}

