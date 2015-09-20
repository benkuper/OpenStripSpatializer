#include "FireflyHub.h"
#include "Hub.h"
FireflyHub::FireflyHub()
{
	printf("FireflyHub constructor\n");
	
}

void FireflyHub::init(int hubIndex, ofColor color)
{
	printf("FireflyHub :: init\n");
	
	Hub::init(hubIndex,color);
	hubType = HUB_TYPE_FIREFLY;

	this->remoteIP = "127.0.0.1"; // default, to be replaced with settings when loaded
	this->remotePort = 8888; //default, to be replaced with settings when loaded

	udp.Create();
}

void FireflyHub::connect()
{
	udp.Connect(remoteIP.c_str(),remotePort);
    udp.SetNonBlocking(true);
}

void FireflyHub::sendLeds(unsigned char * data, int length)
{
	Hub::sendLeds(data, length);

	int maxPacketSize = 1024;
	int byteIndex = 0;
	while(byteIndex < length)
	{
		udp.Send((const char *)data+byteIndex,min(max(length-maxPacketSize,maxPacketSize), length-byteIndex));
		byteIndex += maxPacketSize;
	}

	//send udp data
	//printf("Send to firefly : %s / %i, message length : %i\n",remoteIP.c_str(),remotePort, length);
}

void FireflyHub::loadSettingsInternal(ofxXmlSettings settings)
{
	remoteIP = settings.getValue("remoteIP","127.0.0.1");
	remotePort = settings.getValue("remotePort",8888);
	bonjourName = settings.getValue("bonjourName","");
}

void FireflyHub::saveSettingsInternal(ofxXmlSettings settings)
{
	settings.addValue("remoteIP",remoteIP);
	settings.addValue("remotePort",remotePort);
	settings.addValue("bonjourName",bonjourName);
}

void FireflyHub::clean()
{
	Hub::clean();
	
}