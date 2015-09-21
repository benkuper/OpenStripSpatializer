#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	printf("OSS :: Open Strip Spatializer setup\n");
	
	ofSetWindowTitle("OSS :: Open Strip Spatializer");
	ofSetWindowShape(640,480);			// Set the initial window size

	//ofSetFrameRate(100);
	
    #ifdef WIN32
	spout.setup();
    #elif MAC_OS_X_VERSION_10_6
    syphonClient.setup();
    //syphonClient.set("","")
    #endif
	
	gui.setup();
	ofAddListener(gui.panel->newGUIEvent,this,&ofApp::guiEvent);
	ofAddListener(gui.ledPanel->newGUIEvent,this,&ofApp::guiEvent);

	directPix = new unsigned char[1920*1080*3];

	//Init with black
	memset(directPix,0,1920*1080*3);
	pixels.setFromPixels(directPix,1920,1080,OF_IMAGE_COLOR);

	fileName = "settings.xml";
	loadSettings(fileName);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);
	ofTexture * targetTex = NULL;
    
    #ifdef _WIN32
	bool spoutReceiveOK = spout.receiveTexture();

	if(showTexture && spoutReceiveOK)
	{	
		targetTex = &spout.myTexture;
		spout.myTexture.draw(0, 0, ofGetWidth(),ofGetHeight());
	
	}
    #elif MAC_OS_X_VERSION_10_6
    #endif

	if(targetTex)
	{
		ledManager.draw(targetTex);
	}
   

	//TODO : move to a place where it's not calculated each time (event ?)
	gui.numLedsLabel->setLabel("Led count : "+ofToString(ledManager.ledCount));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{

		case '1':
			loadSettings("settings_1.xml");
			break;
		case '2':
			loadSettings("settings_2.xml");
			break;
		case '3':
			loadSettings("settings_3.xml");
			break;
		case '4':
			loadSettings("settings_4.xml");
			break;
		case '5':
			loadSettings("settings_5.xml");
			break;

		case '6':
			loadSettings("settings_6.xml");
			break;


		case 's':
			saveSettings();
			break;

		case 'l':
			loadSettings("settings.xml");
			break;

		case 'f':
			ledManager.showLeds = !ledManager.showLeds;
			break;

		case 'p':
			gui.panel->toggleMinified();
			break;

		case 'o':
			gui.ledPanel->toggleMinified();
			break;

		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    #ifdef _WIN32
	if(button == 2) spout.spoutreceiver->SelectSenderPanel("/DX11");
    #endif
	
    if(button == 0) ledManager.mousePressed(false);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	ledManager.mouseReleased();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::loadSettings(const std::string &file)
{
	fileName = file;

	//printf("Load Settings : %s\n",fileName.c_str());

	ofxXmlSettings settings;

	if(!ofFile::doesFileExist(fileName))
	{
		ofLogWarning("File does not exists : %s\n",fileName.c_str());
		//return;
	}

	if(settings.loadFile(fileName)){


		ledManager.loadSettings(settings);

		settings.pushTag("global");
		ledManager.setSendRate(settings.getValue("sendRate",ledManager.sendRate));
		gui.setSendRate(ledManager.sendRate);
		settings.popTag();
	}
	else{
		ofLogError("Position file did not load!");
	}
	
	
}

void ofApp::saveSettings()
{
	//printf("Save settings [%s]....",fileName.c_str());
	ofxXmlSettings settings;
	
	settings.addTag("global");
	settings.pushTag("global");
	settings.addValue("sendRate",ledManager.sendRate);
	settings.popTag();

	ledManager.saveSettings(settings);
	
	
	string str;
	settings.copyXmlToString(str);
	//printf("\n ***\n%s\n***\n",str.c_str());
	settings.saveFile(fileName); //puts settings.xml file in the bin/data folder
	
	//printf("Saved !\n");
}


void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	const char * nameC = name.c_str();
	int kind = e.getKind();

	//printf("got event from: %s\n",name.c_str());
	//printf("kind : %i\n",kind);


	switch(kind)
	{
	case OFX_UI_WIDGET_BUTTON:
		if(strcmp(nameC,"Add Strip") == 0)
		{
			if( e.getButton()->getValue()) ledManager.hubs[0]->addStrip(0);
		}else if(strcmp(nameC,"Remove Strip") == 0)
		{
			if( e.getButton()->getValue()) ledManager.hubs[0]->removeStrip(0);
		}
		break;

	case OFX_UI_WIDGET_TOGGLE:

		if(strcmp(nameC,"Show Texture") == 0)
		{
			showTexture = e.getButton()->getValue();
		}else if(strcmp(nameC,"Show Leds") == 0)
		{
			LedManager::showLeds = e.getButton()->getValue();
		}else if(strcmp(nameC,"Show Lines") == 0)
		{
			LedManager::showLines = e.getButton()->getValue();
		}else if(strcmp(nameC,"Show Links") == 0)
		{
			LedManager::showLinks = e.getButton()->getValue();
		}else if(strcmp(nameC,"Show Handles") == 0)
		{
			LedManager::showHandles = e.getButton()->getValue();
		}else if(strcmp(nameC,"Show Contour") == 0)
		{
			LedManager::showContour = e.getButton()->getValue();
		}else if(strcmp(nameC,"Update Leds") == 0)
		{
			LedManager::doUpdateLeds = !LedManager::doUpdateLeds;
		}else if(strcmp(nameC,"Assign Leds") == 0)
		{
			LedManager::assignLeds = !LedManager::assignLeds;
		}
		break;

	case OFX_UI_WIDGET_SLIDER_H:
		if(kind == OFX_UI_WIDGET_SLIDER_H)
		{
			float sliderVal = e.getSlider()->getValue();
			if(strcmp(nameC,"Send Rate") == 0)
			{
				ledManager.setSendRate(sliderVal);
			}
		}
		
		break;
	}
	
}

void ofApp::exit()
{
    #ifdef _WIN32
    spout.exit();
    #endif
	
}