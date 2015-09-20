#pragma once
#include "Hub.h"
#include "ofxNetwork.h"

class FireflyHub : public Hub
{
public:
	FireflyHub();
	~FireflyHub();

	ofxUDPManager udp;

	string remoteIP;
	int remotePort;

	string bonjourName;
	
		
	virtual void sendLeds(unsigned char * data, int length);
	virtual void init(int hubIndex, ofColor color = ofColor::peachPuff);
	virtual void connect();
	virtual void clean();

protected :
	virtual void loadSettingsInternal(ofxXmlSettings settings);
	virtual void saveSettingsInternal(ofxXmlSettings settings);
	
};