#include "ofMain.h"
#include "ofApp.h"

#include "ofGLProgrammableRenderer.h"

string resourcePath;

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setGLVersion(4, 5); /// < select your GL Version here
	ofCreateWindow(settings); ///< create your window here

#ifdef __APPLE__
    // Change the root folder for relative path : set it to Contents/Resources/ inside the .app bundle, instead of data/ folder next to the .app file.
    // With this change, your app will try to find your assets in your app, so don't forget to add your assets in Build Phases/Copy Files phase (the one with Resources for Destination) in Xcode to automatically bundle your assets in app when building project.
    // ----------------------------------------------------------------------------
    char path[2048];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, 2048))
    {
        std::cerr << "Error : couldn't get bundle path resources" << std::endl;
    }
    CFRelease(resourcesURL);
    
    
    //int result = chdir(path); // do not move path because we are still using openframeworks stuff
    resourcePath = ofToString(path);
    resourcePath.append("/");
    std::cout << "Current Path: " << resourcePath << std::endl;
    ofSetDataPathRoot(resourcePath); // openframeworks way to append string
#else
    resourcePath = "";
#endif

	ofRunApp(new ofApp());

}
