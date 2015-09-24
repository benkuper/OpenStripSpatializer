#include "LedStrip.h"
#include "ofApp.h"
#include "LedManager.h"

LedStrip::LedStrip(int plugIndex, int stripIndex, int shape)
{
	this->shape = shape;
	this->plugIndex = plugIndex;
	this->stripIndex = stripIndex;

	numLeds = 0;

	dragHandle = NULL;

	start.x = (ofRandomf()*.5+.5)*ofGetWindowWidth();
	start.y = (ofRandomf()*.5+.5)*ofGetWindowHeight();

	end.x = (ofRandomf()*.5+.5)*ofGetWindowWidth();
	end.y = (ofRandomf()*.5+.5)*ofGetWindowHeight();

	updatePositions();

	
}

void LedStrip::updatePositions()
{
	

	float rad;

	switch(shape)
	{
		case SHAPE_LINE:
			for(int i=0;i<numLeds;i++)
			{
				float pos = i*1.0f/(numLeds-1);
				leds[i]->set(start.getInterpolated(end,pos));
			}
			break;
		
		case SHAPE_CIRCLE:
			rad = start.distance(end);
			for(int i=0;i<numLeds;i++)
			{
				float pos = i*TWO_PI/numLeds;
				
				leds[i]->set(start+ofVec2f(cosf(pos),sinf(pos))*rad);
			}
			break;

		case SHAPE_SQUARE:
			int sideNumLeds = numLeds /4;
			//printf("side Num Leds : %i\n",sideNumLeds);
			ofVec2f p1;
			ofVec2f p2;
			for(int s=0;s<4;s++)
			{
				switch(s)
				{
				case 0:
					p1.x = start.x;
					p1.y = start.y;
					p2.x = end.x;
					p2.y = start.y;
					break;

				case 1:
					p1.x = end.x;
					p1.y = start.y;
					p2.x = end.x;
					p2.y = end.y;
					break;

				case 2:
					p1.x = end.x;
					p1.y = end.y;
					p2.x = start.x;
					p2.y = end.y;
					break;

				case 3:
					p1.x = start.x;
					p1.y = end.y;
					p2.x = start.x;
					p2.y = start.y;
					break;


				}
				int sideLeftLeds = min(sideNumLeds,numLeds-s*4);
				//printf("\t> sideLeftLeds : %i\n",sideLeftLeds);

				for(int i=0;i<sideNumLeds;i++)
				{
					int index = s*sideNumLeds+i;
					
					if(index < numLeds)
					{
						float pos = i*1.0f/(sideLeftLeds-1);
						leds[index]->set(p1.getInterpolated(p2,pos));
						//printf("\t\t index[%i] -> %f  = %i / %i\n",index,pos,leds[index]->x,leds[index]->y);
					}
				}
			}
			break;
	}
}

void LedStrip::updateLedMap(int baseIndex, ofFloatPixels * ledMapPixels)
{
	for(int i=0;i<numLeds;i++) 
	{
		int pixIndex = baseIndex*3;
		ledMapPixels->setColor(pixIndex,leds[i]->x);
		ledMapPixels->setColor(pixIndex+1,leds[i]->y);
		//printf("set color at index %i	%d	%d\n",baseIndex, leds[i]->x,leds[i]->y);
		baseIndex ++;
	}
}


