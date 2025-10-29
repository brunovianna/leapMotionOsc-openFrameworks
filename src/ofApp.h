#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// listening port
#define PORT 12345

// max number of strings to display
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxOscReceiver receiver;

		float rHandXf = 0;
		float rHandYf = 0;
		float rHandZf = 0;
		float lHandXf = 0;
		float lHandYf = 0;
		float lHandZf = 0;

		float rHandOrientationWf = 0;
		float rHandOrientationXf = 0;
		float rHandOrientationYf = 0;
		float rHandOrientationZf = 0;
		float lHandOrientationWf = 0;
		float lHandOrientationXf = 0;
		float lHandOrientationYf = 0;
		float lHandOrientationZf = 0;

		// Wireframe meshes for hands
		ofMesh rHandMesh;
		ofMesh lHandMesh;

		// Nodes to apply position + orientation conveniently
		ofNode rHandNode;
		ofNode lHandNode;

		// scale for the hand wireframe (adjust if OSC units differ)
		float handMeshScale = 100.0f;
};
