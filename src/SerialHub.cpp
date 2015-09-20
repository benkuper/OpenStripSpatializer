#pragma once
#include "SerialHub.h"
#include "ofMain.h"
#include "Hub.h"


SerialHub::SerialHub()
{
	printf("SerialHub constructor\n");
}


void SerialHub::init(int hubIndex, ofColor color)
{
	printf("SerialHub :: init\n");
	Hub::init(hubIndex,color);
	hubType = HUB_TYPE_SERIAL;

	this->port = "COM1"; // default, to be replaced with settings when loaded
	this->baudRate = 9600; //default, to be replaced with settings when loaded
}

void SerialHub::connect()
{
	printf("SerialHub::connect\n");
	printf("open at baud rate :%i\n",baudRate);
	serialOpened = serial.setup(port,baudRate);
	printf(" > Serial opened (%s) ? %i\n",port.c_str(),serialOpened?1:0);

}

void SerialHub::sendLeds(unsigned char * data, int length)
{
	Hub::sendLeds(data, length);

	if(!serial.isInitialized())
	{
		//printf("Hub on port : %s	is not initialized, not sending.\n",port.c_str());
		return;
	}
	
	//for(int i=0;i<realNumBytes;i++) printf("%i",buffer[i]);
	
	unsigned long long m1 = ofGetElapsedTimeMicros();
	serial.writeBytes(data,length);
	unsigned long long elapsed = ofGetElapsedTimeMicros() - m1;
	
	//printf("Serial write : %llu micros\n",elapsed);
}

void SerialHub::loadSettingsInternal(ofxXmlSettings settings)
{
	port = settings.getValue("port","COM1");
	baudRate = settings.getValue("baudRate",9600);
}

void SerialHub::saveSettingsInternal(ofxXmlSettings settings)
{
	settings.addValue("port",port);
	settings.addValue("baudRate",baudRate);
}

void SerialHub::clean()
{
	Hub::clean();
	printf("Clean hub (%s)\n",port.c_str());
	serial.close();
}