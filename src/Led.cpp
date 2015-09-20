#include "Led.h"
#include "LedManager.h"

Led::Led()
{
	radius = 2;
	color.set(ofColor::black);
	//color.set(ofColor::cyan);
}

void Led::draw()
{
	//printf("Draw %f %f\n",x,y);
	
	ofPushStyle();

	ofSetColor(color);
	//ofCircle(x*ofGetWidth(),y*ofGetHeight(),radius);
	ofLine(x*ofGetWidth(),y*ofGetHeight(),x*ofGetWidth(),y*ofGetHeight()-1);
	
	if(LedManager::showContour)
	{
		ofPolyline polyline;
		ofPoint point1(x*ofGetWidth(),y*ofGetHeight());
		polyline.arc(point1,radius,radius,0,360);
		ofSetColor(ofColor::red);
		polyline.draw();
	}
	ofPopStyle();
	//ofLine(x,y,x+1,y);
	//printf("Color : %i\n",color.r);
}

