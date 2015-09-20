#pragma once
#include "Hub.h"

class SerialHub : public Hub
{
public :
	SerialHub();
	~SerialHub();

	string port;
	int baudRate;
	
	ofSerial serial;
	bool serialOpened;

	virtual void sendLeds(unsigned char * data, int length);
	virtual void init(int hubIndex, ofColor color = ofColor::peachPuff);
	virtual void connect();
	virtual void clean();

protected :
	virtual void loadSettingsInternal(ofxXmlSettings settings);
	virtual void saveSettingsInternal(ofxXmlSettings settings);
	
};