void LedStrip::draw(int baseIndex,ofPixels * pixels,ofColor color)
{
	//printf("ofApp pixels width : %i\n",pixels.getWidth());
	float w = ofGetWidth()*1.0f;
	float h = ofGetHeight()*1.0f;

	if(dragHandle != NULL)
	{
		ofVec2f offset = start-end;
		dragHandle->set(ofGetMouseX()/w,ofGetMouseY()/h);

		if(dragHandle == &start) 
		{
			switch(shape)
			{
			case SHAPE_CIRCLE:
			case SHAPE_SQUARE:
				end = start-offset;
				break;
			}
			
		}
		updatePositions();
	}

	ofVec2f startS;
	startS.set(start.x*w,start.y*h);
	ofVec2f endS;
	endS.set(end.x*w,end.y*h);




	if(LedManager::showLines)
	{	
		ofSetColor(color.r,color.g,color.b,100);
		ofLine(startS,endS);

		float rad;
			
		switch(shape)
		{
		case SHAPE_LINE:
			break;

		case SHAPE_CIRCLE:
			rad = start.distance(end);
			ofEllipse(startS,rad*w,rad*h);
			break;

		case SHAPE_SQUARE:
			ofLine(startS.x,startS.y,endS.x,startS.y);
			ofLine(endS.x,startS.y,endS.x,endS.y);
			ofLine(endS.x,endS.y,startS.x,endS.y);
			ofLine(startS.x,endS.y,startS.x,startS.y);
			break;
		}
	}

	if(LedManager::showHandles)
	{
		ofSetColor(color);
		ofCircle(startS.x,startS.y,4);

		ofSetColor(255,255,0);
		ofCircle(endS.x,endS.y,4);

		ofSetColor(color);
		string label = customLabel != ""?customLabel:ofToString(plugIndex)+":"+ofToString(stripIndex);
		ofDrawBitmapString(label,startS.x+5,startS.y+5);
	}

	
	int pw = pixels->getWidth();
	int ph = pixels->getHeight();


	for(int i=0;i<numLeds;i++)
	{
		if(LedManager::assignLeds) 
		{
			unsigned char * pix = pixels->getPixels();
			int pixIndex= baseIndex*4;

			if(leds[i]->x < 1 && leds[i]->y < 1  && leds[i]->x >= 0 && leds[i]->y >= 0)
			{

				leds[i]->color.set(pix[pixIndex],pix[pixIndex+1],pix[pixIndex+2],pix[pixIndex+3]);
			}else
			{
				leds[i]->color.set(0,100);
			}
		}
		
		if(LedManager::showLeds) leds[i]->draw();

		baseIndex++;
	}
}

bool LedStrip::mousePressed(bool force)
{

	ofVec2f mousePos;
	mousePos.set(ofGetMouseX()*1.0f/ofGetWidth(),ofGetMouseY()*1.0f/ofGetHeight());
	if(start.distance(mousePos) < .01f ||force) dragHandle = &start;
	if(end.distance(mousePos) < .01f) dragHandle = &end;	

	bool pressResult =  (dragHandle != NULL && !force);
	return pressResult;

}

void LedStrip::mouseReleased()
{
	dragHandle = NULL;
}


void LedStrip::updateLeds(unsigned char * buffer, int offset)
{
	for(int i=0;i<numLeds;i++) 
	{
		int ledOffset = offset+(i*3);
		//printf("set at buffer : %i, %i	%i	%i\n",ledOffset,(int)(ofClamp(leds[i]->color.r,0,254)),(int)(ofClamp(leds[i]->color.g,0,254)),(int)(ofClamp(leds[i]->color.b,0,254)));
		
        float ledAlpha = (float)((float)leds[i]->color.a/255.0);
		buffer[ledOffset]	= (unsigned char)(ofClamp(leds[i]->color.r*ledAlpha,0,254));
		buffer[ledOffset+1] = (unsigned char)(ofClamp(leds[i]->color.g*ledAlpha,0,254));
		buffer[ledOffset+2] = (unsigned char)(ofClamp(leds[i]->color.b*ledAlpha,0,254));
	}
}


void  LedStrip::saveSettings(ofxXmlSettings settings)
{
	
	settings.addTag("strip");
	settings.pushTag("strip",settings.getNumTags("strip")-1);
	if(customLabel != "") settings.addValue("label",customLabel);
	settings.addValue("startX", start.x);
	settings.addValue("startY", start.y);
	settings.addValue("endX", end.x);
	settings.addValue("endY", end.y);
	settings.addValue("numLeds",numLeds);
	settings.addValue("shape",shape);
	settings.popTag();

	
}

void LedStrip::loadSettings(ofxXmlSettings settings, int stripIndex)
{
	settings.pushTag("strip",stripIndex);

	start.x = settings.getValue("startX",start.x);
	start.y = settings.getValue("startY",start.y);
	end.x = settings.getValue("endX",end.x);
	end.y = settings.getValue("endY",end.y);

	shape = settings.getValue("shape",SHAPE_LINE);

	customLabel = settings.getValue("label","");

	setNumLeds(settings.getValue("numLeds",0));


	settings.popTag();

	updatePositions();

	
}

void LedStrip::setNumLeds(int numLeds)
{
	
	this->numLeds = numLeds;
	//printf("Set num leds :%i\n",numLeds);
	
	leds.clear();
	for(int i=0;i<numLeds;i++) leds.push_back(new Led());
}