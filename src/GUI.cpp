#include "GUI.h"

void GUI::setup()
{
	
	panel = new ofxUISuperCanvas("Parameters [P]");

	panel->setPosition(ofGetWindowWidth()-panel->getGlobalCanvasWidth()-10,10);
	panel->setColorBack(ofxUIColor(100,50));
	panel->setPadding(10);

	/*
	panel->setFontSize(ofxUIWidgetFontType::OFX_UI_FONT_SMALL,5);
	panel->setFontSize(ofxUIWidgetFontType::OFX_UI_FONT_MEDIUM,7);
	*/

	frameRateLabel = panel->addLabel("Framerate");
	
	panel->addToggle("Show Texture", true);

	panel->addToggle("Show Leds", true);
	panel->addToggle("Show Contour", false);
	panel->addToggle("Show Lines", true);
	panel->addToggle("Show Links", true);
	panel->addToggle("Show Handles", true);

	panel->addToggle("Update Leds", true);
	panel->addToggle("Assign Leds", true);

	panel->addToggle("Send to serial", true);
	sendRateSlider = panel->addSlider("Send Rate",1,100,50);
	
	panel->autoSizeToFitWidgets();

	

	//Led Panel

	
	ledPanel = new ofxUISuperCanvas("Led Manager [O]");
	ledPanel->setPosition(10,10);
	ledPanel->setColorBack(ofxUIColor(100,50));
	ledPanel->setPadding(10);

	
	/*
	ledPanel->setFontSize(ofxUIWidgetFontType::OFX_UI_FONT_SMALL,5);
	ledPanel->setFontSize(ofxUIWidgetFontType::OFX_UI_FONT_MEDIUM,7);
	*/
	numLedsLabel = ledPanel->addLabel("Led count");
	ledPanel->addButton("Add Strip",false);
	ledPanel->addButton("Remove Strip",false);

	updateHubList();

	//Events
	
	ofAddListener(ofEvents().windowResized,this,&GUI::windowResized);
	ofAddListener(ofEvents().draw,this,&GUI::onDraw);
}

void GUI::updateHubList()
{
	/*
	LedManager *lm = LedManager::instance;
	if(lm == NULL) return;

	//clear
	ledPanel->removeWidget(hubRadio);

	vector<string> hubNames;
	for(int i=0;i<lm->hubs.size();i++)
	{
		hubNames.push_back("Hub "+ofToString(i));
	}
	
	hubRadio = ledPanel->addRadio("Hubs",hubNames);
	
	if(hubNames.size() > 0) hubRadio->activateToggle("Hub 0");

	*/
}

void GUI::updateStripList()
{
	/*
	LedManager *lm = LedManager::instance;
	if(lm == NULL) return;

	int("0");
	hubRadio->getActive();
	//clear
	for(int i=0;i<lm->
		hubRadios.clear();
	panel->removeWidget("Hub "+i,
	*/
}

void GUI::windowResized(ofResizeEventArgs & args)
{
	panel->setPosition(ofGetWindowWidth()-panel->getGlobalCanvasWidth()-10,10);
}

void GUI::onDraw(ofEventArgs & args)
{
	frameRateLabel->setLabel("Framerate: "+ofToString(ofGetFrameRate(), 2));
}

//tmp
void GUI::setSendRate(float sendRate)
{
	sendRateSlider->setValue(sendRate);
}